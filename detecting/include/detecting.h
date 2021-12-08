//
// Created by d3vyatk4 on 15.11.2021.
//

#ifndef BROBAND_DETECTING_H
#define BROBAND_DETECTING_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <torch/script.h>
#include <vector>

struct Probability {

    double porn;

    Probability() : porn(0) {}

    Probability(const double &prob) : porn(prob) {}

public:
    friend std::ostream &operator<<(std::ostream &out, const Probability &prob);
};

struct Message {
    std::string text = "";
};

template<typename T>
class TorchWrapper {
protected:
    torch::jit::script::Module model;
    double threshold = 0.5;
    Probability prob;

public:
    int set_threshold(double _threshold);

    virtual Probability forward(T &data) = 0;

    virtual int load_model(const std::string &path) = 0;
};

class PornImageDetector : TorchWrapper<torch::Tensor> {
private:
    cv::Mat orig_img;

    cv::Mat base642mat(const std::string &base64_code);

    std::string mat2base64(const cv::Mat &img);

public:
    int load_model(const std::string &path) override;

    cv::Mat load_img(const std::string &path);

    void permutation_channels(cv::Mat &img);

    torch::Tensor preproccesing(cv::Mat &img);

    Probability forward(torch::Tensor &data) override;

    std::string blurring();
};

class PornTextDetector : TorchWrapper<Message> {
public:
    Probability forward(Message &data) override;

    std::string preproccesing(std::string &text);

     std::string text_replace(Message *data);
};

#endif//BROBAND_DETECTING_H
