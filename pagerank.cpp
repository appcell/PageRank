
// pagerank.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "pagerank.h"
#include "MainFrm.h"
#include <iostream>
#include <afxsock.h>
#include <WinInet.h>
#include <afxinet.h>
#include "pagerankDoc.h"
#include "pagerankView.h"
#include <regex>
#define RECVBUFF 4096
#define PORT 80
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


LinkList DefaultList;
// CpagerankApp

BEGIN_MESSAGE_MAP(CpagerankApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CpagerankApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CpagerankApp ����

CpagerankApp::CpagerankApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("pagerank.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CpagerankApp ����

CpagerankApp theApp;



// CpagerankApp ��ʼ��

BOOL CpagerankApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CpagerankDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CpagerankView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
	m_pMainWnd->DragAcceptFiles();
	CString a("www.nju.edu.cn");
	NewPR(a);
	return TRUE;
}

// CpagerankApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CpagerankApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CpagerankApp �Զ������/���淽��

void CpagerankApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CpagerankApp::LoadCustomState()
{
}

void CpagerankApp::SaveCustomState()
{
}

// CpagerankApp ��Ϣ�������



bool PCatch(CString strIP,CString strHost,CString strURI)
{
	CFile mFile(_T("d:\\temp.html"), CFile::modeWrite|CFile::modeCreate);	//�����ļ�
	LPCTSTR lpszAgent = "WinInetGet/0.1";
	HINTERNET hInternet = InternetOpen(lpszAgent,INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);	//����hinternetʵ��

	LPCTSTR lpszServerName = strHost;

	INTERNET_PORT nServerPort = INTERNET_DEFAULT_HTTP_PORT;
	LPCTSTR lpszUserName = NULL;
	LPCTSTR lpszPassword = NULL;
	DWORD dwConnectFlags = 0;
	DWORD dwConnectContext = 0;
	HINTERNET hConnect = InternetConnect(hInternet,lpszServerName, nServerPort,lpszUserName, lpszPassword,INTERNET_SERVICE_HTTP,dwConnectFlags, dwConnectContext);

	LPCTSTR lpszVerb = "GET";
	LPCTSTR lpszObjectName = strURI;
	LPCTSTR lpszVersion = NULL;            //ȫ��ȱʡ
	LPCTSTR lpszReferrer = NULL;
	LPCTSTR *lplpszAcceptTypes = NULL;
	DWORD dwOpenRequestFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
               INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
               INTERNET_FLAG_KEEP_CONNECTION |
               INTERNET_FLAG_NO_AUTH |
               INTERNET_FLAG_NO_AUTO_REDIRECT |
               INTERNET_FLAG_NO_COOKIES |
               INTERNET_FLAG_NO_UI |
               INTERNET_FLAG_RELOAD;
	DWORD dwOpenRequestContext = 0;
	HINTERNET hRequest = HttpOpenRequest(hConnect, lpszVerb, lpszObjectName, lpszVersion,
				   lpszReferrer, lplpszAcceptTypes,
				 dwOpenRequestFlags, dwOpenRequestContext);			//��������
	BOOL bResult = HttpSendRequest(hRequest, NULL, 0, NULL, 0);		//��������
	DWORD dwInfoLevel = HTTP_QUERY_RAW_HEADERS_CRLF;
	DWORD dwInfoBufferLength = 4000;
	BYTE *pInfoBuffer = (BYTE *)malloc(dwInfoBufferLength+1);
	while (!HttpQueryInfo(hRequest, dwInfoLevel, pInfoBuffer, &dwInfoBufferLength, NULL))		//��ͷ�洢��pInfoBuffer��
	{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_INSUFFICIENT_BUFFER)
			 {
			     free(pInfoBuffer);
			     pInfoBuffer = (BYTE *)malloc(dwInfoBufferLength+1);
		   }
		   else
		  {
			InternetCloseHandle(hRequest);
			InternetCloseHandle(hConnect);
			InternetCloseHandle(hInternet);
				return 0;
			       break;
		  }
	}
 
	pInfoBuffer[dwInfoBufferLength] = '/0';
	CString InfoBufferString=(char *)pInfoBuffer;
	free(pInfoBuffer);

	//}

	DWORD dwBytesAvailable;
	while (InternetQueryDataAvailable(hRequest, &dwBytesAvailable, 0, 0))		//���Ĵ洢��pMessageBody��
	{
		 BYTE *pMessageBody = (BYTE *)malloc(dwBytesAvailable+1);
 
		 DWORD dwBytesRead;
		 BOOL bResult = InternetReadFile(hRequest, pMessageBody,
                               dwBytesAvailable, &dwBytesRead);
		 if (!bResult)
		 {
               fprintf(stderr, "InternetReadFile failed, error = %d (0x%x)/n",
                       GetLastError(), GetLastError());
               break;
		 }
 
		 if (dwBytesRead == 0)
               break;  // �ļ�����β��������ѭ��
 
        pMessageBody[dwBytesRead] = '/0';
        mFile.Write(pMessageBody,dwBytesRead);
        free(pMessageBody);
	}

		mFile.Flush();
		mFile.Close();	
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);

		return 1;
}
//ץȡ����


// �½�һ��PR����
bool CpagerankApp::NewPR(CString origin)
{
	CString host,uri,ip;
	host=origin,uri="/",ip=origin;
	PCatch(ip,host,uri);
	DefaultList.AddURL(origin+uri);
	PageAnalyse(origin,uri,ip,uri,host,0);
	DefaultList.Display("d:\\templink.txt","d:\\tempnode.txt");
	prmath(DefaultList.linknum,DefaultList.nodenum);
	//prmath(245,38192);
	return false;
}


void prmath(int linknum, int nodenum)		//PR����
{
	double * prold= new double[linknum];
	double * pr= new double[linknum];
	CString aaa;
	int a[245][245]={0};
	CFile mFile(_T("d:\\tempnode.txt"), CFile::modeRead);
	CString NodeCount,NodeRead;
	int cnt1,cnt2;
	char *NodeReadTemp=new char[mFile.GetLength()+1];
	mFile.Read(NodeReadTemp,mFile.GetLength());
	NodeRead=NodeReadTemp;
	for(int i=0;i<nodenum;i++)
	{
		NodeCount=NodeRead.Left(NodeRead.Find('\n'));
		NodeRead=NodeRead.Right(NodeRead.GetLength()-NodeRead.Find('\n')-1);
		cnt1=atoi(NodeCount.Left(NodeCount.Find(',')))-1;
		NodeCount=NodeCount.Right(NodeCount.GetLength()-NodeCount.Find(',')-1);
		cnt2=atoi(NodeCount)-1;
		a[cnt1][cnt2]++;
	}
	mFile.Close();
	int b[245]={0};
	int indegree[245]={0};

	for(int i=0;i<linknum;i++)
	{
		for(int j=0;j<linknum;j++)
		{
			b[i]+=a[i][j];
		}
	}

	for(int i=0;i<linknum;i++)
	{
		for(int j=0;j<linknum;j++)
		{
			indegree[i]+=a[j][i];
		}
	}
	CFile sFile(_T("d:\\pr.txt"), CFile::modeWrite|CFile::modeCreate);
	for(int i=0;i<linknum;i++)
	{
		CString xyz;
		xyz.Format("%d\n",b[i]);
		sFile.Write(xyz,xyz.GetLength());
	}
	sFile.Close();
	for(int i=0;i<linknum;i++)
	{
		prold[i]=1;
		pr[i]=1;
	}
	int flag=0;
	int t=0;
	int xishu=100;
	while(flag<linknum)
	{

	CFile hFile(_T("d:\\pr.txt"), CFile::modeWrite|CFile::modeCreate);	//�����ļ�

		flag=0;
		for(int i=0;i<linknum;i++)
		{
			pr[i]=0;
			for(int j=0;j<linknum;j++)
			{
				if(a[j][i]!=0)
				{
					double kkk=(ZUNI*pr[j]);
					double asa=(kkk/(double)b[j]);
					pr[i]+=asa;
				}
				//temp1=temp1->next;
				//DefaultList.LinkSearch(i+1)->PR=DefaultList.LinkSearch(i+1)->PR+asa;

			}
			pr[i]+=(1-ZUNI);
			
			if(pr[i]-prold[i]<(double)0.0001)
			{
				flag++;
			}
			/*else
			{
				if(t>10)
				flag--;
			}*/
		}
		for(int k=0;k<linknum;k++)
		{
			prold[k]=pr[k];
		}




		t++;


	for(int j=0;j<linknum;j++)
	{
		
		aaa.Format("%lf\n",pr[j]);
		hFile.Write(aaa,aaa.GetLength());
	}
	hFile.Flush();
	hFile.Close();


	}

	
	CStdioFile aFile(_T("d:\\templink.txt"), CFile::modeRead);	//�����ļ�

	CString * str=new CString[linknum];
	CString tempfile;

	char * templink=new char[aFile.GetLength()];
	aFile.Read(templink,aFile.GetLength());
	
	tempfile=templink;
	for(int i=0;i<linknum;i++)
	{
		str[i]=tempfile.Left(tempfile.Find('\n'));
		tempfile=tempfile.Right(tempfile.GetLength()-tempfile.Find('\n')-1);
	}
	linksort lkst;
	for(int i=0;i<linknum;i++)
	{
		aFile.ReadString(tempfile);
		lkst.addnew(str[i],i,pr[i]*xishu,b[i],indegree[i]);
		//lkst.addnew(tempfile,i,DefaultList.LinkSearch(i+1)->PR,b[i],indegree[i]);
	}
	aFile.Close();
	lkst.display();

}	   //PR����
//PR�ľ���������


linksort::linksort()
{
	head=new SORT;
	head->pr=9999;
	head->d=0;
	head->next=NULL;
	sum=0;
}		//��������
//��������


bool linksort::addnew(CString link, int num, double pr, int d, int indegree)
{
	SORT *temp1, *temp2;
	temp1=temp2=head;
	SORT * aaa=new SORT;
	aaa->link=link;
	aaa->num=num;
	aaa->pr=pr;
	aaa->d=d;
	aaa->indegree=indegree;
	aaa->next=NULL;
	if(sum==0)
	{
			temp1->next=aaa;
			sum++;
			return true;
	}
	else
	{
		int i=0;
		for(i=0;i<sum;i++)
		{
			temp2=temp1;
			temp1=temp1->next;
			if(temp1!=NULL&&temp1->pr<pr)
			{
				temp2->next=aaa;
				aaa->next=temp1;
				sum++;
				return true;
			}
		}
		if(i==sum)
		{
			temp1->next=aaa;
			sum++;
			return true;
		}
	}

	return false;
}
//��������


void linksort::display()
{
	CFile hFile(_T("d:\\prank.txt"), CFile::modeWrite|CFile::modeCreate);	//�����ļ�
	CString hello;
	SORT * temp1, *temp2;
	temp1=temp2=head;
	for(int i=0;i<sum;i++)
	{
		temp1=temp1->next;
		hello.Format("rank:%4d      pr: %10.6lf      num:%4d      out-degree:%4d      in-degree:%4d    link: %s \n",(1+i),temp1->pr,temp1->num+1,temp1->d,temp1->indegree,temp1->link);
		hFile.Write(hello,hello.GetLength());

	}
	hFile.Flush();
	hFile.Close();
}		//��PR����������ļ�
//��PR����������ļ�





	Matrix::Matrix(int x,int nodenum)
	{
		CFile mFile(_T("d:\\hostnode.txt"), CFile::modeRead);	//�����ļ�
		CString temp,tempnode;
		CString uri;
		int cnt1,cnt2;
		char * tempchar=new char[mFile.GetLength()];
		MatrixNode *ptemp;
		MatrixNode *rtemp1, *rtemp2;
		MatrixNode *ctemp1, *ctemp2;
		MatrixNode *addr;
		rtemp1=rtemp2=NULL;
		ctemp1=ctemp2=NULL;

		mFile.Read(tempchar,mFile.GetLength());
		temp=tempchar;
		int i=x/MATBLOCK;
		int j=x%MATBLOCK;

		row=x/MATBLOCK+(x%MATBLOCK==0?0:1);
		col=x/MATBLOCK+(x%MATBLOCK==0?0:1);
		/*for(int tempa=0;tempa<row;tempa++)
		{
			for(int tempb=0;tempb<col;tempb++)
			{
				uri.Format("d:\\tempmat\\%dand%d.txt",tempa,tempb);
				CFile tempFile(_T(uri), CFile::modeWrite|CFile::modeCreate);
				for(int m=0;m<MATBLOCK;m++)
				{
					for(int n=0;n<MATBLOCK;n++)
					{
						tempFile.Write("0",1);
					}
				}
				tempFile.Close();
			}
		}*/


		for(int i=0;i<nodenum;i++)
		{
			addr=chead;
			tempnode=temp.Left(temp.Find('\n'));
			cnt1=atoi((LPCTSTR)tempnode.Left(tempnode.Find(',')));
			cnt2=atoi((LPCTSTR)tempnode.Right(tempnode.GetLength()-tempnode.Find(',')-1));
			temp=temp.Right(temp.GetLength()-temp.Find('\n')-1);
			int numcol, numrow, numcoly,numrowx;
			cnt1--;
			cnt2--;
			numrow=cnt1/MATBLOCK;
			numrowx=cnt1%MATBLOCK;
			numcol=cnt2/MATBLOCK;
			numcoly=cnt2%MATBLOCK;
			uri.Format("d:\\tempmat\\%dand%d.txt",numrow,numcol);
			CFile tempFile(_T(uri), CFile::modeWrite);
			tempFile.Seek(numrowx*MATBLOCK+numcoly,CFile::begin);
			/*char *hello=new char;
			tempFile.Read(hello,1);
			bool aaae=(*hello=='0');*/
			tempFile.Write("1",1);
			
			tempFile.Close();
			if(Search(cnt1+1,cnt2+1)==0)
			{
				AfxMessageBox("hello!");
			}

		}



		delete []tempchar;

		mFile.Close();


	}
	bool Matrix::Search(int x, int y)
	{
		CString uri;
		int numcol, numrow, numcoly,numrowx;
		int cnt1=x;
		int cnt2=y;
		char *temp=new char;
		cnt1--;
		cnt2--;
		numrow=cnt1/MATBLOCK;
		numrowx=cnt1%MATBLOCK;
		numcol=cnt2/MATBLOCK;
		numcoly=cnt2%MATBLOCK;

		uri.Format("d:\\tempmat\\%dand%d.txt",numrow,numcol);
		CFile tempFile(_T(uri), CFile::modeRead);
		tempFile.Seek(numrowx*MATBLOCK+numcoly,CFile::begin);
		tempFile.Read(temp,1);

		tempFile.Close();
		if(*temp=='1') 
		{
			delete temp;
				return 1;
		}
		else
		{ 
			delete temp;
				return 0;
		}
	}
	Matrix::~Matrix()
	{
	}