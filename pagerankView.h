
// pagerankView.h : CpagerankView 类的接口
//

#pragma once


class CpagerankView : public CView
{
protected: // 仅从序列化创建
	CpagerankView();
	DECLARE_DYNCREATE(CpagerankView)

// 特性
public:
	CpagerankDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CpagerankView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // pagerankView.cpp 中的调试版本
inline CpagerankDoc* CpagerankView::GetDocument() const
   { return reinterpret_cast<CpagerankDoc*>(m_pDocument); }
#endif

