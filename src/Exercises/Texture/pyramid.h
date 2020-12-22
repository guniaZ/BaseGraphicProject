//
// Created by gunigma on 24.11.2020.
//

#ifndef INC_3DGRAPHICSEXAMPLES_PYRAMID_H
#define INC_3DGRAPHICSEXAMPLES_PYRAMID_H

#endif //INC_3DGRAPHICSEXAMPLES_PYRAMID_H

#pragma once

#include "glad/glad.h"
#include <iostream>
#include <memory>


class Pyramid  {
public:
    Pyramid();
    ~Pyramid();
    void draw();

//    Pyramid(const Pyramid& rhs) = delete;
//    Pyramid &operator=(const Pyramid& rhs) = delete;
//
//    void operator=(Pyramid&& rhs) = delete;
//    Pyramid(Pyramid&& rhs) = delete;
//
//    std::shared_ptr<Pyramid>pyramid_;


private:
    GLuint vao_;
    GLuint buffer_[2];
    GLuint diffuse_texture_;

};