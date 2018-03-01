// YuantaQuoteAPI.h : main header file for the YUANTAQUOTEAPI application
//

#if !defined(AFX_YUANTAQUOTEAPI_H__9477F97C_2D72_4EF0_BD63_FC6A037D3F27__INCLUDED_)
#define AFX_YUANTAQUOTEAPI_H__9477F97C_2D72_4EF0_BD63_FC6A037D3F27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CYuantaQuoteAPIApp:
// See YuantaQuoteAPI.cpp for the implementation of this class
//

class CYuantaQuoteAPIApp : public CWinApp
{
public:
	CYuantaQuoteAPIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYuantaQuoteAPIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CYuantaQuoteAPIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YUANTAQUOTEAPI_H__9477F97C_2D72_4EF0_BD63_FC6A037D3F27__INCLUDED_)
