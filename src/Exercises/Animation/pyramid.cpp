//
// Created by gunigma on 24.11.2020.
//

#include "pyramid.h"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION

#include "3rdParty/src/stb/stb_image.h"

Pyramid::Pyramid() {

    std::vector<GLfloat> vertices_ = {
            // to są po prostu współrzędne wierzchołków: x,y, z
            // jeśli którąś zmienimy to zmienimy kształt, możemy też przesunąć wszystkie o ten sam wektor i wówczas cały trójkąt się przesunie

            //"podstawa" piramidy"
            -0.5, -0.5, 0.0, 0.5, 0.1, 0.5,//0
            -0.5, 0.5, 0.0, 0.5, 0.1, 0.5, //1
            0.5, -0.5, 0.0, 0.5, 0.1, 0.5, //2
            0.5, 0.5, 0.0, 0.5, 0.1, 0.5, //3

            // i ściany:
            //1, żółta
            0.5, 0.5, 0.0, 0.9, 0.5, 0.0, //3
            0.0, 0.0, 1.0, 0.9, 0.5, 0.0, //
            0.5, -0.5, 0.0, 0.9, 0.5, 0.0, //2

            //2, niebieska
            -0.5, -0.5, 0.0, 0.0, 0.2, 0.8, //
            0.0, 0.0, 1.0, 0.0, 0.2, 0.8, //
            0.5, -0.5, 0.0, 0.0, 0.2, 0.8, //

            //3, zielona
            -0.5, -0.5, 0.0, 0.1, 0.5, 0.0, //
            0.0, 0.0, 1.0, 0.1, 0.5, 0.0, //
            -0.5, 0.5, 0.0, 0.1, 0.5, 0.0, //

            //4, fioletowa
            -0.5, 0.5, 0.0, 0.3, 0.0, 0.6, //1
            0.0, 0.0, 1.0, 0.3, 0.0, 0.6, //
            0.5, 0.5, 0.0, 0.3, 0.0, 0.6, //3

            // jeśli któraś współrzędna wykroczy poza zakres <-1,1> to nie będziemy widzieli całego trójkąta
            //zmiana współrzędnej z niczego nie zmienia (chyba że wykroczymy poza zakres <-1,1>, wówczas "obetnie" nam kawałek trójkąta


    };


//
    std::vector<GLushort> indices_ = {
            0,1,2,1,3,2,
            4,5,6,
            9,8,7,
            10,11,12,
            13,14,15            // te chcemy
    };



    glGenBuffers(2, buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(GLfloat), vertices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLfloat), indices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

Pyramid::~Pyramid() {
    glDeleteVertexArrays(1,&vao_);
    glDeleteBuffers(2, buffer_);
}

void Pyramid::draw() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
}