// CFogFialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "CFogFialog.h"
#include "afxdialogex.h"


// CFogFialog dialog

IMPLEMENT_DYNAMIC(CFogFialog, CDialogEx)

CFogFialog::CFogFialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FOG, pParent)
{

}

CFogFialog::~CFogFialog()
{
}

void CFogFialog::SetDialogData(const FogParams & fog)
{
	this->fog = fog;
}

FogParams CFogFialog::GetDialogData()
{
	return fog;
}

void CFogFialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_FOG_MIN_DIST, fog.MinFogDistance);
	DDV_MinMaxDouble(pDX, fog.MinFogDistance, 0.0, fog.MaxFogDistance);

	DDX_Text(pDX, IDC_EDIT_FOG_MAX_DIST, fog.MaxFogDistance);
	DDV_MinMaxDouble(pDX, fog.MaxFogDistance, fog.MinFogDistance, 1000.0);

	int helper = (int)fog.BlendingFunction;
	DDX_Radio(pDX, IDC_RADIO_FOG_LINEAR, helper);
	fog.BlendingFunction = (FogBlendingFunction)helper;

	DDX_Text(pDX, IDC_EDIT_FOG_EXP_FACT, fog.Exponent);
	DDV_MinMaxDouble(pDX, fog.Exponent, 0.0, 2.0);
}


BEGIN_MESSAGE_MAP(CFogFialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FOG_COLOR, &CFogFialog::OnBnClickedButtonFogColor)
END_MESSAGE_MAP()


// CFogFialog message handlers


void CFogFialog::OnBnClickedButtonFogColor()
{
	COLORREF color = RGB(fog.Color[0], fog.Color[1], fog.Color[2]);
	
	CColorDialog dlg(color);
	if (dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		fog.Color[0] = GetRValue(color);
		fog.Color[1] = GetGValue(color);
		fog.Color[2] = GetBValue(color);
	}
}
