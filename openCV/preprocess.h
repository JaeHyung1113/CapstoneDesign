#pragma once
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void load_cascade(CascadeClassifier& cascade, string fname) {
	String path = "C:/opencv455/sources/data/haarcascades/";
	String full_name = path + fname;

	CV_Assert(cascade.load(full_name));
}

Mat preprocessing(Mat image) {
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	equalizeHist(gray, gray);

	return gray;
}
