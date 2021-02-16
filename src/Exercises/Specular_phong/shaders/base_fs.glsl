#version 410

layout(location=0) out vec4 vFragColor;
in vec2 vertex_texture_coordinates;
in vec3 vertex_normal_in_vs;
in vec3 vertex_position_in_vs;

layout(std140) uniform Light{
    vec3 position_in_vs;
    vec3 color;
    vec3 a;
    vec3 ambient;

} light;

layout(std140) uniform Material {
    vec3 Kd;
    uint Kd_map;
    vec3 Ks;
    uint Ks_map;
    float Ns;
    uint Ns_map;
} material;

float M_PI = 3.1415926;

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform sampler2D shininess_map;

void main() {

    //wektor normalny:
    vec3 normal = normalize(vertex_normal_in_vs);
    if (gl_FrontFacing){
        normal = -normal;
    }

    vec3 view_vector = normalize(-vertex_position_in_vs);
    vec3 light_vec = light.position_in_vs - vertex_position_in_vs;
    float r = length(light_vec);
    light_vec = light_vec /r;
    float attenuation = 1.0f/(light.a[0]+light.a[1]*r+light.a[2]*r*r);
    float light_in = max(0.0, dot(normal,light_vec))*attenuation;

    vec4 diffuse_color;
    if (material.Kd_map>0) {
        diffuse_color.a = texture(diffuse_map, vertex_texture_coordinates).a;
        diffuse_color.rgb = texture(diffuse_map, vertex_texture_coordinates).rgb*material.Kd;
    } else {
        diffuse_color.a = 1;
        diffuse_color.rgb = material.Kd;
    }

    vec3 specular_color;
    if (material.Ks_map>0) {
        specular_color = texture(specular_map, vertex_texture_coordinates).rgb*material.Ks;
    } else {
        specular_color = material.Ks;
    }

    float shininess;
    if (material.Ns_map>0) {
        shininess = texture(shininess_map, vertex_texture_coordinates).r*material.Ns;
    } else {
        shininess = material.Ns;
    }

    vec3 half_vector = normalize(light_vec +view_vector);

    // z tego śmiesznego wzoru m+8/8pi itd...
    float specular = max(0.0, dot(normal,half_vector));
    specular = pow(specular,shininess);
    specular =  ((shininess + 8.0f) / (8*M_PI))*specular;

    vFragColor.a = diffuse_color.a;
    vFragColor.rgb = diffuse_color.rgb *light.ambient.rgb;
    vFragColor.rgb += light_in*diffuse_color.rgb*light.color;
    vFragColor.rgb += light_in*light.color.rgb*specular*specular_color;
}
