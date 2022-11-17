#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

enum {
    singleCamera = 0,
};

enum Salmon {
    Green114 = 114,
    Yellow128 = 128,
    Red250 = 250
};

enum Khaki {
    Green140 = 140,
    Yellow230 = 230,
    Red240 = 240
};

enum YellowGreen {
    Green50 = 50,
    Yellow205 = 205,
    Red154 =  154
};

enum DarkMagenta {
    Green139 = 139,
    Yellow0 = 0,
    Red139 = 139
};

using namespace cv;
using namespace std;

CascadeClassifier faceCascadeFrontalFace, faceCascadeEyse;
std::string  path = "output.avi";
std::string configFolder = "../ConfigFolder/";

void detectAndDrawEyesAndFace(Mat& frame) {
    Mat frameGray;
    cvtColor(frame, frameGray, COLOR_BGR2GRAY, 0);
    equalizeHist(frameGray, frameGray);

    std::vector<Rect> faces;
    faceCascadeFrontalFace.detectMultiScale(frame, faces, 1.3, 5);
    std::vector<Rect> eyes;

    for(int i = 0; i < faces.size(); ++i) {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(YellowGreen::Green50,
                                                                                                YellowGreen::Yellow205,
                                                                                                YellowGreen::Red154), 6);

        rectangle(frame, Point(0, 0), Point(100, 50), Scalar(Salmon::Green114,
                                                             Salmon::Yellow128,
                                                             Salmon::Red250),
                                                             FILLED);

        putText(frame, to_string(faces.size()), Point(10, 40), FONT_HERSHEY_DUPLEX, 1, Scalar(Khaki::Green140,
                                                                                              Khaki::Yellow230,
                                                                                              Khaki::Red240),1);

        Mat eyesMat = frameGray(faces[i]);
        faceCascadeEyse.detectMultiScale(eyesMat, eyes);

        for(auto & eye : eyes) {
            Point eyeCenter( faces[i].x + eye.x + eye.width/2, faces[i].y + eye.y + eye.height/2 );
            int radius = cvRound((eye.width + eye.height) * 0.25);
            circle( frame, eyeCenter, radius, Scalar(DarkMagenta:: Green139,
                                                     DarkMagenta::Yellow0,
                                                     DarkMagenta::Red139), 4);
        }
    }
}


void ChangingContrastImage(Mat& frame) {
    Mat new_image = Mat::zeros(frame.size(), frame.type());
    double alpha = 1;
    int beta = 2;
    frame.convertTo(new_image, 1, alpha, beta);
}

void PrintPercentFPS(vector<double>& resultDuration, int countOfFrame) {
    for(int i = 0; i < resultDuration.size(); ++i) {
        std::cout << i+1 <<": percentFPS " << resultDuration[i] / static_cast<double>(countOfFrame) * 100 << "%" << "\n";
    }
}

int main() {
    cv::VideoCapture videoCapture(singleCamera);
    assert(videoCapture.isOpened());

    faceCascadeFrontalFace.load(configFolder +"haarcascade_frontalface_default.xml");
    assert(!faceCascadeFrontalFace.empty());

    faceCascadeEyse.load(configFolder + "haarcascade_eye_tree_eyeglasses.xml");
    assert(!faceCascadeEyse.empty());

    Mat  frame;

    long long countOfFrame = 0;

    std::chrono::time_point<std::chrono::high_resolution_clock> newFrameTime,
                                                                prevFrameTime,
                                                                startTotalTime,
                                                                endTotalTime;
    double resultDurationThreadReadingTime = 0,
           resultDurationTransformationFrame = 0,
           resultOutputFrame = 0,
           resultDurationWaitKey = 0;

    while(true) {
        startTotalTime = std::chrono::high_resolution_clock::now();
        prevFrameTime = std::chrono::high_resolution_clock::now();
        videoCapture >> frame;
        assert(!frame.empty());
        newFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationThreadReadingTime(newFrameTime - prevFrameTime);

        prevFrameTime = std::chrono::high_resolution_clock::now();
        if( waitKey(1) == static_cast<int>('q')) break;
        newFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> durationWaitKeyTime(newFrameTime - prevFrameTime);

        prevFrameTime  =  std::chrono::high_resolution_clock::now();
        flip(frame, frame, 1);
        detectAndDrawEyesAndFace(frame);
        ChangingContrastImage(frame);
        newFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration <double> durationTransformationFrame(newFrameTime - prevFrameTime);

        prevFrameTime  =  std::chrono::high_resolution_clock::now();
        imshow("Show Frame", frame);
        newFrameTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration <double> durationOutputFrame(newFrameTime - prevFrameTime);

        endTotalTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> totalTime = (endTotalTime - startTotalTime);
        int FPS = static_cast<int> (1 / totalTime.count());
        std::cout<< "FPS: " << FPS << "\n";

        resultDurationThreadReadingTime +=  durationThreadReadingTime.count() / totalTime.count();
        resultDurationWaitKey += durationWaitKeyTime.count() / totalTime.count();
        resultDurationTransformationFrame += durationTransformationFrame.count() / totalTime.count();
        resultOutputFrame += durationOutputFrame.count() / totalTime.count();

        countOfFrame++;
    }

    vector<double> resultDuration = {resultDurationThreadReadingTime,
                                     resultDurationWaitKey,
                                     resultDurationTransformationFrame,
                                     resultOutputFrame};

    PrintPercentFPS(resultDuration, countOfFrame);

    return 0;
}
