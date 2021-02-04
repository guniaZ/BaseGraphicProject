//
// Created by gunigma on 24.11.2020.
//

#ifndef INC_3DGRAPHICSEXAMPLES_QUAD_H
#define INC_3DGRAPHICSEXAMPLES_QUAD_H

#endif //INC_3DGRAPHICSEXAMPLES_QUAD_H

#pragma once

#include "glad/glad.h"



class Quad  {
public:
    Quad ();
    ~Quad ();
    void draw();


private:
    GLuint vao_;
    GLuint n_indices_;
    GLuint buffer_[2];
    GLuint diffuse_texture_;

};

// w app.cpp nie trzeba nic zmieniać
// w szaderze vertexów też nic nie trzeba zmienić
// jedyne zmiany na początku tylko w szaderze fragmentów
