#include"preprocess.h"


VideoCapture capture;

Point2d calc_center(Rect obj) {
	Point2d c = (Point2d)obj.size() / 2.0;
	Point2d center = (Point2d)obj.tl() + c;

	return center;
}

void ExtractionFrames(VideoCapture& capture) {
	Mat frame[5];
	Mat return_skin[5];
	Mat frame_test[5];
	Mat dst[5];
	Mat mask(3, 3, CV_8UC3, Scalar(0));
	Point h_m = mask.size() / 2;
	CascadeClassifier face_cascade, eyes_cascade;
	load_cascade(face_cascade, "haarcascade_frontalface_alt2.xml");
	load_cascade(eyes_cascade, "haarcascade_eye.xml");
	for(int i=0;i<sizeof frame/ sizeof frame[0];i++) {
		capture.read(frame[i]);
		if (frame[i].empty()) break;
		Mat gray = preprocessing(frame[i]);

		vector<Rect>faces, eyes;
		vector<Point2d>eyes_center;
		face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(100, 100));
		//cvtColor(frame[i], dst[i], COLOR_BGR2YCrCb);
		//inRange(dst[i], Scalar(0, 133, 77), Scalar(255, 173, 127), dst[i]); //YCrCb에서 사람 피부의 영역  Y=0~255, Cr=133~173, Cb= 77~127
		//return_skin[i] = (dst[i].size(), CV_8UC3, Scalar(0));
		//add(frame[i], Scalar(0), return_skin[i], dst[i]);
		//imshow("test", return_skin[i]);
		//	waitKey();
		

		if (faces.size() > 0) {
			eyes_cascade.detectMultiScale(gray(faces[0]), eyes, 1.15, 7, 0, Size(25, 20));

			if (eyes.size() == 2) {
				for (size_t i = 0;i < eyes.size();i++) {
					Point2d center = calc_center(eyes[i] + faces[0].tl());
				}
			}

			//rectangle(frame[i], faces[0], Scalar(255, 0, 0), 2);
			cvtColor(frame[i], dst[i], COLOR_BGR2YCrCb);
			inRange(dst[i], Scalar(0, 133, 77), Scalar(255, 173, 127), dst[i]); //YCrCb에서 사람 피부의 영역  Y=0~255, Cr=133~173, Cb= 77~127
			return_skin[i] = (dst[i].size(), CV_8UC3, Scalar(0));
			add(frame[i], Scalar(0), return_skin[i], dst[i]);
			rectangle(return_skin[i], faces[0], Scalar(255, 0, 0), 2);
			//imshow("test", return_skin[i]);
			imshow("123", return_skin[i]);
			waitKey();
			while(!return_skin[i].data) {
				
			}
		}
	}
}

int main() {
	capture.open(0);
	if (!capture.isOpened()) {

		cout << "카메라가 연결되지 않았습니다." << endl;
		exit(1);	
	}

	ExtractionFrames(capture);
	waitKey();
	destroyAllWindows();
	return 0;
}
