
// ImageStitchingDlg.cpp : ���� ����
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


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CImageStitchingDlg ��ȭ ����



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


// CImageStitchingDlg �޽��� ó����

BOOL CImageStitchingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CImageStitchingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
	// �̹��� �����ֱ�
	cv::imshow("Left", input_imgs[0]);
	cv::imshow("Right", input_imgs[1]);

	// �̹��� ũ�� ������
	//Size size(1200, 800);
	//resize(input_imgs[0], input_imgs[0], size);
	//resize(input_imgs[1], input_imgs[1], size);

	// ��� �̹��� ����
	Mat gray_image1;
	Mat gray_image2;

	// �÷� �̹����� ��� �̹����� ����
	cvtColor(input_imgs[1], gray_image1, COLOR_RGB2GRAY);
	cvtColor(input_imgs[0], gray_image2, COLOR_RGB2GRAY);

	// SIFT�� �̿��ؼ� Ư¡�� ã�� ��ũ���Ϳ� Ư¡ ���� 
	Ptr<SIFT> detector = SIFT::create();
	vector<KeyPoint> keypoints_left, keypoints_right;
	Mat descriptors_left, descriptors_right;

	detector->detectAndCompute(gray_image1, noArray(), keypoints_left, descriptors_left);
	detector->detectAndCompute(gray_image2, noArray(), keypoints_right, descriptors_right);

	// Flann ��İ� K-Nearest Neighbor �˰����� �̿��ؼ� Ư¡�� ��Ī

	//vector<vector<DMatch>> knn_matches;
	//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	//matcher->knnMatch(descriptors_left, descriptors_right, knn_matches, 2);

	// Ư¡�� ��Ī
	cv::BFMatcher brue_force_matcher = cv::BFMatcher(4, true);
	std::vector< cv::DMatch > matches;

	brue_force_matcher.match(descriptors_left, descriptors_right, matches);

	vector<DMatch> good_matches;

	good_matches = matches;
	
	// ���� ��Ī�� �ɷ����� �۾�
	//const float ratio_thresh = 0.7f;
	//for (size_t i = 0; i < matches.size(); i++)
	//{
	//	if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
	//	{
	//		good_matches.push_back(matches[i]);
	//	}
	//}

	
	//����

	sort_matches_increasing(good_matches);

	//Ư¡�� 10���� ����

	if (good_matches.size() > 10)
	{
		good_matches.resize(10);
	}

	cv::Mat output_image;

	// Ư¡�� ��Ī��Ȳ �׸���

	cv::drawMatches(
		input_imgs[0], keypoints_left,
		input_imgs[1], keypoints_right,
		good_matches,
		output_image);

	cv::imshow("Matches", output_image);

	// ���� ��Ī���� Ư¡������ ������
	vector<Point2f> left;
	vector<Point2f> right;

	for (int i = 0; i < good_matches.size(); i++)
	{
		left.push_back(keypoints_left[good_matches[i].queryIdx].pt);
		right.push_back(keypoints_right[good_matches[i].trainIdx].pt);
	}

	// ȣ��׷��� ��� ã��
	Mat H = findHomography(left, right, RANSAC);

	// �̹����� �����ϴµ� ȣ��׷��� ��� ���
	Mat result;

	warpPerspective(input_imgs[1], result, H, Size(input_imgs[1].cols + input_imgs[0].cols, input_imgs[1].rows));
	Mat half(result, Rect(0, 0, input_imgs[0].cols, input_imgs[0].rows));
	input_imgs[0].copyTo(half);

	// �̹��� �������� ���� ���� �κ��� ���� ó��
	vector<Point> nonBlackList;
	nonBlackList.reserve(result.rows * result.cols);

	// ���� �κ��� ������ �ʴ� �͵��� ���Ϳ� �߰�
	for (int j = 0; j < result.rows; ++j)
		for (int i = 0; i < result.cols; ++i)
		{
			if (result.at<Vec3b>(j, i) != Vec3b(0, 0, 0))
			{
				nonBlackList.push_back(Point(i, j));
			}
		}

	// ����Ʈ���� ���δ� �簢�� ����
	Rect bb = boundingRect(nonBlackList);

	// ��� �̹����� �����ֱ�
	imshow("Reult", result(bb));

	waitKey(0);
	
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}


void CImageStitchingDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	char szFilter[] = " All Files(*.*)|*.*|";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (CString)szFilter, NULL);

	if (IDOK == dlg.DoModal())
	{
		// �̹��� ��� ȹ��
		CString img_path = dlg.GetPathName();

		// Image Path �ֱ�
		Edit_File_Inputimage1.SetWindowTextA(dlg.GetFileTitle());

		// imread�� �о����
		Mat matImg = imread((LPSTR)(LPCSTR)img_path);
		input_imgs.push_back(matImg);
	}
}


void CImageStitchingDlg::OnBnClickedOpenbutton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	char szFilter[] = " All Files(*.*)|*.*|";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, (CString)szFilter, NULL);

	if (IDOK == dlg.DoModal())

	{

		// �̹��� ��� ȹ��
		CString img_path = dlg.GetPathName();

		// Image Path �ֱ�
		Edit_File_Inputimage2.SetWindowTextA(dlg.GetFileTitle());

		// imread�� �о����
		Mat matImg = imread((LPSTR)(LPCSTR)img_path);
		input_imgs.push_back(matImg);
	}
}
