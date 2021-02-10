//
// Created by pbialas on 05.08.2020.
//



#pragma once

//#include <vector>
//
//#include "quad.h"
//
//#include "camera_controler.h"
//#include "glad/glad.h"
//#include "glm/glm.hpp"
//
//
//#include "Application/application.h"
//#include "Application/utils.h"
//#include "3rdParty/src/stb/stb_image.h"

#include <utility>
#include <vector>
#include <chrono>
#include <glm/matrix.hpp>

#include "Application/application.h"
#include "Application/utils.h"
#include "glad/glad.h"
#include "camera_controler.h"
#include "quad.h"
#include "3rdParty/src/stb/stb_image.h"

struct Light {
    glm::vec4 position;
    glm:: vec4 position_in_vs;
    glm::vec4 color;
    glm::vec4 a;
    glm::vec4 ambient;
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
        quad_1.destroy(quad_);
        quad_1.deallocate(quad_, 1);
        stbi_image_free(data_);


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
    GLuint u_material_buffer_;
    std::allocator<Quad> quad_1;
    GLuint shininess_texture_ = 0;
    uint8_t *data_ = nullptr;

    Light light_;

};