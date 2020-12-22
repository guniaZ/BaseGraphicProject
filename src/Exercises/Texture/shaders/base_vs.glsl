#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec4 a_vertex_color;

layout(std140)  uniform Transformations {
//    mat4 P;
//    mat4 V;
      mat4 PVM;

};

out vec2 vertex_texture;

void main() {
    vertex_texture=a_vertex_texture;
    gl_Position = PVM*a_vertex_position;
}
