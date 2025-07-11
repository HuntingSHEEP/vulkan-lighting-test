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
layout(location = 4) in float iTime;

layout(location = 0) out vec4 outColor;

vec3 rand(vec3 p) {
    float a = fract(sin(dot(p, vec3(12.9898, 78.233, 25.906))) * 43758.5453);
    float b = fract(sin(dot(p, vec3(4.898, 7.23, 10.945))) * 23421.6312);
    float c = fract(sin(dot(p, vec3(53.5253, 82.143, 28.971))) * 23421.6312);
    return normalize(2.0 * vec3(a, b, c) - vec3(1));
}

float noise(vec3 p) {
    vec3 cell = floor(p);
    vec3 pos = fract(p);
    vec3 v1 = rand(cell + vec3(0.0, 0.0, 0.0));
    vec3 v2 = rand(cell + vec3(1.0, 0.0, 0.0));
    vec3 v3 = rand(cell + vec3(0.0, 1.0, 0.0));
    vec3 v4 = rand(cell + vec3(1.0, 1.0, 0.0));
    vec3 v5 = rand(cell + vec3(0.0, 0.0, 1.0));
    vec3 v6 = rand(cell + vec3(1.0, 0.0, 1.0));
    vec3 v7 = rand(cell + vec3(0.0, 1.0, 1.0));
    vec3 v8 = rand(cell + vec3(1.0, 1.0, 1.0));
    float d1 = dot(v1, vec3(0, 0, 0) - pos);
    float d2 = dot(v2, vec3(1, 0, 0) - pos);
    float d3 = dot(v3, vec3(0, 1, 0) - pos);
    float d4 = dot(v4, vec3(1, 1, 0) - pos);
    float d5 = dot(v5, vec3(0, 0, 1) - pos);
    float d6 = dot(v6, vec3(1, 0, 1) - pos);
    float d7 = dot(v7, vec3(0, 1, 1) - pos);
    float d8 = dot(v8, vec3(1, 1, 1) - pos);
    pos = smoothstep(0.0, 1.0, pos);
    float m1 = mix(d1, d2, pos.x);
    float m2 = mix(d3, d4, pos.x);
    float m3 = mix(m1, m2, pos.y);
    float m4 = mix(d5, d6, pos.x);
    float m5 = mix(d7, d8, pos.x);
    float m6 = mix(m4, m5, pos.y);
    float m7 = mix(m3, m6, pos.z);
    return m7 * 1.15470053838;
}

float fbm(vec3 p, int octs) {
    float scale = 0.7;
    float mag = 0.0;
    for (int i = 1; i < 50; i++) {
        mag += noise(p * scale) / scale;
        scale *= 2.0;
        if (i == octs) {
            return mag;
        }
    }
    return mag;
}

void main() {

    vec2 newUV = vec2(fragTexCoord.x, fragTexCoord.y * 0.5);
    vec4 fireTex = vec4(fragColor, 1.0) * texture(texSampler, newUV);
    
    vec3 noise = vec3(fbm(vec3(11.0 * newUV.x, 6.0 * newUV.y + 0.5 * iTime, iTime), 7));
    
    noise = abs(noise) + 0.98;
    
    // Output to screen
    outColor = vec4(fireTex.xyz * noise, 1.0);;
}
