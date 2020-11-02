
// ImageStitchingDlg.h : 헤더 파일
//

#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

// CImageStitchingDlg 대화 상자
class CImageStitchingDlg : public CDialogEx
{
// 생성입니다.
public:
	CImageStitchingDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGESTITCHING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
