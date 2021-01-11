//
// Created by gunigma on 24.11.2020.
//

#ifndef INC_3DGRAPHICSEXAMPLES_PYRAMID_H
#define INC_3DGRAPHICSEXAMPLES_PYRAMID_H

#endif //INC_3DGRAPHICSEXAMPLES_PYRAMID_H

#pragma once

#include "glad/glad.h"


class Pyramid  {
public:
    Pyramid();
    ~Pyramid();
    void draw();

//    Pyramid(const Pyramid& rhs) = delete;                         //to tu było
//    Pyramid &operator=(const Pyramid& rhs) = delete;
//
//    void operator=(Pyramid&& rhs) = delete;
//    Pyramid(Pyramid&& rhs) = delete;




private:
    GLuint buffer_[2];
    GLuint vao_;

};