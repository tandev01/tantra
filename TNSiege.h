/****************************************************************************************

	�ۼ��� : �����(spencerj@korea.com)
	�ۼ��� : 2003-09-09

	������ :
	������ :

	������Ʈ�� : 

	���� : 

// ��¡��
     - static : (x,y), point
     - dynamic : owner, clan(or tribe)
// �������� �����ϸ�, ������ data�� �м��ؼ� entry table�� �����Ѵ�.
// entry table : ��� ID, clan
// entry size : 200 = (����*1 + ����*4) * 20(�� ��ġ�� �ִ� slot��) * 2(���հ���)
// ������ zone�� ������ �ϰ� �Ǹ� entry table���� �ڽ��� ��� ID�� �˻��ؼ� clan�� �����Ѵ�.(������ ����)
// �˻� ���, entry�� ������ ���Ҽ� clan�� ������ �ȴ�.
// �������� entry ��� ����
// ���� etnry table Ȯ��

// PC ���� ��, ���� ���¿��ο� ���� clan ������ ��������ش�.
  - �Ϲ� ������ ���� default�� ����Ѵ�.
  - ���� ������ ���� the entry of siege��  �˻��ؼ� clan�� ��������ش�.

// �������� �ܰ�?
  - ������ ���� ����

  - ������ ��û
	'��û UI'�� ���ؼ� ���� guild�� ��û ����. => client/server ���� ���� ��û �޽��� & ó�� routine
	'��û UI' ������ ��������? ���������� ���޵Ǵ� ��¥��? ���� �����۾��� �� �� �ִ� ��¥��?
	MSG_APPLY_SIEGE

  - ������ ��û ����

  - ������ �غ� �ܰ�
    - ���� ������ 30�� ���뿡 ��� PC�� save zone���� �̵���Ų��.(or Ư�� ��ġ�� �̵��� ��Ų��. Ư�� ��ġ�� ����� �о���ϴ� ������ �����ؾ��Ѵ�.) => event/task
	- ��ü zone�� safety property �߰�(�������� �ؼ��� ���� ������ �ȵǰ� �Ѵ�.) => event/task
	- ������ ���� ��� ��ü�� all immunity�� �߰����ش�.(�̰��� ��� field�� ���� �Ӽ��� �־��� ������ ���ص� �� ���ϴ�. Ȯ�� �ʿ�!) => event/task
	- �������ϴ� user�鿡�� zone settings �޽��� ����(�������� ���� bit flag)
	  // client targeting ����
	  MSG_SET_ZONE_SETTINGS

  - ������ ����
    - ��ü zone���� safety property ���� => event/task
	- ��¡�� ���� ���� ��, �̺�Ʈ�� call �ؼ� �ٸ� ��¡���� pop �ϰ� �Ѵ�.(��¡�� ������ ���� ������ �̿�, 10�� �̻��� delay�� �־�� �Ѵ�.) => monster AI & event/task
		// symbol�� ���� �Ŀ� �ٷ�(���) pop �ȴٸ� ����� ���ϴ�. �׷��� delay�� 10�� �̻� �־�� �Ѵ�. ���� �����ϱ� ���� ����� �����Ǿ�� �Ѵ�.
		// 35(=7*5) ���� event/task
		// 35���� event�� 1�� �Ŀ� �����ϱ� ���� event/task
		=> eTNAct_PopSymbolForSiege(21020)
	
  - ������ ����	
	- ��� ������ safety field�� �����ؼ� �� �̻� ������ ������� �ʰ� �Ѵ�. => event/task
	- ���� ���� => event/task
	- ���� �û� ���� ���� => event/task
	
  - ������ �û�    
    - ��¡�� ������ ���� ����� �����ؼ� ���ڸ� �����Ѵ�. => event/task, eTNAct_JudgeTheSiege(21010)
		�� clan�� ������ ��� ����� ���ְ� ������ ���ڸ� ����Ѵ�.
		����� �ý��� ���ο��� ó���Ѵ�.
    - ��� PC�� save zone���� �̵���Ų��.(�� owner���� ����� ��� �ٸ� PC�� save zone���� �̵���ų �� �ִ�.) => event/task
		�� owner���� ����� �������� �ű�� ���� routine�� �ʿ� => eTNAct_KickOutLosser(21000)
    - ���� safety field ���� but ��ƴ�.
	- zone�� ���� �ִ� ��� PC���� clan�� �Ϲ� clan���� �����Ѵ�. => eTNAct_RecoverClanToOriginal(21030)
	=> ������ � clan���� �����ϴ� generic routine�� �����.
	MSG_CHANGE_TRIMURITI

  - �������� ���� �޸�Ʈ �κ� ����
	????

  - ��Ȱ�� ���ؼ��� �����Ӵ԰� �۾�
	// ������ ���� ��Ȱ ��� ����ȭ, ���� ��ġ�� ���� Ư������?
	
=> ���� ������ �ִ� ��� PC���� clan�� ������ �������ִ� routine�� �־�� �Ѵ�. => ChangePCClanAll()

* ������ ���������� �̰ܼ� ���� ������ ���·� �Ǵ� �Ϳ� ���� �������� �׽����� �ʿ��ϴ�.

****************************************************************************************/
#ifndef __TNSiege_h__
#define __TNSiege_h__

// ������� ���� ����� �� �ִ�.

// if( eCountryKorea == g_eCountryID ) strncpy( kMsg.szMsg, g_pMessageStringTable[_KickedByGM], sizeof(kMsg.szMsg)) ;
// else


struct TNGUILD_INFO
{
	int iID ;
	DWORD dwMark ;
	char szName[SZGUILD_LENGTH] ;
} ;

struct TNCastle
{
	//int iOwner ; // guild id
	//char	szGuildName[SZNAME_LENGTH] ;
	//DWORD	dwMark;

	TNGUILD_INFO kGuild ;
	int		iOwnerFriend;
	CTime	kTimeOccupied ;
} ;




class TNSiege
{
public :
	TNSiege() ;
	~TNSiege() ;

	void Init() ;

/*
���� ��¡��	K-3	2435 
����1 ��¡��	K-3	2436 
����2 ��¡��	K-3	2437 
����3 ��¡��	K-3	2438 
���Ҽ� ��¡��	K-3	2439 
*/

	enum { eSiege_SymbolCount = 11, eSiege_MaxEntry = 40, eSiege_Army = 4, } ; // ������ ����
	enum { eSide_Defense = 0, } ;
	enum { eSmblTrb_CastleOwner = 2435 , eSmblTrb_Symbol1 = 2436, eSmblTrb_Symbol2 = 2437, eSmblTrb_Symbol3 = 2438, eSmblTrb_Symbol4 = 2439, } ;
	enum { eApplyFee_Defense = 1000000, eApplyFee_Siege1 = 12000000, eApplyFee_Siege2 = 10000000, eApplyFee_Siege3 = 8000000, eApplyFee_SiegeSupport = 1000000, } ;
	enum { eDate_NotSelected = 0, eDate_Friday8HH, eDate_Friday10HH, eDate_Saturday8HH, eDate_Saturday10HH, eDate_Sunday8HH, } ;

// Public Operations
public :
	int GetEntry( int a_iClanSlot, int a_iExpandSlot, TNGUILD_INFO& a_kGuild ) ;
	int FindFreeSlot( int a_iClanSlot ) ;
	int LoadData() ;
	//int BuildEntry() ;
	void InitEntry() ;
	int RegisterEntry( int a_iGuildID, int a_iClanSlot, int a_iExpandSlot, int a_iPC ) ;
	int SearchEntry( int a_iGuildID ) ; // #clan. if failed, return -1 ;
	int GiveUpSiege( int a_iGuildID );
	int RemoveEntry( int a_iGuildID );
	int ChangePCClanAll( int a_iClan ) ; // ���� �����ϰ� �ִ� ��� PC�� clan�� �������ش�. ���ڰ� 0�̸�, SearchEntry()�� �̿��ؼ� �ڽ��� clan�� Ȯ���Ѵ�. 0�� �ƴϸ�, ��� PC�� �� #clan���� ����, -1�̸� ���� clan���� ����
	void CheckSymbols() ;
	int CaptureSymbol( int a_iClan, int a_iSymbol, int a_iCapturer ) ; // � clan�� � symbol�� ���ѾҴ�.
	void JudgeSiege() ;
	void SelectDate( int a_iDateNum, bool a_bCheck=true ) ;
	void InstallSymbols() ; // symbol���� pop ��Ű�� handle�� �����Ѵ�.
	void DestroySymbols() ;
	void Print() ;
	int SaveData( char* a_pFileName =".\\Data\\Castle.txt" ) ;
	void RefreshEntry();

private :
	int ChangeOwner( int a_iGuildID ) ;
	
	

// Public Properties
public :
	inline int get_Started() { return m_iStarted ; }
	void set_Started( int a_iFlag ) ; //{ m_iStarted = a_iFlag ; } // 1�̸� ������ ����, 0�̸� ������ ���� ����
	inline int get_Winner() { return m_iWinner ; } 
	inline int get_DateToHold() { return m_iDateToHold ; }
	inline void set_ExpiryOftheTerm( int a_iFlag ) { m_iExpiryOftheTerm = a_iFlag ; } // 1�̸� �Ⱓ����(������û�Ұ�), 0�̸� �Ⱓ��(������û����)
	inline int get_ExpiryOftheTerm() { return m_iExpiryOftheTerm ; }
	void get_Symbols( int* a_irgSymbol ) ;
	void get_Owner( TNCastle* a_kOwner ) { (*a_kOwner) = m_kOwner ; } ;
	int get_OwnerGuild() { return m_kOwner.kGuild.iID; }
	void RegisterCastleOwner() ;
	
// Attributes
private :

	int m_iStarted ; // ������ ���� ����
	int m_iExpiryOftheTerm ; // ������û�Ⱓ ����
	int m_iWinner ; // clan ����, 19~23
	TNCastle m_kOwner ; //  ������ ����
	//int m_irgEntry[eSiege_Army][eSiege_MaxEntry] ; // owner, clan // �������� ���۵� ��, Ȯ���ȴ�.
	TNGUILD_INFO m_krgEntry[eSiege_Army][eSiege_MaxEntry] ;
	//char m_sgrgLeader[eSiege_Army+1][SZNAME_LENGTH] ;
	//int m_irgLeader[eSiege_Army] ; // �������� leader�� ���
	int m_irgSymbol[eSiege_SymbolCount][2] ; // handles of the symbol // handle�� �����ؾ��� ���� ������ ����� �� �ִ�.
	//int m_irgSymbolClan[eSiege_SymbolCount] ; // �� symbol���� clan, �̰��� ���� m_irgSymbol���� ���� �߿��� ����
	int m_iDateToHold ; // �������ϴ� �ð�, 0/1/2/3(0�� default�� �������� ���� ���̴�.)
};



extern TNSiege g_kSiege ;

#endif