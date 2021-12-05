//
// Created by d3vyatk4 on 16.11.2021.
//

#include "detecting.h"

#define RESNET32 "../../model/annotation_resnet34_porn_model.pt"

int main() {

    // создаем объект
    PornImageDetector img_detector;

    // загружаем модель
    img_detector.load_model(RESNET32);

    // читаем изображение
    cv::Mat img = img_detector.load_img("../../test.jpg");

    // предобработка
    torch::Tensor img_tensor = img_detector.preproccesing(&img);

    // прогоняем по сетке изображение
    Probability probability = img_detector.forward(&img_tensor);

    std::cout << probability;

    // блюрим при необходимости
    img_detector.blurring();

    return 0;
}
