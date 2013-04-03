
// pagerank.h : pagerank Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#define ZUNI 0.85
#define MATBLOCK 700
// CpagerankApp:
// �йش����ʵ�֣������ pagerank.cpp
//

class CpagerankApp : public CWinAppEx
{
public:
	CpagerankApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	bool PageCatch(CString strIP,CString strHost,CString strURI);
	// �½�һ��PR����
	bool NewPR(CString origin);
	// ��ҳ�����ݽ��з�����ץȡ�������
	void PageAnalyse(CString ohost,CString olink,CString ip,CString uri,CString host,int StackCount);
	bool AddressFilter(CString host, CString uri);
};
bool PCatch(CString strIP,CString strHost,CString strURI);

struct LinkNode
{
	int i;
	int j;
	LinkNode * next;
};


struct LinkTitle
{
public:
	CString link;
	double PR;
	int InCount;
	int OutCount;


	struct LinkTitleNode * InLink;
	struct LinkTitle * next;
	LinkTitle()
	{
		InLink=NULL;
		next=NULL;
		InCount=0;
		OutCount=0;
		PR=1.0;
	}
};

struct LinkTitleNode
{
	struct LinkTitle * LinkFull;
	short LinkID;
	struct LinkTitleNode * next;

	LinkTitleNode()
	{
		LinkFull=NULL;
		LinkID=-1;
		next=NULL;
	}
};

class LinkList
{
public:		//��Ա
	struct LinkTitle * linkHead;
	int linknum;
	int nodenum;

public:		//����
	LinkList();
	~LinkList();
	LinkTitle * LinkSearch(int i);
	int AddURL(CString link);
	bool URLRepeat(CString link);
	void AddNode(int i, int j);
	int URLSearch(CString link);
	void Display(CString LinkAddress,CString NodeAddress);
};

struct LinkStackNode
{
	CString ip;
	CString host;
	CString uri;
	LinkStackNode * next;
	LinkStackNode * prev;
};
class LinkStack
{
public:
	LinkStackNode * head;
	LinkStackNode * tail;

	int num;


public:
	LinkStack();
	~LinkStack();

	void push(CString ip, CString host, CString uri);

	LinkStackNode pop();
};
void prmath(int linknum, int nodenum);
class SORT
{
public:
	double pr;
	int num;
	int d;
	int indegree;
	CString link;
	SORT * next;

};

class linksort
{
public:
	SORT * head;
	int sum;
	
public:
	linksort();
	bool addnew(CString link, int num, double pr, int d, int indegree);
	void display();
};


extern CpagerankApp theApp;
extern LinkList DefaultList;
extern LinkList HostList;

extern int NodeMat[800][800];

typedef struct MatrixNode
{
	bool omat[MATBLOCK][MATBLOCK];
	MatrixNode *right, *down;
}MatrixNode;


class Matrix
{
	MatrixNode *chead, *rhead;
	int row, col;

public:
	Matrix(int x,int nodenum);

	bool Search(int x, int y);
	~Matrix();

};