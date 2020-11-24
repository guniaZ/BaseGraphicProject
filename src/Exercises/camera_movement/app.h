//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>


#include "Application/application.h"
#include "Application/utils.h"
#include "app.h"
#include "camera.h"
#include "camera_controler.h"
#include "glad/glad.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;;
    void framebuffer_resize_callback(int w, int h) override;
    void frame() override;

    // ta funkcja tutaj robi robotę! :
    void scroll_callback(double xoffset, double yoffset) override{
        Application::scroll_callback(xoffset, yoffset);
        camera()->zoom(yoffset / 30.0);
    }

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;

    void set_camera(Camera *camera) { camera_ = camera; }

    Camera *camera() { return camera_; }

    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    CameraControler *controler_;

    void set_controler(CameraControler *controler) { controler_ = controler; }



private:
    GLuint vao_;
    Camera *camera_;
    GLuint u_pvm_buffer;

};