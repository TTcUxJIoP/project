#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <string>

using namespace cv;
using namespace std;
const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;
static void on_low_H_thresh_trackbar(int, void *) {
  low_H = min(high_H - 1, low_H);
  setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *) {
  high_H = max(high_H, low_H + 1);
  setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *) {
  low_S = min(high_S - 1, low_S);
  setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *) {
  high_S = max(high_S, low_S + 1);
  setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *) {
  low_V = min(high_V - 1, low_V);
  setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *) {
  high_V = max(high_V, low_V + 1);
  setTrackbarPos("High V", window_detection_name, high_V);
}

double pix_toMmetr(Point p1, Point p2) {
  double result =
      sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
  return result / 3.79;
}
class fps_meter {
public:
  double tiks_current = 0;
  double tiks_summ = 0;
  unsigned long int cnt_frames = 0;
  unsigned long int _cnt_frames = 0;

  fps_meter() {}
  ~fps_meter() {}
  void step_Frametime(double now_time) {
    cnt_frames++;
    _cnt_frames++;
    tiks_summ += now_time;
  }
  float calc_middle_fps() { return (_cnt_frames / tiks_summ) * 1000000000; }
};

class rasstoyanie {
public:
  int now_rasstoyanie = 0;

  rasstoyanie() {}
  ~rasstoyanie() {}
  double add_rasstoyanie(int now_rasstoyanie) {
    now_rasstoyanie += now_rasstoyanie;
    return now_rasstoyanie;
  }
  void null_rasstoyanie() { now_rasstoyanie = 0; }
};
#ifdef test_env
  #define main_func main_function 
#endif
#ifndef test_env 
  #define main_func main
#endif

int main_func() {
  double fps_buff = 0;
  int x = 0;
  int y = 0;
  VideoCapture cap(0);
  namedWindow(window_capture_name);
  namedWindow(window_detection_name);
  int lastx = 0;
  int lasty = 0;
  createTrackbar("Low H", window_detection_name, &low_H, max_value_H,
                 on_low_H_thresh_trackbar);
  createTrackbar("High H", window_detection_name, &high_H, max_value_H,
                 on_high_H_thresh_trackbar);
  createTrackbar("Low S", window_detection_name, &low_S, max_value,
                 on_low_S_thresh_trackbar);
  createTrackbar("High S", window_detection_name, &high_S, max_value,
                 on_high_S_thresh_trackbar);
  createTrackbar("Low V", window_detection_name, &low_V, max_value,
                 on_low_V_thresh_trackbar);
  createTrackbar("High V", window_detection_name, &high_V, max_value,
                 on_high_V_thresh_trackbar);
  Mat frame, frame_HSV, frame_threshold;
  IplImage **dst = 0;
  auto path = frame;
  int iter = 0;
  fps_meter framer;
  rasstoyanie navigator;
  while (true) {
    int64 start = cv::getTickCount();

    cap >> frame;
    if (iter == 0) {
      path = frame;
      cout << "Камеры нет)";
      iter++;
    }

    if (frame.empty()) {
      break;
    }
    cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
    inRange(frame_HSV, Scalar(low_H, low_S, low_V),
            Scalar(high_H, high_S, high_V), frame_threshold);
    int Xc = 0;
    int Yc = 0;
    int counter = 0; // счётчик числа белых пикселей
    inRange(frame_HSV, Scalar(53, 55, 147), Scalar(83, 160, 255),
            frame_threshold);

    auto moments1 = cv::moments(frame_threshold, 1);
    int dM01 = moments1.m01;
    int dM10 = moments1.m10;
    int dArea = moments1.m00;

    if (dArea > 300) {
      x = int(dM10 / dArea);
      y = int(dM01 / dArea);
      if ((int(fps_buff) * int(sqrt((x - lastx) * (x - lastx) +
                                    (y - lasty) * (y - lasty)))) > 80) {

        circle(frame, Point(x, y), 10, Scalar(255, 0, 0), -1);
        CvFont font;
        cv::putText(frame,
                    "speed pix/t= " +
                        std::to_string(int(fps_buff) *
                                       int(sqrt((x - lastx) * (x - lastx) +
                                                (y - lasty) * (y - lasty)))),
                    Point(x, y), cv::FONT_HERSHEY_PLAIN, 2.0,
                    cv::Scalar(255, 0, 0));
        navigator.add_rasstoyanie(
            int(sqrt((x - lastx) * (x - lastx) + (y - lasty) * (y - lasty))));
        cv::putText(frame,
                    "speed mm/s = " + std::to_string(int(pix_toMmetr(
                                          Point(x, y), Point(lastx, lasty)))),
                    Point(300, 450), cv::FONT_HERSHEY_PLAIN, 2.0,
                    cv::Scalar(255, 0, 0));
        line(path, Point(lastx, lasty), Point(x, y), Scalar(255, 0, 0), 5);
      } else {
        circle(frame, Point(x, y), 10, Scalar(255, 0, 0), -1);
        CvFont font;
        cv::putText(frame, "speed pix/t= 0", Point(x, y),
                    cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(255, 0, 0));
        cv::putText(frame, "speed mm/s = 0", Point(300, 450),
                    cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(255, 0, 0));
      }
    }
    cv::putText(
        frame,
        "FPS middle = " +
            std::to_string(round(framer.calc_middle_fps() * 1000) / 1000),
        Point(280, 415), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(255, 0, 0));
    cv::putText(frame, "Frames: " + std::to_string(framer.cnt_frames),
                Point(20, 415), cv::FONT_HERSHEY_PLAIN, 2.0,
                cv::Scalar(0, 255, 0));
    cv::putText(frame, "Distance: " + std::to_string(navigator.now_rasstoyanie),
                Point(20, 20), cv::FONT_HERSHEY_PLAIN, 2.0,
                cv::Scalar(0, 255, 0));
    lastx = x;

    lasty = y;
    imshow(window_capture_name, path);
    imshow(window_detection_name, frame_threshold);
    char key;
    key = (char)waitKey(30);
    if (key == 'q' || key == 27) {

      break;
    }
    if (key == 'w' || key == 28) {

      framer.cnt_frames = 0;
      navigator.now_rasstoyanie = 0;
    }
    double fps = cv::getTickFrequency() / (cv::getTickCount() - start);
    framer.step_Frametime(cv::getTickCount() - start);
    std::cout << "FPS : " << fps << std::endl;
    std::cout << "cnt_gened fps : " << framer.cnt_frames << std::endl;
    std::cout << "FPS_middle : " << framer.calc_middle_fps() << std::endl;
    fps_buff = fps;
  }
  return 0;
}
