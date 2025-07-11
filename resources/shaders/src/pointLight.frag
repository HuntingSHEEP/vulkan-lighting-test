#version 450

layout(location=0) in vec2 fragOffset;
layout(location=0) out vec4 outColor;

layout(push_constant) uniform Push {
    vec3 position;
    vec4 color;
} push;

void main(){
    float dist = sqrt(dot(fragOffset, fragOffset));
    if(dist >= 1.0)
        discard;

    outColor = vec4(push.color.xyz * push.color.w, 1.0) * (1-dist);
}