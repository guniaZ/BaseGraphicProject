#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec2 a_vertex_texture;

out vec2 vertex_texture;

layout(std140)  uniform Transformations {
    mat4 P;
    mat4 V;
};


void main() {
    vertex_texture=a_vertex_texture;
    gl_Position = P*V*a_vertex_position;
}
