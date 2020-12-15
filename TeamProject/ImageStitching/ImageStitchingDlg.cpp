
// ImageStitchingDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ImageStitching.h"
#include "ImageStitchingDlg.h"
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<Mat> input_imgs;


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageStitchingDlg 대화 상자



CImageStitchingDlg::CImageStitchingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMAGESTITCHING_DIALOG, pParent)
	, m_radio_resolution(0)
	, m_radio_calibration(0)
	, m_radio_interpolation(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageStitchingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_RESOLUTION_INPUTIMAGE1, m_radio_resolution);
	DDX_Radio(pDX, IDC_RADIO_CALIBRATION_INPUTIMAGE1, m_radio_calibration);
	DDX_Radio(pDX, IDC_RADIO_INTERPOLATION_BILINEAR, m_radio_interpolation);
	DDX_Control(pDX, IDC_EDIT_INPUTIMAGE1, Edit_File_Inputimage1);
	DDX_Control(pDX, IDC_EDIT_INPUTIMAGE2, Edit_File_Inputimage2);
}

BEGIN_MESSAGE_MAP(CImageStitchingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CImageStitchingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CImageStitchingDlg::OnBnClickedCancel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_RESOLUTION_INPUTIMAGE1, IDC_RADIO_RESOLUTION_INPUTIMAGE2, OnClickedRadioResolution)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_CALIBRATION_INPUTIMAGE1, IDC_RADIO_CALIBRATION_OFF, OnClickedRadioCalibration)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_INTERPOLATION_BILINEAR, IDC_RADIO_INTERPOLATION_B_SPLINE, OnClickedRadioInterpolation)
	ON_BN_CLICKED(IDC_OPENBUTTON1, &CImageStitchingDlg::OnBnClickedOpenbutton1)
	ON_BN_CLICKED(IDC_OPENBUTTON2, &CImageStitchingDlg::OnBnClickedOpenbutton2)
END_MESSAGE_MAP()


// CImageStitchingDlg 메시지 처리기

BOOL CImageStitchingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CImageStitchingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CImageStitchingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CImageStitchingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void sort_matches_increasing(std::vector< cv::DMatch >& matches)
{
	for (int i = 0; i < matches.size(); i++)
	{
		for (int j = 0; j < matches.size() - 1; j++)
		{
			if (matches[j].distance > matches[j + 1].distance)
			{
				auto temp = matches[j];
				matches[j] = matches[j + 1];
				matches[j + 1] = temp;
			}
		}
	}
}

void CImageStitchingDlg::OnBnClickedOk()
{
	// 이미지 보여주기
	cv::imshow("Left", input_imgs[0]);
	cv::imshow("Right", input_imgs[1]);

	// 이미지 크기 재조정
	//Size size(1200, 800);
	//resize(input_imgs[0], input_imgs[0], size);
	//resize(input_imgs[1], input_imgs[1], size);

	// 흑백 이미지 생성
	Mat gray_image1;
	Mat gray_image2;

	// 컬러 이미지를 흑백 이미지로 변경
	cvtColor(input_imgs[1], gray_image1, COLOR_RGB2GRAY);
	cvtColor(input_imgs[0], gray_image2, COLOR_RGB2GRAY);

	// SIFT를 이용해서 특징점 찾고 디스크립터에 특징 저장 
	Ptr<SIFT> detector = SIFT::create();
	vector<KeyPoint> keypoints_left, keypoints_right;
	Mat descriptors_left, descriptors_right;

	detector->detectAndCompute(gray_image1, noArray(), keypoints_left, descriptors_left);
	detector->detectAndCompute(gray_image2, noArray(), keypoints_right, descriptors_right);

	// Flann 방식과 K-Nearest Neighbor 알고리즘을 이용해서 특징점 매칭

	//vector<vector<DMatch>> knn_matches;
	//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	//matcher->knnMatch(descriptors_left, descriptors_right, knn_matches, 2);

	// 특징점 매칭
	cv::BFMatcher brue_force_matcher = cv::BFMatcher(4, true);
	std::vector< cv::DMatch > matches;

	brue_force_matcher.match(descriptors_left, descriptors_right, matches);

	vector<DMatch> good_matches;

	good_matches = matches;
	
	// 좋은 매칭만 걸러내는 작업
	//const float ratio_thresh = 0.7f;
	//for (size_t i = 0; i < matches.size(); i++)
	//{
	//	if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
	//	{
	//		good_matches.push_back(matches[i]);
	//	}
	//}

	
	//정렬

	sort_matches_increasing(good_matches);

	//특징점 10개로 제한

	if (good_matches.size() > 10)
	{
		good_matches.resize(10);
	}

	cv::Mat output_image;

	// 특징점 매칭현황 그리기

	cv::drawMatches(
		input_imgs[0], keypoints_left,
		input_imgs[1], keypoints_right,
		good_matches,
		output_image);

	cv::imshow("Matches", output_image);

	// 좋은 매칭들의 특징점들을 얻어오기
	vector<Point2f> left;
	vector<Point2f> right;

	for (int i = 0; i < good_matches.size(); i++)
	{
		left.push_back(keypoints_left[good_matches[i].queryIdx].pt);
		right.push_back(keypoints_right[good_matches[i].trainIdx].pt);
	}

	// 호모그래피 행렬 찾기
	Mat H = findHomography(left, right, RANSAC);

	// 이미지를 보정하는데 호모그래피 행렬 사용
	Mat result;

	warpPerspective(input_imgs[1], result, H, Size(input_imgs[1].cols + input_imgs[0].cols, input_imgs[1].rows));
	Mat half(result, Rect(0, 0, input_imgs[0].cols, input_imgs[0].rows));
	input_imgs[0].copyTo(half);

	// 이미지 보정으로 생긴 검은 부분을 위한 처리
	vector<Point> nonBlackList;
	nonBlackList.reserve(result.rows * result.cols);

	// 검은 부분을 가지지 않는 것들을 벡터에 추가
	for (int j = 0; j < result.rows; ++j)
		for (int i = 0; i < result.cols; ++i)
		{
			if (result.at<Vec3b>(j, i) != Vec3b(0, 0, 0))
			{
				nonBlackList.push_back(Point(i, j));
			}
		}

	// 포인트들을 감싸는 사각형 생성
	Rect bb = boundingRect(nonBlackList);

	// 결과 이미지를 보여주기
	imshow("Reult", result(bb));

	waitKey(0);
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CImageStitchingDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CImageStitchingDlg::OnClickedRadioResolution(UINT msg)
{
	UpdateData(TRUE);
	switch (m_radio_resolution)
	{
	case 0:	// Input Image 1
		break;
	case 1: // Input Image 2
		break;
	}
}

void CImageStitchingDlg::OnClickedRadioCalibration(UINT msg)
{
	UpdateData(TRUE);
	switch (m_radio_calibration)
	{
	case 0: // Input Image 1
		break;
	case 1: // Input Image 2
		break;
	case 2: // Average
		break;
	case 3: // OFF
		break;
	}
}

void CImageStitchingDlg::OnClickedRadioInterpolation(UINT msg)
{
	UpdateData(TRUE);
	switch (m_radio_interpolation)
	{
	case 0: // Bilinear Interpolation
		break;
	case 1: // Bicubic Interpolation
		break;
	case 2: // B-Spline Interpolation
		break;

	}
}

void CImageStitchingDlg::OnBnClickedOpenbutton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szFilter[] = " All Files(*.*)|*.*|";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (CString)szFilter, NULL);

	if (IDOK == dlg.DoModal())
	{
		// 이미지 경로 획득
		CString img_path = dlg.GetPathName();

		// Image Path 넣기
		Edit_File_Inputimage1.SetWindowTextA(dlg.GetFileTitle());

		// imread로 읽어오기
		Mat matImg = imread((LPSTR)(LPCSTR)img_path);
		input_imgs.push_back(matImg);
	}
}


void CImageStitchingDlg::OnBnClickedOpenbutton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szFilter[] = " All Files(*.*)|*.*|";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (CString)szFilter, NULL);

	if (IDOK == dlg.DoModal())

	{

		// 이미지 경로 획득
		CString img_path = dlg.GetPathName();

		// Image Path 넣기
		Edit_File_Inputimage2.SetWindowTextA(dlg.GetFileTitle());

		// imread로 읽어오기
		Mat matImg = imread((LPSTR)(LPCSTR)img_path);
		input_imgs.push_back(matImg);
	}
}
