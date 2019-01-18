#pragma once

#include "Fog.h"

// CFogFialog dialog

class CFogFialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFogFialog)

public:
	CFogFialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CFogFialog();

	//dialog interface
	void SetDialogData(const FogParams& fog);
	FogParams GetDialogData();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FOG };
#endif

protected:
	FogParams fog;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFogColor();
};
