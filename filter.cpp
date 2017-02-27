/**
 * This is a implement of mean filter.
 * The image is saved by Mat of OpenCV.
 *
 * author: GaoZiwen.
 */

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

void convolution(Mat& src, Mat& templet, Mat& dst)
{
	if (src.empty() || templet.empty())
	{
		std::cout << "Exception: src or templet empty!" << std::endl;
		return;
	}
    
	if (templet.rows % 2 == 0 || templet.cols % 2 == 0)
	{
		std::cout << "Exception: templet must have odd size!" << std::endl;
		return;
	}

    dst.create(src.rows, src.cols, src.type());

	const double* templet_data = templet.ptr<double>(0);

    int nc = src.channels();
    int x_pos = templet.cols / 2;
	int y_pos = templet.rows / 2;
    for (int i = y_pos; i < src.rows - y_pos; i++)
	{
		uchar* data_out = dst.ptr<uchar>(i);
		for (int j = x_pos; j < src.cols - x_pos; j++)
		{
			for (int chl = 0; chl < nc; chl++)
			{
				double sum = 0;
				for (int k1 = 0; k1 < templet.rows; k1++)
				{
					const uchar* data_in = src.ptr<uchar>(i - y_pos + k1);
					for (int k2 = 0; k2 < templet.cols; k2++)
					{
						sum = sum + (double)data_in[(j - x_pos + k2) * nc + chl] * templet_data[k1 * templet.rows + k2];
					}
				}

				data_out[j * nc + chl] = (uchar)sum; 
			}
		}
	}
	
}

void meanFilter(Mat& src, Mat& dst, int ksize = 3)
{
	Mat templet(ksize, ksize, CV_64F, Scalar(0));

	
	for(int i = 0; i < ksize; i++)
	{
		double* data = templet.ptr<double>(i);
		for(int j = 0; j < ksize; j++)
		{
			data[j] = (double)1.0 / ksize / ksize;
		}
	}
    
	convolution(src, templet, dst);
}

int main()
{
	Mat origin = imread("/Users/gaoziwen/Downloads/laoe.jpg");
	
	imshow("Origin", origin);

	Mat m1, m2;

    int size;
	std::cin >> size;

	blur(origin, m1, Size(size, size));

    meanFilter(origin, m2, size);

	imshow("OpencvBlur", m1);

	imshow("MyBlur", m2);

	waitKey(0);

	return 0;
}
