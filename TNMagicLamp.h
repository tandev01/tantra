/****************************************************************************************

	�ۼ��� : �����(spencerj@korea.com)
	�ۼ��� : 2005-12-27

	������ :
	������ :

	������Ʈ�� : 

	���� : 

****************************************************************************************/
#ifndef __TNMagicLamp_h__
#define __TNMagicLamp_h__


#include "TNDeck1000.h"


class TNMagicLamp
{
public :
	TNMagicLamp();
	~TNMagicLamp();

	void Init();
	

//Public Operations
public :
	int DrawMonsterCard();
	void AddMonsterCard( int a_iIndex, int a_iMonsterID, int a_iRating );
	void ShuffleMonsterCard();

	int LoadData( char* a_pFileName );

// Attributes
private :
	enum { eML_CountOfMonsterToBeSummon = 13, };

	TNDeck1000 m_kRating;
	int m_irgMonsterToBeSummon[eML_CountOfMonsterToBeSummon];


};

extern TNMagicLamp g_krgMagicLamp[5];


#endif