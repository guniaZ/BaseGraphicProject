//
// Created by me - Gunia
//


#include "app.h"
//#include "quad.h"

#include <iostream>
#include <vector>
#include <tuple>


#include "Application/utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>


void SimpleShapeApplication::init() {

    glEnable(GL_FRAMEBUFFER_SRGB);


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");



    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }


    auto u_transformations_index = glGetUniformBlockIndex(program,"Transformations");
    if (u_transformations_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    }else {
        glUniformBlockBinding(program, u_transformations_index, 1);
    }

    auto u_diffuse_map_location = glGetUniformLocation(program, "diffuse_map");
    if (u_diffuse_map_location ==-1){
        std::cerr << "Nie mogę znaleźć cosia o nazwie diffuse map\n";
    }else{
        glUniform1i(u_diffuse_map_location,0);
    }

    auto u_light_index = glGetUniformBlockIndex(program, "Light");
    if (u_light_index==GL_INVALID_INDEX){
        std::cout<<"Nie ma świateła" <<std::endl;
    }
    else{
        glad_glUniformBlockBinding(program, u_light_index, 2);
    }

    quad_ = new Quad();



    set_camera(new Camera);
    camera()->look_at(glm::vec3{0.0,0.0,7.0},
                      glm::vec3{0.0f,0.0f,0.0f},
                      glm::vec3{0.0,0.0,1.0});
    set_controler(new CameraControler(camera()));



    glGenBuffers(1, &ubo_handle_);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4)+ 3*sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);

    set_camera(new Camera);
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    float aspect_ = (float)w/h;
    float fov_ = glm::pi<float>()/3.0;
    float near_ = 0.5f;
    float far_ = 100.0f;

    camera()->perspective(fov_, aspect_, near_, far_);
    camera()-> look_at(glm::vec3{0.0,0.0,8.0},
                       glm::vec3{0.5,0.5,0.0},
                       glm::vec3{0.0,1.0,1.0});
    set_controler(new CameraControler(camera()));

    auto V = camera() -> view();

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &camera()->projection()[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),sizeof(glm::mat4), &V);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    glGenBuffers(1, &u_light_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 3*sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);


    //tu się zaczyna magia światła
    light_.position = V*glm::vec4(0.0f, 1.0f, 0.0f,1.0f);
    std::cout<<glm::to_string(light_.position)<<std::endl;
    light_.color = glm::vec4(1.0f, 1.0f, 1.0f,1.0f);
    light_.a = glm::vec4(1.0f, 0.0f, 1.0f,0.0f);
    light_.ambient = glm::vec3(0.8f, 0.2f, 0.2f);

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &light_.position[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4),&light_.color[0] );
    glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(glm::vec4), sizeof(glm::vec4),&light_.a[0] );
    glBufferSubData(GL_UNIFORM_BUFFER, 3*sizeof(glm::vec4), sizeof(glm::vec4), &light_.ambient[0]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_handle_); //jużdawno powinnaś była zmienić tą nazwe xd
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_light_buffer_);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);


    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);


}

void SimpleShapeApplication::cleanup() {
    delete quad_;
}


void SimpleShapeApplication::frame() {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_);


    auto P = camera() -> projection();
    auto VM = camera() -> view();
    auto R = glm::mat3(VM);
    auto N = glm::transpose(glm::inverse(R));

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &P[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &N[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4)+sizeof(glm::vec4), sizeof(glm::vec3), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4)+ 2*sizeof(glm::vec4), sizeof(glm::vec3), &N[2]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    light_.position = VM*glm::vec4(0.0f, 0.0f,1.0f, 1.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &light_.position[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    quad_ -> draw();

}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    camera() -> set_aspect((float)  w / h);
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