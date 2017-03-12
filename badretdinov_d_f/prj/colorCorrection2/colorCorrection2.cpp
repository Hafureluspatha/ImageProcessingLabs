#include <opencv2/opencv.hpp>
#include<vector>
#include<highgui.h>

using namespace cv;
using namespace std;

Mat image;

vector<int> countBoundsOfOneChannel(int percentage, vector<int> oneVector) {
	vector<int> result{ 0, 255 };
	long long int overallSum(0);
	for (int i = 0; i < oneVector.size(); ++i)
	{
		overallSum += oneVector[i];
	}
	//overallSum = image.rows * image.cols * 0.03;
	int threePercentMark(overallSum * 0.01 * percentage);
	long int tempSum(0);

	//lower bound
	for (int i = 0; i < oneVector.size(); ++i)
	{
		tempSum += oneVector[i];
		if (tempSum >= threePercentMark)
		{
			result[0] = i;
			break;
		}
	}
	tempSum = 0;

	//upper bound
	for (int i = 255; i > -1; --i)
	{
		tempSum += oneVector[i];
		if (tempSum >= threePercentMark)
		{
			result[1] = i;
			break;
		}
	}
	return result;
}

double linearTransformationFunction(int minValue, int maxValue, int x) {
	return 255 * (x - minValue) / static_cast<double>(maxValue - minValue);
}

void changeImage(int trackbarPosition)
{
	vector<Mat> channels;
	vector<vector<int>> hist(3, vector<int>(256));
	split(image, channels);
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
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
		bounds = countBoundsOfOneChannel(trackbarPosition, hist[i]);
		if (bounds[0] < leftMin) {
			leftMin = bounds[0];
		}
		if (bounds[1] > rightMax) {
			rightMax = bounds[1];
		}
	}
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			channels[0].at<uchar>(i, j) = saturate_cast<uchar>(linearTransformationFunction(leftMin, rightMax,
				static_cast<int>(channels[0].at<uchar>(i, j))));
			channels[1].at<uchar>(i, j) = saturate_cast<uchar>(linearTransformationFunction(leftMin, rightMax,
				static_cast<int>(channels[1].at<uchar>(i, j))));
			channels[2].at<uchar>(i, j) = saturate_cast<uchar>(linearTransformationFunction(leftMin, rightMax,
				static_cast<int>(channels[2].at<uchar>(i, j))));
		}
	}
	merge(channels, image);
}

void trackbarCallback(int position)
{
	int k = position;
	changeImage(position);
	imshow("some", image);
}

int main(int argc, char* argv[])
{
	/*argc = 2;
	char* foo[2] = { "", "C:/1ImageProcessing/Programs/1colorCorrection1Images/2.jpg"};
	argv = foo;*/
	cv::String keys = "{@src |<none>| name}";
	int scale = 2;
	CommandLineParser parser(argc, argv, keys);
	if (!parser.has("@src"))
	{
		cout << "Specify a path to the picture.";
		getchar();
		return 1;
	}
	cv::String path = parser.get<String>("@src");
	image = imread(path);
	cvNamedWindow("Color_correction", 1);
	cvCreateTrackbar("Scale", "Color_correction", &scale, 100, trackbarCallback);
	trackbarCallback(scale);
	waitKey(0);
	return 0;
}