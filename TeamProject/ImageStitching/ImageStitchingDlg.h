
// ImageStitchingDlg.h : ��� ����
//

#pragma once
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

using namespace cv;
using namespace std;

// CImageStitchingDlg ��ȭ ����
class CImageStitchingDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CImageStitchingDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGESTITCHING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_radio_resolution;
	int m_radio_calibration;
	int m_radio_interpolation;
	afx_msg void OnClickedRadioResolution(UINT msg);
	afx_msg void OnClickedRadioCalibration(UINT msg);
	afx_msg void OnClickedRadioInterpolation(UINT msg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	CEdit Edit_File_Inputimage1, Edit_File_Inputimage2;

public:
	afx_msg void OnBnClickedOpenbutton1();
	afx_msg void OnBnClickedOpenbutton2();

};
