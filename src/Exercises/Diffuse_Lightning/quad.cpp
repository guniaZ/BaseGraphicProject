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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0)); //tym razem pamiętaj żeby zmienić!
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(5 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[1]);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    auto texture_filename = std::string(PROJECT_DIR) + "/Textures/silver.png";

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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    // trzeba uważać jaką mamy teksture!! czy mamy rgb czy rgba:
    if (n_channels ==3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if(n_channels ==4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else{
        std::cerr <<"unsuported number of channels ("<<n_channels<<")in texture \n";
    }
//    auto status = glGetError();
//    //i jeszcze jedno sprawdzonko, coby się nie sypało głupio
//    if (status != GL_NO_ERROR){
//        std::cerr <<"Error, znowu ci nie działa hehe"<< status << "\n";
//    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Quad::~Quad() {
    glDeleteVertexArrays(1,&vao_);
    glDeleteBuffers(2, buffer_);
    glDeleteTextures(1, &diffuse_texture_);
}

void Quad::draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, n_indices_, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

}