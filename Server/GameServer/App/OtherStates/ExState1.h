#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _ExState1_h_
#define _ExState1_h_

#include "..\States\State.h"
#include "..\States\VisualEffect.h"

class CExState1 : public CState
{
	tagExState						m_stStateParam;
	//##����ʱ���Ǻ���Ϊ��λ
	//LONG							m_lTimeToKeep;

public:
	CExState1						( tagExState &stState );
	CExState1						();
	virtual ~CExState1				();

	virtual BOOL OnUpdateProperties ();

	virtual BOOL	Begin			( CMoveShape* pUser, CMoveShape* pSufferer );
	virtual BOOL	Begin			( CMoveShape* pUser, LONG lX, LONG lY );
	virtual BOOL	Begin			( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion );

	virtual VOID	AI				();
	virtual VOID	End				();

	virtual VOID	OnChangeRegion	( LONG );
	virtual DWORD	GetRemainedTime ();

	//##���л�
	virtual VOID	Serialize		( vector<BYTE>& vOut );
	//##�����л�
	virtual VOID	Unserialize		( BYTE* pBytes, LONG& lPos );
};


class CExState1VisualEffect : public CVisualEffect
{
public:
	virtual VOID		UpdateVisualEffect	( CState* pParent, DWORD dwCmd );
};

#endif