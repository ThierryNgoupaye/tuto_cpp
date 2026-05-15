//
// Created by thierry on 15/05/2026.
//

#include <gtest/gtest.h>
#include "Camera.h"


class CameraTest : public testing::Test {
protected:
    T_Camera camera;   // partagé entre tous les tests

    // code executé avant chaque test
    void SetUp() override {
        camera.initialiser();
        camera.setResolution(1920, 1080);
    }


    // code executé après chaque test
    void TearDown() override {
        camera.fermer();
    }
};




TEST_F(CameraTest, ResolutionCorrectement) {
    // SetUp déjà appelé
    EXPECT_EQ(camera.getLargeur(), 1920);
    EXPECT_EQ(camera.getHauteur(), 1080);
}


TEST_F(CameraTest, CapturerFrameValide) {
    // SetUp() déjà appelé → camera prête !
    const auto frame = camera.capturerFrame();
    EXPECT_FALSE(frame.empty());
}


TEST_F(CameraTest, ChangerResolution) {
    camera.setResolution(1280, 720);
    EXPECT_EQ(camera.getLargeur(), 1280);
    EXPECT_EQ(camera.getHauteur(), 720);
}



