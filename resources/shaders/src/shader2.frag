#version 450
layout(binding = 1) uniform sampler2D texSampler;

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

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 fragPosition;
layout(location = 4) in float number;

layout(location = 0) out vec4 outColor;

void main() {

    vec3 normalizedNormal = normalize(normal);
    vec3 material = vec3(31/255., 28/255., 33/255.) * 0.5;
    vec3 diffuseLight = vec3(0., 0., 0.); //vec3(100/255., 150/255., 50/255.);
    //diffuseLight *= 0.0001

    for(int i=0; i < ubo.numberOfLights; i++){
        PointLight light = ubo.pointLights[i];
        vec3 directionToLight = light.position.xyz - fragPosition;
        //vec3 fragmentToLight_normalized = normalize(light.position.xyz - fragPosition);
        float attenuation = 400/dot(directionToLight,directionToLight);
        float cosAngIncidence = max(dot(normalizedNormal, normalize(directionToLight)), 0.);
        vec3 intensity = light.color.xyz * light.color.w * attenuation;
        diffuseLight += intensity * cosAngIncidence;
    }
    
    outColor = vec4(diffuseLight * material, 1.0) * texture(texSampler, fragTexCoord);

/*
    vec3 pointLightPosition = push.position; 
    vec3 fragmentToLight_vector = pointLightPosition - fragPosition;
    vec3 fragmentToLight_normalized = normalize(pointLightPosition - fragPosition);

    float Ia = 1;
    float ka = 0.04;
    float Ip = 1.0;
    float kd = 0.7;

    float dl = length(fragmentToLight_vector);
    float c1 = 0.1;
    float c2 = 0.2;
    float c3 = 0.4;

    float fatt = min(1./(c1 + c2*dl + c3*dl*dl), 1.);

    float I = Ia*ka + Ip*kd*fatt * max(dot(normalizedNormal, fragmentToLight_normalized), 0.); 

    outColor = vec4(material * I, 1.0);
    //outColor = vec4(material * I, 1.0) * texture(texSampler, fragTexCoord);

    */
}
