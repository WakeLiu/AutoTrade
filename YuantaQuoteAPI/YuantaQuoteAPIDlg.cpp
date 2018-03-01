// YuantaQuoteAPIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YuantaQuoteAPI.h"
#include "YuantaQuoteAPIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYuantaQuoteAPIDlg dialog

CYuantaQuoteAPIDlg::CYuantaQuoteAPIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYuantaQuoteAPIDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYuantaQuoteAPIDlg)
	m_strID = _T("");
	m_strIP = _T("");
	m_strPORT = _T("");
	m_strPASS = _T("");
	m_strStatus = _T("");
	m_strSymbol = _T("");
	m_strUpdateMode = _T("");
	m_strAddMktRegStatus = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYuantaQuoteAPIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYuantaQuoteAPIDlg)
	DDX_Control(pDX, IDC_QUOTE_LIST, m_ctrlIDC_QUOTE_LIST);
	DDX_Control(pDX, IDC_YUANTAQUOTECTRL1, YuantaQuote_);
	DDX_Text(pDX, IDC_ID, m_strID);
	DDX_Text(pDX, IDC_IP, m_strIP);
	DDX_Text(pDX, IDC_PORT, m_strPORT);
	DDX_Text(pDX, IDC_PASS, m_strPASS);
	DDX_Text(pDX, IDC_Status, m_strStatus);
	DDX_Text(pDX, IDC_Symbol, m_strSymbol);
	DDX_Text(pDX, IDC_UpdateMode, m_strUpdateMode);
	DDX_Text(pDX, IDC_AddMktRegStatus, m_strAddMktRegStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CYuantaQuoteAPIDlg, CDialog)
	//{{AFX_MSG_MAP(CYuantaQuoteAPIDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDConnect, OnConnect)
	ON_BN_CLICKED(IDAddMktReg, OnAddMktReg)
	ON_BN_CLICKED(IDDelMktReg, OnDelMktReg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYuantaQuoteAPIDlg message handlers

BOOL CYuantaQuoteAPIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//Grid名稱
	m_ctrlIDC_QUOTE_LIST.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(0,"商品代號",LVCFMT_CENTER,80);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(1,"參考價",LVCFMT_CENTER,80);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(2,"開盤價",LVCFMT_CENTER,80);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(3,"最高價",LVCFMT_CENTER,80);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(4,"最低價",LVCFMT_CENTER,80);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(5,"漲停價",LVCFMT_CENTER,80);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(6,"跌停價",LVCFMT_CENTER,80);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(7,"成交時間",LVCFMT_CENTER,80);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(8,"成交價位",LVCFMT_CENTER,80);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(9,"成交數量",LVCFMT_CENTER,80);
	m_ctrlIDC_QUOTE_LIST.InsertColumn(10,"總成交量",LVCFMT_CENTER,80);

	m_strIP = _T("quote.yuantafutures.com.tw");
	m_strPORT = _T("80");
	m_strID = _T("");
	m_strSymbol = _T("TXFA1");
	m_strUpdateMode = _T("4");

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CYuantaQuoteAPIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CYuantaQuoteAPIDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CYuantaQuoteAPIDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CYuantaQuoteAPIDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CYuantaQuoteAPIDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BEGIN_EVENTSINK_MAP(CYuantaQuoteAPIDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CYuantaQuoteAPIDlg)
	ON_EVENT(CYuantaQuoteAPIDlg, IDC_YUANTAQUOTECTRL1, 1 /* OnMktStatusChange */, OnOnMktStatusChangeYuantaquotectrl1, VTS_I4 VTS_BSTR)
	ON_EVENT(CYuantaQuoteAPIDlg, IDC_YUANTAQUOTECTRL1, 5 /* OnGetMktAll */, OnOnGetMktAllYuantaquotectrl1, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	ON_EVENT(CYuantaQuoteAPIDlg, IDC_YUANTAQUOTECTRL1, 2 /* OnRegError */, OnOnRegErrorYuantaquotectrl1, VTS_BSTR VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CYuantaQuoteAPIDlg::OnOnMktStatusChangeYuantaquotectrl1(long Status, LPCTSTR Msg) 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Status)->SetWindowText(TLinkStatusRtnCn(Status)+_T(" -> ")+Msg);	
	
}

void CYuantaQuoteAPIDlg::OnOnGetMktAllYuantaquotectrl1(LPCTSTR symbol, LPCTSTR RefPri, LPCTSTR OpenPri, LPCTSTR HighPri, LPCTSTR LowPri, LPCTSTR UpPri, LPCTSTR DnPri, LPCTSTR MatchTime, LPCTSTR MatchPri, LPCTSTR MatchQty, LPCTSTR TolMatchQty, LPCTSTR BestBuyQty, LPCTSTR BestBuyPri, LPCTSTR BestSellQty, LPCTSTR BestSellPri, LPCTSTR FDBPri, LPCTSTR FDBQty, LPCTSTR FDSPri, LPCTSTR FDSQty) 
{
	int  index=0;
	// TODO: Add your control notification handler code here
	if (m_mapSymbal.Lookup(symbol,index))
	{
		for (int i=0;i<m_ctrlIDC_QUOTE_LIST.GetItemCount();i++)
		{
			if(!m_ctrlIDC_QUOTE_LIST.GetItemText(i,0).Compare(symbol))
			{
				index = i;
				break;
			}
		}
		m_ctrlIDC_QUOTE_LIST.SetItemText(index,0,symbol);
	}
	else
	{
		index = m_ctrlIDC_QUOTE_LIST.InsertItem(0,symbol);
		m_mapSymbal.SetAt(symbol,index);
	}

	m_ctrlIDC_QUOTE_LIST.SetItemText(index,1,RefPri);
	m_ctrlIDC_QUOTE_LIST.SetItemText(index,2,OpenPri);
	m_ctrlIDC_QUOTE_LIST.SetItemText(index,3,HighPri);
	m_ctrlIDC_QUOTE_LIST.SetItemText(index,4,LowPri);
	m_ctrlIDC_QUOTE_LIST.SetItemText(index,5,UpPri);
	m_ctrlIDC_QUOTE_LIST.SetItemText(index,6,DnPri);
	m_ctrlIDC_QUOTE_LIST.SetItemText(index,7,MatchTime);
	m_ctrlIDC_QUOTE_LIST.SetItemText(index,8,MatchPri);
	m_ctrlIDC_QUOTE_LIST.SetItemText(index,9,MatchQty);
	m_ctrlIDC_QUOTE_LIST.SetItemText(index,10,TolMatchQty);
	
}

void CYuantaQuoteAPIDlg::OnOnRegErrorYuantaquotectrl1(LPCTSTR symbol, long updmode, long ErrCode) 
{
	CString strRtn;
	// TODO: Add your control notification handler code here
	strRtn.Format("symbol[%s] updmode[%d] ErrCode[%s]", symbol, updmode, TRegErrorRtnCn(ErrCode));
	GetDlgItem(IDC_AddMktRegStatus)->SetWindowText(strRtn);
	
}

void CYuantaQuoteAPIDlg::OnConnect() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	YuantaQuote_.SetMktLogon(m_strID,m_strPASS,m_strIP,m_strPORT);
	
}

void CYuantaQuoteAPIDlg::OnAddMktReg() 
{
	int RtnCode=0;
	UpdateData(TRUE);

	RtnCode = YuantaQuote_.AddMktReg(m_strSymbol,m_strUpdateMode);
	GetDlgItem(IDC_AddMktRegStatus)->SetWindowText(TAddMktRegRtnCn(RtnCode));
	
}

void CYuantaQuoteAPIDlg::OnDelMktReg() 
{
	int RtnCode=0;
	UpdateData(TRUE);

	RtnCode = YuantaQuote_.DelMktReg(m_strSymbol);
	GetDlgItem(IDC_AddMktRegStatus)->SetWindowText(TDelMktRegRtnCn(RtnCode));	
	
}

CString CYuantaQuoteAPIDlg::TLinkStatusRtnCn(long TLinkStatus)
{
	CString strRtn="";

	switch (TLinkStatus)
	{
		case 0:
			strRtn.Format("[ %d ] 未進行連線",TLinkStatus);
			break;
        case 1:
            strRtn.Format("[ %d ] LinkReady",TLinkStatus);
			break;
        case 2:
            strRtn.Format("[ %d ] 登入成功",TLinkStatus);
			break;
        case -1:
            strRtn.Format("[ %d ] 網路連線中斷",TLinkStatus);
			break;
        case -2:
            strRtn.Format("[ %d ] 網路連線失敗",TLinkStatus);
			break;
		default:
            strRtn.Format("[ %d ] 未知錯誤",TLinkStatus);
			break;
	}

	return strRtn;
}

CString CYuantaQuoteAPIDlg::TAddMktRegRtnCn(long TLinkStatus)
{
	CString strRtn="";

	switch (TLinkStatus)
	{
		case 0:
			strRtn.Format("[ %d ] 註冊成功",TLinkStatus);
			break;
        case 1:
            strRtn.Format("[ %d ] 註冊商品錯誤(長?<4 或 >13)",TLinkStatus);
			break;
        case 2:
            strRtn.Format("[ %d ] 註冊方式錯誤",TLinkStatus);
			break;
        case 3:
            strRtn.Format("[ %d ] 註冊方式錯誤",TLinkStatus);
			break;
		default:
            strRtn.Format("[ %d ] 未知錯誤",TLinkStatus);
			break;
	}

	return strRtn;
}

CString CYuantaQuoteAPIDlg::TDelMktRegRtnCn(long TLinkStatus)
{
	CString strRtn="";

	switch (TLinkStatus)
	{
		case 0:
			strRtn.Format("[ %d ] 取消註冊成功",TLinkStatus);
			break;
        case 1:
            strRtn.Format("[ %d ] 取消註冊商品錯誤(長?<4 或 >13)",TLinkStatus);
			break;
        case 2:
            strRtn.Format("[ %d ] 取消註冊方式錯誤",TLinkStatus);
			break;
        case 3:
            strRtn.Format("[ %d ] 取消註冊方式錯誤",TLinkStatus);
			break;
		default:
            strRtn.Format("[ %d ] 未知錯誤",TLinkStatus);
			break;
	}

	return strRtn;
}

CString CYuantaQuoteAPIDlg::TRegErrorRtnCn(long TLinkStatus)
{
	CString strRtn="";

	switch (TLinkStatus)
	{
        case 1:
            strRtn.Format("[ %d ] 尚未登入",TLinkStatus);
			break;
        case 2:
            strRtn.Format("[ %d ] 該商品不存在",TLinkStatus);
			break;
		default:
            strRtn.Format("[ %d ] 未知錯誤",TLinkStatus);
			break;
	}

	return strRtn;
}

