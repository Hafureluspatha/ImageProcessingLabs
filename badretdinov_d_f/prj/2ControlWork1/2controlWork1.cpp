#include <opencv2/opencv.hpp>
#include<vector>
#include<highgui.h>
#include<Windows.h>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	Mat image(400, 1200, CV_8UC1);
	const Scalar black(0);
	const Scalar grey(128);
	const Scalar white(255);

	rectangle(image, Rect(0, 0, 200, 200), grey, -1, 8, 0);
	rectangle(image, Rect(200, 0, 200, 200), white, -1, 8, 0);
	rectangle(image, Rect(400, 0, 200, 200), black, -1, 8, 0);
	rectangle(image, Rect(600, 0, 200, 200), white, -1, 8, 0);
	rectangle(image, Rect(800, 0, 200, 200), black, -1, 8, 0);
	rectangle(image, Rect(1000, 0, 200, 200), grey, -1, 8, 0);

	rectangle(image, Rect(0, 200, 200, 200), grey, -1, 8, 0);
	rectangle(image, Rect(200, 200, 200, 200), white, -1, 8, 0);
	rectangle(image, Rect(400, 200, 200, 200), black, -1, 8, 0);
	rectangle(image, Rect(600, 200, 200, 200), white, -1, 8, 0);
	rectangle(image, Rect(800, 200, 200, 200), black, -1, 8, 0);
	rectangle(image, Rect(1000, 200, 200, 200), grey, -1, 8, 0);

	rectangle(image, Rect(30, 30, 140, 140), black, -1, 8, 0);
	rectangle(image, Rect(230, 30, 140, 140), black, -1, 8, 0);
	rectangle(image, Rect(430, 30, 140, 140), grey, -1, 8, 0);
	rectangle(image, Rect(630, 30, 140, 140), grey, -1, 8, 0);
	rectangle(image, Rect(830, 30, 140, 140), white, -1, 8, 0);
	rectangle(image, Rect(1030, 30, 140, 140), white, -1, 8, 0);

	circle(image, Point(100, 300), 80, black, -1, 8, 0);
	circle(image, Point(300, 300), 80, black, -1, 8, 0);
	circle(image, Point(500, 300), 80, grey, -1, 8, 0);
	circle(image, Point(700, 300), 80, grey, -1, 8, 0);
	circle(image, Point(900, 300), 80, white, -1, 8, 0);
	circle(image, Point(1100, 300), 80, white, -1, 8, 0);

	imshow("some", image);

	Mat firstDerivative, secondDerivative, gradient;
	char ker[2][2] = { { 1, 0 }, { 0, -1 } };
	char ker2[2][2] = { { 0, 1 }, {-1, 0 } };

	Mat kernel1(2, 2, CV_8SC(1), ker);
	Mat kernel2(2, 2, CV_8SC(1), ker2);
	filter2D(image, firstDerivative, CV_32F, kernel1, Point(-1, -1), 0, BORDER_DEFAULT);
	filter2D(image, secondDerivative, CV_32F, kernel2, Point(-1, -1), 0, BORDER_DEFAULT);
	Mat result(firstDerivative.rows, firstDerivative.cols, CV_32F);
	for (int i = 0; i < firstDerivative.rows; ++i)
	{
		for (int j = 0; j < firstDerivative.cols; ++j)
		{
			result.at<float>(i, j) = sqrt(firstDerivative.at<float>(i, j) * firstDerivative.at<float>(i, j) +
				secondDerivative.at<float>(i, j) * secondDerivative.at<float>(i, j)) * 127 / 319.0 + 127;
		}
	}
	// We've got all 3 not converted channels, and the general one is normalized to 0 256.
	// IDK how to convert them to LAB and what walues they should be. Should figure this out.

	firstDerivative *= 127 / 255.0;
	firstDerivative += 127;
	Mat convertedfirstDerivative;
	firstDerivative.convertTo(convertedfirstDerivative, CV_8U);

	Mat labImage, threeChannelsConvertedImage;
	cvtColor(convertedfirstDerivative, threeChannelsConvertedImage, CV_GRAY2BGR);
	imshow("", threeChannelsConvertedImage);
	waitKey(0);


	cvtColor(threeChannelsConvertedImage, labImage, CV_BGR2Lab);
	imshow("1dsf", labImage);
	waitKey(0);
}