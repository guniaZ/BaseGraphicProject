//
// Created by gunigma on 24.11.2020.
//

#include "quad.h"

#include <vector>
#include<iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION

#include "3rdParty/src/stb/stb_image.h"


Quad::Quad() {

    std::vector<GLfloat> vertices_ = {
            // dobra, teraz tu będzie kwadracik

            //kwadrat o boku dwa, środek w 0,0 więc rogi (-1, -1), (1,-1), (-1, 1), (1,1)
            -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,//0
             1.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,//1
             1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0,//2
            -1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0,//3

    };

//
    std::vector<GLushort> indices_ = {
            0,1,2,
            2,3,0,
    };

    n_indices_=indices_.size();

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &buffer_[1]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[1]);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(GLfloat), vertices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &buffer_[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLushort), indices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[1]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0)); //tym razem pamiętaj żeby zmienić!
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(5 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER,0);

}

Quad::~Quad() {
    material_1_.destroy(material_);
    material_1_.deallocate(material_,1);
}

void Quad::draw() {

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, n_indices_, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);

}

