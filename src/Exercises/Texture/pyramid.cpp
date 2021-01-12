//
// Created by gunigma on 24.11.2020.
//

#include "pyramid.h"
//#include "app.h"
#include <vector>
#include<iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "3rdParty/src/stb/stb_image.h"


Pyramid::Pyramid() {

    std::vector<GLfloat> vertices_ = {
            // to są po prostu współrzędne wierzchołków: x,y, z
            // jeśli którąś zmienimy to zmienimy kształt, możemy też przesunąć wszystkie o ten sam wektor i wówczas cały trójkąt się przesunie

            //"podstawa" piramidy"
            -0.5, -0.5, 0.0, 0.1910, 0.5,//0
            -0.5, 0.5, 0.0, 0.5, 0.8090, //1
            0.5, -0.5, 0.0, 0.5, 0.1910, //2
            0.5, 0.5, 0.0, 0.8090,0.5, //3

            0.0, 0.0, 1.0, 1.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 1.0,
            0.0, 0.0, 1.0, 1.0, 1.0,





    };

//
    std::vector<GLushort> indices_ = {
            0,1,2,
            1,3,2,
            3,4,2,
            2,5,0,
            0,6,1,
            1,7,3};// te chcemy



    glGenVertexArrays(1, &vao_);
    glGenBuffers(2, buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(GLfloat), vertices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLushort), indices_.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat))); //tu musi być 5* loool
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    auto texture_filename = std::string(PROJECT_DIR) + "/Textures/multicolor.png";

    stbi_set_flip_vertically_on_load(true);

    int width, height, n_channels;
    uint8_t *data = stbi_load(texture_filename.c_str(),&width, &height, &n_channels, 0);
    //sprawdzonko:
    if (data == nullptr){
        std::cerr<<"nie mogem załadować obrazka z pliku:'"<<texture_filename<<" przykro mi \n";
    } else {
        std::cout <<"read" <<width<<"x"<<height<<"x"<<n_channels<<"obraz z pliku:" <<texture_filename<< "\n";
    }

    glGenTextures(1,&diffuse_texture_);
    glActiveTexture(GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//    auto status = glGetError();
//    //i jeszcze jedno sprawdzonko, coby się nie sypało głupio
//    if (status != GL_NO_ERROR){
//        std::cerr <<"Error, znowu ci nie działa hehe"<< status << "\n";
//    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

 Pyramid::~Pyramid() {
    glDeleteVertexArrays(1,&vao_);
    glDeleteBuffers(2, buffer_);
}

void Pyramid::draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}