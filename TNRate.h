/****************************************************************************************

	�ۼ��� : �����(spencerj@korea.com)
	�ۼ��� : 2003-09-09

	������ :
	������ :

	������Ʈ�� : 

	���� : 

****************************************************************************************/
#ifndef __TNRate_h__
#define __TNRate_h__


class TNRate
{
public :
	TNRate() ;
	~TNRate() ;
	
	void Init( int a_iRate, int a_iMaxRate=100 ) ;

//Public Operations
public :
	bool Random() ;
	inline int get_Rate() { return m_iRate ; }

private :
	void Rolling() ;

private :
	int m_iIndex ;	
	int m_iRate ;
	int m_iMaxRate ;
	short m_sRange ;
	char m_chOffset ;
	char m_chLastOffset ;
	short m_sSize ;
	char m_chrgFlag[13] ; // 13(=100/8)

	//int m_iSize ;
	//char* m_pFlags ;
};




#endif