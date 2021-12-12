//
// Created by d3vyatk4 on 15.11.2021.
//

#ifndef BROBAND_DETECTING_H
#define BROBAND_DETECTING_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <torch/script.h>
#include <vector>

typedef std::vector<std::string> Token;

struct Probability {

    double porn;

    Probability() : porn(0) {}

    Probability(const double &prob) : porn(prob) {}

public:
    friend std::ostream &operator<<(std::ostream &out, const Probability &prob);
};

template<typename T>
class TorchWrapper {
protected:
    torch::jit::script::Module model;
    double threshold = 0.5;
    Probability prob;

public:
    TorchWrapper();

    virtual int set_threshold(double _threshold);

    virtual Probability forward(T &data) = 0;

    virtual int load_model(const std::string &path);
};

class PornImageDetector : public TorchWrapper<torch::Tensor> {
private:
    cv::Mat orig_img;

    static cv::Mat base642mat(const std::string &base64_code);

    static std::string mat2base64(const cv::Mat &img);

public:

    PornImageDetector() = default;

    PornImageDetector(const std::string &path_to_model);

    ~PornImageDetector();

    cv::Mat load_img(const std::string &base64_code);

    static void permutation_channels(cv::Mat &img);

    torch::Tensor preproccesing(cv::Mat &img);

    Probability forward(torch::Tensor &data) override;

    std::string blurring();
};

class PornTextDetector : public TorchWrapper<Token> {
private:
    std::string msg;

    std::vector<std::string> get_stopwords();

public:

    PornTextDetector();

    void remove_bad_syms_words(Token &token);

    Probability forward(Token &data) override;

    Token preproccesing(std::string &text);

    std::string text_replace(Token data);
};

#endif//BROBAND_DETECTING_H
