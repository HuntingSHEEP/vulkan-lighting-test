/* 
 Na podstawie materiału https://www.youtube.com/watch?v=f4s1h2YETNY

 Wraz z modelem kuli może służyć jako rodzaj skyboxa
*/
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

//Funkcja kolorująca
vec3 palette(float t){
    vec3 a = vec3(0.4, 0.3, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(0.5, 0.25, 1.0);
    vec3 d = vec3(0.3, 0.1, 0.7);

    return a + b*cos( 6.28318*(c*t + d));
}

void main() {
    //skopiowanie koordynatów tekstury do zmiennej uv, aby móc na niej dokonywać przekształceń 
    vec2 uv = fragTexCoord;

    //środek modelu
    vec2 middle = vec2(0.5, 0.5);

    //zmienna agregująca kolor fragmentu
    vec3 finalColor = vec3(0.0);

        
    for(float i=0.0; i<3; i++){
        //podział przestrzeni na 4 podsektory
        uv = fract(uv * 2.);
        
        //odległość fragmentu od środka modelu
        float d = length(middle - uv);

        //pobranie koloru, modyfikatorem jest czas oraz obrót pętli 
        vec3 col = palette(d + i*.2 + iTime*.5);
        
        //modyfikacja wyliczonego koła aby pulsowało w zależności od czasu
        d *= sin(iTime);

        //sinus zwraca wartości ujemne, obcie wykresu za pomocą modułu
        d = abs(d);

        //wytworzenie pojedynczych kropek 
        d = pow(.005/d, 3);

        //dodanie obliczonego koloru
        finalColor += col * d;

        //utworzenie rozlewających się plam
        finalColor = fract(finalColor * 3);
    }

    outColor = vec4(finalColor * vec3(0.8, 0.0, 0.6) , 1.0);
}