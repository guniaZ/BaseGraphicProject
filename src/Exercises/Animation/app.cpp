//
// Created by me - Gunia
//


#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "camera.h"
#include "Application/utils.h"
//#include "camera_controler.h"
//#include "rotation.h"
#include "pyramid.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

glm::mat4 rotation(float elapsed_time, float rotation_period, const glm::vec3 &axis){
    auto rotation_angle = 2.0f * glm::pi<float>()*elapsed_time/rotation_period;
    return glm::rotate(glm::mat4(1.0f), rotation_angle, axis);
}

glm::mat4 orbital_motion(float elapsed_time, float a, float b, float orbital_rotation_period){
    auto orbital_rotation_angle = 2.0f * glm::pi<float>()*elapsed_time/orbital_rotation_period;
    auto x = a*std::cos(orbital_rotation_angle);
    auto y = b*std::sin(orbital_rotation_angle);
    auto T = glm::translate(glm::mat4(1.0f), glm::vec3{x,y,0});

    return T;
};

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");



    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

   // Pyramid = new Pyramid;

    auto u_transformation_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformation_index == GL_INVALID_INDEX) { std::cout << "Cannot find Transformations uniform block in program" << std::endl; }
    else {
        glUniformBlockBinding(program, u_transformation_index, 1);
    }
    // TO BYło
//    auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
//    if (u_modifiers_index == GL_INVALID_INDEX) { std::cout << "Cannot find Modifiers uniform block in program" << std::endl; }
//    else {
//        glUniformBlockBinding(program, u_modifiers_index, 0);
//    }

    glGenBuffers(2, ubo_handle_);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_[1]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

    set_camera(new Camera);
    camera()-> look_at(glm::vec3{15.0,10.0,5.0}, glm::vec3{0.0,0.0,0.0}, glm::vec3{0.0,0.0,0.0});
    set_controler(new CameraControler(camera()));

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    auto aspect = (float)w/h;
    camera()->perspective(glm::pi<float>() /4.0, aspect, 0.1f, 100.0f);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_handle_[1]);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    pyramid_ = new Pyramid;

    start_ = std::chrono::steady_clock::now();
    rotation_period = 4.0f;




//    // TU WSZYSTKO ZNIKA bo mamy Pyramid już
//
}



void SimpleShapeApplication::frame() {

    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start_);

    auto orbital_rotation_period = 20.0f;
    auto a = 10.0f;
    auto b = 8.0f;
    auto r_moon = 3.0f;
    auto moon_rotation_period = 10.0f;
    auto moon_orbital_rotation_period = 10.0f;

    auto seconds = elapsed_time.count();
    auto R_earth = glm::rotate(glm::mat4(1.0f), rotation_period, glm::vec3{0.0,0.0,1.0});
    auto O_earth = glm::translate(glm::mat4(1.0f), glm::vec3(a, b, orbital_rotation_period));

    auto PVM = camera()->projection() *camera()->view()*O_earth* R_earth;


    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),&PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    pyramid_->draw();

    auto S_moon = glm::scale(glm::mat4(1.0f), {0.5f, 0.5f, 0.5f});
    auto R_moon = glm::rotate(glm::mat4(1.0f), moon_rotation_period, glm::vec3{0.0f, 0.0f, 0.0f});
    auto O_moon = glm::translate(glm::mat4(1.0f),glm::vec3(r_moon, r_moon, moon_orbital_rotation_period));

    PVM = camera()->projection()*camera()->view()* O_earth*O_moon*R_moon*S_moon;

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),&PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    pyramid_ ->draw();
    auto satellite_rotation_period = 2.0f;
    auto satellite_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/satellite_rotation_period;

    auto S_sat = glm::scale(glm::mat4(1.0f), {0.2f, 0.2f, 0.2f});
    auto R_sat = glm::rotate(glm::mat4(1.0f), glm::pi<float>()/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    auto O_sat = glm::translate(glm::mat4(1.0f), glm::vec3(1.5, 1.5, 2.0f));



    auto tilt_sat = glm::rotate(glm::mat4(1.0f), glm::pi<float>()/2.0f, glm::vec3{0.0, 0.0, 0.0});

    PVM = camera()->projection()*camera()->view()* O_earth * tilt_sat * O_sat *R_sat *S_sat;

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    pyramid_->draw();

//  STARE poszło precz

}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    camera() -> set_aspect((float)  w / h);
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }

}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}

//void SimpleShapeApplication::key_callback(int key, int scancode, int action, int mods) {
//    Application::key_callback(key,scancode,action,mods);
//    if(key==GLFW_KEY_SPACE && action==GLFW_PRESS){
//        paused_=!paused_;
//        if(paused_){
//            pause_start_ =std::chrono::steady_clock::now();
//        }else{
//            start_+=(std::chrono::steady_clock::now()-pause_start_);
//        }
//    }
//}