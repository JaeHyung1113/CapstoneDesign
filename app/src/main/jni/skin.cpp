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
    } else
        __android_log_print(ANDROID_LOG_DEBUG, "skin :: ",
                            "CascadeClassifier로 로딩 성공 %s", nativeFileNameString);


    env->ReleaseStringUTFChars(cascade_file_name, nativeFileNameString);

    return ret;

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_capstonedesign_MainActivity_loadImage(JNIEnv *env, jobject thiz,
                                                       jstring image_file_name, jlong img) {

    Mat &img_input = *(Mat *) img;

    const char *nativeFileNameString = env->GetStringUTFChars(image_file_name, 0);

    string baseDir("");
    baseDir.append(nativeFileNameString);
    const char *pathDir = baseDir.c_str();

    img_input = imread(pathDir, IMREAD_COLOR);
    CV_Assert(img_input.empty());

}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_capstonedesign_MainActivity_detectSkin(JNIEnv *env, jobject thiz,
                                                        jlong cascade_classifier_face,
                                                        jlong addr_input_image,
                                                        jlong addr_result_image) {
    Mat &matInput = *(Mat *) addr_input_image;
    Mat &matResult = *(Mat *) addr_result_image;
    Mat image_resize;
    Mat bgrImg;
    Mat return_skin;
    Mat dst;
    int count = 0;
    uchar B = 0, G = 0, R = 0;
    float sumB = 0, sumG = 0, sumR = 0;
    vector<Mat> hls_image(3);

    resize(matInput, image_resize, Size(matInput.size().width / 8, matInput.size().height / 8));
    __android_log_print(ANDROID_LOG_INFO, "skin :: ", "resize width: %d, resize height: %d",
                        image_resize.size().width, image_resize.size().height);


    Mat gray = preprocessing(image_resize);
    vector<Rect> faces;
    ((CascadeClassifier *) cascade_classifier_face)->detectMultiScale(gray, faces, 1.1, 3, 0,
                                                                      Size(100, 100));

    cvtColor(image_resize, dst, COLOR_BGR2YCrCb);
    inRange(dst, Scalar(50, 143, 77), Scalar(255, 173, 127),
            dst); //YCrCb에서 사람 피부의 영역  Y=0~255, Cr=133~173, Cb= 77~127
    return_skin = (dst.size(), CV_8UC3, Scalar(0));
    add(image_resize, Scalar(0), return_skin, dst);
    //rectangle(return_skin, faces[0], Scalar(255, 0, 0), 2);
    rectangle(return_skin, faces[0], Scalar(255, 0, 0), 2);
    matResult = return_skin;

    for (int j = faces[0].y; j < faces[0].y + faces[0].height; j++) {
        for (int k = faces[0].x; k < faces[0].x + faces[0].width; k++) {
            split(return_skin, hls_image);
            if (return_skin.at<Vec3b>(j, k)[0] != 0 && return_skin.at<Vec3b>(j, k)[1] != 0 &&
                return_skin.at<Vec3b>(j, k)[2] != 0) {
                B = return_skin.at<Vec3b>(j, k)[0];
                G = return_skin.at<Vec3b>(j, k)[1];
                R = return_skin.at<Vec3b>(j, k)[2];

                sumB += (float) B;
                sumG += (float) G;
                sumR += (float) R;
                count++;
            } else continue;
        }
    }
    float blue = sumB / count;
    float green = sumG / count;
    float red = sumR / count;
/*
    float C = 0, Y = 0, M = 0, K = 0;
    float max, min, v, s, h;
*/
    float arr[2];

    arr[0] = blue / 255;
    arr[1] = green / 255;
    arr[2] = red / 255;

    string r = to_string(arr[2]);
    r += "//";
    r += to_string(arr[1]);
    r += "//";
    r += to_string(arr[0]);

    jstring ret = env->NewStringUTF(r.c_str());


/*
    max = MAX(arr[0], arr[1]);
    max = MAX(max, arr[2]);

    min = MIN(arr[0], arr[1]);
    min = MIN(min, arr[2]);

    v = max;
    s = (max != 0.0) ? (max - min) / max : 0.0;

    float delta = max - min;
    if (arr[2] == max)
        h = (arr[1] - arr[0]) / delta;         // 색상이 Yello와 Magenta사이
    else if (arr[1] == max)
        h = 2.0 + (arr[0] - arr[2]) / delta;     // 색상이 Cyan와 Yello사이
    else if (arr[0] == max)
        h = 4.0 + (arr[2] - arr[1]) / delta;    // 색상이 Magenta와 Cyan사이
    h *= 60.0;
    if (h < 0.0)                            // 색상값을 각도로 바꿈
        h += 360.0;

    K = 1 - max;
    C = (1 - arr[2] - K) / (1 - K);
    M = (1 - arr[1] - K) / (1 - K);
    Y = (1 - arr[0] - K) / (1 - K);

    __android_log_print(ANDROID_LOG_INFO, "skin :: ", " sumB: %f sumG: %f, sumR: %f, count: %d",
                        sumB, sumG, sumR, count);
    __android_log_print(ANDROID_LOG_INFO, "skin :: ", " B: %f G: %f R: %f", blue, green, red);
    __android_log_print(ANDROID_LOG_INFO, "skin :: ", " B': %f G': %f R': %f, max: %f", arr[0],
                        arr[1], arr[2], max);
    __android_log_print(ANDROID_LOG_INFO, "skin :: ", " C: %f, M, %f Y: %f, K: %f", C, M, Y, K);
    __android_log_print(ANDROID_LOG_INFO, "skin :: ", " H: %f, S, %f V: %f", h, s, v);
*/

    return ret;
}
