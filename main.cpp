#include "main.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;


Mat src, src_gray, dst;

const char* window_name = "Edge Map";

int top, bottom, left_border, right_border;
int borderType = BORDER_CONSTANT;

/// @@@@@@@@@@ Custom Laplacian function @@@@@@@@@@

Mat filtrLaplace(Mat src) {

	Mat output = src.clone();

	for (int y = 1; y < src.rows - 1; y++) {
		for (int x = 1; x < src.cols - 1; x++) {
			int sum = src.at<uchar>(y - 1, x)
				+ src.at<uchar>(y + 1, x)
				+ src.at<uchar>(y, x - 1)
				+ src.at<uchar>(y, x + 1)
				- 4 * src.at<uchar>(y, x);

			output.at<uchar>(y, x) = saturate_cast<uchar>(sum);
		}
	}

	return output;

}

int main(int argc, char** argv)
{
	CommandLineParser parser(argc, argv, "{@input | rose_blur.png | input image}");
	src = imread(samples::findFile(parser.get<String>("@input")), IMREAD_COLOR); // Load an image
	if (src.empty())
	{
		std::cout << "Could not open or find the image!\n" << std::endl;
		std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
		return -1;
	}


	/// @@ Adding borders the image @@ [This is not the right way, it seems]
	top = (int)(0.05 * src.rows);
	bottom = top;
	left_border = (int)(0.05 * src.cols);
	right_border = left_border;
	copyMakeBorder(src, dst, top, bottom, left_border, right_border, borderType, 0);
	imshow(window_name, dst);


	namedWindow(window_name, WINDOW_AUTOSIZE);


	/// @@@@@@@@@ Find Blur or Not @@@@@@@@@

	Scalar mean, stdev;

	// **** Laplacian from OpenCV ****
	//Laplacian(src, src_gray, CV_64F);
	//meanStdDev(src_gray, mean, stdev, Mat());

	
	// **** Custom Laplacian ****
	Mat filtrLaplace_out = filtrLaplace(src);
	meanStdDev(filtrLaplace_out, mean, stdev, Mat());
	
	cout << "mean is: " << mean << endl;
	cout << "stdev is: " << stdev << endl;

	double variance = stdev.val[0] * stdev.val[0];
	double threshold = 2000;

	cout << "Variance is: " << variance << endl;


	if (variance <= threshold) {
		cout << "Blurry" << endl;

	}
	else {
		cout << "Not blurry" << endl;
	}


	waitKey(0);
	return 0;


}
