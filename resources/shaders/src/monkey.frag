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
    float iTime;
} ubo;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 fragPosition;
layout(location = 4) in float iTime;

layout(location = 0) out vec4 outColor;

void main() {
    vec2 uv = fragTexCoord.xy;

    float x = uv.x;
    float y = uv.y;
    
    float ncol = 81.0;
    float nrow = 117.0;
    
    float row = floor(x * nrow);
    float col = floor(y * ncol);
    
    outColor = vec4(1.0, 1.0, 1.0, 1.0);

    if(mod(row, 2.0) == 0.0 && mod(col, 2.0) == 0.0)
    {
        outColor = vec4(vec3(0.0), 1.0);
    }
    else if(mod(row, 2.0) == 1.0 && mod(col, 2.0) == 1.0)
    {
        float green = 1.2 - ( fract(x * nrow) + fract(y * ncol) );
        float blue = ( fract(x * nrow) + fract(y * ncol) );
        outColor = vec4(0.0, green, blue, 1.0);
    }
}
