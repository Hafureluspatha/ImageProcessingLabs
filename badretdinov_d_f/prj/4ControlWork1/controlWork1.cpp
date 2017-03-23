#include <opencv2/opencv.hpp>
#include<vector>
#include<highgui.h>

using namespace cv;
using namespace std;

void changeImage(int leftTrackbarPosition, int rightTrackbarPosition)
{
	/*vector<Mat> channels;
	vector<vector<int>> hist(3, vector<int>(256));
	correctedImage = originalImage.clone();
	split(correctedImage, channels);
	for (int i = 0; i < correctedImage.rows; i++)
	{
		for (int j = 0; j < correctedImage.cols; j++)
		{
			hist[0][(int)(channels[0].at<uchar>(i, j))]++;
			hist[1][(int)(channels[1].at<uchar>(i, j))]++;
			hist[2][(int)(channels[2].at<uchar>(i, j))]++;
		}
	}

	int leftMin(255);
	int rightMax(0);
	vector<int> bounds;

	for (int i = 0; i < hist.size(); ++i)
	{
		bounds = countBoundsOfOneChannel(leftTrackbarPosition, rightTrackbarPosition, hist[i]);
		if (bounds[0] < leftMin) {
			leftMin = bounds[0];
		}
		if (bounds[1] > rightMax) {
			rightMax = bounds[1];
		}
	}
	for (int i = 0; i < correctedImage.rows; i++)
	{
		for (int j = 0; j < correctedImage.cols; j++)
		{
			channels[0].at<uchar>(i, j) = saturate_cast<uchar>(linearTransformationFunction(leftMin, rightMax,
				static_cast<int>(channels[0].at<uchar>(i, j))));
			channels[1].at<uchar>(i, j) = saturate_cast<uchar>(linearTransformationFunction(leftMin, rightMax,
				static_cast<int>(channels[1].at<uchar>(i, j))));
			channels[2].at<uchar>(i, j) = saturate_cast<uchar>(linearTransformationFunction(leftMin, rightMax,
				static_cast<int>(channels[2].at<uchar>(i, j))));
		}
	}
	merge(channels, correctedImage);*/
}

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

	Mat outputImage;
	int sz[2][2] = { {1, 0}, {0, -1} };
	Mat kernel(2, sz, CV_8UC(1), Scalar::all(0));
	//
	filter2D(image, outputImage, CV_8U, kernel, Point(-1, -1), 0, BORDER_DEFAULT);
	imshow("", image);
	waitKey(0);
}