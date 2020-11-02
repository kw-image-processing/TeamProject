// ViewerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageStitching.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"


// CViewerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewerDlg, CDialogEx)

CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INPUTIMAGE1_DIALOG, pParent)
{

}

CViewerDlg::~CViewerDlg()
{
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
END_MESSAGE_MAP()


// CViewerDlg 메시지 처리기입니다.

void CViewerDlg::DisplayImage(Mat DispMat, bool bInOut)
{
	if (bInOut)
	{
		GetDlgItem(IDC_PICTURE_IN)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicOri.GetDC();
		m_PicOri.GetClientRect(&rect);
	}
	else
	{
		GetDlgItem(IDC_PICTURE_OUT)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicProc.GetDC();
		m_PicProc.GetClientRect(&rect);
	}

	IplImage* Ori = &IplImage(DispMat);
	IplImage* Resize = NULL;

	ResizeImage(Ori, &Resize, &rect);
	DisplayBitmap(m_pDC, rect, Resize);
}

void CViewerDlg::DisplayBitmap(CDC* pDC, CRect rect, IplImage*DispIplImage)
{
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biWidth = DispIplImage->width;
	bitmapInfo.bmiHeader.biHeight = DispIplImage->height;

	if (DispIplImage->nChannels == 3)
	{
		IplImage* ReverseImg = cvCreateImage(CvSize(DispIplImage->width, DispIplImage->height), 8, 3);

		for (int h = 0; h < DispIplImage->height; h++)
		{
			for (int w = 0; w < DispIplImage->width; w++)
			{
				ReverseImg->imageData[h * DispIplImage->widthStep + w * 3] = DispIplImage->imageData[(DispIplImage->height - h) * DispIplImage->widthStep + w * 3];
				ReverseImg->imageData[h * DispIplImage->widthStep + w * 3 + 1] = DispIplImage->imageData[(DispIplImage->height - h) * DispIplImage->widthStep + w * 3 + 1];
				ReverseImg->imageData[h * DispIplImage->widthStep + w * 3 + 2] = DispIplImage->imageData[(DispIplImage->height - h) * DispIplImage->widthStep + w * 3 + 2];
			}
		}

		bitmapInfo.bmiHeader.biBitCount = DispIplImage->depth * DispIplImage->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, DispIplImage->width, DispIplImage->height, ReverseImg->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
	}
	else
	{
		IplImage* ReverseImg = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 1);

		for (int h = 0; h < DispIplImage->height; h++)
		{
			for (int w = 0; w < DispIplImage->width; w++)
			{
				ReverseImg->imageData[h * DispIplImage->widthStep + w] = DispIplImage->imageData[(DispIplImage->height - h) * DispIplImage->widthStep + w];
			}
		}

		IplImage * tmp = cvCreateImage(cvGetSize(DispIplImage), DispIplImage->depth, 3);
		cvCvtColor(ReverseImg, tmp, CV_GRAY2BGR);

		bitmapInfo.bmiHeader.biBitCount = tmp->depth * tmp->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom, 0, 0, tmp->width, tmp->height, tmp->imageData, &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
		cvReleaseImage(&tmp);
	}
}

void CViewerDlg::ResizeImage(IplImage* src, IplImage** dst, CRect* rect)
{
	int ndst_width = 0, ndst_height = 0;

	// 먼저 영상의 가로와 세로 중 장축을 구한다
	if (src->width > src->height)
	{
		ndst_width = rect->Width();
		ndst_height = (src->height + rect->Width()) / src->width;
	}
	else
	{
		ndst_width = (src->width * rect->Height()) / src->height;
		ndst_height = rect->Height();
	}

	(*dst) = cvCreateImage(CvSize(ndst_width, ndst_height), IPL_DEPTH_8U, src->nChannels);

	cvResize(src, (*dst));
	rect->right = rect->left + ndst_width;
	rect->bottom = rect->top + ndst_height;
}

unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal)
{
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++)
	{
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}

void MemFree2D(unsigned char** Mem, int nHeight)
{
	for (int n = 0; n < nHeight; n++)
	{
		delete[] Mem[n];
	}
	delete[] Mem;
}