/****************************************************************************************

	���ϸ� : TNKalaSystem.h
	�ۼ��� : �����(spencerj@korea.com)
	�ۼ��� : 2004-10-19

	������ :
	������ :

	������Ʈ�� : 

	���� : 


****************************************************************************************/
#ifndef __TNKalaSystem_h__
#define __TNKalaSystem_h__

#include "Basedef.h"



struct TNKALA_LOCATION
{
	int iWhere ;
	int iAltar ; // altar�� kala�� ��ġ���� ��, altar ��ȣ
	int iMonsterHandle ; // altar�� ��ġ�ϰ� �ִ� monster handle
	int iX, iY ; // ground�� ��ġ���� ���� ��ǥ
	int iPCHandle ; // kala-core�� PC�� ������ ���, ������ handle
	int iInventorySlot ; // inventory������ kala-core ��ġ
	unsigned int uiTimePossessed ;
} ;

struct TNKALA_ALTAR
{
	//short sID ;
	//short sTrimuriti ;
	//short x ;
	//short y ;
	int iKala ;
} ;


class TNKalaSystem
{
public :
	TNKalaSystem() ;
	~TNKalaSystem() ;

	void Init() ;

	

// Public Operations
public :
	inline void ClearKalaLoc( int a_iLocSlot ) { Print( a_iLocSlot ) ; ChangeInfo( a_iLocSlot, eLoc_NoWhere, 0, 0, 0, 0, 0, 0 ) ; }
	int ChangeInfo( int a_iLocSlot, int a_iWhere, int a_iAltar, int a_iMonsterHandle, int a_iX, int a_iY, int a_iPCHandle, int a_iInventorySlot ) ;
	int ChangeKalaLoc( int a_iSrcWhere, int a_iSrcAltar, int a_iSrcMonsterHandle, int a_iSrcX, int a_iSrcY, int a_iSrcPCHandle, int a_iDestWhere, int a_iDestAltar, int a_iDestMonsterHandle, int a_iDestX, int a_iDestY, int a_iDestPCHandle, int a_iTaker=0 ) ;
	int FindKalaLoc( int a_iWhere, int a_iAltar, int a_iMonsterHandle, int a_iX, int a_iY, int a_iPCHandle, int a_iInventorySlot ) ;
	int FindEmptySlot() ;
	void Set( int a_iWhere, int a_iAltar, int a_iMonsterHandle ) ;
	void PrintScreen( char* a_pBuffer ) ;
	void Print( char* a_pBuffer=NULL ) ;
	void Print( int a_iLocSlot, int a_iCaller=0 ) ;
	int CheckCountNRecovery() ;
	int CheckKalaOnAltar() ;
	int FindKalaCoreOnTheGround( int& a_iX, int& a_iY ) ;
	void DropKalaCoreInPC() ;
	int FindEmptyAltar( int a_iClan ) ;
	int IsEmptyAltar( int a_iSlot ) { if( 0 > a_iSlot || eKalaAltar_MaxCount <= a_iSlot ) return -1 ; if( 0 < m_krgAltar[a_iSlot].iKala ) return m_krgAltar[a_iSlot].iKala ; return -1 ; } 
	int InstallKala( int a_iTribe, int a_iClan, int a_iSlot, int a_iPCHandle=0 ) ;

// Public Properties
	//void get_AltarCoord( int a_iSlot, int& a_iX, int& a_iY ){ if( 0 > a_iSlot || eKalaAltar_MaxCount <= a_iSlot ) return ; a_iX = m_krgAltar[a_iSlot].x ; a_iY = m_krgAltar[a_iSlot].y ; } 


// Attributes
public :
	enum { eLoc_NoWhere = 0, eLoc_Altar = 1, eLoc_Ground, eLoc_Inventory, } ;
	//enum { eKala_MaxCount = 9, eKalaAltar_MaxCount = 15, } ;


private :	

	int m_iKalaOnAltar ;
	int m_iKalaCoreInPC ;
	int m_iKalaCoreOnGround ;
	TNKALA_LOCATION m_krgKala[eKala_MaxCount] ;
	TNKALA_ALTAR m_krgAltar[eKalaAltar_MaxCount] ;
} ;

extern TNKalaSystem g_kKalaSystem ;

#endif //__TNKalaSystem_h__