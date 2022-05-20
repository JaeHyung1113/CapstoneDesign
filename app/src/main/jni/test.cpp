#include <jni.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//
// Created by hwc11 on 2022-05-12.
//


extern "C"
JNIEXPORT void JNICALL
Java_com_example_capstonedesign_MainActivity_test(JNIEnv *env, jobject thiz, jlong mat_addr_input,
                                                  jlong mat_addr_result) {
    Mat &matInput = *(Mat *)mat_addr_input;
    Mat &matResult = *(Mat *)mat_addr_result;
    Mat skin;

    cvtColor(matInput, skin, COLOR_BGR2YCrCb);
    inRange(skin, Scalar(0,133,77), Scalar(255,173,127), skin);
    matResult = (skin.size(), CV_8UC3, Scalar(0));
    add(matInput, Scalar(0), matResult, skin);
}

