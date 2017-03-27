#include <opencv2/opencv.hpp>
#include<vector>
#include<highgui.h>
#include<Windows.h>

using namespace cv;
using namespace std;

Mat originalImage;
Mat correctedImage;
Mat greyImage;
Mat convertedBluredGreyImage;
Mat overallResult;
int blurExtent = 0;
int leftScale = 0;
int rightScale = 0;
char gx[3][3] = { { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } };
char gy[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };

vector<int> countBoundsOfOneChannel(int leftPercentage, int rightPercentage, vector<int> oneVector) {
	vector<int> result{ 0, 255 };
	long long int overallSum(0);
	for (int i = 0; i < oneVector.size(); ++i)
	{
		overallSum += oneVector[i];
	}
	int leftPercentageVolume(overallSum * 0.01 * leftPercentage);
	int rightPercentageVolume(overallSum * 0.01 * rightPercentage);
	long int tempSum(0);

	//lower bound
	for (int i = 0; i < oneVector.size(); ++i)
	{
		tempSum += oneVector[i];
		if (tempSum >= leftPercentageVolume){
			result[0] = i;
			break;
		}
	}
	tempSum = 0;

	//upper bound
	for (int i = 255; i > -1; --i)
	{
		tempSum += oneVector[i];
		if (tempSum >= rightPercentageVolume){
			result[1] = i;
			break;
		}
	}
	return result;
}

double linearTransformationFunction(int minValue, int maxValue, int x) {
	return 255 * (x - minValue) / static_cast<double>(maxValue - minValue);
}

void colorCorrectImage(int leftTrackbarPosition, int rightTrackbarPosition)
{
	vector<Mat> channels;
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
	merge(channels, correctedImage);
}

void DetectEdges()
{
	Mat resultX, resultY;
	Mat kernelX(3, 3, CV_8SC(1), gx);
	Mat kernelY(3, 3, CV_8SC(1), gy);
	filter2D(convertedBluredGreyImage, resultX, CV_32F, kernelX, Point(0, 0), 0, BORDER_DEFAULT);
	filter2D(convertedBluredGreyImage, resultY, CV_32F, kernelY, Point(0, 0), 0, BORDER_DEFAULT);
	Mat result(resultX.rows, resultX.cols, CV_32F);
	for (int i = 0; i < resultX.rows; ++i)
	{
		for (int j = 0; j < resultX.cols; ++j)
		{
			result.at<float>(i, j) = sqrt(resultX.at<float>(i, j) * resultX.at<float>(i, j) +
				resultY.at<float>(i, j) * resultY.at<float>(i, j)) * 127 / 319.0 + 127;
		}
	}

	result.convertTo(overallResult, CV_8U);
	imshow("Corrected_Image", overallResult);

}

void blurTrackbarCallback(int position, void* info)
{
	Mat kernel = Mat::ones(position, position, CV_32F) / (position * position);
	Mat bluredGreyImage;
	filter2D(greyImage, bluredGreyImage, CV_32F, kernel, Point(0, 0), 0, BORDER_DEFAULT);
	bluredGreyImage.convertTo(convertedBluredGreyImage, CV_8U);
	DetectEdges();
}

bool DirectoryExists(LPCSTR path)
{
	DWORD dwAttrib = GetFileAttributes(path);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

int main(int argc, char* argv[])
{
	//color correction(2 parameters), blur(1 parameter), edge detection
	/* Plan
	get the image
	set the window with 1 trackbar
	color correct
	monochrom
	blur(custom core and stuff)
	edge detect (sobel masks and their summary)
	*/

	cv::String keys =
		"{@src |<none>| path to file}"
		"{@blur |1 | blur power}"
		"{@leftScale |0 | left side scale}"
		"{@rightScale|0 | right side scale}"
		;
	CommandLineParser parser(argc, argv, keys);
	if (!parser.has("@src")){
		cout << "Specify a path to the picture.";
		getchar();
		return 1;
	}

	cv::String imagePath;
	leftScale = stoi(parser.get<String>("@leftScale"));
	rightScale = stoi(parser.get<String>("@rightScale"));
	imagePath = parser.get<String>("@src");
	blurExtent = stoi(parser.get<String>("@blur"));

	originalImage = imread(imagePath);
	if (!originalImage.data){
		cout << "Wrong path to file - " << imagePath;
		getchar();
		return 1;
	}

	namedWindow("Corrected_Image");
	createTrackbar("Blur", "Corrected_Image", &leftScale, 100, blurTrackbarCallback);
	colorCorrectImage(leftScale, rightScale);
	cvtColor(correctedImage, greyImage, CV_BGR2GRAY);
	blurTrackbarCallback(blurExtent, &greyImage);
	waitKey(0);
}