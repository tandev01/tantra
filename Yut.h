#pragma once

#include "BaseDef.h"
#include "EventDefines.h"
#include "TNDeck10000.h"

#include <map>
#include <deque>

class CYut
{
private:
	struct BETUSER
	{
		char	pName[SZNAME_LENGTH];	//	�����ɸ����̸�
		DWORD	dwCount;				//	���ð���
		DWORD	dwWinMoney;				//	���ݾ�
	};

	struct SAVEBETUSER
	{
		char	pName[SZNAME_LENGTH];	//	�����ɸ����̸�
		DWORD	dwCount;				//	������������
	};

	std::deque<BYTE> m_dequePostPosition;
	std::map<char*, WORD, HTStr> m_mapUserMoney;						//	������ �����ݾ�
	std::map<char*, std::map<WORD, WORD>, HTStr> m_mapTable;			//	�����Ǵ� �ɸ��Ϳ� �ش��ϴ� �ʹ迭�ε����� �����Ѵ�.
	
	TNDeck10000	m_clsYut;
	BETUSER	m_arrarroPosition[MAX_POSITION][MAX_USER];					//	�����Ǵ� �ɸ��ͺ��� �������
	DWORD	m_dwProgress;												//	�����Ȳ(basedef.h�� ���ǵǾ�����)
	DWORD	m_dwTotBetMoney;											//	��ü ���ñݾ�
	DWORD	m_dwSavedTotBetMoney;										//	��÷�ڰ� ��� �̿��Ǵ� ��ü ���ñݾ�
	DWORD	m_dwTotPositionBetMoney[MAX_POSITION];						//	��ü ��ġ�����ñݾ�
	BYTE	m_byResult;
	SAVEBETUSER	m_arrUserMoney[MAX_BETUSER];
	int		m_byYutPosition;
	DWORD	m_dwOwnerMoney;
public:
	CYut(void);
	~CYut(void);

	void	HT_Init();
	WORD	HT_wUserBet(char* pName, WORD wPosition, int iMoney);		//	�����Ǻ� �ɸ����� ���ð����� �ø���.


	bool	HT_bIsBetOn();
	bool	HT_bIsPlayOn();
	void	HT_SetBet(DWORD dwFlag);
	void	HT_SetPlay(DWORD dwFlag);

	void	HT_ResetMoney();
	void	HT_SaveMoney();
	void	HT_ReadSaveMoney();
	void	HT_ShareMoney();

	BYTE	HT_byYutPlay();
	BYTE	HT_byGetResult();
	BYTE	HT_byMoveYut();
	BYTE	HT_byGetPosition();
	BYTE	HT_byGetPrePosition();
	HS2D_COORD	m_uniPosition[MAX_POSITION];
	void	HT_GetPositionBetMoney(DWORD* pdwMoney);
	void	HT_GetPositionBetMoney(char* pName, DWORD* pdwMoney);
	void	HT_WriteLog();
	void	HT_WriteLog(char* pName);
	DWORD	HT_GetMoney(char* pName);
	void	HT_SetYutMoney(char* pName, DWORD dwMoney);
	DWORD	GetOwnerMoney();
	void	SetOwnerMoney(DWORD dwMoney);

	DWORD	m_dwStep;
};
