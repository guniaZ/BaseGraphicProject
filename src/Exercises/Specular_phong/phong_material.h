//
// Created by gunigma on 10.02.2021.
//
#pragma once
#include <glm/vec3.hpp>

#ifndef INC_3DGRAPHICSEXAMPLES_PHONG_MATERIAL_H
#define INC_3DGRAPHICSEXAMPLES_PHONG_MATERIAL_H


struct PhongMaterial {
    alignas(16) glm::vec3 Kd;
    alignas(4) uint32_t Kd_map;
    alignas(16) glm::vec3 Ks;
    alignas(4) uint32_t Ks_map;
    alignas(4) float Ns;
    alignas(4) uint32_t Ns_map;
};

#endif //INC_3DGRAPHICSEXAMPLES_PHONG_MATERIAL_H