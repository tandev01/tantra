/****************************************************************************

	���ϸ� : TNSiege.cpp

	�ۼ��� : �����(spencerj@korea.com)
	�ۼ��� : 2005-07-07

	Tab size : 4 spaces
	������Ʈ�� : Tantra


	���� : 
		- 
		- 

****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "TNDebug.h"
#include "Basedef.h"
#include "CMob.h"
#include "TNSiege.h"
#include "Server.h"
#include "Language.h"
#include "CGuild.h"
#include "SendFunc.h"

//extern CUser					pUser    [MAX_USER];

//extern int                      g_iZoneID ;
extern HWND                     hWndMain;
extern STRUCT_MOB				pMonsterData[MAX_MONSTER_DATA] ;
extern unsigned short         pMobGrid   [MAX_GRIDY][MAX_GRIDX];
extern int						g_iCastleOwner;

TNSiege g_kSiege ;


// static : (x,y), point
short g_srgSymbolForSiege[TNSiege::eSiege_SymbolCount][3] = 
{
	656, 618, 3,
	455, 609, 2,
	646, 761, 2,
	527, 770, 1,
	399, 762, 1,
	373, 618, 1,
	453, 456, 1,
	365, 387, 1,
	547, 436, 1,
	626, 418, 1,
	549, 308, 1,
} ;
/*
// 2005.09.14, 3��° ����
373	619	1	---->	373	618
365	387	1			
453	456	1			
547	436	1			
626	419	1	---->	627	418
548	308	1	---->	549	308


// 2005.10.04, 4��° ����
3����¡��
   ������ǥ(655,618)
   ������ǥ(454,609)
   ������ǥ(646,761)
   ������ǥ(527,770)
   ������ǥ(398,763)
   ������ǥ(373,619)
   ������ǥ(452,460)
   ������ǥ(365,386)
   ������ǥ(547,437)
   ������ǥ(626,419)
   ������ǥ(549.308)
*/


TNSiege::TNSiege() : m_iStarted(0), m_iWinner(0), m_iDateToHold(0), m_iExpiryOftheTerm(0)
{
	Init() ;
}



TNSiege::~TNSiege()
{
	
}


void TNSiege::Init()
{
	memset( m_krgEntry, 0, sizeof(m_krgEntry) ) ;
	memset( m_irgSymbol, 0, sizeof(m_irgSymbol) ) ;
	memset( &m_kOwner, 0, sizeof(m_kOwner) ) ;
	//memset( m_sgrgLeader, 0, sizeof(m_sgrgLeader) ) ;
}


// entry
//int TNSiege::BuildEntry()
//{
//
//	return eTNRes_Succeeded ;
//}
// ������ �ʱ�ȭ, ������ ����, ��¡�� clan�� ������
// entry �ʱ�ȭ
void TNSiege::InitEntry()
{
	memset( m_krgEntry, 0, sizeof(m_krgEntry) ) ;
	strcpy( m_krgEntry[eSide_Defense][0].szName, g_pMessageStringTable[_DefenderName] ); // �����̶�� �̸� ���	
}


void TNSiege::RegisterCastleOwner()
{
	m_krgEntry[eSide_Defense][0] = m_kOwner.kGuild ;
	int iAlly = m_kOwner.iOwnerFriend ;
	if( 0 > iAlly || MAX_USER <= iAlly ) iAlly = 0 ;
	RegisterEntry( iAlly, 0, 1, 1 ) ;
}


void TNSiege::set_Started( int a_iFlag ) //{ m_iStarted = a_iFlag ; } // 1�̸� ������ ����, 0�̸� ������ ���� ����
{
	//SYSTEMTIME st ;
	//GetLocalTime( &st ) ;

	RegisterCastleOwner() ;
	//m_krgEntry[eSide_Defense][0] = m_kOwner.kGuild ;
	//strcpy( m_sgrgLeader[eSiege_Army], g_pMessageStringTable[_Independent] ) ;

	m_iStarted = a_iFlag ;
	if( m_iStarted )
	{
		for( int i = 1 ; i < eSiege_Army ; ++i )
		{		
			int iGuildIndex = GetGuild( m_krgEntry[i][0].iID, FLAG_OPEN ) ;
			if( 0 > iGuildIndex || MAX_USER <= iGuildIndex )
			{
				#ifdef __TN_TOP_LOG__
				{
					char chBuf[512] = { 0,0,0, } ;
					sprintf(chBuf, "[%dMM%dDD %2dH%2dM%2dS] set_Started() > Invalid Guild Index(%d) \r\n"
						, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond, iGuildIndex
						) ;
					WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
				}
				#endif 
				continue ;
			}

			//strcpy( m_sgrgLeader[i], pGuild[iGuildIndex].GUILD.GuildName ) ;
		}
	}
}


// ������ ��û�� ���� ������ ���� ���̴�. �̰��� �޽����� �޾��� ��, ó���ϴ� ������� �����Ѵ�.
// �ϳ��� guild�� ��û�� �ϸ�, ���� ��嵵 ã�Ƽ� �ڵ����� ����� ������� �Ѵ�.
// �̹� ��ϵǾ� �ִٸ�, fail ������� �Ѵ�.
int TNSiege::RegisterEntry( int a_iGuildID, int a_iClanSlot, int a_iExpandSlot, int a_iPC )
{
	if( 1 == m_iExpiryOftheTerm ) return eTNRes_ExpiryOftheTerm ; // ��û�Ⱓ�� ����Ǿ���.
	if( 0 > a_iClanSlot || eSiege_Army <= a_iClanSlot ) return eTNRes_Failed ;
	if( 0 > a_iExpandSlot || eSiege_MaxEntry <= a_iExpandSlot ) return eTNRes_Failed ;
	if( 0 == a_iClanSlot && 0 == a_iExpandSlot ) return eTNRes_Failed; // ���� ���ַδ� ��Ͻ�û�� �� ����.
	if( 0 >= a_iGuildID ) return eTNRes_InvalidGuild ;
	if( 0 >= a_iPC || MAX_USER <= a_iPC ) return eTNRes_Failed ;
	if( 0 < m_krgEntry[a_iClanSlot][a_iExpandSlot].iID ) return eTNRes_NotEmpty; // �̹� slot�� ��ϵǾ� �ִ�.

	if( 0 < a_iExpandSlot ) // assist�� ��� ��û�� �Ѵٸ�, ���ְ� �̹� ��ϵǾ� �ִ��� Ȯ���ؾ��Ѵ�.
	{
		if( 0 == m_krgEntry[a_iClanSlot][0].iID )
		{// �޽��� ���, _NoClanLeader
			SendClientMessage( a_iPC, g_pMessageStringTable[_NoClanLeader] ) ;
			return eTNRes_Failed;
		}
	}

	int iRes = SearchEntry( a_iGuildID ) ; // entry���� �ش� ��尡 �̹� ��ϵǾ� �ִ��� ã�´�.
	if( -1 < iRes ) return eTNRes_AlreadyRegisteredInSiegeEntry ; // �̹� ��ϵǾ� �ִ�.

	int iGuildIndex = GetGuild( a_iGuildID, FLAG_OPEN ) ; // guild ������ �˾ƿ´�.
	if( 0 > iGuildIndex || MAX_USER <= iGuildIndex )
	{
		return eTNRes_InvalidGuild ;
	}

	m_krgEntry[a_iClanSlot][a_iExpandSlot].iID = a_iGuildID ;
	strcpy( m_krgEntry[a_iClanSlot][a_iExpandSlot].szName, pGuild[iGuildIndex].GUILD.GuildName ) ;
	m_krgEntry[a_iClanSlot][a_iExpandSlot].dwMark = pGuild[iGuildIndex].GUILD.Mark;

	#ifdef __TN_TOP_LOG__
	{
		//SYSTEMTIME st ;
		//GetLocalTime( &st ) ;
		char chBuf[512] = { 0,0,0, } ;
		sprintf(chBuf, "[%dMM%dDD %2dH%2dM%2dS] RegisterEntry() > %s applies to the siege. guild:%d, ClanSlot:%d, ExpandSlot:%d, Name:%s, Mark:%d \r\n"
			, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond
			, pMob[a_iPC].MOB.szName
			, a_iGuildID, a_iClanSlot, a_iExpandSlot
			, m_krgEntry[a_iClanSlot][a_iExpandSlot].szName
			, m_krgEntry[a_iClanSlot][a_iExpandSlot].dwMark
			) ;
		WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
	}
	#endif  

	if( 0 == a_iExpandSlot ) // �����̶��, ...
	{// ���� ��û�� �ߴٰ� ������ ����� �Ѵ�.
		int iTxtIndex = _Post1stAllyForSiege ;
		if( 1 == a_iClanSlot ) iTxtIndex = _Post1stAllyForSiege ;
		else if( 2 == a_iClanSlot ) iTxtIndex = _Post2ndAllyForSiege ;
		else if( 3 == a_iClanSlot ) iTxtIndex = _Post3rdAllyForSiege ;

		char szNotify[1024] = { 0,0,0, } ;
		sprintf( szNotify, g_pMessageStringTable[iTxtIndex], m_krgEntry[a_iClanSlot][a_iExpandSlot].szName ) ;
		PostMessageToWorld( szNotify, eTNClr_White, eTNClr_BG, iTxtIndex ) ;
	}

	SaveData() ; // ��� ��û�� ���� ����Ѵ�.
	return eTNRes_Succeeded ;
}


//@Return
//	-1 : Failed, 19~23 : entry�� ��� �ִ�.
int TNSiege::SearchEntry( int a_iGuildID )
{
	if( 0 >= a_iGuildID ) return -1;// �Էµ� guild ID�� �߸��Ǿ� ���� ���

	for( int iClanSlot = 0 ; iClanSlot < eSiege_Army ; ++iClanSlot )
	{
		for( int iExpandSlot = 0 ; iExpandSlot < eSiege_MaxEntry ; ++iExpandSlot )
		{
			if( a_iGuildID == m_krgEntry[iClanSlot][iExpandSlot].iID ) return (iClanSlot + eTNClan_CastleOwner) ;
		}
	}

	return -1 ;
}


// entry���� ����� ����Ѵ�. ���� Ż��
int TNSiege::GiveUpSiege( int a_iGuildID )
{
	for( int iClanSlot = 0 ; iClanSlot < eSiege_Army ; ++iClanSlot )
	{
		for( int iExpandSlot = 0 ; iExpandSlot < eSiege_MaxEntry ; ++iExpandSlot )
		{
			if( a_iGuildID == m_krgEntry[iClanSlot][iExpandSlot].iID ) return eTNRes_Succeeded;
		}
	}

	return eTNRes_Failed ;
}



// entry���� ����� ����Ѵ�. ���� Ż��
int TNSiege::RemoveEntry( int a_iGuildID )
{
	for( int iClanSlot = 0 ; iClanSlot < eSiege_Army ; ++iClanSlot )
	{
		for( int iExpandSlot = 0 ; iExpandSlot < eSiege_MaxEntry ; ++iExpandSlot )
		{
			if( a_iGuildID == m_krgEntry[iClanSlot][iExpandSlot].iID )
			{
				memset( &m_krgEntry[iClanSlot][iExpandSlot], 0, sizeof(m_krgEntry[iClanSlot][iExpandSlot]) );
				return eTNRes_Succeeded;
			}
		}
	}

	return eTNRes_Failed ;
}


// �������� ���Ǵ� ��¡������ ��ġ�Ѵ�.
void TNSiege::InstallSymbols()
{
	#ifdef __TN_TOP_LOG__
	{
		//SYSTEMTIME st ;
		//GetLocalTime( &st ) ;
		char chBuf[512] = { 0,0,0, } ;
		sprintf(chBuf, "[%dMM%dDD %2dH%2dM%2dS] InstallSymbols() > \r\n"
			, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond
			) ;
		WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
	}
	#endif  

	for( int i = 0 ; i < eSiege_SymbolCount ; ++ i )
	{
		m_irgSymbol[i][0] = pMob[1000].Summon( eSmblTrb_CastleOwner, 1, eTNPrdt_PopRaise, eTNCls_Warrior, 0, eTNClan_CastleOwner, 0, g_srgSymbolForSiege[i][0],  g_srgSymbolForSiege[i][1], 0, false, 0, MAX_USER+i, 710 ) ;
		m_irgSymbol[i][1] = eTNClan_CastleOwner ;

		#ifdef __TN_TOP_LOG__
		{
			char chBuf[512] = { 0,0,0, } ;
			sprintf(chBuf, "\tHndl:%d, Clan:%d, (%d,%d)\r\n"
				, m_irgSymbol[i][0], m_irgSymbol[i][1], g_srgSymbolForSiege[i][0],  g_srgSymbolForSiege[i][1]
				) ;
			WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
		}
		#endif  
	}
}

void TNSiege::DestroySymbols()
{
	for( int i = 0 ; i < eSiege_SymbolCount ; ++ i )
	{
		KillMonster( m_irgSymbol[i][0] ) ;
		m_irgSymbol[i][1] = 0 ;
	}
}


// 32�� �ֱ⸶�� symbol���� �˻��Ѵ�.
void TNSiege::CheckSymbols()
{
	if( !m_iStarted ) return ;

	for( int i = 0 ; i < eSiege_SymbolCount ; ++ i )
	{
		int iMob = m_irgSymbol[i][0] ;
		if( MAX_USER > iMob || MAX_MOB <= iMob )
		{
			// ���� �α׸� ���ܾ� �Ѵ�. �ɰ��� ���̴�.
			continue ;
		}

		if( pMob[iMob].IsDead() )
		{
			if( pMob[iMob].IsWaitToPop() ) continue ;
			// �׾� �ְ� ��� �ð��� �Ϸ�ƴ�. ���� pop ������� �Ѵ�.
			int iTribe = eSmblTrb_CastleOwner ;
			switch( m_irgSymbol[i][1] )
			{
			case eTNClan_Siege1 :
				iTribe = eSmblTrb_Symbol1 ;
				break ;
			case eTNClan_Siege2 :
				iTribe = eSmblTrb_Symbol2 ;
				break ;
			case eTNClan_Siege3 :
				iTribe = eSmblTrb_Symbol3 ;
				break ;
			case eTNClan_Siege4 :
				iTribe = eSmblTrb_Symbol4 ;
				break ;
			default :
				iTribe = eSmblTrb_CastleOwner ;
				break ; 
			}

			int iSymbol = pMob[1000].Summon( iTribe, 1, eTNPrdt_PopRaise, eTNCls_Warrior, 0, m_irgSymbol[i][1], 0, g_srgSymbolForSiege[i][0],  g_srgSymbolForSiege[i][1], 0, false, 0, iMob, 730 ) ;

			#ifdef __TN_TOP_LOG__
			{
				//SYSTEMTIME st ;
				//GetLocalTime( &st ) ;
				char chBuf[512] = { 0,0,0, } ;
				sprintf(chBuf, "[%dMM%dDD %2dH%2dM%2dS] CheckSymbols() > %dth symbol(%d) is poped at (%d,%d). Tribe:%d, Clan:%d, (symbol:%d) \r\n"
					, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond
					, i, iMob, pMob[iMob].TargetX, pMob[iMob].TargetY
					, iTribe, pMob[iMob].m_byClan
					, iSymbol
					) ;
				WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
			}
			#endif 
		}
	}
}



// symbol�� onkilled()�ÿ� ����� event�� ���ؼ� call �� ���̴�.
// �̰Ϳ� ���ؼ� clan�� ��ϵǰ�, �������� check�ÿ� Ȯ�εǾ ���õ� clan�� symbol�� pop�ϰ� �� ���̴�.
int TNSiege::CaptureSymbol( int a_iClan, int a_iSymbol, int a_iCapturer/*npc�ϼ��� �ִ�.*/ )
{
	if( eTNClan_CastleOwner > a_iClan || eTNClan_Siege4 < a_iClan ) return eTNRes_Failed ;
	if( MAX_USER > a_iSymbol || MAX_MOB <= a_iSymbol ) return eTNRes_InvalidHandle ;
	if( 0 >= a_iCapturer || MAX_MOB <= a_iCapturer ) return eTNRes_InvalidHandle ;

	//SYSTEMTIME st ;
	//GetLocalTime( &st ) ;

	for( int i = 0 ; i < eSiege_SymbolCount ; ++i )
	{
		if( a_iSymbol == m_irgSymbol[i][0] )
		{
			m_irgSymbol[i][1] = a_iClan ;

			#ifdef __TN_TOP_LOG__
			{
				char chBuf[512] = { 0,0,0, } ;
				sprintf(chBuf, "[%dMM%dDD %2dH%2dM%2dS] CaptureSymbol() > %dth symbol(%d, L(%d,%d)) is captured by Mob(%d/%s). capture's clan:%d \r\n"
					, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond
					, i, m_irgSymbol[i][0]
					, pMob[m_irgSymbol[i][0]].TargetX, pMob[m_irgSymbol[i][0]].TargetY
					, a_iCapturer, pMob[a_iCapturer].MOB.szName
					, m_irgSymbol[i][1]
					) ;
				WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
			}
			#endif 

			int iTxtIndex = _PostTheStateOfCapturingTheSymbol1 ;
			if( 3 == g_srgSymbolForSiege[i][2] ) iTxtIndex = _PostTheStateOfCapturingTheSymbol3 ;
			else if( 2 == g_srgSymbolForSiege[i][2] ) iTxtIndex = _PostTheStateOfCapturingTheSymbol2 ;

			char* szGuildName = NULL ;
			//int iClanLeader = 0 ;
			if( eTNClan_CastleOwner == a_iClan ) szGuildName = (char*)m_krgEntry[0][0].szName ; //iClanLeader = 1 ;
			else if( eTNClan_Siege1 == a_iClan ) szGuildName = (char*)m_krgEntry[1][0].szName ; //iClanLeader = 1 ;
			else if( eTNClan_Siege2 == a_iClan ) szGuildName = (char*)m_krgEntry[2][0].szName ; //iClanLeader = 2 ;
			else if( eTNClan_Siege3 == a_iClan ) szGuildName = (char*)m_krgEntry[3][0].szName ; //iClanLeader = 3 ;
			else if( eTNClan_Siege4 == a_iClan ) szGuildName = (char*)g_pMessageStringTable[_Independent] ; //iClanLeader = 4 ;

			char szMsg[1024] = { 0,0,0, } ;
			sprintf( szMsg, g_pMessageStringTable[iTxtIndex], szGuildName, g_srgSymbolForSiege[i][0], g_srgSymbolForSiege[i][1] ) ;
			PostMessageToZone( szMsg ) ;

			return eTNRes_Succeeded ;
		}
	}

	// �α׸� ���ܾ� �Ѵ�.
	// a_iSymbol�� ã�� �� ���ٴ� ���̴�.

	return eTNRes_Failed ;
}


// symbol ������ ������ �ջ��Ͽ� ������ ���� ���ڸ� �����Ѵ�.
void TNSiege::JudgeSiege()
{
	int irgClan[eTNClan_Reserved] = { 0,0,0, } ;
	//SYSTEMTIME st ;
	//GetLocalTime( &st ) ;
	#ifdef __TN_TOP_LOG__
	{
		char chBuf[512] = { 0,0,0, } ;
		sprintf(chBuf, "[%dMM%dDD %2dH%2dM%2dS] JudgeSiege() > \r\n"
			, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond
			) ;
		WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
	}
	#endif 

	for( int i = 0 ; i < eSiege_SymbolCount ; ++i )
	{
		int iClan = m_irgSymbol[i][1] ;
		if( eTNClan_CastleOwner > iClan || eTNClan_Reserved <= iClan )
		{
			// �α׸� �����.
			continue ;
		}

		// ���� ��� �� ���� ȹ���ߴ��� �α׸� �����.
		irgClan[iClan] += g_srgSymbolForSiege[i][2] ;

		#ifdef __TN_TOP_LOG__
		{
			char chBuf[512] = { 0,0,0, } ;
			sprintf(chBuf, "\tclan %d: %d\r\n", iClan, irgClan[iClan] ) ;
			WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
		}
		#endif 
	}

	// �� symbol�� ������, �� clan�� �� �հ�
	m_iWinner = eTNClan_CastleOwner ;
	int iHighestPoint = 0 ;
	for( int iClan = eTNClan_CastleOwner ; iClan <= eTNClan_Siege4 ; ++iClan )
	{
		if( iHighestPoint < irgClan[iClan] )
		{
			iHighestPoint = irgClan[iClan] ;
			m_iWinner = iClan ;
		}
		// �����ڰ� ������ ��쿡 ���� �߰� ó���� �ʿ��ϴ�.
	}

	char szMsg[1024] = { 0,0,0, } ;	
	sprintf( szMsg, g_pMessageStringTable[_PostTheResultOfTheSiege], irgClan[eTNClan_CastleOwner], irgClan[eTNClan_Siege1], irgClan[eTNClan_Siege2], irgClan[eTNClan_Siege3], irgClan[eTNClan_Siege4] ) ;
	PostMessageToWorld( szMsg ) ;

	#ifdef __TN_TOP_LOG__
	{
		char chBuf[512] = { 0,0,0, } ;
		sprintf(chBuf, "\r\nWinner > clan:(%d,%d), CastleOwner:%d, Siege1:%d, Siege2:%d, Siege3:%d, Siege4:%d  \r\n"
			, m_iWinner, iHighestPoint
			, irgClan[eTNClan_CastleOwner], irgClan[eTNClan_Siege1], irgClan[eTNClan_Siege2], irgClan[eTNClan_Siege3], irgClan[eTNClan_Siege4]
			) ;
		WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
	}
	#endif 

	char szPreviousOwner[GUILDNAME_LENGTH] = { 0,0,0, } ;
	strcpy( szPreviousOwner, m_kOwner.kGuild.szName ) ;

	m_iDateToHold = eDate_NotSelected ;
	int iWinnerGuild = m_kOwner.kGuild.iID ;
	if( eTNClan_Siege4 == m_iWinner ) // ������ ���� ������ �̱� ���, ���� ������ ���·� �ȴ�.
	{
		iWinnerGuild = 0;
		m_iDateToHold = eDate_Saturday10HH ;
		PostMessageToWorld( g_pMessageStringTable[_PostTheCastleIsFree] ) ;
	}
	else if( eTNClan_CastleOwner == m_iWinner )
	{
		char szMsg[1024] = { 0,0,0, } ;	
		sprintf( szMsg, g_pMessageStringTable[_PostSuccessToDefense], szPreviousOwner ) ;
		PostMessageToWorld( szMsg ) ;
	}
	else
	{
		if( eTNClan_Siege1 == m_iWinner ) iWinnerGuild = m_krgEntry[1][0].iID ;
		else if( eTNClan_Siege2 == m_iWinner ) iWinnerGuild = m_krgEntry[2][0].iID ;
		else if( eTNClan_Siege3 == m_iWinner ) iWinnerGuild = m_krgEntry[3][0].iID ;

		int iGuildIndex = GetGuild( iWinnerGuild, FLAG_OPEN ) ;
		if( 0 < iGuildIndex || MAX_USER > iGuildIndex )
		{
			char szMsg[1024] = { 0,0,0, } ;	
			sprintf( szMsg, g_pMessageStringTable[_PostTheWinnerOfTheSiege], szPreviousOwner, pGuild[iGuildIndex].GUILD.GuildName ) ;
			PostMessageToWorld( szMsg ) ;			
		}
		else
		{
			// �α׸� �����.
			#ifdef __TN_TOP_LOG__
			{
				char chBuf[512] = { 0,0,0, } ;
				sprintf(chBuf, "\r\nERROR >> �������� �̰�µ�, ������ ���ID(%d)�� invalid�ϴ�.\r\n", iWinnerGuild ) ;
				WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
			}
			#endif 
			iWinnerGuild = 0 ;
			m_iDateToHold = eDate_Saturday10HH ;
			PostMessageToWorld( g_pMessageStringTable[_PostTheCastleIsFree] ) ;
		}
	}

	ChangeOwner( iWinnerGuild ) ;
	
	InitEntry() ;// m_kOwner�� �ʱ�ȭ���� �ʰ� entry ���� �ʱ�ȭ�Ѵ�.
	RegisterCastleOwner();// m_kOwner ������ �̿��Ͽ� �������� ������ ���ش�.
	SaveData() ; // ����� entry�� �����Ѵ�.
}


int TNSiege::ChangeOwner( int a_iGuildID )
{
	//SYSTEMTIME st ;
	//GetLocalTime( &st ) ;
	CTime kTime( g_kSystemTime ) ;

	memset( &m_kOwner, 0, sizeof(m_kOwner) ) ;

	m_kOwner.kGuild.iID = a_iGuildID ;
	m_kOwner.kTimeOccupied = kTime ;
	g_iCastleOwner = a_iGuildID;

	#ifdef __TN_TOP_LOG__
	{
		char chBuf[512] = { 0,0,0, } ;
		sprintf(chBuf, "\r\n\r\n[UpdateCastle] [%dYY%dMM%dDD %2dH%2dM%2dS] > The castle is occupied by a guild(%d) at %dYY%dMM%dDD %dHH%dMI \r\n"
			, g_kSystemTime.wYear, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond
			, m_kOwner.kGuild.iID
			, m_kOwner.kTimeOccupied.GetYear(), m_kOwner.kTimeOccupied.GetMonth()
			, m_kOwner.kTimeOccupied.GetDay(), m_kOwner.kTimeOccupied.GetHour()
			, m_kOwner.kTimeOccupied.GetMinute()
			) ; 
		WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
	}
	#endif

	int iGuildIndex = GetGuild( a_iGuildID, FLAG_OPEN ) ;
	if( 0 > iGuildIndex || MAX_USER <= iGuildIndex )
	{
		return eTNRes_Failed ;
	}

	m_kOwner.kGuild.dwMark = pGuild[iGuildIndex].GUILD.Mark; //�������� ��� ����
	strcpy( m_kOwner.kGuild.szName, pGuild[iGuildIndex].GUILD.GuildName ) ;// �������� ������ ���
	m_kOwner.iOwnerFriend = GetGuildID( pGuild[iGuildIndex].GUILD.AlliedGuildName1 ) ; // �������� ���ձ�� ID Ȯ��
	

	//strcpy( m_sgrgLeader[0], m_kOwner.szGuildName ) ;
	//strcpy( m_sgrgLeader[4], g_pMessageStringTable[_Independent] ) ;

	{
		char chBuf[512] = { 0,0,0, } ;
		sprintf(chBuf, "\t Information about Guild > ID:%d, Name:%s, Mark:%d, Ally:%d \r\n\r\n"
			, m_kOwner.kGuild.iID, m_kOwner.kGuild.szName, m_kOwner.kGuild.dwMark, m_kOwner.iOwnerFriend
			) ; 
		WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
	}

	return eTNRes_Succeeded ;
}


void TNSiege::SelectDate( int a_iDateNum, bool a_bCheck )
{
	if( 0 >= a_iDateNum || 5 < a_iDateNum ) return ;
	if( a_bCheck && (0 < m_iDateToHold) ) return ; //  1ȸ ������ �ϸ� ������ �� ����.

	m_iDateToHold = a_iDateNum ;
	SaveData() ;

	//SYSTEMTIME st ;
	//GetLocalTime( &st ) ;
	#ifdef __TN_TOP_LOG__
	{
		char chBuf[512] = { 0,0,0, } ;
		sprintf(chBuf, "\r\n\r\n[SelectDate] [%dYY%dMM%dDD %2dHH%2dMI%2dSS] > Date:%d(1:friday 8hh, 2:friday 10hh, 3:saturday 8hh, 4:saturday 10hh, 5:sunday 8hh)\r\n"
			, g_kSystemTime.wYear, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond
			, m_iDateToHold
			) ; 
		WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
	}
	#endif

	TriggerEvent( 0, 98/*���� ��¥ ���� �̺�Ʈ*/, 0, 0, 0, 1231 ) ;
}


int TNSiege::LoadData()
{
	if( eZone_Castle != g_iZoneID ) return eTNRes_Failed ;

	char szData[64] = { 0,0,0, } ;
	char szCommand[32] = { 0,0,0, } ;
    char szArgument1[32] = { 0,0,0, } ;
    char szArgument2[32] = { 0,0,0, } ;
	char szArgument3[32] = { 0,0,0, } ;
	char szArgument4[32] = { 0,0,0, } ;
	char szArgument5[32] = { 0,0,0, } ;
	char szArgument6[32] = { 0,0,0, } ;
	char szArgument7[32] = { 0,0,0, } ;
    int iArgument1 = 0, iArgument2 = 0, iArgument3 = 0, iArgument4 = 0, iArgument5 = 0 ;
	
	FILE* fin ;
	fin = fopen( ".\\Data\\Castle.txt", "rt" ) ;
	if( NULL == fin ) 
	{ 
		MessageBox( hWndMain, "Not found Castle.txt in data folder", "Failed to initialize data about the castle.", NULL ) ; 
		return eTNRes_Failed ; 
	}

	//SYSTEMTIME st ;
	//GetLocalTime( &st ) ;
	int iYear, iMonth, iDay, iHour, iMinute ;
	bool bDateTimePatched = true ;
	bool bLoaded = false ;

	while( true )
	{
		char* ret = fgets( (char*)szData, 64, fin ) ;
		if( NULL == ret ) break ;

		iYear = iMonth = iDay = iHour = iMinute = 0 ;
		sscanf( szData, "%s %s %s %s %s %s %s %s", szCommand, szArgument1, szArgument2, szArgument3, szArgument4, szArgument5, szArgument6, szArgument7 ) ;
	
		switch( szData[1] )
		{
		case 'C' : // owner of the castle
			{
				iArgument1 = atoi( szArgument1 ) ; // owner #
				iYear = atoi( szArgument2 ) ; // date occupied
				iMonth = atoi( szArgument3 ) ;
				iDay = atoi( szArgument4 ) ;
				iHour = atoi( szArgument5 ) ;
				iMinute = atoi( szArgument6 ) ;

				bLoaded = true ;
				m_kOwner.kGuild.iID = iArgument1 ;
				g_iCastleOwner = iArgument1 ;

				if( (0 == iYear) && (0 == iMonth) && (0 == iDay) )
				{
					bDateTimePatched = false ;
					CTime kTime( g_kSystemTime ) ;
					m_kOwner.kTimeOccupied = kTime ;
				}
				else
				{
					CTime kTime( iYear, iMonth, iDay, iHour, iMinute, 0 ) ;
					m_kOwner.kTimeOccupied = kTime ;
				}

				{
					char chBuf[512] = { 0,0,0, } ;
					sprintf(chBuf, "\r\n\r\n[LoadCastleData] [%dYY%dMM%dDD %2dH%2dM%2dS] > %The castle is occupied by a guild(%d) in %d-%d-%d-%d-%d\r\n"
						, g_kSystemTime.wYear, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond
						, iArgument1
						, iYear, iMonth, iDay, iHour, iMinute
						) ; 
					WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
				}

				CGuild guild;
				ReadGuildFile(m_kOwner.kGuild.iID, &guild);
				strncpy( m_kOwner.kGuild.szName, guild.GUILD.GuildName, SZGUILD_LENGTH);
				m_kOwner.iOwnerFriend = GetGuildID(guild.GUILD.AlliedGuildName1);
				m_kOwner.kGuild.dwMark = guild.GUILD.Mark;

				m_krgEntry[0][0] = m_kOwner.kGuild ;

				//strcpy( m_sgrgLeader[0], m_kOwner.szGuildName ) ;
				//strcpy( m_sgrgLeader[4], g_pMessageStringTable[_Independent] ) ;
			}
			break ;
		case 'D' : //date
			{
				iArgument1 = atoi( szArgument1 ) ; // #Date to hold the siege.
				m_iDateToHold = iArgument1 ; // ���ֵ� ��¥
				if( eDate_NotSelected > m_iDateToHold || eDate_Sunday8HH < m_iDateToHold ) m_iDateToHold = eDate_NotSelected ;
			}
			break ;
		case 'E' : // entry
			{
				iArgument1 = atoi( szArgument1 ) ; // level
				iArgument2 = atoi( szArgument2 ) ; // 
				iArgument3 = atoi( szArgument3 ) ; // 
				iArgument4 = atoi( szArgument4 ) ;
				iArgument5 = atoi( szArgument5 ) ;

				m_krgEntry[0][iArgument1].iID = iArgument2 ;
				m_krgEntry[1][iArgument1].iID = iArgument3 ;
				m_krgEntry[2][iArgument1].iID = iArgument4 ;
				m_krgEntry[3][iArgument1].iID = iArgument5 ;

			}
			break ;
		}
	}

	fclose( fin ) ;

	RefreshEntry();
	//for( int iClanSlot = 0 ; iClanSlot < eSiege_Army ; ++iClanSlot )
	//{
	//	for( int iExpandSlot = 0 ; iExpandSlot < eSiege_MaxEntry ; ++iExpandSlot )
	//	{
	//		if( 0 >= m_krgEntry[iClanSlot][iExpandSlot].iID ) continue ;
	//		CGuild kGuild ;
	//		ReadGuildFile( m_krgEntry[iClanSlot][iExpandSlot].iID, &kGuild ) ;
	//		strncpy( m_krgEntry[iClanSlot][iExpandSlot].szName, kGuild.GUILD.GuildName, SZGUILD_LENGTH ) ;
	//		m_krgEntry[iClanSlot][iExpandSlot].dwMark = kGuild.GUILD.Mark ;
	//	}
	//}

	if( (false == bDateTimePatched) || (false == bLoaded) ) SaveData() ;

	return eTNRes_Succeeded ;
}


int TNSiege::SaveData( char* a_pFileName )
{
	HANDLE hFile ;
	//hFile = CreateFile( ".\\Data\\Castle.txt",					  // file name
	hFile = CreateFile( a_pFileName,					  // file name
                                GENERIC_WRITE,   // open for writing 
                                FILE_SHARE_WRITE,             // do not share 
                                NULL,                         // no security 
                                CREATE_ALWAYS,                  // open or create 
                                FILE_ATTRIBUTE_NORMAL,        // normal file 
                                NULL ) ;

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return eTNRes_Failed ;
	}

	DWORD dwRetWrite = 0 ;
	int nBufLength = 0 ;	
	char szData[256] = { 0,0,0, } ;

	memset( szData, 0, sizeof(szData) ) ;
	sprintf( szData, "/C %d %d %d %d %d %d\r\n/D %d\r\n"
		, m_kOwner.kGuild.iID
		, m_kOwner.kTimeOccupied.GetYear(), m_kOwner.kTimeOccupied.GetMonth()
		, m_kOwner.kTimeOccupied.GetDay(), m_kOwner.kTimeOccupied.GetHour()
		, m_kOwner.kTimeOccupied.GetMinute()
		, m_iDateToHold
		) ;
	nBufLength = strlen( szData ) ;
	WriteFile( hFile, szData, nBufLength, &dwRetWrite, NULL ) ;

	for( int i = 0 ; i < eSiege_MaxEntry ; ++i )
	{
		memset( szData, 0, sizeof(szData) ) ;
		sprintf( szData, "/E %2d: %4d %4d %4d %4d\r\n"
			, i, m_krgEntry[0][i].iID, m_krgEntry[1][i].iID, m_krgEntry[2][i].iID, m_krgEntry[3][i].iID
			) ;
		nBufLength = strlen( szData ) ;
		WriteFile( hFile, szData, nBufLength, &dwRetWrite, NULL ) ;
	}

	CloseHandle( hFile ) ;

	return eTNRes_Succeeded ;
}


void TNSiege::get_Symbols( int* a_irgSymbol )
{
	for( int i = 0 ; i < eSiege_SymbolCount ; ++i )
        a_irgSymbol[i] = m_irgSymbol[i][1] ;
}

void TNSiege::Print()
{
	#ifdef __TN_TOP_LOG__
	{
		//SYSTEMTIME st ;
		//GetLocalTime( &st ) ;
		char chBuf[512] = { 0,0,0, } ;
		sprintf(chBuf, "\r\n[%dMM%dDD %2dH%2dM%2dS] Print() > \r\n"
			, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond
			) ;
		WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
	}
	#endif  

	for( int i = 0 ; i < eSiege_SymbolCount ; ++i )
	{
		int iMob = m_irgSymbol[i][0] ;
		if( MAX_USER > iMob || MAX_MOB <= iMob ) continue ;
		#ifdef __TN_TOP_LOG__
		{
			char chBuf[512] = { 0,0,0, } ;
			sprintf(chBuf, "\t%d. Hndl:%d, Clan:%d, L(%d,%d), Mode:%d, FSM:%d\r\n"
				, i, m_irgSymbol[i][0], m_irgSymbol[i][1], pMob[iMob].TargetX, pMob[iMob].TargetY
				, pMob[iMob].Mode, pMob[iMob].m_eFSM
				) ;
			WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
		}
		#endif  
	}
}



int TNSiege::GetEntry( int a_iClanSlot, int a_iExpandSlot, TNGUILD_INFO& a_kGuild  )
{
	if( 0 > a_iClanSlot || eSiege_Army <= a_iClanSlot ) return eTNRes_Failed ;
	if( 0 > a_iExpandSlot || eSiege_MaxEntry <= a_iExpandSlot ) return eTNRes_Failed ;

	a_kGuild = m_krgEntry[a_iClanSlot][a_iExpandSlot] ;
/*
	int iGuildIndex = GetGuild( a_iGuildID, FLAG_OPEN ) ;
	if( 0 > iGuildIndex || MAX_USER <= iGuildIndex )
	{
		#ifdef __TN_TOP_LOG__
		{
			SYSTEMTIME st ;
			GetLocalTime( &st ) ;
			char chBuf[512] = { 0,0,0, } ;
			sprintf(chBuf, "[%dMM%dDD %2dH%2dM%2dS] GetEntry() > Invalid Guild Index(%d,ID:%d), ClanSlot:%d, ExpandSlot:%d \r\n"
				, g_kSystemTime.wMonth, g_kSystemTime.wDay, g_kSystemTime.wHour, g_kSystemTime.wMinute, g_kSystemTime.wSecond, iGuildIndex, a_iGuildID, a_iClanSlot, a_iExpandSlot
				) ;
			WriteLog( chBuf, g_szrgLogFileName[eLogFileName_Castle] ) ;
		}
		#endif 
		return eTNRes_InvalidGuild ;
	}

	strcpy( a_szGuildName, pGuild[iGuildIndex].GUILD.GuildName ) ;
	a_dwGuildMark = pGuild[iGuildIndex].GUILD.Mark;
*/
	return eTNRes_Succeeded ;
}


int TNSiege::FindFreeSlot( int a_iClanSlot )
{
	for( int i = 0 ; i < eSiege_MaxEntry ; i += 2 )
	{
		if( 0 == m_krgEntry[a_iClanSlot][i].iID ) return i ;
	}
	
	return -1 ;
}


void TNSiege::RefreshEntry()
{
	for( int iClanSlot = 0 ; iClanSlot < eSiege_Army ; ++iClanSlot )
	{
		for( int iExpandSlot = 0 ; iExpandSlot < eSiege_MaxEntry ; ++iExpandSlot )
		{
			if( 0 >= m_krgEntry[iClanSlot][iExpandSlot].iID ) continue ;
			CGuild kGuild ;
			ReadGuildFile( m_krgEntry[iClanSlot][iExpandSlot].iID, &kGuild ) ;
			strncpy( m_krgEntry[iClanSlot][iExpandSlot].szName, kGuild.GUILD.GuildName, SZGUILD_LENGTH ) ;
			m_krgEntry[iClanSlot][iExpandSlot].dwMark = kGuild.GUILD.Mark ;
		}
	}
}