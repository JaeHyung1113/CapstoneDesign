#include"preprocess.h"


VideoCapture capture;

Point2d calc_center(Rect obj) {
   Point2d c = (Point2d)obj.size() / 2.0;
   Point2d center = (Point2d)obj.tl() + c;

   return center;
}

Mat* ExtractionFrames(VideoCapture& capture) {
   Mat frame[50];
   CascadeClassifier face_cascade, eyes_cascade;
   load_cascade(face_cascade, "haarcascade_frontalface_alt2.xml");
   load_cascade(eyes_cascade, "haarcascade_eye.xml");
   for(int i=0;i<sizeof frame/ sizeof frame[0];i++) {
      capture.read(frame[i]);
      if (frame[i].empty()) break;
      Mat gray = preprocessing(frame[i]);

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

         rectangle(frame[i], faces[0], Scalar(255, 0, 0), 2);
      }
   }
   return frame;
}

//Mat preprocessing(Mat image) {
//   Mat gray;
//   cvtColor(image, gray, COLOR_BGR2GRAY);
//   equalizeHist(gray, gray);
//
//   return gray;
//}

Mat* skin(Mat* frame) {
   
   Mat dst, return_skin[50];
   for (int i = 0;i < sizeof frame / sizeof frame[0];i++) {
      cvtColor(frame[i], dst, COLOR_BGR2YCrCb);  //ycrcb 색채 이론이 조도에 대한 민감도가 제일 둔해서 이걸로 사용
   //                              //결과 영상도 제일 깔끔하게 나오는 듯
   ////skin.convertTo(skin, CV_8UC3);
      inRange(dst, Scalar(0, 133, 77), Scalar(255, 173, 127), dst); //YCrCb에서 사람 피부의 영역  Y=0~255, Cr=133~173, Cb= 77~127
      return_skin[i] = (dst.size(), CV_8UC3, Scalar(0));
      add(frame[i], Scalar(0), return_skin[i], dst);
      //rectangle(face, faces[0], Scalar(255, 0, 0), 2);
      //imshow("test", face);
      imshow("123", return_skin[i]);
      waitKey();
   }
   return return_skin;
}

int main() {
   capture.open(0);
   if (!capture.isOpened()) {

      cout << "카메라가 연결되지 않았습니다." << endl;
      exit(1);   
   }

   Mat* frames = ExtractionFrames(capture);
   Mat* skins = skin(frames);

   namedWindow("origin", WINDOW_AUTOSIZE);
   namedWindow("skin", WINDOW_AUTOSIZE);

   moveWindow("origin", 100, 100);
   moveWindow("skin", 120, 120);
      //imshow("origin", inputImg);
      //imshow("skin", skinImg);
      //waitKey(0);
   destroyAllWindows();
   return 0;
}
