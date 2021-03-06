#pragma once

#include "AntiAliasing.h"

// CAntiAliasingDialog dialog

class CAntiAliasingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAntiAliasingDialog)

public:
	CAntiAliasingDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAntiAliasingDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANTIALIAS_DLG };
#endif

public:
	AntiAliasing antiAliasing;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
};
