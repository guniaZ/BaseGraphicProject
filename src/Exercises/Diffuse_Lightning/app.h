//
// Created by pbialas on 05.08.2020.
//



#pragma once

#include <vector>

#include "quad.h"

#include "camera_controler.h"
#include "glad/glad.h"
#include "glm/glm.hpp"


#include "Application/application.h"
#include "Application/utils.h"

struct Light {
    glm::vec4 position;
    glm::vec4 color;
    glm::vec4 a;
    glm::vec3 ambient;
};

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;

    void framebuffer_resize_callback(int w, int h) override;

    void frame() override;

    void cleanup() override;

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

    Quad *quad_;

    //

    void set_controler(CameraControler *controler) { controler_ = controler; }



private:
    GLuint vao_;
    Camera *camera_;
    CameraControler *controler_;
    GLuint ubo_handle_;
    GLuint u_light_buffer_;

    Light light_;

};