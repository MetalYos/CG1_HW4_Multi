// CSensDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "CSensDialog.h"
#include "afxdialogex.h"


// CSensDialog dialog

IMPLEMENT_DYNAMIC(CSensDialog, CDialogEx)

CSensDialog::CSensDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SENS_DLG, pParent)
	, m_max_trans(300)
	, m_min_trans(10)
	, m_max_rot(20)
	, m_min_rot(3)
	, m_max_scale(300)
	, m_min_scale(10)
{
	m_transSens = (m_max_trans + m_min_trans)/2;
	m_scaleSens = (m_max_scale + m_min_scale)/2;
	m_rotSens =   (m_max_rot   + m_min_rot  )/2;
}

CSensDialog::~CSensDialog()
{
}

void CSensDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_ROTATION_SENS_SLIDER, m_rotSens);
	DDX_Slider(pDX, IDC_TRANSLATION_SENS_SLIDER, m_transSens);
	DDX_Slider(pDX, IDC_SCALING_SENS_SLIDER, m_scaleSens);
	DDX_Control(pDX, IDC_ROTATION_SENS_SLIDER, m_rotCtrl);
	DDX_Control(pDX, IDC_TRANSLATION_SENS_SLIDER, m_transCtrl);
	DDX_Control(pDX, IDC_SCALING_SENS_SLIDER, m_scaleCtrl);

}

Vec4 CSensDialog::GetSensitivity()
{
	return Vec4(static_cast<double>(m_transSens),
				static_cast<double>(m_rotSens),
				static_cast<double>(m_scaleSens));
}


BEGIN_MESSAGE_MAP(CSensDialog, CDialogEx)
END_MESSAGE_MAP()


// CSensDialog message handlers


BOOL CSensDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_transCtrl.SetRange(m_min_trans, m_max_trans);
	m_rotCtrl.SetRange(m_min_rot, m_max_rot);
	m_scaleCtrl.SetRange(m_min_scale, m_max_scale);

	m_transCtrl.SetSelection(m_min_trans, m_max_trans);
	m_rotCtrl.SetSelection(m_min_rot, m_max_rot);
	m_scaleCtrl.SetSelection(m_min_scale, m_max_scale);

	m_transCtrl.SetPos(m_transSens);
	m_rotCtrl.SetPos(m_rotSens);
	m_scaleCtrl.SetPos(m_scaleSens);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
