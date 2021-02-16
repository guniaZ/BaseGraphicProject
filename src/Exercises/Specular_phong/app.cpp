//
// Created by me - Gunia
//


#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include <glm/gtx/transform.hpp>

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


// i teraz just like that:
    xe::set_uniform_block_binding(program, "Light", 1);
    xe::set_uniform_block_binding(program, "Transformations", 0);
    xe::set_uniform_block_binding(program, "Material", 2);

    glGenBuffers(1, &ubo_handle_);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, ubo_handle_);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4)+ 3*sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER,0, ubo_handle_);

    glGenBuffers(1, &u_light_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Light), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER,1, u_light_buffer_);

    glGenBuffers(1, &u_material_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_material_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(PhongMaterial), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER,2, u_material_buffer_);

    quad_ = quad_1.allocate(1);
    quad_1.construct(quad_);
    if(!quad_){
        std::cout << "Nie zrobilem quada :(" << "\n";
    }
    else{
        std::cout << "Zrobilem quada!" << "\n";
    }


    set_camera(new Camera);
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    float aspect_ = (float)w/h;
    float fov_ = glm::pi<float>()/3.0;
    float near_ = 0.5f;
    float far_ = 100.0f;

    camera()->perspective(fov_, aspect_, near_, far_);
    camera()-> look_at(glm::vec3{0.0,0.0,4.0},
                       glm::vec3{0.0,0.0,0.0},
                       glm::vec3{0.1,0.0,1.0});
    set_controler(new CameraControler(camera()));

    // tu się zaczyna magia światła

    light_.position = glm::vec4(0.0f, 0.0f, 1.0f,1.0f);
    std::cout<<glm::to_string(light_.position)<<std::endl;
    light_.color = glm::vec4(1.0f, 1.0f, 1.0f,1.0f);
    light_.a = glm::vec4(1.0f, 0.0f, 1.0f,0.0f);
    light_.ambient = glm::vec4(0.3f, 0.1f, 0.3f, 1.0f);

    //tekstury, poczebujemy dwie:
    GLuint textures_[2]{ 0, 0};
    glGenTextures(2, textures_);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures_[0]);
    xe::load_texture(std::string(PROJECT_DIR) + "/Textures/plastic.png");
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures_[1]);
    xe::load_texture(std::string(PROJECT_DIR) + "/Textures/shininess.png");
    glBindTexture(GL_TEXTURE_2D, 0);

    auto material_1_ = quad_->getMaterial_allocator().allocate(1);
    quad_->getMaterial_allocator().construct(material_1_);
    material_1_->Kd = glm::vec3(1.0, 1.0, 1.0);
    material_1_->Kd_map = textures_[0];                            //tu zmień z 0!
    material_1_->Ks = glm::vec3(0.05, 0.05, 0.05);
    material_1_->Ks_map = 0;
    material_1_->Ns = 1000.0f;
    material_1_->Ns_map = textures_[1];                            //tu zmień z 0!
    quad_->setMaterial(material_1_);



    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);


    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    xe::set_uniform1i(program,"diffuse_map",0);
    xe::set_uniform1i(program,"specular_map",1);
    xe::set_uniform1i(program,"shininess_map",2);


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
    light_.position_in_vs = VM*light_.position;
    //std::cout<<glm::to_string(light_.position_in_vs)<<std::endl; // debil przecież to się milion razy wyświetli xd

    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 4*sizeof(glm::vec4), &light_.position_in_vs[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, u_material_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(PhongMaterial), quad_->getMaterial());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &P[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &N[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4)+sizeof(glm::vec4), sizeof(glm::vec3), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4)+ 2*sizeof(glm::vec4), sizeof(glm::vec3), &N[2]);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    if(quad_->getMaterial()->Kd_map > 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, quad_->getMaterial()->Kd_map);
    }

    if(quad_->getMaterial()->Ks_map>0) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, quad_->getMaterial()->Ks_map);
    }
    if(quad_->getMaterial()->Ns_map>0) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, quad_->getMaterial()->Ns_map);
    }

    glFrontFace(GL_CW);
    quad_ -> draw();
    glBindTexture(GL_TEXTURE_2D,0);

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