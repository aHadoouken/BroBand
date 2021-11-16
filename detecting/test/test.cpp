//
// Created by d3vyatk4 on 16.11.2021.
//

#include "gtest/gtest.h"
#include "detecting.h"

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
    EXPECT_EQ(msgWrap.senderId, 0);
    EXPECT_EQ(msgWrap.chatId, 0);
    EXPECT_EQ(msgWrap.isReplace, false);

}

TEST(ST_IMAGE, default_value_image) {

    Image img;
    EXPECT_EQ(img.data.size(), 0);
}

TEST(ST_IMAGE_WRAPPER, default_value_image_wrapper) {

    ImageWrapper imgWrap;
    EXPECT_EQ(imgWrap.img, nullptr);
    EXPECT_EQ(imgWrap.senderId, 0);
    EXPECT_EQ(imgWrap.chatId, 0);
    EXPECT_EQ(imgWrap.isBlur, false);

}

TEST(IMAGE_DETECTOR, class_img_detect) {

    PornImageDetector imgDetector;

    EXPECT_EQ(imgDetector.set_threshold(0), 0);
    EXPECT_EQ(imgDetector.load_model(0), 0);
    EXPECT_EQ(imgDetector.forward(nullptr), nullptr);
    EXPECT_EQ(imgDetector.blurring(nullptr), nullptr);

}

TEST(MESSAGE_DETECTOR, class_msg_detect) {

    PornTextDetector msgDetector;

    EXPECT_EQ(msgDetector.set_threshold(0), 0);
    EXPECT_EQ(msgDetector.load_model(0), 0);
    EXPECT_EQ(msgDetector.forward(nullptr), nullptr);
    EXPECT_EQ(msgDetector.text_replace(nullptr), nullptr);

}

int main(int argc, char *argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}