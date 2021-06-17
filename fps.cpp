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
/*
max_value_H - максимальное значение ползунка
*/
const int max_value_H = 360 / 2;
/*
max_value- максимальное значение другого ползунка.
*/
const int max_value = 255;
/*
window_capture_name - название окна
*/
const String window_capture_name = "Video Capture";
/*
window_detection_name - название второго окна
*/
const String window_detection_name = "Object Detection";
/*
минимальные значения цветовой палитры
*/
int low_H = 0, low_S = 0, low_V = 0;
/*
максимальные значения цветовой палитры
*/
int high_H = max_value_H, high_S = max_value, high_V = max_value;

/*
действие, при изменении значения ползунков
*/
static void on_low_H_thresh_trackbar(int, void *) {
  low_H = min(high_H - 1, low_H);
  setTrackbarPos("Low H", window_detection_name, low_H);
}
/*
действие, при изменении значения ползунков
*/
static void on_high_H_thresh_trackbar(int, void *) {
  high_H = max(high_H, low_H + 1);
  setTrackbarPos("High H", window_detection_name, high_H);
}
/*
действие, при изменении значения ползунков
*/
static void on_low_S_thresh_trackbar(int, void *) {
  low_S = min(high_S - 1, low_S);
  setTrackbarPos("Low S", window_detection_name, low_S);
}
/*
действие, при изменении значения ползунков
*/
static void on_high_S_thresh_trackbar(int, void *) {
  high_S = max(high_S, low_S + 1);
  setTrackbarPos("High S", window_detection_name, high_S);
}
/*
действие, при изменении значения ползунков
*/
static void on_low_V_thresh_trackbar(int, void *) {
  low_V = min(high_V - 1, low_V);
  setTrackbarPos("Low V", window_detection_name, low_V);
}
/*
действие, при изменении значения ползунков
*/
static void on_high_V_thresh_trackbar(int, void *) {
  high_V = max(high_V, low_V + 1);
  setTrackbarPos("High V", window_detection_name, high_V);
}
/*
ахождение расстояния в пикселях и перевод его в метры.
*/
double pix_toMmetr(Point p1, Point p2) {
  double result =
      sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
  return result / 3.79;
}
/**
 * класс, считающий FPS.
 */
class fps_meter {
public:
  double tiks_current = 0;
  double tiks_summ = 0;
  unsigned long int cnt_frames = 0;
  unsigned long int _cnt_frames = 0;
/**
 * конструктор класса fps_meter
 */

  fps_meter() {}
  /**
 * деструктор класса fps_meter
 */
  ~fps_meter() {}
  /**
 * передается колличество тиков с начала обработки нового кадра, функция сохраняет данные и увеличивает счетчик фпс
 */
  void step_Frametime(double now_time) {
    cnt_frames++;
    _cnt_frames++;
    tiks_summ += now_time;
  }
  /**
 * рассчитывает средний фпс исходя из текущих данных
 */
  float calc_middle_fps() { return (_cnt_frames / tiks_summ) * 1000000000; }
};
/**
 * класс, считающий расстояние пройденное объектом.
 */
class rasstoyanie {
public:
  int now_rasstoyanie = 0;

/**
 * конструктор класса rastoyanie
 */
  rasstoyanie() {}
  /**
 * деструктор класса rastoyanie
 */
  ~rasstoyanie() {}
  /**
 * передавется расстояние которое прошел объект за 1 кадр, все данные суммируются в переменную now_rasstoyanie
 */
  double add_rasstoyanie(int now_rasstoyanie_b) {
    now_rasstoyanie += now_rasstoyanie_b;
    return now_rasstoyanie;
  }
  /**
 * по событию (в данной версии по нажатию кнопи 'w') происходит обнуление общего пройденного расстояния.
 */
  void null_rasstoyanie() { now_rasstoyanie = 0; }
};
#ifdef test_env
  #define main_func main_function 
#endif
#ifndef test_env 
  #define main_func main
#endif

int main_func() {
  /**
 * создание и зануление FPS в начале работы программы
 */
  double fps_buff = 0;
  /**
 * создание и зануление координаты х
 */
  int x = 0;
  /**
 * создание и зануление координаты y
 */
  int y = 0;
  /**
 * создание видеопотока
 */
  VideoCapture cap(0); 
  namedWindow(window_capture_name);
  namedWindow(window_detection_name);
  /**
 * создание и зануление координаты х, которую мы сохраняем в момент данного кадра для последующих вычислений в следующем кдаре
 */
  int lastx = 0;
  /**
 * создание и зануление координаты y, которую мы сохраняем в момент данного кадра для последующих вычислений в следующем кдаре
 */
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
/**
 *создание матрицы преобразований
 */
  Mat frame, frame_HSV, frame_threshold;
   /**
 * создание изображения
 */
  IplImage **dst = 0;
   /**
 * создание буфера на изображение
 */
  auto path = frame;
  int iter = 0;
  /**
 * создание экземпляра класса для подсчета fps
 */
  fps_meter framer;
  /**
 * создание экземпляра класса для подсчета расстояний
  */
  rasstoyanie navigator;
  while (true) {
    /**
 * переменная, содержащая время начала обработки кадра
 */
    int64 start = cv::getTickCount();
    cap >> frame; //получение 1 кадра
    
    if (iter == 0) {
      path = frame;
      cout << "Камеры нет)"; //проверка существования кадра
      iter++;
    }

    if (frame.empty()) { // проверка кадра
      break;
    }
    cvtColor(frame, frame_HSV, COLOR_BGR2HSV);//преобразование rgb to hsv
    inRange(frame_HSV, Scalar(low_H, low_S, low_V),
            Scalar(high_H, high_S, high_V), frame_threshold);//шакаливание
    inRange(frame_HSV, Scalar(53, 55, 147), Scalar(83, 160, 255),
            frame_threshold);// сразу с зеленым цветом без лишнего геморроя
/**
 * вычисление моментов изображения
 */
    auto moments1 = cv::moments(frame_threshold, 1); 
    int dM01 = moments1.m01;    //computing.... моменты по x
    int dM10 = moments1.m10;    //computing.... моменты по y
    int dArea = moments1.m00;   //computing.... моменты в общем случае

    if (dArea > 300) {
      x = int(dM10 / dArea);
      y = int(dM01 / dArea);
      if ((int(fps_buff) * int(sqrt((x - lastx) * (x - lastx) +
                                    (y - lasty) * (y - lasty)))) > 80) {

        circle(frame, Point(x, y), 10, Scalar(255, 0, 0), -1); //рисование точки в центре моментов изображения(искогомго объекта)
  /**
 * создание шрифта, используемого для вывода
 */
        CvFont font;
        cv::putText(frame,
                    "speed pix/t= " +
                        std::to_string(int(fps_buff) *
                                       int(sqrt((x - lastx) * (x - lastx) +
                                                (y - lasty) * (y - lasty)))),
                    Point(x, y), cv::FONT_HERSHEY_PLAIN, 2.0,
                    cv::Scalar(255, 0, 0));
        navigator.add_rasstoyanie(  //суммирование расстояний
            int(sqrt((x - lastx) * (x - lastx) + (y - lasty) * (y - lasty))));
        cv::putText(frame,
                    "speed mm/s = " + std::to_string(int(pix_toMmetr(
                                          Point(x, y), Point(lastx, lasty)))),
                    Point(300, 450), cv::FONT_HERSHEY_PLAIN, 2.0,
                    cv::Scalar(255, 0, 0));
        line(path, Point(lastx, lasty), Point(x, y), Scalar(255, 0, 0), 5); //линия вектора
      } else { //когда оъект стоит
        circle(frame, Point(x, y), 10, Scalar(255, 0, 0), -1);
        /**
 * создание шрифта, используемого для вывода
 */
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
    /**
 * создание переменной буффера ввода пользователя 
 */
    char key;
    key = (char)waitKey(30);
    if (key == 'q' || key == 27) { //выход 

      break;
    }
    if (key == 'w' || key == 28) { //обнуление

      framer.cnt_frames = 0;
      navigator.now_rasstoyanie = 0;
    }
    /**
 * переменная подсчета FPS
 */
    double fps = cv::getTickFrequency() / (cv::getTickCount() - start);
    framer.step_Frametime(cv::getTickCount() - start);
    std::cout << "FPS : " << fps << std::endl;
    std::cout << "cnt_gened fps : " << framer.cnt_frames << std::endl;
    std::cout << "FPS_middle : " << framer.calc_middle_fps() << std::endl;
    fps_buff = fps;
  }
  return 0;
}
