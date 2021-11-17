//
// Created by d3vyatk4 on 16.11.2021.
//

#include "gtest/gtest.h"
#include "detecting.h"
#include "Exception.h"

TEST(ST_PROBABILITY, default_value_probability) {

    Probability prob;
    EXPECT_EQ(prob.predict_proba, nullptr);
}

TEST(ST_MESSAGE, default_value_message) {

    Message msg;
    EXPECT_EQ(msg.text, "");
}

TEST(ST_MESSAGE_WRAPPER, default_value_message_wrapper) {

    MessageWrapper msgWrap;
    EXPECT_EQ(msgWrap.msg, nullptr);
    EXPECT_EQ(msgWrap.sender_id, 0);
    EXPECT_EQ(msgWrap.chat_id, 0);
    EXPECT_EQ(msgWrap.is_replace, false);

}

TEST(ST_IMAGE, default_value_image) {

    Image img;
    EXPECT_EQ(img.data.size(), 0);
}

TEST(ST_IMAGE_WRAPPER, default_value_image_wrapper) {

    ImageWrapper imgWrap;
    EXPECT_EQ(imgWrap.img, nullptr);
    EXPECT_EQ(imgWrap.sender_id, 0);
    EXPECT_EQ(imgWrap.chat_id, 0);
    EXPECT_EQ(imgWrap.is_blur, false);

}

TEST(IMAGE_DETECTOR, set_threshold) {

    PornImageDetector imgDetector;
    EXPECT_EQ(imgDetector.set_threshold(0), 0);
}

TEST(IMAGE_DETECTOR, load_model) {

    PornImageDetector imgDetector;
    EXPECT_EQ(imgDetector.load_model(""), 0);
}

TEST(IMAGE_DETECTOR, forward) {

    PornImageDetector imgDetector;
    EXPECT_THROW(imgDetector.forward(nullptr), NotImplemented);
}

TEST(IMAGE_DETECTOR, blurring) {

    PornImageDetector imgDetector;
    EXPECT_EQ(imgDetector.blurring(nullptr), nullptr);
}

TEST(MESSAGE_DETECTOR, set_threshold) {

    PornTextDetector msgDetector;
    EXPECT_EQ(msgDetector.set_threshold(0), 0);
}

TEST(MESSAGE_DETECTOR, load_model) {

    PornTextDetector msgDetector;
    EXPECT_EQ(msgDetector.load_model(""), 0);
}

TEST(MESSAGE_DETECTOR, forward) {

    PornTextDetector msgDetector;
    EXPECT_THROW(msgDetector.forward(nullptr), NotImplemented);
}

TEST(MESSAGE_DETECTOR, text_replace) {

    PornTextDetector msgDetector;
    EXPECT_EQ(msgDetector.text_replace(nullptr), nullptr);
}

int main(int argc, char *argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}