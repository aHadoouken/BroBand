//
// Created by d3vyatk4 on 15.11.2021.
//

#ifndef BROBAND_DETECTING_H
#define BROBAND_DETECTING_H

#include <vector>
#include <iostream>

struct Probability {
    std::vector<double>* predict_proba;

    Probability() : predict_proba(nullptr) {}
};

struct Message {
    std::string text = "";
};

struct MessageWrapper {
    Message* msg;
    std::size_t senderId;
    std::size_t chatId;
    bool isReplace;

    MessageWrapper() : msg(nullptr), senderId(0), chatId(0), isReplace(false) {}
};

struct Image {
    std::vector<double> data;
};

struct ImageWrapper {
    Image* img;
    std::size_t senderId;
    std::size_t chatId;
    bool isBlur;

    ImageWrapper() : img(nullptr), senderId(0), chatId(0), isBlur(false) {}
};

template <typename T>
class TorchWrapper {
private:
    std::string saved_model = "";
    double threshold = 0.5;
    Probability prob;

public:
    virtual int set_threshold(double _threshold) = 0;

    virtual int load_model(std::string path) = 0;

    virtual Probability* forward(T* data) = 0;

};

class PornImageDetector : TorchWrapper<Image> {
public:
    int set_threshold(double _threshold) override;

    int load_model(std::string path) override;

    Probability* forward(Image* data) override;

    ImageWrapper* blurring(Image* data);

};

class PornTextDetector : TorchWrapper<Message> {
public:
    int set_threshold (double _threshold) override;

    int load_model(std::string path) override;

    Probability* forward(Message* data) override;

    MessageWrapper* text_replace(Message* data);
};



#endif //BROBAND_DETECTING_H
