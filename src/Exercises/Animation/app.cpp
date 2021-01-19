//
// Created by me - Gunia
//


#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>

#include "Application/utils.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>



void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");



    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    pyramid_ = new Pyramid;

    auto u_transformation_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformation_index == GL_INVALID_INDEX) { std::cout << "Cannot find Transformations uniform block in program" << std::endl; }
    else {
        glUniformBlockBinding(program, u_transformation_index, 1);
    }


    glGenBuffers(1, &ubo_handle_);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

    set_camera(new Camera);
    set_controler(new CameraControler(camera()));
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    float aspect_ = (float)w/h;
    float fov_ = glm::pi<float>()/3.0;
    float near_ = 0.5f;
    float far_ = 100.0f;

    camera()->perspective(fov_, aspect_, near_, far_);
    camera()-> look_at(glm::vec3{0.0,0.0,20.0},
                       glm::vec3{0.0,0.0,0.0},
                       glm::vec3{0.0,1.0,1.0});


    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &camera()->projection()[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),sizeof(glm::mat4), &camera()->view()[0]);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_handle_);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);


    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);


    start_ = std::chrono::steady_clock::now();
    rotation_period = 4.0f;




//    // TU WSZYSTKO ZNIKA bo mamy Pyramid już
//
}



void SimpleShapeApplication::frame() {

    //dobra, po kolei, bez bycia spryciarzem, po kolei

    //ustalamy czas i kąt obrotu
    auto now = std::chrono::steady_clock::now();
    auto elapsed_time_ = std::chrono::duration_cast<std::chrono::duration<float>>(now - start_).count();
    float rotation_angle = glm::two_pi<float>()*elapsed_time_/rotation_period;


    //ZIEMIA:

    auto orbital_rotation_period = 20.0f;
    auto orbital_rotation_angle = 2.0f * glm::pi<float>()*elapsed_time_/orbital_rotation_period;
    auto a = 10.0f;
    auto b = 8.0f;

    //czy cos i sinus sa dobrze? ciężko stwierdzić, jak zacznie wyglądać lepiej to zobaczymy
    float x = a*cos(orbital_rotation_angle);
    float y = b*sin(orbital_rotation_angle);


    //no dobra, zmieniłam na x,y, 0
    glm::mat4 R_earth = glm::rotate(glm::mat4(1.0f), rotation_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 O_earth = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));

    auto orbited = camera()-> projection() *camera()-> view()*O_earth*R_earth;
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_);
    glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(glm::mat4),&orbited[0]);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    pyramid_ -> draw();



    //KSIĘŻYC
    auto moon_rotation_period = 10.0f;
    auto moon_rotation_angle = 2.0f * glm::pi<float>()*elapsed_time_/moon_rotation_period;

    //r_monn = 3 więc:
    float x_moon = 3.0*cos(moon_rotation_angle);
    float y_moon = 3.0*sin(moon_rotation_angle);

    glm::mat4 S_moon = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)); //tak Pan pokazywał
    glm::mat4 R_moon = glm::rotate(glm::mat4(1.0f), moon_rotation_angle, glm::vec3{0.0f, 0.0f, 1.0f});
    glm::mat4 O_moon = glm::translate(glm::mat4(1.0f),glm::vec3(x_moon, y_moon, 0));

    glm::mat4 moon_transform = camera()->projection()*camera()->view()*O_earth*O_moon*R_moon*S_moon;

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_);
    glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(glm::mat4),&moon_transform[0]);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    pyramid_ -> draw();


    // SATELITA
    auto satellite_rotation_period = 2.0f;
    auto satellite_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time_/satellite_rotation_period;
    float x_sat = 1.5*cos(satellite_rotation_angle);
    float y_sat= 1.5*sin(satellite_rotation_angle);

    auto S_sat = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f)); // tak miał Pan
    auto R_sat = glm::rotate(glm::mat4(1.0f), satellite_rotation_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    auto O_sat = glm::translate(glm::mat4(1.0f), glm::vec3(x_sat, y_sat,0));

    auto tilt_sat = glm::rotate(glm::mat4(1.0f),glm::pi<float>()/2.0f,glm::vec3{1.0f, 0.0f,0.0f});

    glm::mat4 sat_transform = camera()->projection()*camera()->view()*O_earth*tilt_sat*O_sat*R_sat*S_sat;


    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_);
    glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(glm::mat4),&sat_transform[0]);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    pyramid_ -> draw();

//  STARE poszło precz

}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    auto aspect = (float)w/h;
    camera() -> set_aspect(aspect);
}

//ta funkcja tutaj robi robotę! :
void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    camera()->zoom(yoffset / 30.0);
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