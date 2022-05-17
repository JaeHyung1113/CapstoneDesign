#include "preprocess.h"

Point2d calc_center(Rect obj) {
	Point2d c = (Point2d)obj.size() / 2.0;
	Point2d center = (Point2d)obj.tl() + c;
	
	return center;
}

int main() {
	CascadeClassifier face_cascade, eyes_cascade;
	load_cascade(face_cascade, "haarcascade_frontalface_alt2.xml");
	load_cascade(eyes_cascade, "haarcascade_eye.xml");

	Mat image = imread("C:/Users/yoonc/OneDrive - sunmoon.ac.kr/바탕 화면/영상처리/13.jpg", IMREAD_COLOR);
	CV_Assert(image.data);
	Mat gray = preprocessing(image);

	vector<Rect>faces, eyes;
	vector<Point2d>eyes_center;
	face_cascade.detectMultiScale(gray, faces, 1.15, 2, 0, Size(100, 100));

	if (faces.size() > 0) {
		eyes_cascade.detectMultiScale(gray(faces[0]), eyes, 1.15, 7, 0, Size(25, 20));

		if (eyes.size() == 2) {
			for (size_t i = 0;i < eyes.size();i++) {
				Point2d center = calc_center(eyes[i] + faces[0].tl());
			}
		}

		rectangle(image, faces[0], Scalar(255, 0, 0), 2);
		imshow("image", image);
		waitKey();
	}
	return 0;
}
