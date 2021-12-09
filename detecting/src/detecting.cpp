//
// Created by d3vyatk4 on 15.11.2021.
//

#include "detecting.h"
#include "Exception.h"

#include <opencv2/opencv.hpp>     // for image proccessing
#include <torch/script.h>         // for torchScript
#include <boost/beast.hpp>        // for base64

#define HEIGHT 224
#define WIDTH 224

std::vector<double> MEAN = {0.485, 0.456, 0.406};
std::vector<double> STD = {0.229, 0.224, 0.225};

std::ostream &operator<<(std::ostream &out, const Probability &prob)
{
    out << "[" << 1 - prob.porn << "," << prob.porn << "]"
        << "\n";
    return out;
}

template<typename T>
TorchWrapper<T>::TorchWrapper() {}

template<typename T>
int TorchWrapper<T>::set_threshold(double _threshold)
{
    if (_threshold > 0.0 && _threshold < 1.0)
    {
        threshold = _threshold;
    }
    else
    {
        throw std::logic_error("Threshold can be from 0 to 1");
    }

    return 0;
}

template<typename T>
int TorchWrapper<T>::load_model(const std::string &path)
{
    try
    {
        model = torch::jit::load(path);
        // Error - основной класс ошибок
    }
    catch (const c10::Error &ex)
    {
        std::cerr << "Error loading the model\n";
        return 1;
    }

    std::cout << "Successful load DL model\n";
    return 0;
}

PornImageDetector::PornImageDetector() {}

Probability PornImageDetector::forward(torch::Tensor &img)
{
    torch::NoGradGuard no_grad;// turn off trainable function

    torch::Tensor output = model.forward({img}).toTensor();

    // вектор вероятностей принадлежности классам size = 2
    std::tuple<torch::Tensor, torch::Tensor> result = torch::max(torch::softmax(output, 1) , 1);

    torch::Tensor proba_ = std::get<0>(result);
    torch::Tensor index = std::get<1>(result);

    auto proba = proba_.accessor<float, 1>();
    auto idx = index.accessor<long, 1>();

    if (!idx[0]) {
        Probability probability(1 - proba[0]);
        prob.porn = 1 - proba[0];
        return probability;
    } else {
        Probability probability(proba[0]);
        prob.porn = proba[0];
        return probability;
    }

}

std::string PornImageDetector::blurring()
{

    if (prob.porn > threshold)
    {
        cv::GaussianBlur(orig_img, orig_img,
                         cv::Size(31, 31),
                         0);
    }

    return mat2base64(orig_img);
}

void PornImageDetector::permutation_channels(cv::Mat &img)
{
    // каналы изображения
    switch (img.channels())
    {
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
}

cv::Mat PornImageDetector::base642mat(const std::string &base64_code)
{
    std::string dest;

    // кодирование base64 -> jpg -> Mat
    dest.resize(boost::beast::detail::base64::decoded_size(base64_code.size()));
    auto const result = boost::beast::detail::base64::decode(&dest[0], base64_code.data(), base64_code.size());
    dest.resize(result.first);

    std::vector<uchar> data(dest.begin(), dest.end());
    return cv::imdecode(cv::Mat(data), 1);
}

std::string PornImageDetector::mat2base64(const cv::Mat &img)
{
    std::string dest;

    // кодирование Mat -> jpg -> base64
    std::vector<unsigned char> buffer;
    cv::imencode(".jpg", img, buffer);

    dest.resize(boost::beast::detail::base64::encoded_size(buffer.size()));

    boost::beast::detail::base64::encode(&dest[0], buffer.data(), buffer.size());

    return dest;
}

cv::Mat PornImageDetector::load_img(const std::string &base64_code)
{
    // с этим объектом будут происходить манипуляции
    cv::Mat img = base642mat(base64_code);

    // этот объект блюрим --- оригинал
    orig_img = base642mat(base64_code);

    try
    {
        // BGR to RGB
        permutation_channels(img);
    }
    catch (const cv::Exception &ex)
    {
        // если не получилось, работаем с BGR
        std::cout << "Using BGR format for image\n";
    }

    return img;
}

torch::Tensor PornImageDetector::preproccesing(cv::Mat &img)
{
    // так как вход сети 224 х 224 кропаем изображение
    cv::Size target_size(WIDTH, HEIGHT);
    cv::resize(img, img, target_size);

    img.convertTo(img, CV_32FC3, 1 / 255.0);

    torch::Tensor img_tensor = torch::from_blob(img.data,
                                                {img.rows, img.cols, img.channels()},
                                                c10::kFloat);

    img_tensor = img_tensor.permute({2, 0, 1});

    // добавляем фиктивную ось в тензор (will be dim = 4)
    img_tensor.unsqueeze_(0);

    //    img_tensor = torch::data::transforms::Normalize<>(MEAN, STD)(img_tensor);

    return img_tensor.clone();
}


Probability PornTextDetector::forward(Message &data)
{
    throw NotImplemented();
}

std::string PornTextDetector::text_replace(Message *data) {
    throw NotImplemented();
}
