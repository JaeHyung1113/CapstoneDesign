#include"preprocess.h"


VideoCapture capture;

Mat skin(Mat frame) {

	Mat dst, return_skin;
	cvtColor(frame, dst, COLOR_BGR2YCrCb);  //ycrcb 색채 이론이 조도에 대한 민감도가 제일 둔해서 이걸로 사용
											//결과 영상도 제일 깔끔하게 나오는 듯
	//skin.convertTo(skin, CV_8UC3);
	inRange(dst, Scalar(0,133,77), Scalar(255,173,127), dst); //YCrCb에서 사람 피부의 영역  Y=0~255, Cr=133~173, Cb= 77~127
	return_skin = (dst.size(), CV_8UC3, Scalar(0));
	add(frame, Scalar(0), return_skin, dst);
	//skin.convertTo(skin, CV_8UC3);
	//vector<Mat>hls_video(3);
	//split(hlsImg, hls_image);
	//split(dst, hls_video);


	//for (int i = 0;i < dst.rows;i++) {
	//	for (int j = 0;j < dst.cols;j++) {
	//		/*uchar h = hlsImg.at<Vec3b>(i, j)[0];
	//		uchar l = hlsImg.at<Vec3b>(i, j)[1];
	//		uchar s = hlsImg.at<Vec3b>(i, j)[2];*/
	//		uchar y = dst.at<Vec3b>(i, j)[0];
	//		uchar cb = dst.at<Vec3b>(i, j)[1];
	//		uchar cr = dst.at<Vec3b>(i, j)[2];

	//		//double ls_ratio = (double)l / (double)s;
	//		//bool skinPixel = (s >= 50) && (ls_ratio > 0.5) && (ls_ratio < 3.0) && ((h <= 14) || (h >= 165));
	//		bool skinPixel = (y >= 0) && (y <= 255) && (cb >= 77) && (cb <= 127) && (cr >= 133 ) && (cr <= 173);
	//		if (skinPixel == false) {
	//			/*skinImg.at<Vec3b>(i, j)[0] = 0;
	//			skinImg.at<Vec3b>(i, j)[1] = 0;
	//			skinImg.at<Vec3b>(i, j)[2] = 0;*/
	//			dst.at<Vec3b>(i, j)[0] = 0;
	//			dst.at<Vec3b>(i, j)[1] = 0;
	//			dst.at<Vec3b>(i, j)[2] = 0;

	//		}
	//	}
	//}
	return return_skin;
}
int main() {
	capture.open(0);
	if (!capture.isOpened())
	{
		cout << "카메라가 연결되지 않았습니다." << endl;
		exit(1);
	}

	Mat frame[2];	
	capture >> frame[0];
	

	//Mat inputImg, hlsImg, skinImg;

	//inputImg = imread("C:/Users/yoonc/Desktop/영상처리/test2.jpg", IMREAD_COLOR);
	//skinImg = inputImg.clone();

	//cvtColor(inputImg, hlsImg, COLOR_BGR2HLS);
	//vector<Mat>hls_image(3);
	//cvtColor(frame[0], frame[1], COLOR_BGR2HLS);
	//vector<Mat>hls_video(3);
	////split(hlsImg, hls_image);
	//split(frame[1], hls_video);
	namedWindow("origin", WINDOW_AUTOSIZE);
	namedWindow("skin", WINDOW_AUTOSIZE);

	moveWindow("origin", 100, 100);
	moveWindow("skin", 120, 120);

	//imshow("origin", inputImg);
	//imshow("skin", skinImg);
	//waitKey(0);
	while (1) {

		capture.read(frame[0]);
		capture.read(frame[1]);
		frame[1]=skin(frame[1]);
		imshow("origin", frame[0]);
		imshow("skin", frame[1]);
		if (waitKey(30) == 27) break;
	}
	frame->release();
	destroyAllWindows();
	return 0;
}
