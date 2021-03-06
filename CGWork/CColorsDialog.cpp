// CColorsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "CColorsDialog.h"
#include "afxdialogex.h"
#include "ALMath.h"

// CColorsDialog dialog

IMPLEMENT_DYNAMIC(CColorsDialog, CDialogEx)

CColorsDialog::CColorsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COLOR_DLG, pParent)
	, IsDiscoMode(FALSE)
{
	BackgroundColor = RGB(0, 0, 0);
	WireframeColor = RGB(255, 255, 255);
	NormalColor = RGB(255, 0, 0);
	IsDiscoMode = false;
}

CColorsDialog::~CColorsDialog()
{
}

void CColorsDialog::DoDataExchange(CDataExchange* pDX)
{	
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_IS_DISCO_CHECKBOX, IsDiscoMode);
}


BEGIN_MESSAGE_MAP(CColorsDialog, CDialogEx)
	ON_BN_CLICKED(IDC_WIREFRAME_COLOR_BUTTON, &CColorsDialog::OnBnClickedWireframeColorButton)
	ON_BN_CLICKED(IDC_NORMAL_COLOR_BUTTON, &CColorsDialog::OnBnClickedNormalColorButton)
	ON_BN_CLICKED(IDC_BACKGROUND_COLOR_BUTTON, &CColorsDialog::OnBnClickedBackgroundColorButton)
	ON_BN_CLICKED(IDC_IS_DISCO_CHECKBOX, &CColorsDialog::OnBnClickedIsDiscoCheckbox)
	ON_BN_CLICKED(IDCANCEL, &CColorsDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CColorsDialog message handlers


void CColorsDialog::OnBnClickedWireframeColorButton()
{
	CColorDialog colorDialog(WireframeColor);
	if (colorDialog.DoModal() == IDOK)
	{
		WireframeColor = colorDialog.GetColor();
	}
}


void CColorsDialog::OnBnClickedNormalColorButton()
{
	CColorDialog colorDialog(NormalColor);
	if (colorDialog.DoModal() == IDOK)
	{
		NormalColor = colorDialog.GetColor();
	}
}


void CColorsDialog::OnBnClickedBackgroundColorButton()
{
	CColorDialog colorDialog(BackgroundColor);
	if (colorDialog.DoModal() == IDOK)
	{
		BackgroundColor = colorDialog.GetColor();
	}
}




void CColorsDialog::OnBnClickedIsDiscoCheckbox()
{
	IsDiscoMode = !IsDiscoMode;
}


BOOL CColorsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_oldWireframeColor = WireframeColor;
	m_oldNormalColor = NormalColor;
	m_oldBackgroundColor = BackgroundColor;
	m_oldIsDiscoMode = IsDiscoMode;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CColorsDialog::OnBnClickedCancel()
{
	WireframeColor = m_oldWireframeColor;
	NormalColor = m_oldNormalColor;
	BackgroundColor = m_oldBackgroundColor;
	IsDiscoMode = m_oldIsDiscoMode;
	CDialogEx::OnCancel();
}
