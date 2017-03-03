#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
  string imgInPath("C:/1ImageProcessing/Programs/image.jpg");
  Mat image(imread(imgInPath, IMREAD_ANYDEPTH | IMREAD_ANYCOLOR));
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
  imshow("", histImage);
  waitKey(0);
}