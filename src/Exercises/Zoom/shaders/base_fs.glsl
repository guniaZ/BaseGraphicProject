#version 410


in vec4 vertex_color;

layout(std140) uniform Modifiers {
    float strength;
    vec3 light;
};

layout(location=0) out vec4 vFragColor;

void main() {
    //vFragColor.a=1.0;
   // glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_handle);
    //vFragColor.rgb = vertex_color;
    vFragColor.a = vertex_color.a;
    vFragColor.rgb = strength*light*vertex_color.rgb;
}
