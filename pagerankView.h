
// pagerankView.h : CpagerankView ��Ľӿ�
//

#pragma once


class CpagerankView : public CView
{
protected: // �������л�����
	CpagerankView();
	DECLARE_DYNCREATE(CpagerankView)

// ����
public:
	CpagerankDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CpagerankView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // pagerankView.cpp �еĵ��԰汾
inline CpagerankDoc* CpagerankView::GetDocument() const
   { return reinterpret_cast<CpagerankDoc*>(m_pDocument); }
#endif

