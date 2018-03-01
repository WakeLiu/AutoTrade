// YuantaQuoteAPIDlg.h : header file
//
//{{AFX_INCLUDES()
#include "yuantaquote.h"
//}}AFX_INCLUDES
#include <afxtempl.h>

#if !defined(AFX_YUANTAQUOTEAPIDLG_H__304DBAD4_888D_4B98_BF7B_71268B754188__INCLUDED_)
#define AFX_YUANTAQUOTEAPIDLG_H__304DBAD4_888D_4B98_BF7B_71268B754188__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CYuantaQuoteAPIDlg dialog

class CYuantaQuoteAPIDlg : public CDialog
{
// Construction
public:
	CYuantaQuoteAPIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CYuantaQuoteAPIDlg)
	enum { IDD = IDD_YUANTAQUOTEAPI_DIALOG };
	CListCtrl	m_ctrlIDC_QUOTE_LIST;
	CYuantaQuote	YuantaQuote_;
	CString	m_strID;
	CString	m_strIP;
	CString	m_strPORT;
	CString	m_strPASS;
	CString	m_strStatus;
	CString	m_strSymbol;
	CString	m_strUpdateMode;
	CString	m_strAddMktRegStatus;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYuantaQuoteAPIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	CString  TLinkStatusRtnCn(long TLinkStatus);
	CString  TAddMktRegRtnCn(long TLinkStatus);
	CString  TDelMktRegRtnCn(long TLinkStatus);
	CString  TRegErrorRtnCn(long TLinkStatus);
	CMap <CString, LPCTSTR, int, int>  m_mapSymbal;

	// Generated message map functions
	//{{AFX_MSG(CYuantaQuoteAPIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnOnMktStatusChangeYuantaquotectrl1(long Status, LPCTSTR Msg);
	afx_msg void OnOnGetMktAllYuantaquotectrl1(LPCTSTR symbol, LPCTSTR RefPri, LPCTSTR OpenPri, LPCTSTR HighPri, LPCTSTR LowPri, LPCTSTR UpPri, LPCTSTR DnPri, LPCTSTR MatchTime, LPCTSTR MatchPri, LPCTSTR MatchQty, LPCTSTR TolMatchQty, LPCTSTR BestBuyQty, LPCTSTR BestBuyPri, LPCTSTR BestSellQty, LPCTSTR BestSellPri, LPCTSTR FDBPri, LPCTSTR FDBQty, LPCTSTR FDSPri, LPCTSTR FDSQty);
	afx_msg void OnOnRegErrorYuantaquotectrl1(LPCTSTR symbol, long updmode, long ErrCode);
	afx_msg void OnConnect();
	afx_msg void OnAddMktReg();
	afx_msg void OnDelMktReg();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YUANTAQUOTEAPIDLG_H__304DBAD4_888D_4B98_BF7B_71268B754188__INCLUDED_)
