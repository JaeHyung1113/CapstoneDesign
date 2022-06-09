#include"preprocess.h"


VideoCapture capture;

void ExtractionFrames(VideoCapture& capture) {
	Mat frame[5];
	Mat return_skin[5];
	Mat frame_test[5];
	Mat dst[5];
	Mat mask(3, 3, CV_8UC3, Scalar(0));
	Point h_m = mask.size() / 2;
	CascadeClassifier face_cascade;
	load_cascade(face_cascade, "haarcascade_frontalface_alt2.xml");
	Mat HSV_img, HSV_arr[3];
	for (int i = 0;i < sizeof frame / sizeof frame[0];i++) {

		capture.read(frame[i]);
		if (frame[i].empty()) break;

		Mat gray = preprocessing(frame[i]);
		vector<Rect>faces;
		face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(100, 100));

		cvtColor(frame[i], dst[i], COLOR_BGR2YCrCb);
		inRange(dst[i], Scalar(0, 133, 77), Scalar(255, 173, 127), dst[i]); //YCrCb에서 사람 피부의 영역  Y=0~255, Cr=133~173, Cb= 77~127
		return_skin[i] = (dst[i].size(), CV_8UC3, Scalar(0));
		add(frame[i], Scalar(0), return_skin[i], dst[i]);
	rectangle(return_skin[i], faces[0], Scalar(255, 0, 0), 2);


		imshow("123", return_skin[i]);
		waitKey();

		/*while (!return_skin[i].data) {
			for (int j = h_m.y;j < return_skin[i].rows - h_m.y;j++) {
				for (int k = h_m.x;k < return_skin[i].cols - h_m.x;k++) {
					Point start = Point(k, j) - h_m;
					Rect roi(start, mask.size());
				}
			}
		}*/
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
