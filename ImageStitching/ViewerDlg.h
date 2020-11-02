#pragma once
#include "ImageStitchingDlg.h"

using namespace cv;

// CViewerDlg 대화 상자입니다.

class CViewerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewerDlg)

public:
	CViewerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CViewerDlg();

	unsigned char **ch_in_1, **ch_in_2, **ch_in_3;
	unsigned char **ch_in_gray;

	int nHeight_in, nWidth_in;
	int nFormat;
	bool isImageOpened;

	CString InpFileName;

	CStatic m_PicOri;
	CStatic m_PicProc;

	// Display용 멤버 변수 및 메소드
	Mat Ori_Img, Proc_Img;
	CRect rect;
	CDC* m_pDC;

	void DisplayImage(Mat DispMat, bool bInOut);
	void DisplayBitmap(CDC* pDC, CRect rect, IplImage* DispIplImage);
	void ResizeImage(IplImage* src, IplImage** dst, CRect* rect);

	enum ImageFormat
	{
		FORMAT_RGB,
		FORMAT_YUV444,
		FORMAT_YUV420,
		FORMAT_GRAY
	};

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUTIMAGE1_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
