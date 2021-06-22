#include <opencv2/core/core.hpp>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define test_env 1

#include "doctest.h"
#include "fps.cpp"


TEST_CASE("test for class rasstoyanie") {
    rasstoyanie test_rasstoyanie;
    for(int i = 0; i < 100; i++)
        test_rasstoyanie.add_rasstoyanie(i);
    CHECK(test_rasstoyanie.now_rasstoyanie == 4950); //4950 посчитанно с помощью формулы суммы арифметической прогрессии 

}

TEST_CASE("test for class rasstoyaniе function nll_rasstoyanie") {
    rasstoyanie test_rasstoyanie;
    for(int i = 0; i < 100; i++)
        test_rasstoyanie.add_rasstoyanie(i);
    test_rasstoyanie.null_rasstoyanie();
    CHECK(test_rasstoyanie.now_rasstoyanie == 0); //0 так как обнулили
}

TEST_CASE("test for class fps_meter") {
    fps_meter test_fps_meter;
    for(int i = 0; i < 100; i++)
            test_fps_meter.step_Frametime(i);
    CHECK(int(test_fps_meter.calc_middle_fps()/10000000) == 2);
}				
TEST_CASE("test for FPStometer") { // проверка корректности работы формулы расстояния и перевода расстояния в миллиметры
    CHECK(double(pix_toMmetr(Point(1,1),Point(2,1) )) == 1/3.79);
    CHECK(double(pix_toMmetr(Point(1,1),Point(2,2) )) == sqrt(2)/3.79);
    CHECK(double(pix_toMmetr(Point(7,14),Point(243,54) )) == sqrt((243-7)*(243-7)+(54-14)*(54-14))/3.79);
}		
TEST_CASE("run") { // проверка корректности работы формулы расстояния и перевода расстояния в миллиметры
    CHECK(double(pix_toMmetr(Point(1,1),Point(2,1) )) == 1/3.79);
    CHECK(double(pix_toMmetr(Point(1,1),Point(2,2) )) == sqrt(2)/3.79);
    CHECK(double(pix_toMmetr(Point(7,14),Point(243,54) )) == sqrt((243-7)*(243-7)+(54-14)*(54-14))/3.79);
}		 