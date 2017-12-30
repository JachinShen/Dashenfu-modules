//C++ standard libs
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <algorithm>

//OpenCV libs
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//Namespaces
using namespace std;
using namespace cv;
int cmp(Rect & s1, Rect & s2){return s1.x < s2.x;}
bool isWhite(Mat & region)
{
	int j = 0;
	int nl = region.rows;
	int nc = region.cols*region.channels();
	for (int k = 0; k < nl; k++)
	{
		uchar* data = region.ptr<uchar>(k);
		for (int i = 0; i < nc; i++)
		{
			if (data[i] == 255)
			{
				j = j + 1;
				if (j == 5)
				{
					return true;
				}
			}
		}
	}
	return false;	
}
vector<int> recognizeNum(Mat & src)
{
	Mat hsv;
	cvtColor(src, hsv, COLOR_BGR2HSV_FULL);
	vector<Mat> hsvSplits;
	split(hsv, hsvSplits);

	Mat red_high, red_low, red, v_high, s_high;
	threshold(hsvSplits[0], red_high, 220, 255, THRESH_BINARY);
	threshold(hsvSplits[0], red_low, 20, 255, THRESH_BINARY_INV);
	bitwise_or(red_high, red_low, red);
	threshold(hsvSplits[2], v_high, 230, 255, THRESH_BINARY);
	bitwise_and(red, v_high, red);
	threshold(hsvSplits[1], s_high, 180, 255, THRESH_BINARY_INV);
	bitwise_and(red, s_high, red);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(red.clone(), contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<Rect> digit;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contourArea(contours[i]) < 10)
			continue;
		digit.push_back(boundingRect(contours[i]));
	}
	//cout << digit[0] << digit[1] << digit[2] << digit[3] << digit[4]; disorder 
	sort(digit, cmp);

	vector<Mat> roi;
	for (int i = 0; i < digit.size(); i++)
	{
		roi.push_back(red(Rect(digit[i].x, digit[i].y, digit[i].width, digit[i].height)));
	}

	vector<int> num;
	for (int i = 0; i < digit.size(); i++)
	{
		int height = roi[i].rows;
		int width = roi[i].cols;
		Mat up, bottom, left_up, right_up, left_bottom;
		up = roi[i](Rect(2 * width / 7, 0, 3 * width / 7, height / 7));
		bottom = roi[i](Rect(2 * width / 7, 6 * height / 7, 3 * width / 7, height / 7));
		//cout << endl << bottom<<endl;
		left_up = roi[i](Rect(0, height / 7, 2 * width / 7, 2 * height / 7));
		right_up = roi[i](Rect(5 * width / 7, height / 7, 2 * width / 7, 2 * height / 7));
		left_bottom = roi[i](Rect(0, 4 * height / 7, 2 * width / 7, 2 * height / 7));
		bool is_up = isWhite(up);
		bool is_bottom = isWhite(bottom);
		bool is_left_up = isWhite(left_up);
		bool is_right_up = isWhite(right_up);
		bool is_left_bottom = isWhite(left_bottom);
		if (is_up==false)
		{
			if (is_left_up==true)
				//cout<< "4";
				num.push_back(4);
			else
				//cout<< "1";
				num.push_back(1);
		}
		else if ((is_up == true) && (is_bottom == false))
		{
			//cout << "7";
			num.push_back(7);
		}
		else if ((is_up == true) && (is_bottom == true) && (is_right_up == false))
		{
			if (is_left_bottom == true)
				//cout << "6";
				num.push_back(6);
			else
				//cout << "5";
				num.push_back(5);
		}
		else if ((is_up == true) && (is_bottom == true) && (is_right_up == true))
		{
			if (is_left_up == true)
			{
				if (is_left_bottom == true)
				{
					//cout << "8";
					num.push_back(8);
				}
				else
					//cout << "9";
					num.push_back(9);
			}
			else
			{
				if (is_left_bottom == true)
				{
					//cout << "2";
					num.push_back(2);
				}
				else
					//cout << "3";
					num.push_back(3);
			}
		}
	}
	return num;
}

int main(int argc, char** argv)
{
	Mat src = imread(argv[1],1);
	vector<int> num = recognizeNum(src);
	for (int i = 0; i < num.size(); i++)
	{
		cout << num[i] << endl;
	}

}
