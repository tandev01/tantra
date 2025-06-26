/****************************************************************************************

	���ϸ� : TNScheduler.h
	�ۼ��� : �����(spencerj@korea.com)
	�ۼ��� : 2004-10-19

	������ :
	������ :

	������Ʈ�� : 

	���� : 
	
	���ϻ��� : ��ϵǾ� �ִ� schedule�� 1�ð� �ֱ�� �ݺ������� �����Ѵ�.(cycle ����)
	           �ӽ������� ��ϵǾ� ����Ǵ� event���� 1ȸ�� ������ �� �ʿ䰡 �ִ�.
			   �̷� ���࿡ �־�� ����ġ�� ���� ���� ���ۿ� ������ �ִ�.

****************************************************************************************/
#ifndef __TNScheduler_h__
#define __TNScheduler_h__

struct TNSchedule
{
	unsigned int uiTime ; // 0�̸� disable �����̴�.
	unsigned int uiFreq ; // uiTerm�� 0�̸� ���� 1ȸ�� �����ϴ� ���̴�.
	int iEvent ; // 0�̸� ��ϵ� event�� ���� ���̴�.
} ;


class TNScheduler
{
public :
	TNScheduler() ;
	~TNScheduler() ;

	void Init() ;

	enum { eSchedule_MaxCount = 512, eTerm_Day = 86400000, eTerm_Hour = 3600000, eTerm_Minute = 60000, eTerm_Second = 1000, } ;

// Public Operations
public :
	void Load() ;
	void CheckNRunSchedule( unsigned int a_uiNow ) ;
	int AddSchedule( unsigned int a_uiNow, unsigned int a_uiGap, unsigned int a_uiFreq, int a_iEvent ) ;

// Public Properties
public :
	

private :	
	TNSchedule m_krgSchedule[eSchedule_MaxCount] ;
	int m_iLast ;
} ;


extern TNScheduler g_kScheduler ;

#endif //__TNScheduler_h__