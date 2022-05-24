#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

string path = "";  //파일 저장 경로인데 우리는 얼굴 프레임 파일 저장 따로없이 바로 계산할것이기 때문에 나중에 삭제 실험해보려면 하셈
string type = ".jpg"; //파일 저장 확장자


void ExtractionFrames(VideoCapture& capture) {
    double fps = capture.get(CAP_PROP_FPS);
    cout << fps << endl;

    int delay = cvRound(10000 / fps);
    int index = 1;

    while (index < 50) {
        Mat frame;
        stringstream ss;

        capture.read(frame);
        if (frame.empty()) break;

        ss << path << index << type;
        string filename = ss.str();
        ss.str("");

        imwrite(filename, frame);
        ss << "save image" << index << type;
        string save_notice = ss.str();
        cout << save_notice << endl;

        index++;
    }
}

int main() {
    VideoCapture capture;
    capture.open(0);
    ExtractionFrames(capture);
    waitKey();
    return 0;
}
