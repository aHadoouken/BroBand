//
// Created by d3vyatk4 on 16.11.2021.
//

#include "gtest/gtest.h"
#include "detecting.h"
#include "Exception.h"

TEST(TEST_STRUCTURE, probability) {

    Probability prob;
    EXPECT_EQ(prob.predict_proba.size(), 0);
}

TEST(TEST_STRUCTURE, message) {

    Message msg;
    EXPECT_EQ(msg.text, "");
}

TEST(TEST_STRUCTURE, message_wrapper) {

    MessageWrapper msgWrap;
    EXPECT_EQ(msgWrap.msg, nullptr);
    EXPECT_EQ(msgWrap.sender_id, 0);
    EXPECT_EQ(msgWrap.chat_id, 0);
    EXPECT_EQ(msgWrap.is_replace, false);

}

TEST(TEST_STRUCTURE, image) {

    Image img;
    EXPECT_EQ(img.data.size(), 0);
}

TEST(TEST_STRUCTURE, image_wrapper) {

    ImageWrapper imgWrap;
    EXPECT_EQ(imgWrap.img, nullptr);
    EXPECT_EQ(imgWrap.sender_id, 0);
    EXPECT_EQ(imgWrap.chat_id, 0);
    EXPECT_EQ(imgWrap.is_blur, false);

}

TEST(IMAGE_DETECTOR, set_threshold) {

    PornImageDetector imgDetector;
    double _threshold = 0.5;

    EXPECT_EQ(imgDetector.set_threshold(_threshold), 0);
}

TEST(IMAGE_DETECTOR, load_model) {

    PornImageDetector imgDetector;
    std::string path = "model/image_detect.pth";

    EXPECT_EQ(imgDetector.load_model(path), 0);
}

TEST(IMAGE_DETECTOR, forward) {

    PornImageDetector imgDetector;
    Image* img;

    Probability prob_true;
    prob_true.predict_proba = {0.2, 0.8};

    Probability pred_prob = imgDetector.forward(img);

    for (int i = 0; i < 2; ++i) {
        EXPECT_DOUBLE_EQ(pred_prob.predict_proba[i], prob_true.predict_proba[i]);
    }
}

TEST(IMAGE_DETECTOR, blurring) {

    PornImageDetector imgDetector;
    Image* img = nullptr;
    ImageWrapper* img_wrap = imgDetector.blurring(img);

    EXPECT_TRUE(img_wrap->is_blur);
}

TEST(MESSAGE_DETECTOR, set_threshold) {

    PornTextDetector msgDetector;
    double _threshold = 0.5;

    EXPECT_EQ(msgDetector.set_threshold(_threshold), 0);
}

TEST(MESSAGE_DETECTOR, load_model) {

    PornTextDetector msgDetector;
    std::string path = "model/image_detect.pth";

    EXPECT_EQ(msgDetector.load_model(path), 0);
}

TEST(MESSAGE_DETECTOR, forward) {

    PornTextDetector msgDetector;
    Message* msg;

    Probability prob_true;
    prob_true.predict_proba = {0.2, 0.8};

    Probability pred_prob = msgDetector.forward(msg);

    for (int i = 0; i < 2; ++i) {
        EXPECT_DOUBLE_EQ(pred_prob.predict_proba[i], prob_true.predict_proba[i]);
    }
}

TEST(MESSAGE_DETECTOR, text_replace) {

    PornTextDetector msgDetector;
    Message* msg = nullptr;
    MessageWrapper* msg_wrap = msgDetector.text_replace(msg);

    EXPECT_TRUE(msg_wrap->is_replace);
}

int main(int argc, char *argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
