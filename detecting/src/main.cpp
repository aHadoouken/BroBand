//
// Created by d3vyatk4 on 16.11.2021.
//

#include "detecting.h"
#include <boost/algorithm/string.hpp>

#define MODEL_IMG "../../model/annotation_resnet34_porn_model.pt"
#define MODEL_TXT "../../model/annotation_porn_text_detector.pt"

#define TEST_IMAGE "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"

int main() {
    
    PornImageDetector img_detector;
    
    img_detector.set_threshold(0.7);
    
    img_detector.load_model(MODEL_IMG);
    
    cv::Mat img = img_detector.load_img(TEST_IMAGE);
    
    torch::Tensor img_tensor = img_detector.preproccesing(img);
    
    Probability probability = img_detector.forward(img_tensor);
    
    std::cout << probability;
    
    std::string base64_code = img_detector.blurring();
    
    PornTextDetector txt_detector;
    
    txt_detector.load_model(MODEL_TXT);
    
    std::vector<std::string> txt{};

    txt.push_back(std::string("порно смотреть онлайн"));
    txt.push_back(std::string("изучение с++ в рамках учебного поекта"));
    txt.push_back(std::string("сок и виски под окном"));
    txt.push_back(std::string("использование разных проституток"));
    txt.push_back(std::string("убля шлюхи и проститутки"));
    
    for (std::string word : txt) {
        
        auto res = txt_detector.preproccesing(word);

        auto prob = txt_detector.forward(res);

        std::cout << "\n";
        std::cout << word << "\n";
        std::cout << prob;
        std::cout << txt_detector.text_replace() << "\n\n";
    }

    return 0;
}
