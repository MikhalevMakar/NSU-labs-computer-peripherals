#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <ctime>

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

int main() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    cv::VideoCapture videoCapture(singleCamera);
    assert(videoCapture.isOpened());

    faceCascadeFrontalFace.load("../ConfigFolder/haarcascade_frontalface_default.xml");
    assert(!faceCascadeFrontalFace.empty());

    faceCascadeEyse.load("../ConfigFolder/haarcascade_eye_tree_eyeglasses.xml");
    assert(!faceCascadeEyse.empty());

    Mat  frame, image;

    int countFrame = 0;

    while(true) {
        videoCapture >> frame;
        assert(!frame.empty());

        detectAndDrawEyesAndFace(frame);
        ChangingContrastImage(frame);
        imshow("Show Frame", frame);
        if( waitKey(1) == static_cast<int>('q')) break;

        countFrame++;
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("FPS %lf \n", static_cast<double>(countFrame) / (end.tv_sec - start.tv_sec +
                                                                         0.000000001 * (end.tv_nsec - start.tv_nsec)));
    return 0;
}
