#version 450

vec2 OFFSETS[6] = vec2[](
    vec2(1, -1),
    vec2(-1, -1),
    vec2(-1, 1),
    vec2(1, -1), 
    vec2(-1, 1),
    vec2(1, 1) 
);

const float LIGHT_RADIUS = 0.07;
const vec3 lightLocalCentre = {0, 0, 0};

struct PointLight {
    vec4 position; //ignore w
    vec4 color; // w is intensity
};

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    PointLight pointLights[100];
    int numberOfLights;
} ubo;

layout(location = 0) out vec2 fragOffset;

void main(){
    fragOffset = OFFSETS[gl_VertexIndex];
    mat4 modelView = ubo.view * ubo.model;
    vec3 cameraRightWorld = {modelView[0][0], modelView[1][0], modelView[2][0]};
    vec3 cameraUpWorld    = {modelView[0][1], modelView[1][1], modelView[2][1]};

    cameraRightWorld *= -1;

    vec3 localPosition = lightLocalCentre 
        + LIGHT_RADIUS * fragOffset.x * cameraRightWorld
        + LIGHT_RADIUS * fragOffset.y * cameraUpWorld;
        
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(localPosition, 1.0);
    
}