#include"preprocess.h"


VideoCapture capture;

void ExtractionFrames(Mat image) {
	Mat frame[5];
	Mat return_skin;
	Mat dst;
	int count = 0;
	uchar B = 0, G = 0, R = 0;
	float sumB = 0, sumG = 0, sumR = 0;
	float C, Y, M, K;
	vector<Mat>hls_image(3);
	CascadeClassifier face_cascade;
	load_cascade(face_cascade, "haarcascade_frontalface_alt2.xml");
	Mat hls_img[1];

	Mat gray = preprocessing(image);
	vector<Rect>faces;
	face_cascade.detectMultiScale(gray, faces, 1.1, 3, 0, Size(100, 100));

	cvtColor(image, dst, COLOR_BGR2YCrCb);
	inRange(dst, Scalar(50, 143, 77), Scalar(255, 173, 127), dst); //YCrCb에서 사람 피부의 영역  Y=0~255, Cr=133~173, Cb= 77~127
	return_skin = (dst.size(), CV_8UC3, Scalar(0));
	add(image, Scalar(0), return_skin, dst);
	//rectangle(return_skin, faces[0], Scalar(255, 0, 0), 2);

	imshow("결과 영상", return_skin);
	waitKey();
	for (int j = faces[0].y;j < faces[0].y + faces[0].height;j++) {
		for (int k = faces[0].x;k < faces[0].x + faces[0].width;k++) {
			split(return_skin, hls_image);
			if (return_skin.at<Vec3b>(j, k)[0] != 0 && return_skin.at<Vec3b>(j, k)[1] != 0 && return_skin.at<Vec3b>(j, k)[2] != 0) {
				B = return_skin.at<Vec3b>(j, k)[0];
				G = return_skin.at<Vec3b>(j, k)[1];
				R = return_skin.at<Vec3b>(j, k)[2];
				sumB += (float)B;
				sumG += (float)G;
				sumR += (float)R;
				count++;
			}
			else continue;
		}
	}
	C = 255 - (sumR / count);
	Y = 255 - (sumB / count);
	M = 255 - (sumG / count);
	cout << "C : " << C << endl;
	cout << "M : " << M << endl;
	cout << "Y : " << Y << endl;
}

int main() {
	Mat image = imread("C:/Users/yoonc/OneDrive - sunmoon.ac.kr/바탕 화면/얼굴 테스트용/KakaoTalk_20220611_175004447.jpg", IMREAD_COLOR);
	Mat image_resize;
	resize(image, image_resize, Size(image.size().width / 4, image.size().height / 4));
	ExtractionFrames(image_resize);
	waitKey();
	destroyAllWindows();
	return 0;
}
