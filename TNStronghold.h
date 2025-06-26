/****************************************************************************************

	작성자 : 정재웅(spencerj@korea.com)
	작성일 : 2003-09-09

	수정자 :
	수정일 :

	프로젝트명 : 

	설명 : 

****************************************************************************************/
#ifndef __TNStronghold_h__
#define __TNStronghold_h__


class TNStronghold
{
public :
	TNStronghold() ;
	~TNStronghold() ;
	

	void Init( int a_iMax ) ;
	enum { eDeck_MaxSize = 10000, };

//Public Operations
public :
	char Random() ;
	void Shuffle() ;
	void AddCard( char a_chCard, int a_iCount ) ;
	void ClearCards() ;
	

private :
	int m_iID ;
	int m_iOwner ; // guild ID
};




#endif