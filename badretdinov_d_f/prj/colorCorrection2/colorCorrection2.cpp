#include <opencv2/opencv.hpp>
#include<vector>
#include<highgui.h>

using namespace cv;
using namespace std;

Mat originalImage;
Mat correctedImage;
int leftScale = 3;
int rightScale = 3;

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
		if (tempSum >= leftPercentageVolume)
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
		if (tempSum >= rightPercentageVolume)
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

void changeImage(int leftTrackbarPosition, int rightTrackbarPosition)
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

void drawHistogram(string window, Mat image)
{
	vector<Mat> imout;
	split(image, imout);

	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform(true);
	bool accumulate(false);
	Mat b_hist, g_hist, r_hist;
	calcHist(&imout[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&imout[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&imout[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow(window, histImage);
}

void leftTrackbarCallback(int position)
{
	changeImage(position, rightScale);
	imshow("Corrected_Image", correctedImage);
	drawHistogram("Corrected_Histo", correctedImage);
}

void rightTrackbarCallback(int position)
{
	changeImage(leftScale, position);
	imshow("Corrected_Image", correctedImage);
	drawHistogram("Corrected_Histo", correctedImage);
}

int main(int argc, char* argv[])
{
	cv::String keys = 
		"{@src |<none>| path to file}"
		"{@savePath |savedImage | path to save file}"
		"{@leftScale |3 | left side scale}"
		"{@rightScale|3 | right side scale}"
		;
	CommandLineParser parser(argc, argv, keys);
	if (!parser.has("@src"))
	{
		cout << "Specify a path to the picture.";
		getchar();
		return 1;
	}
	leftScale = 3;
	rightScale = 3;
	cv::String pathToSave = "savedImage";
	cv::String imagePath = "";
	try{
		leftScale = stoi(parser.get<String>("@leftScale"));
		rightScale = stoi(parser.get<String>("@rightScale"));
		imagePath = parser.get<String>("@src");
		pathToSave = parser.get<String>("@savePath");
	}
	catch (exception e){
		cout << "Something is wrong with parameters you entered.";
		getchar();
		return 1;
	}

	originalImage = imread(imagePath);
	
	cvNamedWindow("Original_Image");
	cvNamedWindow("Original_Histo");
	cvNamedWindow("Corrected_Image");
	cvNamedWindow("Corrected_Histo");

	cvCreateTrackbar("LeftScale", "Corrected_Image", &leftScale, 100, leftTrackbarCallback);
	cvCreateTrackbar("RightScale", "Corrected_Image", &rightScale, 100, rightTrackbarCallback);

	cvMoveWindow("Original_Image", 0, 0);
	cvMoveWindow("Corrected_Image", 750, 0);
	cvMoveWindow("Original_Histo", 0, 280);
	cvMoveWindow("Corrected_Histo", 80, 250);

	imshow("Original_Image", originalImage);
	drawHistogram("Original_Histo", originalImage);
	leftTrackbarCallback(leftScale);
	waitKey(0);
	try{
		imwrite(pathToSave, correctedImage);
		cout << "Changed image successfully saved in" << pathToSave;
		waitKey(0);
	}
	catch(exception e){
		cout << "Wrong path for saving - " << pathToSave;
		waitKey(0);
	}
	return 0;
}