//
// Created by gunigma on 24.11.2020.
//

#ifndef INC_3DGRAPHICSEXAMPLES_QUAD_H
#define INC_3DGRAPHICSEXAMPLES_QUAD_H

#endif //INC_3DGRAPHICSEXAMPLES_QUAD_H

#pragma once
#include <iostream>
#include "glad/glad.h"
#include "phong_material.h"



class Quad  {
public:
    Quad ();
    ~Quad ();
    void draw();
    void setMaterial(PhongMaterial *new_material){
        material_ = new_material;
    };
    PhongMaterial *getMaterial(){
        return material_;
    };
    std::allocator<PhongMaterial>&getMaterial_allocator(){
        return material_1_;
    }


private:
    GLuint vao_;
    GLuint n_indices_;
    GLuint buffer_[2]={0,0};
    GLuint diffuse_texture_;
    PhongMaterial *material_ = nullptr;
    std::allocator<PhongMaterial> material_1_{};

};

// w app.cpp nie trzeba nic zmieniać
// w szaderze vertexów też nic nie trzeba zmienić
// jedyne zmiany na początku tylko w szaderze fragmentów
// brzmiało prościej niż było xD
