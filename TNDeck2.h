/****************************************************************************************

	�ۼ��� : �����(spencerj@korea.com)
	�ۼ��� : 2003-09-09

	������ :
	������ :

	������Ʈ�� : 

	���� : TNDeck class�� size 100���θ� �����ؼ� static���� �������� ��.

****************************************************************************************/
#ifndef __TNDeck2_h__
#define __TNDeck2_h__



class TNDeck2
{
public :
	TNDeck2() ;
	~TNDeck2() ;
	
	void Init() ;

//Public Operations
public :
	char Random() ;
	void Shuffle() ;
	void AddCard( char a_chNum, int a_iCount ) ;
	void ClearCards() ;
	

private :
	int m_iIndex ;
	int m_iSize ;

	char m_chrgCard[100] ;
};




#endif