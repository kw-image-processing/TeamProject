﻿#include <opencv2\opencv.hpp>
#include <opencv2\stitching.hpp>
#include <time.h>
#include <tchar.h>

#define DEBUG 1

using namespace std;
using namespace cv;

#define HISTMATCH_EPSILON 0.000001
#define BTM_DEBUG
// Compute histogram and CDF for an image with mask
void do1ChnHist(const Mat_<uchar>& img, const Mat_<uchar>& mask, Mat_<double>& h, Mat_<double>& cdf)
{
    for (size_t p = 0; p < img.total(); p++)
    {
        if (mask(p) > 0)
        {
            uchar c = img(p);
            h(c) += 1.0;
        }
    }

    normalize(h, h, 1, 0, NORM_MINMAX);

    cdf(0) = h(0);
    for (int j = 1; j < 256; j++)
    {
        cdf(j) = cdf(j - 1) + h(j);
    }

    normalize(cdf, cdf, 1, 0, NORM_MINMAX);
}

// match histograms of 'src' to that of 'dst', according to both masks
void histMatchRGB(Mat& src, const Mat& src_mask, const Mat& dst, const Mat& dst_mask)
{
#ifdef BTM_DEBUG
    namedWindow("original source", CV_WINDOW_AUTOSIZE);
    imshow("original source", src);
    namedWindow("original query", CV_WINDOW_AUTOSIZE);
    imshow("original query", dst);
#endif

    vector<Mat_<uchar>> chns, chns1;
    split(src, chns);
    split(dst, chns1);

    for (int i = 0; i < 3; i++)
    {
        Mat_<double> src_hist = Mat_<double>::zeros(1, 256);
        Mat_<double> dst_hist = Mat_<double>::zeros(1, 256);
        Mat_<double> src_cdf = Mat_<double>::zeros(1, 256);
        Mat_<double> dst_cdf = Mat_<double>::zeros(1, 256);

        do1ChnHist(chns[i], src_mask, src_hist, src_cdf);
        do1ChnHist(chns1[i], dst_mask, dst_hist, dst_cdf);

        uchar last = 0;

        Mat_<uchar> lut(1, 256);
        for (int j = 0; j < src_cdf.cols; j++)
        {
            double F1j = src_cdf(j);

            for (uchar k = last; k < dst_cdf.cols; k++)
            {
                double F2k = dst_cdf(k);
                if (abs(F2k - F1j) < HISTMATCH_EPSILON || F2k > F1j)
                {
                    lut(j) = k;
                    last = k;
                    break;
                }
            }
        }

        LUT(chns[i], lut, chns[i]);
    }

    Mat res;
    merge(chns, res);


    res.copyTo(src);
}


int _tmain(int argc, _TCHAR* argv[])
{
    vector<Mat> images;
    Mat matLeftImage;
    Mat matRightImage;

    matLeftImage = imread("3.jpg", IMREAD_COLOR);
    matRightImage = imread("6.jpg", IMREAD_COLOR);

    Mat mask = Mat(matLeftImage.size(), CV_8U, Scalar(255));

    histMatchRGB(matRightImage, mask, matLeftImage, mask);

    images.push_back(matLeftImage);
    images.push_back(matRightImage);

    Mat result;

    Stitcher stitcher = Stitcher::createDefault();

    int ok = stitcher.stitch(images, result);

    cout << ok << "        " << stitcher.OK << endl;
    if (ok == stitcher.OK)
    {
        imshow("Result", result);
        waitKey(0);
    }
    else
    {
        cout << "Fail Stitching" << endl;
    }

    return 0;

}