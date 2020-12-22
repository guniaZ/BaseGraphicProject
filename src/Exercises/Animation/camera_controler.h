//
// Created by gunigma on 24.11.2020.
//

#ifndef INC_3DGRAPHICSEXAMPLES_CAMERA_CONTROLER_H
#define INC_3DGRAPHICSEXAMPLES_CAMERA_CONTROLER_H

#endif //INC_3DGRAPHICSEXAMPLES_CAMERA_CONTROLER_H

#include "camera.h"

class CameraControler {
public:
    CameraControler(Camera *camera): camera_(camera), LMB_pressed_(false), scale_(0.015f){};
    CameraControler():camera_(nullptr) {}
    //CameraControler(Camera* camera):camera_(camera) {}

    void set_camera(Camera *camera) { camera_ = camera; }

    void rotate_camera(float dx, float dy) {
        camera_->rotate_around_center(-scale_ * dy, camera_->x());
        camera_->rotate_around_center(-scale_ * dx, camera_->y());
    }

    void mouse_moved(float x, float y) {
        if (LMB_pressed_) {
            auto dx = x - x_;
            auto dy = y - y_;
            x_ = x;
            y_ = y;

            rotate_camera(dx, dy);
        }
    };

    //naciśnięcie i puszczenie przycisku na myszce:
    void LMB_pressed(float x, float y) {
        LMB_pressed_ = true;
        x_ = x;
        y_ = y;
    };

    void LMB_released(float x, float y) {
        LMB_pressed_ = false;
        auto dx = x - x_;
        auto dy = y - y_;

        rotate_camera(dx, dy);
    };

    void zoom(float y_offset) {
        auto x = camera_->fov() / glm::pi<float>(); // zamiana na zakres  [0,1]
        auto y = inverse_logistic(x); // tu na (-inf, inf)
        y += y_offset;
        auto fov = logistic(y) * glm::pi<float>();
        camera_-> set_fov(fov);
    }


private:
    Camera *camera_;
    bool LMB_pressed_;
    float x_, y_;
    float scale_;
};

