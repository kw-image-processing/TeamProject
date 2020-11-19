#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#define ATD at<double>
using namespace std;
using namespace cv;

// Declaration of functions
Mat HarrisConerDetector(Mat input);
Mat getCornerThreshold(Mat input);
void drawingResult(const char* file_name, Mat input, Mat output, Mat hist);

// Declaration of parameters
int blockSize = 3;
int maskSize = 5;
double k = 0.04f;
int thres = 10;

int main()
{
	// Pre-processing
	Mat input = imread("house.jpg");
	Mat input_cv = input.clone();
	Mat input_gray;
	cvtColor(input, input_gray, COLOR_BGR2GRAY);
	equalizeHist(input_gray, input_gray);

	// Mathematical algorithm
	Mat output = HarrisConerDetector(input_gray);
	Mat harris_norm;
	for (int j = 0; j < output.rows; j++)
	{
		for (int i = 0; i < output.cols; i++)
			if (output.ATD(j, i) < 0)
				output.ATD(j, i) = 0; //neglect negative values of candidates
	}
	normalize(output, harris_norm, 0, 255, NORM_MINMAX, CV_8U); //normalize candidates pixel values
	vector<Point2i> corner;
	for (int j = 0; j < harris_norm.rows; j++)
	{
		for (int i = 0; i < harris_norm.cols; i++)
			if (thres < harris_norm.at<unsigned char>(j, i))
				corner.push_back(Point2i(i, j));
	}
	for (int k = 0; k < corner.size(); k++)
		circle(input, corner[k], 1, Scalar(0, 0, 255), 3); //drawing points over the threshold 
	cvtColor(harris_norm, harris_norm, COLOR_GRAY2BGR);
	imshow("output", harris_norm);
	imshow("input", input);
	corner.clear();

	waitKey(0);
	return 0;
}

Mat HarrisConerDetector(Mat input) {
	int height = input.rows;
	int width = input.cols;

	// Sobel mask
	Mat sx_mask(blockSize, blockSize, CV_64FC1);
	Mat sy_mask(blockSize, blockSize, CV_64FC1);
	if (blockSize == 5)
	{
		sx_mask = (Mat_<double>(5, 5) << 
			2, 1, 0, -1, -2,
			2, 1, 0, -1, -2,
			4, 2, 0, -2, -4,
			2, 1, 0, -1, -2,
			2, 1, 0, -1, -2);
		sy_mask = (Mat_<double>(5, 5) << 
			2, 2, 4, 2, 2,
			1, 1, 2, 1, 1,
			0, 0, 0, 0, 0,
			-1, -1, -2, -1, -1,
			-2, -2, -4, -2, -2);
	}
	else if (blockSize == 3) {
		sx_mask = (Mat_<double>(3, 3) << 
			1, 0, -1,
			2, 0, -2,
			1, 0, -1);
		sy_mask = (Mat_<double>(3, 3) << 
			1, 2, 1,
			0, 0, 0,
			-1, -2, -1);
	}
	// Derivative matrix
	int intval = floor(blockSize / 2);
	Mat dxdx = Mat::zeros(height, width, CV_64FC1);
	Mat dydy = Mat::zeros(height, width, CV_64FC1);
	Mat dxdy = Mat::zeros(height, width, CV_64FC1);

	for (int j = intval; j < height - intval; j++)
		for (int i = intval; i < width - intval; i++)
		{
			double dx = 0;
			double dy = 0;
			for (int y = -intval; y <= intval; y++)
				for (int x = -intval; x <= intval; x++)
				{
					dx += (double)input.data[(j + y) * width + (i + x)] * sx_mask.ATD((y + intval), (x + intval));
					dy += (double)input.data[(j + y) * width + (i + x)] * sy_mask.ATD((y + intval), (x + intval));
				}
			dxdx.ATD(j, i) = dx * dx;
			dydy.ATD(j, i) = dy * dy;
			dxdy.ATD(j, i) = dx * dy;
		}

	// Gaussian mask
	Mat g_mask(maskSize, maskSize, CV_64FC1);
	if (maskSize == 5) {
		g_mask = (Mat_<double>(5, 5) << 1, 4, 6, 4, 1,
			4, 16, 24, 16, 4,
			6, 24, 36, 24, 6,
			4, 16, 24, 16, 4,
			1, 4, 6, 4, 1);
		g_mask /= 256.0f;
	}
	else if (maskSize == 3) {
		g_mask = (Mat_<double>(3, 3) << 1, 2, 1,
			2, 4, 2,
			1, 2, 1);
		g_mask /= 16.0f;
	}
	// Weighted matrix
	for (int j = intval; j < height - intval; j++)
		for (int i = intval; i < width - intval; i++)
		{
			double dx2 = 0;
			double dy2 = 0;
			double dxy = 0;
			for (int y = -intval; y <= intval; y++)
				for (int x = -intval; x <= intval; x++)
				{
					dx2 += dxdx.ATD((j + y), (i + x)) * g_mask.ATD((y + intval), (x + intval));
					dy2 += dydy.ATD((j + y), (i + x)) * g_mask.ATD((y + intval), (x + intval));
					dy2 += dxdy.ATD((j + y), (i + x)) * g_mask.ATD((y + intval), (x + intval));
				}
			dxdx.ATD(j, i) = dx2;
			dydy.ATD(j, i) = dy2;
			dxdy.ATD(j, i) = dxy;
		}

	Mat output(height, width, CV_64FC1);
	for (int j = intval; j < height - intval; j++)
		for (int i = intval; i < width - intval; i++) {
			double dx2 = dxdx.ATD(j, i);
			double dy2 = dydy.ATD(j, i);
			double dxy = dxdy.ATD(j, i);
			output.ATD(j, i) = (double)(dx2 * dy2 - dxy * dxy - k * (dx2 + dy2) * (dx2 + dy2));
		}
	return output;
}