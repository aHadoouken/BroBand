//
// Created by d3vyatk4 on 15.11.2021.
//

#include "detecting.h"
#include "Exception.h"


int PornImageDetector::set_threshold(double _threshold) {
    return 0;
}

int PornImageDetector::load_model(std::string path) {
    return 0;
}

Probability PornImageDetector::forward(Image *data) {
    throw NotImplemented();
}

ImageWrapper* PornImageDetector::blurring(Image *data) {
    throw NotImplemented();
}

int PornTextDetector::set_threshold(double _threshold) {
    throw NotImplemented();
}


int PornTextDetector::load_model(std::string path) {
    return 0;
}

Probability PornTextDetector::forward(Message *data) {
    throw NotImplemented();
}

MessageWrapper *PornTextDetector::text_replace(Message *data) {
    throw NotImplemented();
}
