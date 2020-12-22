#version 410


in vec4 vertex_texture;

layout(std140) uniform Modifiers {
    float strength;
    vec3 light;
};

layout(location=0) out vec4 vFragColor;

uniform sampler2D diffuse_map;

void main() {
    //vFragColor.a=1.0;
   // glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_handle);
    //vFragColor.rgb = vertex_color;
    vFragColor.a = texture(diffuse_map, vertex_texture);
}
