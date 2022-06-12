#include <jni.h>
#include <opencv2/opencv.hpp>
#include <android/log.h>


using namespace cv;
using namespace std;


Mat preprocessing(Mat image) {
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    return gray;
}


extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_capstonedesign_MainActivity_loadCascade(JNIEnv *env, jobject thiz,
                                                         jstring cascade_file_name) {

    const char *nativeFileNameString = env->GetStringUTFChars(cascade_file_name, 0);

    string baseDir("");
    baseDir.append(nativeFileNameString);
    const char *pathDir = baseDir.c_str();

    jlong ret = 0;
    ret = (jlong) new CascadeClassifier(pathDir);
    if (((CascadeClassifier *) ret)->empty()) {
        __android_log_print(ANDROID_LOG_DEBUG, "skin :: ",
                            "CascadeClassifier로 로딩 실패  %s", nativeFileNameString);
    }
    else
        __android_log_print(ANDROID_LOG_DEBUG, "skin :: ",
                            "CascadeClassifier로 로딩 성공 %s", nativeFileNameString);


    env->ReleaseStringUTFChars(cascade_file_name, nativeFileNameString);

    return ret;

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_capstonedesign_MainActivity_detectSkin(JNIEnv *env, jobject thiz,
                                                        jlong cascade_classifier_face,
                                                        jlong addr_input_image,
                                                        jlong addr_result_image) {
    Mat &matInput = *(Mat *) addr_input_image;
    Mat &matResult = *(Mat *) addr_result_image;

    Mat bgrImg;
    Mat return_skin;
    Mat dst;
    int count = 0;
    uchar B = 0, G = 0, R = 0;
    float sumB = 0, sumG = 0, sumR = 0;
    float C = 0, Y = 0, M = 0;
    vector<Mat>hls_image(3);


    cvtColor(matInput, bgrImg, COLOR_RGBA2BGR);


    Mat gray = preprocessing(bgrImg);
    vector<Rect>faces;
    ((CascadeClassifier *) cascade_classifier_face)->detectMultiScale( gray, faces, 1.1, 3, 0, Size(100, 100) );

    cvtColor(matInput, dst, COLOR_BGR2YCrCb);
    inRange(dst, Scalar(0, 128, 73), Scalar(255, 170, 158), dst); //YCrCb에서 사람 피부의 영역  Y=0~255, Cr=133~173, Cb= 77~127
    return_skin = (dst.size(), CV_8UC3, Scalar(0));
    add(matInput, Scalar(0), return_skin, dst);
    //rectangle(return_skin, faces[0], Scalar(255, 0, 0), 2);

    matResult = return_skin;

    for (int j = 0;j < return_skin.rows; j++) {
        for (int k = 0;k < return_skin.cols; k++) {
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
    C =  255 - (sumR / count);
    Y =  255 - (sumB / count);
    M =  255 - (sumG / count);
    __android_log_print(ANDROID_LOG_INFO, "skin :: ", " sumB: %f sumG: %f, sumR: %f, count: %d", sumB, sumG, sumR, count);
    __android_log_print(ANDROID_LOG_INFO, "skin :: ", " C: %f Y: %f M: %f", C, Y, M);

}