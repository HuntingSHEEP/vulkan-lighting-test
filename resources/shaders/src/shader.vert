#version 450

struct PointLight {
    vec4 position; 
    vec4 color;
};

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    PointLight pointLights[100];
    int numberOfLights;
    float iTime;
    vec3 cameraPosition;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec3 fragPosition;

void main() {
    vec4 modelGlobalCoords = ubo.model * vec4(inPosition , 1.0);

    mat3 normalMatrix = mat3(ubo.model);
    normalMatrix = transpose(inverse(normalMatrix));
    normal = normalize(normalMatrix * inNormal);

    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragPosition = modelGlobalCoords.xyz;

    gl_Position = ubo.projection * ubo.view * modelGlobalCoords;
}