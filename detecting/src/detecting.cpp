//
// Created by d3vyatk4 on 15.11.2021.
//

#include "detecting.h"
#include "Exception.h"

#include <opencv2/opencv.hpp>                 // for image proccessing
#include <torch/csrc/api/include/torch/cuda.h>// for cuda
#include <torch/script.h>                     // for torchScript

#define HEIGHT 224
#define WIDTH 224

std::vector<double> MEAN = {0.485, 0.456, 0.406};
std::vector<double> STD = {0.229, 0.224, 0.225};

std::ostream &operator<<(std::ostream &out, const Probability &prob) {

    out << "[" << 1 - prob.porn << "," << prob.porn << "]"
        << "\n";
    return out;
}

template<typename T>
int TorchWrapper<T>::set_threshold(double _threshold) {

    if (_threshold > 0.0 && _threshold < 1.0) {
        threshold = _threshold;
    } else {
        throw std::logic_error("Threshold can be from 0 to 1");
    }

    return 0;
}

int PornImageDetector::load_model(const std::string &path) {

    try {
        model = torch::jit::load(path);
    } catch (const c10::Error &ex) {// Error - основной класс ошибок

        std::cerr << "Error loading the model\n";
        return 1;
    }

    std::cout << "Successful load resnet34 model\n";
    return 0;
}

Probability PornImageDetector::forward(torch::Tensor img) {

    torch::NoGradGuard no_grad;// turn off trainable function

    torch::Tensor output = model.forward({img}).toTensor();

    torch::Tensor softmax_output = torch::softmax(output, 1);// вектор вероятностей принадлежности классам size = 2

    std::tuple<torch::Tensor, torch::Tensor> result = torch::max(softmax_output, 1);

    torch::Tensor proba = std::get<0>(result);

    auto tmp = proba.accessor<float, 1>();

    Probability probability;
    probability.porn = tmp[0];

    prob.porn = tmp[0];

    return probability;
}

ImageWrapper *PornImageDetector::blurring(torch::Tensor data) {

    cv::Mat img = load_img("../../test.jpg");

    if (prob.porn > threshold) {
        cv::GaussianBlur(img, img, cv::Size(35, 35), 0);
    }

    cv::imwrite("../../2014.jpg", img);
}

cv::Mat PornImageDetector::load_img(const std::string &path) {

    cv::Mat img = cv::imread(path);
    cv::Size target_size(WIDTH, HEIGHT);// так как вход сети 224 х 224
    cv::resize(img, img, target_size);

    switch (img.channels()) {// каналы изображения

        case 4:
            cv::cvtColor(img, img, cv::COLOR_BGRA2RGB);
            break;

        case 3:
            cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
            break;

        case 1:
            cv::cvtColor(img, img, cv::COLOR_GRAY2RGB);
            break;

        default:
            throw std::runtime_error("Incorrect image depth!");
    }

    return img;
}

torch::Tensor PornImageDetector::preproccesing(cv::Mat img) {

    img.convertTo(img, CV_32FC3, 1 / 255.0);

    torch::Tensor img_tensor = torch::from_blob(img.data,
                                                {img.rows, img.cols, img.channels()},
                                                c10::kFloat);

    img_tensor = img_tensor.permute({2, 0, 1});

    img_tensor.unsqueeze_(0);// добавляем фиктивную ось в тензор (will be dim = 4)

    //    img_tensor = torch::data::transforms::Normalize<>(MEAN, STD)(img_tensor);

    return img_tensor.clone();
}

Probability PornTextDetector::forward(Message data) {
    throw NotImplemented();
}

MessageWrapper *PornTextDetector::text_replace(Message *data) {
    throw NotImplemented();
}
