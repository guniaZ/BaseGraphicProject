//
// Created by pbialas on 05.08.2020.
//



#pragma once

#include <vector>

#include "pyramid.h"
#include "camera.h"  // tak tak brawo, includuj sobie po raz drugi a potem się dziw że nie działa -,-
#include "camera_controler.h"
#include "glad/glad.h"
#include "glm/glm.hpp"


#include "Application/application.h"
#include "Application/utils.h"


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;;
    void framebuffer_resize_callback(int w, int h) override;
    void frame() override;

    // ta funkcja tutaj robi robotę! :
    void scroll_callback(double xoffset, double yoffset) override;

    void mouse_button_callback(int button, int action, int mods) override;

    void cursor_position_callback(double x, double y) override;

    void set_camera(Camera *camera) { camera_ = camera; }

    Camera *camera() { return camera_; }

    ~SimpleShapeApplication() {
        delete controler_;
        if(camera_){
        delete camera_;}

    }

    Pyramid *pyramid_;

    //

    void set_controler(CameraControler *controler) { controler_ = controler; }



private:
    GLuint vao_;
    Camera *camera_;
    CameraControler *controler_;
    GLuint ubo_handle_;

};