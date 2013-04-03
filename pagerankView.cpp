
// pagerankView.cpp : CpagerankView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "pagerank.h"
#endif

#include "pagerankDoc.h"
#include "pagerankView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CpagerankView

IMPLEMENT_DYNCREATE(CpagerankView, CView)

BEGIN_MESSAGE_MAP(CpagerankView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CpagerankView 构造/析构

CpagerankView::CpagerankView()
{
	// TODO: 在此处添加构造代码

}

CpagerankView::~CpagerankView()
{
}

BOOL CpagerankView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CpagerankView 绘制

void CpagerankView::OnDraw(CDC* /*pDC*/)
{
	CpagerankDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CpagerankView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CpagerankView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CpagerankView 诊断

#ifdef _DEBUG
void CpagerankView::AssertValid() const
{
	CView::AssertValid();
}

void CpagerankView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CpagerankDoc* CpagerankView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CpagerankDoc)));
	return (CpagerankDoc*)m_pDocument;
}
#endif //_DEBUG


// CpagerankView 消息处理程序
