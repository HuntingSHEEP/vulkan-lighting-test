//Ustandaryzowany vertex shader, wszystkie są takie same. Rysują obiekty w przestrzeni na podstawie macierzy MVP
#version 450

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
    float iTime;
} ubo;

layout(push_constant) uniform Push {
    vec3 position;
    vec4 color;
} push;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec3 fragPosition;
layout(location = 4) out float iTime;

void main() {
    //gl_Position = ubo.MVP * vec4(inPosition , 1.0); //ubo.proj * ubo.view * ubo.model * vec4(inPosition , 1.0);
    iTime = ubo.iTime;
    
    vec4 modelGlobalCoords = ubo.model * vec4(inPosition , 1.0);
    modelGlobalCoords += vec4(push.position, 0.0);

    gl_Position = ubo.projection * ubo.view * modelGlobalCoords;
    fragColor = inColor;
    fragTexCoord = inTexCoord;

    mat3 normalMatrix = mat3(ubo.model);
    normalMatrix = inverse(normalMatrix);
    normalMatrix = transpose(normalMatrix);
    normal = normalize(normalMatrix * inNormal);

    fragPosition = modelGlobalCoords.xyz;
}