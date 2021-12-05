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

public:
    friend std::ostream &operator<<(std::ostream &out, const Probability &prob);
};

struct Message {
    std::string text = "";
};

struct MessageWrapper {
    Message *msg;
    size_t sender_id;
    size_t chat_id;
    bool is_replace;

    MessageWrapper() : msg(nullptr), sender_id(0), chat_id(0), is_replace(false) {}
};

struct ImageWrapper {
    cv::Mat *img;
    size_t sender_id;
    size_t chat_id;
    bool is_blur;

    ImageWrapper() : img(nullptr), sender_id(0), chat_id(0), is_blur(false) {}
};

template<typename T>
class TorchWrapper {
protected:
    torch::jit::script::Module model;
    double threshold = 0.5;
    Probability prob;

public:
    virtual Probability forward(T *data) = 0;

    virtual int load_model(const std::string &path) = 0;

    int set_threshold(double _threshold);
};

class PornImageDetector : TorchWrapper<torch::Tensor> {
private:
    cv::Mat orig_img;

public:
    int load_model(const std::string &path) override;

    cv::Mat load_img(const std::string &path);

    void permutation_channels(cv::Mat *img);

    torch::Tensor preproccesing(cv::Mat *img);

    Probability forward(torch::Tensor *data) override;

    ImageWrapper *blurring();
};

class PornTextDetector : TorchWrapper<Message> {
public:
    Probability forward(Message* data) override;

    std::string preproccesing(std::string &text);

    MessageWrapper *text_replace(Message *data);
};


#endif//BROBAND_DETECTING_H
