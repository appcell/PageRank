
// pagerankView.cpp : CpagerankView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CpagerankView ����/����

CpagerankView::CpagerankView()
{
	// TODO: �ڴ˴���ӹ������

}

CpagerankView::~CpagerankView()
{
}

BOOL CpagerankView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CpagerankView ����

void CpagerankView::OnDraw(CDC* /*pDC*/)
{
	CpagerankDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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


// CpagerankView ���

#ifdef _DEBUG
void CpagerankView::AssertValid() const
{
	CView::AssertValid();
}

void CpagerankView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CpagerankDoc* CpagerankView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CpagerankDoc)));
	return (CpagerankDoc*)m_pDocument;
}
#endif //_DEBUG


// CpagerankView ��Ϣ�������
