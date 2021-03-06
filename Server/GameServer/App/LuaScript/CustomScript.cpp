///
/// @file CustomScript.cpp
/// @brief Some custom lua script to custom the script module.
/// @author Kevin Lynx
///
#include "stdafx.h"
#include "ScriptSys.h"
#include "lua.hpp"

/* SetGHook lua script:
-- Record user defined global variables
declaredNames = { }

-- Declare a global variable, the initval can be nil,
-- so that you can check whether a variable exists by nil.
function declare( name, initval )
	rawset( _G, name, initval )
	declaredNames[name] = true
end

-- Undeclare a global variable
function undeclare( name )
	rawset( _G, name, nil )
	declaredNames[name] = nil
end

-- Replace _G table's metatable
setmetatable( _G, {
	__newindex = function ( t, k, v )
		if not declaredNames[k] then
			error( 'attempt to write to undeclared variable: ' .. k, 2 )
		else
			rawset( t, k, v )
		end
	end,

	__index = function( _, k )
		if not declaredNames[k] then
			error( 'attempt to read undeclared variable: '..k, 2 )
		else
			return nil
		end
	end,
})
*/
static int SetGHook( lua_State *L )
{
    // see comment above
    const unsigned char B[] = {
	0x2D, 0x2D, 0x20, 0x52, 0x65, 0x63, 0x6F, 0x72, 0x64, 0x20, 0x75, 0x73, 0x65, 0x72, 0x20, 0x64, 
	0x65, 0x66, 0x69, 0x6E, 0x65, 0x64, 0x20, 0x67, 0x6C, 0x6F, 0x62, 0x61, 0x6C, 0x20, 0x76, 0x61, 
	0x72, 0x69, 0x61, 0x62, 0x6C, 0x65, 0x73, 0x0D, 0x0A, 0x64, 0x65, 0x63, 0x6C, 0x61, 0x72, 0x65, 
	0x64, 0x4E, 0x61, 0x6D, 0x65, 0x73, 0x20, 0x3D, 0x20, 0x7B, 0x20, 0x7D, 0x0D, 0x0A, 0x0D, 0x0A, 
	0x2D, 0x2D, 0x20, 0x44, 0x65, 0x63, 0x6C, 0x61, 0x72, 0x65, 0x20, 0x61, 0x20, 0x67, 0x6C, 0x6F, 
	0x62, 0x61, 0x6C, 0x20, 0x76, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6C, 0x65, 0x2C, 0x20, 0x74, 0x68, 
	0x65, 0x20, 0x69, 0x6E, 0x69, 0x74, 0x76, 0x61, 0x6C, 0x20, 0x63, 0x61, 0x6E, 0x20, 0x62, 0x65, 
	0x20, 0x6E, 0x69, 0x6C, 0x2C, 0x0D, 0x0A, 0x2D, 0x2D, 0x20, 0x73, 0x6F, 0x20, 0x74, 0x68, 0x61, 
	0x74, 0x20, 0x79, 0x6F, 0x75, 0x20, 0x63, 0x61, 0x6E, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x20, 
	0x77, 0x68, 0x65, 0x74, 0x68, 0x65, 0x72, 0x20, 0x61, 0x20, 0x76, 0x61, 0x72, 0x69, 0x61, 0x62, 
	0x6C, 0x65, 0x20, 0x65, 0x78, 0x69, 0x73, 0x74, 0x73, 0x20, 0x62, 0x79, 0x20, 0x6E, 0x69, 0x6C, 
	0x2E, 0x0D, 0x0A, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x64, 0x65, 0x63, 0x6C, 
	0x61, 0x72, 0x65, 0x28, 0x20, 0x6E, 0x61, 0x6D, 0x65, 0x2C, 0x20, 0x69, 0x6E, 0x69, 0x74, 0x76, 
	0x61, 0x6C, 0x20, 0x29, 0x0D, 0x0A, 0x09, 0x72, 0x61, 0x77, 0x73, 0x65, 0x74, 0x28, 0x20, 0x5F, 
	0x47, 0x2C, 0x20, 0x6E, 0x61, 0x6D, 0x65, 0x2C, 0x20, 0x69, 0x6E, 0x69, 0x74, 0x76, 0x61, 0x6C, 
	0x20, 0x29, 0x0D, 0x0A, 0x09, 0x64, 0x65, 0x63, 0x6C, 0x61, 0x72, 0x65, 0x64, 0x4E, 0x61, 0x6D, 
	0x65, 0x73, 0x5B, 0x6E, 0x61, 0x6D, 0x65, 0x5D, 0x20, 0x3D, 0x20, 0x74, 0x72, 0x75, 0x65, 0x0D, 
	0x0A, 0x65, 0x6E, 0x64, 0x0D, 0x0A, 0x0D, 0x0A, 0x2D, 0x2D, 0x20, 0x55, 0x6E, 0x64, 0x65, 0x63, 
	0x6C, 0x61, 0x72, 0x65, 0x20, 0x61, 0x20, 0x67, 0x6C, 0x6F, 0x62, 0x61, 0x6C, 0x20, 0x76, 0x61, 
	0x72, 0x69, 0x61, 0x62, 0x6C, 0x65, 0x0D, 0x0A, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 
	0x20, 0x75, 0x6E, 0x64, 0x65, 0x63, 0x6C, 0x61, 0x72, 0x65, 0x28, 0x20, 0x6E, 0x61, 0x6D, 0x65, 
	0x20, 0x29, 0x0D, 0x0A, 0x09, 0x72, 0x61, 0x77, 0x73, 0x65, 0x74, 0x28, 0x20, 0x5F, 0x47, 0x2C, 
	0x20, 0x6E, 0x61, 0x6D, 0x65, 0x2C, 0x20, 0x6E, 0x69, 0x6C, 0x20, 0x29, 0x0D, 0x0A, 0x09, 0x64, 
	0x65, 0x63, 0x6C, 0x61, 0x72, 0x65, 0x64, 0x4E, 0x61, 0x6D, 0x65, 0x73, 0x5B, 0x6E, 0x61, 0x6D, 
	0x65, 0x5D, 0x20, 0x3D, 0x20, 0x6E, 0x69, 0x6C, 0x0D, 0x0A, 0x65, 0x6E, 0x64, 0x0D, 0x0A, 0x0D, 
	0x0A, 0x2D, 0x2D, 0x20, 0x52, 0x65, 0x70, 0x6C, 0x61, 0x63, 0x65, 0x20, 0x5F, 0x47, 0x20, 0x74, 
	0x61, 0x62, 0x6C, 0x65, 0x27, 0x73, 0x20, 0x6D, 0x65, 0x74, 0x61, 0x74, 0x61, 0x62, 0x6C, 0x65, 
	0x0D, 0x0A, 0x73, 0x65, 0x74, 0x6D, 0x65, 0x74, 0x61, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x28, 0x20, 
	0x5F, 0x47, 0x2C, 0x20, 0x7B, 0x0D, 0x0A, 0x09, 0x5F, 0x5F, 0x6E, 0x65, 0x77, 0x69, 0x6E, 0x64, 
	0x65, 0x78, 0x20, 0x3D, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x28, 0x20, 
	0x74, 0x2C, 0x20, 0x6B, 0x2C, 0x20, 0x76, 0x20, 0x29, 0x0D, 0x0A, 0x09, 0x09, 0x69, 0x66, 0x20, 
	0x6E, 0x6F, 0x74, 0x20, 0x64, 0x65, 0x63, 0x6C, 0x61, 0x72, 0x65, 0x64, 0x4E, 0x61, 0x6D, 0x65, 
	0x73, 0x5B, 0x6B, 0x5D, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x0D, 0x0A, 0x09, 0x09, 0x09, 0x65, 0x72, 
	0x72, 0x6F, 0x72, 0x28, 0x20, 0x27, 0x61, 0x74, 0x74, 0x65, 0x6D, 0x70, 0x74, 0x20, 0x74, 0x6F, 
	0x20, 0x77, 0x72, 0x69, 0x74, 0x65, 0x20, 0x74, 0x6F, 0x20, 0x75, 0x6E, 0x64, 0x65, 0x63, 0x6C, 
	0x61, 0x72, 0x65, 0x64, 0x20, 0x76, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6C, 0x65, 0x3A, 0x20, 0x27, 
	0x20, 0x2E, 0x2E, 0x20, 0x6B, 0x2C, 0x20, 0x32, 0x20, 0x29, 0x0D, 0x0A, 0x09, 0x09, 0x65, 0x6C, 
	0x73, 0x65, 0x0D, 0x0A, 0x09, 0x09, 0x09, 0x72, 0x61, 0x77, 0x73, 0x65, 0x74, 0x28, 0x20, 0x74, 
	0x2C, 0x20, 0x6B, 0x2C, 0x20, 0x76, 0x20, 0x29, 0x0D, 0x0A, 0x09, 0x09, 0x65, 0x6E, 0x64, 0x0D, 
	0x0A, 0x09, 0x65, 0x6E, 0x64, 0x2C, 0x0D, 0x0A, 0x0D, 0x0A, 0x09, 0x5F, 0x5F, 0x69, 0x6E, 0x64, 
	0x65, 0x78, 0x20, 0x3D, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x28, 0x20, 0x5F, 
	0x2C, 0x20, 0x6B, 0x20, 0x29, 0x0D, 0x0A, 0x09, 0x09, 0x69, 0x66, 0x20, 0x6E, 0x6F, 0x74, 0x20, 
	0x64, 0x65, 0x63, 0x6C, 0x61, 0x72, 0x65, 0x64, 0x4E, 0x61, 0x6D, 0x65, 0x73, 0x5B, 0x6B, 0x5D, 
	0x20, 0x74, 0x68, 0x65, 0x6E, 0x0D, 0x0A, 0x09, 0x09, 0x09, 0x65, 0x72, 0x72, 0x6F, 0x72, 0x28, 
	0x20, 0x27, 0x61, 0x74, 0x74, 0x65, 0x6D, 0x70, 0x74, 0x20, 0x74, 0x6F, 0x20, 0x72, 0x65, 0x61, 
	0x64, 0x20, 0x75, 0x6E, 0x64, 0x65, 0x63, 0x6C, 0x61, 0x72, 0x65, 0x64, 0x20, 0x76, 0x61, 0x72, 
	0x69, 0x61, 0x62, 0x6C, 0x65, 0x3A, 0x20, 0x27, 0x2E, 0x2E, 0x6B, 0x2C, 0x20, 0x32, 0x20, 0x29, 
	0x0D, 0x0A, 0x09, 0x09, 0x65, 0x6C, 0x73, 0x65, 0x0D, 0x0A, 0x09, 0x09, 0x09, 0x72, 0x65, 0x74, 
	0x75, 0x72, 0x6E, 0x20, 0x6E, 0x69, 0x6C, 0x0D, 0x0A, 0x09, 0x09, 0x65, 0x6E, 0x64, 0x0D, 0x0A, 
	0x09, 0x65, 0x6E, 0x64, 0x2C, 0x0D, 0x0A, 0x7D, 0x29, 0x0D, 0x0A
};
    return luaL_loadbuffer( L, (const char*) B, sizeof( B ), "SetGHook script" ) ||
        lua_pcall( L, 0, 0, 0 );
}

int ScriptSys::RunCustomScript()
{
    int ret = SetGHook( m_MainState );
    return ret;
}
