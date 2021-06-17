#Компьютерное зрение
![GitHub Logo](https://miro.medium.com/max/3200/1*Cvwue5II1QjdXIhSPGkVUw.png)


####Распознование объектов на картинке методом цветового анализа, измерение его скорости и пройденного им расстояния.

#  Установка вручную
Чтобы установить последнюю версию OpenCV убедитесь, что вы удалили библиотеки из системы командой
```
sudo apt-get autoremove libopencv-dev python-opencv
```
###1. Обновление Ubuntu или Debian до актуального состояния
Откройте терминал и выполните:
```
sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade && sudo apt-get autoremove
```
###2. Установка зависимостей
Теперь установите зависимости:
Средства сборки:
```
sudo apt-get install build-essential cmake
```
Работа с изображениями:
```
sudo apt-get install zlib1g-dev libjpeg-dev libwebp-dev libpng-dev libtiff5-dev libjasper-dev libopenexr-dev libgdal-dev
```
###3. Видеоввод/вывод:
```
sudo apt-get install libdc1394-22-dev libavcodec-dev libavformat-dev libswscale-dev libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev yasm libfaac-dev libopencore-amrnb-dev libopencore-amrwb-dev libv4l-dev libxine2-dev
```
Библиотеки параллельного исполнения программ и линейной алгебры:
```
sudo apt-get install libtbb-dev libeigen3-dev
```
###4. Загрузка и распаковка OpenCV
Зайдите на официальный сайт OpenCV и скачайте последнюю версию для Linux. После этого распакуйте скачанный файл.
###5. Компиляция и установка OpenCV
В вашем терминале, убедитесь, что вы находитесь в каталоге OpenCV и выполните следующие команды:
```
mkdir build
cd build
cmake -DWITH_QT=ON -DWITH_OPENGL=ON -DFORCE_VTK=ON -DWITH_TBB=ON -DWITH_GDAL=ON -DWITH_XINE=ON -DBUILD_EXAMPLES=ON ..
make -j4
sudo make install
```
###6. Завершение установки

Теперь, настройте OpenCV:
```
sudo ldconfig
```
Теперь, вы установили OpenCV. И вы можете запустить проект.
#Распаковка репозитория
```
cd ~
mkdir UF
cd UF
git clone ..........................
```
## Выполнение 
Выполнение проекта
```
make run
```
Тесты
```
make test
```

# Переменные, классы, функции, используемые в проекте

## Глобальные переменные:
1. max_value_H - максимальное значение ползунка
2. max_value- максимальное значение другого ползунка.
3. window_capture_name - название окна
4. window_detection_name - название второго окна
5. low_H, low_S, low_V - минимальные значения цветовой палитры


## Функции:
1. on_low_H_thresh_trackbar, on_high_V_thresh_trackbar, on_low_V_thresh_trackbar, on_low_S_thresh_trackbar, on_high_S_thresh_trackbar - действия, при изменении значения ползунков
1. pix_toMmetr - нахождение расстояния в пикселях и перевод его в метры.
1. main() - точка входа в программу.

## Классы: 
###fps_meter:
 класс, считающий FPS. 
 - step_Frametime(double now_time) - передается колличество тиков с начала обработки нового кадра, функция сохраняет данные и увеличивает счетчик фпс
 
 - calc_middle_fps() - рассчитывает средний фпс исходя из текущих данных
 
###rasstoyanie :

 класс, считающий расстояние пройденное объектом. 
 - double add_rasstoyanie(int now_rasstoyanie) - передавется расстояние которое прошел объект за 1 кадр, все данные суммируются в переменную now_rasstoyanie
 
 - null_rasstoyanie() - по событию (в данной версии по нажатию кнопи 'w') происходит обнуление общего пройденного расстояния.
 
 
 #Принцип работы кода
 - 