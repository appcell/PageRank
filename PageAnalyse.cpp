#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "pagerank.h"
#include "MainFrm.h"
#include <iostream>
#include <afxsock.h>
#include "pagerankDoc.h"
#include "pagerankView.h"
#include <regex>
#define RECVBUFF 4096
#define PORT 80
#define STACKMAX 4
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
LinkList HostList;
int NodeMat[800][800]={0};
	LinkStack::LinkStack()
	{
		head=new LinkStackNode;
		head->ip="";
		head->host="";
		head->uri="";
		head->next=NULL;
		tail=head;
		num=0;
	}
	LinkStack::~LinkStack()
	{
		LinkStackNode * p1, *p2;
		p1=p2=head;
		for(int i=0;i<num;i++)
		{
			p2=p1;
			p1=p1->next;
			delete p2;
		}
		delete p1;
	}
	void LinkStack::push(CString ip, CString host, CString uri)
	{
		LinkStackNode * temp=new LinkStackNode;
		temp->host=host;
		temp->ip=ip;
		temp->uri=uri;
		temp->next=NULL;
		temp->prev=tail;
		tail->next=temp;
		tail=temp;
		num++;
	}
	LinkStackNode LinkStack::pop()
	{
		LinkStackNode temp;
		temp.host=tail->host;
		temp.ip=tail->ip;
		temp.uri=tail->uri;
		temp.next=NULL;
		temp.prev=NULL;
		LinkStackNode * p1=tail;
		tail=tail->prev;
		tail->next=NULL;
		num--;
		delete p1;
		return temp;
	}
	LinkList::LinkList()
	{
		linkHead=new LinkTitle;
		linkHead->link="";
		linkHead->next=NULL;
		/*nodeHead=new struct LinkNode;
		nodeHead->i=NULL;
		nodeHead->j=NULL;
		nodeHead->next=NULL;*/
		linknum=nodenum=0;
	}
	LinkList::~LinkList()
	{
		LinkTitle * temp1, *temp2;
		LinkNode * temp3, *temp4;
		temp1=temp2=linkHead;
		/*temp3=temp4=nodeHead;*/
		if(linknum==0)
		{
			delete []temp1;
			delete []temp3;
		}
		for(int i=0;i<linknum;i++)
		{
			temp2=temp1;
			temp1=temp1->next;
			delete []temp2;
		}
		for(int i=0;i<nodenum;i++)
		{
			temp4=temp3;
			temp3=temp3->next;
			delete []temp4;
		}

	}
	int LinkList::AddURL(CString link)	//新增URL
	{
		LinkTitle * temp1=linkHead;
		if(URLRepeat(link)) return URLSearch(link);
		LinkTitle * temp=new LinkTitle;
		temp->link=link;
		temp->next=NULL;
		int flag=0;
		for(flag=0;flag<linknum;flag++)
		{
			temp1=temp1->next;
		}
		temp1->next=temp;
		linknum++;
		return (linknum);
	}
	bool LinkList::URLRepeat(CString link)	//判断url是否已存在
	{
		return URLSearch(link)==(-1)?false:true;
	}
	void LinkList::AddNode(int i, int j)	//加入从i到j的新边，应从j入手 
	{
		LinkTitle * LinkI=LinkSearch(i);
		LinkTitle * LinkJ=LinkSearch(j);
		if(LinkJ->InCount==0)
		{
			LinkJ->InLink=new struct LinkTitleNode;
			LinkJ->InLink->LinkFull=LinkI;
			LinkJ->InLink->LinkID=i;
			LinkJ->InCount++;
			LinkI->OutCount++;
			nodenum++;
			return;
		}
		else
		{
			struct LinkTitleNode * temp=LinkJ->InLink;
			struct LinkTitleNode * temp2=LinkJ->InLink;
			for (int flag=0;flag<LinkJ->InCount;flag++)
			{
				if(temp->LinkID==i) return;
				if(temp->LinkID>i&&temp2->LinkID<i)
				{
					struct LinkTitleNode * NewNode=new struct LinkTitleNode;
					NewNode->LinkFull=LinkI;
					NewNode->LinkID=i;
					NewNode->next=temp;
					temp2->next=NewNode;
					LinkJ->InCount++;
					LinkI->OutCount++;
					nodenum++;
					return;

				}
				temp2=temp;
				temp=temp->next;
				
			}
			struct LinkTitleNode * NewNode=new struct LinkTitleNode;
			NewNode->LinkFull=LinkI;
			NewNode->LinkID=i;
			temp2->next=NewNode;
			LinkJ->InCount++;
			LinkI->OutCount++;
			nodenum++;
			return;
		}
	}
	LinkTitle * LinkList::LinkSearch(int i)		
	{
		LinkTitle * temp=linkHead;
		for(int flag=0;flag<i;flag++)
		{
			temp=temp->next;
		}
		return temp;
	}
	//根据编号找到对应的链接的指针

	int LinkList::URLSearch(CString link)
	{
		LinkTitle * temp=linkHead;
		for(int flag=0;flag<linknum;flag++)
		{
			temp=temp->next;
			if(temp->link==link) return (flag+1);
		}
		return (-1);
	}
	//根据给出的URL找到对应的行/列号，找不到返回-1

	void LinkList::Display(CString LinkAddress, CString NodeAddress)
	{
		CFile mFile(_T(LinkAddress), CFile::modeWrite|CFile::modeCreate);
		int i,j,k;
		LinkTitle *temp1, *temp2;
		temp1=temp2=linkHead;
		LinkTitleNode *temp3,*temp4;
		
		for(i=0;i<linknum;i++)
		{
			temp2=temp1;
			temp1=temp1->next;
			CString a;
			a.Format("%s\n",temp1->link);
			mFile.Write(a,a.GetLength());
		}
		mFile.Flush();
		mFile.Close();
		CFile nFile(_T(NodeAddress), CFile::modeWrite|CFile::modeCreate);
		temp1=temp2=linkHead->next;
		for(j=0;j<linknum;j++)
		{
			CString a;
			a.Format("ID=%d:",j+1);
			nFile.Write(a,a.GetLength());
			temp3=temp1->InLink;
			for(int flag=0;flag<temp1->InCount;flag++)
			{
				a.Format(" %d ,",temp3->LinkID);
				nFile.Write(a,a.GetLength());
				temp3=temp3->next;
			}
			a.Format("\n");
			nFile.Write(a,a.GetLength());
			temp1=temp1->next;
		}
		nFile.Flush();
		nFile.Close();
	}
	//输出到文件




bool RegexMatch(CString str, CString reg)
{
	std::regex_constants::syntax_option_type fl = std::regex_constants::icase;
	const std::regex r(reg,fl);
	bool a=std::regex_match(std::string(str),r);
	return a;
}
//对链接进行正则匹配
bool LinkFilter(CString link) 
{
	CString reg=".*(\\.css|\\.js|\\.tar|\\.ico|\\.png|\\.jpg|\\.gif|\\.doc|\\.xls|\\.msc|\\.rar|\\.ppt|\\.jar|\\.zip|\\.wmv|\\.exe|about:blank|\\.wma|\\.mht|\\.pdf|mailto).*|.*#.*|.*(\\w)+(\\.\\w+)*@(\\w)+((\\.\\w+)+).*|.*%.*";
	//CString reg=".*(\\.css|about:blank|\\.ico|mailto).*|.*#.*|.*(\\w)+(\\.\\w+)*@(\\w)+((\\.\\w+)+).*|.*%.*";
	return RegexMatch(link,reg);
}
 //链接过滤

void CpagerankApp::PageAnalyse(CString ohost,CString olink,CString ip,CString uri,CString host,int StackCount)
{
	if(StackCount>STACKMAX) return;
	CString sRead("");
	CString blank("     ");

	CFile aFile(_T("d:\\templink2.txt"), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	aFile.SeekToEnd();//先定位到文件尾部
	CString writetemp;
	aFile.Write("\n\n\n",3);
	for(int stacktemp=0;stacktemp<StackCount;stacktemp++)
	{
		aFile.Write(blank,blank.GetLength());
	}
	writetemp.Format("========Stack: %d,  Upper:%s  ==========\n",StackCount,(ohost+olink));
	aFile.Write(writetemp,writetemp.GetLength());
	aFile.Close();




	CFile mFile(_T("d:\\temp.html"), CFile::modeRead);	//从文件中读取字符串
	int jjj;
	jjj=mFile.GetLength();
	if(jjj==0)
	{
		mFile.Close();
		PCatch(ip,ohost,olink);
		mFile.Open(_T("d:\\temp.html"), CFile::modeRead);
	}
	char * temp=new char[jjj+1];
	mFile.Read(temp,mFile.GetLength());
	temp[jjj]='/0';
	sRead=temp;

	mFile.Close();
	free(temp);
	sRead.TrimRight("");
	int linkFirst,linkLast,linkLast2;
	CString link;
	CString prefix("href=");
	CString suffix;
	LinkStack stack;
	linkFirst=sRead.Find(prefix);
	/*if(linkFirst==(-1)&&sRead.Find("404")==(-1)&&sRead.Find("URL")==(-1)&&sRead.Find("Moved")==(-1)&&sRead.Find("<")==(-1)&&sRead.Find("对")==(-1))
	{
		AfxMessageBox("miemiemie");
	}*/
	while(linkFirst=sRead.Find(prefix))
	{
		if(linkFirst!=(-1))
		{
			sRead=sRead.Right(sRead.GetLength()-linkFirst-prefix.GetLength());
			if(sRead[0]=='\"'||sRead[0]=='\'')
			{
				suffix=sRead[0];
				sRead=sRead.Right(sRead.GetLength()-1);
			}
			else
			{
				suffix=" ";
			}
			linkLast=sRead.Find(suffix);
			linkLast2=sRead.Find('>');
			link=sRead.Left(linkLast2<linkLast?linkLast2:linkLast);
			if(link.GetLength()==0) continue;
			if(link.Find("javascript:")!=-1)
			{
				link=link.Left(link.Find("javascript:"));
			}
			if(link.Find(' ')!=-1)
			{
				link.Delete(link.Find(' '));
			}
			if(link.Find('\n')!=-1)
			{
				link.Delete(link.Find('\n'));
			}
			if(link.Find("\"+refer+\"")!=-1)
			{
				link=link.Left(link.Find("\"+refer+\""));
			}
			if(link.Find("://")!=-1)
			{
				if(RegexMatch(link,".*(http://).*")!=1)
						continue;
			}
			
			CString nhost,nuri,nip;
			char trans=10;
			if(link.Find(trans)!=-1)
			{
				link.Delete(link.Find(trans));
			}

			if(LinkFilter(link)==true) continue;
			if(link.Find('?')!=-1&&link.Find('/',link.Find('?'))!=-1) continue;
			if(link.Find('\\')!=-1)
			{
				link=link.Left(link.Find('\\'));
			}
			while(link.Find("&amp;")!=-1)		//将转义的&换成正常的&
			{
				CString andtemp=link.Right(link.GetLength()-link.Find("&amp;")-5);
				link=link.Left(link.Find("&amp;")+1);
				link=link+andtemp;
			}
			if(link.Find(";")!=-1)		//如果转义之后还有分号，跳过
			{
				continue;
			}

			bool inOutJudge;		//判断内联还是外联，外链为0，内联为1
			if(link.Find("http://")==-1&&link.Find("https://")==-1)
			{
				if(RegexMatch(link,".*(\\.net|\\.com|\\.edu\\.cn|\\.cn|\\.edu|\\.org).*")&&ohost.Find("chin.nju")!=-1)
				{
						inOutJudge=0;
				}
				else
				{
					inOutJudge=1;
				}
			}
			else
			{
						inOutJudge=0;
			}
			if(inOutJudge==1)	//内联
			{
				if(link[0]=='/')		//以/开头的绝对路径
				{
					nhost=host;
					nip=ip;
					if(RegexMatch(link,"^.*/.*\\..*$")||RegexMatch(link,"^.*/.*/$")||link=="/"||link.Find('?')!=-1)	//如果以/*.*或者/.../结尾或者含?或者只是/，保持原样
					{
						nuri=link;
					}
					else		//否则在结尾加/
					{
						nuri=link+'/';
					}
					

				}
				else		//其他开头的相对路径
				{
					CString tempuri;
					nhost=host,nip=ip,tempuri=uri;
					//=========对原路径的预处理==========
					if(tempuri.Find('?')!=-1)		//如果原路径含?，剥到?以前
					{
						tempuri=tempuri.Left(tempuri.Find('?'));		//先减到以/结尾
					}
					if(RegexMatch(tempuri,"^.*/.*\\..*$"))		//如果原路径以/*.*结尾
					{
						tempuri=tempuri.Left(tempuri.ReverseFind('/'));		//先减到以/结尾
					}
					//=========正式开始层级处理==========
					while(link.GetLength()>1&&link[0]=='.')			//开头有.的情况
					{
						if(link[1]=='/')			//以./开头，目录不变
						{
							link=link.Right(link.GetLength()-link.Find('/')-1);
							
						}
						else if(link[1]=='.'&&link.GetLength()>2&&link[2]=='/')		//以../开头，返回上级目录
						{
							link=link.Right(link.GetLength()-link.Find('/')-1);		//link剥掉一层
							if(tempuri.Find('/')!=-1)
							{
								tempuri=tempuri.Left(tempuri.ReverseFind('/'));	//tempuri剥掉一层
							}
						}
						else
						{
							break;
						}

					}
					if(tempuri.GetLength()==0||tempuri[tempuri.GetLength()-1]!='/')
					{
						tempuri+='/';
					}


					nuri=tempuri+link;



				}
			}
			else	//外链
			{
				if(link.Find("http://")!=-1)
				{
					if(link.Find("http:///")!=-1)
					{
						continue;
					}
					else
					{
						link=link.Right(link.GetLength()-link.Find("http://")-7);
					}
				}
				else if(link.Find("https://")!=-1)
				{
					if(link.Find("https:///")!=-1)
					{
						continue;
					}
					else
					{
						link=link.Right(link.GetLength()-link.Find("https://")-7);
					}
				}
				
				if(link.GetLength()>0&&link[link.GetLength()-1]=='/')
				{
					nhost=link.Left(link.Find('/'));
					link=link.Right(link.GetLength()-link.Find("/"));
				}
				else
				{
					if(link.Find('/')==-1)
					{
						nhost=link;
						link="/";
					}
					else
					{
						nhost=link.Left(link.Find('/'));
						link=link.Right(link.GetLength()-link.Find("/"));
						if(!RegexMatch(link,"^.*/.*\\..*$"))
						{
							link+='/';
						}
					}
				}

				nuri=link;
				nip=nhost;
			}
			if(link.Find('.')!=-1&&link.Find('/',link.Find('.'))!=-1) continue;
			if(AddressFilter(nhost,nuri)==true) continue;
			int maohao=nhost.Find(':');
			if(maohao!=-1)
			{
				nhost=nhost.Left(maohao);
				nip=nhost;
			}
			if(DefaultList.URLRepeat(nhost+nuri)==1)		//抓取到重复链接时
			{
				int j=DefaultList.URLSearch(nhost+nuri);
				int i=DefaultList.URLSearch(ohost+olink);
				DefaultList.AddNode(i,j);
				continue;
			}
			else
			{
				CFile yFile(_T("d:\\templink2.txt"), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
				yFile.SeekToEnd();//先定位到文件尾部
				for(int stacktemp=0;stacktemp<StackCount;stacktemp++)
				{
					yFile.Write(blank,blank.GetLength());
				}
				yFile.Write(nhost+nuri+'\n',(nhost+nuri).GetLength()+1);
				yFile.Close();
			}
			int j=DefaultList.AddURL(nhost+nuri);
			int i=DefaultList.URLSearch(ohost+olink);
			DefaultList.AddURL(nhost+nuri);
			if(i!=-1&&i!=0&&j!=-1&&j!=0&&i!=j)
			{
				DefaultList.AddNode(i,j);

			}
			if(DefaultList.linknum%200==0)
			{
				DefaultList.Display("d:\\templink.txt","d:\\tempnode.txt");
			}
			
			stack.push(nip,nhost,nuri);			
		}
		else
		{
				break;
		}
	}
		

	int stacknum_origin=stack.num;
	
	for(int flag=0; flag<stacknum_origin; flag++)
	{
		LinkStackNode temp=stack.pop();
		if(temp.host!=ohost)
		{
			if(PCatch(temp.ip,temp.host,temp.uri))
				PageAnalyse(temp.host,temp.uri,temp.ip,temp.uri,temp.host,(StackCount+1));
		}
		else
		{
			if(StackCount==STACKMAX)
				return;
			if(PCatch(temp.ip,temp.host,temp.uri))
				PageAnalyse(temp.host,temp.uri,temp.ip,temp.uri,temp.host,(StackCount+1));
		}

	}
}
// 对页面内容进行分析并抓取深层链接

bool CpagerankApp::AddressFilter(CString host, CString uri)
{
	if(host.Find("nju.edu.cn")==-1) return true;
	
	return false;

}
//对域名进行过滤
