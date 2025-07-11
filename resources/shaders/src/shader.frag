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

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 fragPosition;

layout(location = 0) out vec4 outColor;

//````````````````````````````````````````````````````````````````````````
const float constant    = 1.0;
const float linear      = 0.09;
const float quadratic   = 0.032;
const float PI          = 3.14159265359;

vec3 ambientColor(){
    float Iamb = 0.2;
    vec3 ambColor = vec3(0.9, 0.85, 0.85);
    return Iamb * ambColor;
}

vec3 lambertColor(vec3 norml){
    vec3 lColor = vec3(0.);

    for(int i=0; i<ubo.numberOfLights; i++){
        PointLight light = ubo.pointLights[i];
        float Idi = light.color.w;
        vec3 lPos = vec3(light.position.xyz);
        vec3 lightDir = lPos - fragPosition;

        //wektor od powierzchni do źródła światła
        vec3 Li = normalize(lightDir);

        //tłumienie natężenia w zależności od odległości do źródła
        float dist = length(lightDir);
        float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

        //suma kolorów ze wszystkich źródeł
        lColor += light.color.xyz * Idi * max(0.0, dot(norml, Li)) * attenuation;
    };

    return lColor;
}

vec3 phongColor(vec3 norml, vec3 v, float specularStrength, float shininess){
    vec3 phColor = vec3(0.);

    for(int i=0; i<ubo.numberOfLights; i++){
        PointLight light = ubo.pointLights[i];
        float Idi = light.color.w;
        vec3 lPos = vec3(light.position.xyz);
        vec3 lightDir = lPos - fragPosition;
        
        //wektor od powierzchni do źródła światła
        vec3 Li = normalize(lightDir);  

        //wektor Li odbity zwierciadlanie od powierzchni
        vec3 Ri = normalize(reflect(-Li, norml));  //normalize( 2*norml*dot(norml, Li) - Li); //normalize(reflect(-Li, norml));  

        //tłumienie natężenia w zależności od odległości do źródła
        float dist = length(lightDir);
        float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

        //jeśli światło pada na powierzchnię, to sumuj kolory ze wszystkich źródeł
        phColor += light.color.xyz * Idi * specularStrength * pow(max(dot(Ri, v), 0.0), shininess) * attenuation * max(dot(norml, Li), 0.0); 
    };

    return phColor;
}


vec3 blinnPhongColor(vec3 norml, vec3 v, float specularStrength, float shininess){
    vec3 phColor = vec3(0.);

    for(int i=0; i<ubo.numberOfLights; i++){
        PointLight light = ubo.pointLights[i];
        float Idi = light.color.w;
        vec3 lPos = vec3(light.position.xyz);
        
        //wektor od powierzchni do źródła światła
        vec3 Li = normalize(lPos - fragPosition); 

        //wektor połówkowy pomiędzy Li a obserwatorem
        vec3 H = normalize(Li + v);

        vec3 lightDir = lPos - fragPosition;
        float dist = length(lightDir);
        float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

        //jeśli światło pada na powierzchnię, to sumuj kolory ze wszystkich źródeł 
        phColor += light.color.xyz * Idi * specularStrength * pow(max(dot(H, norml), 0.0), 4*shininess) * attenuation * max(dot(norml, Li), 0.0); 
    };

    return phColor;
}


vec3 computeTangent(vec3 N) {
    vec3 T = vec3(1.0, 0.0, 0.0);  // Początkowy wektor dla tangenty

    // Jeśli normalna jest prawie równoległa do (1, 0, 0), wybierz (0, 1, 0)
    if (abs(N.x) > 0.999) {
        T = vec3(0.0, 1.0, 0.0);
    }

    // Wyznacz bitangentę
    vec3 B = normalize(cross(N, T));
    
    // Ponownie oblicz tangentę, aby upewnić się, że jest prostopadła do normalnej
    T = cross(B, N);

    return T;
}

vec3 computeBitangent(vec3 N, vec3 T) {
    return normalize(cross(N, T));
}

//α to szorstkość powierzchni, przy czym wartość  
//0 odpowiada idealnie gładkiej powierzchni, a wartość 1 odpowiada bardzo szorstkiej powierzchni.
float D_GGX(vec3 n, vec3 h, float alpha){
    float a = pow(max(dot(n, h), 0.0), 2);
    float b = alpha * alpha - 1;
    return (alpha * alpha) / (PI * pow( a * b + 1, 2));
}

float D_Beckmann(vec3 n, vec3 h, float alpha) {
    float a = alpha * alpha;
    float cosThetaH = max(dot(n, h), 0.00001);
    float cosThetaH2 = cosThetaH * cosThetaH;

    // Kąt pomiędzy wektorem normalnym a połowicznym
    float tanThetaH2 = (1.0 - cosThetaH2) / cosThetaH2;
    
    float D = exp(-tanThetaH2 / a) / (a * cosThetaH2 * cosThetaH2);
    return D;
}

float D_Gauss(vec3 n, vec3 h, float alpha, float c){
    vec3 nNormalized = normalize(n);
    vec3 hNormalized = normalize(h);

    float delta = acos(dot(nNormalized, hNormalized));
    float ex = delta / alpha;
    return c * exp((-1) * ex * ex);
}


float D_Ward(vec3 N, vec3 H, float alpha_x, float alpha_y) {
    vec3 tangent = computeTangent(N);
    vec3 bitangent = computeBitangent(N, tangent);
    vec3 X = tangent;
    vec3 Y = bitangent;


    float NdotH = max(dot(N, H), 0.000001);
    float NdotH2 = NdotH * NdotH;

    float HdotX = dot(H, X);
    float HdotY = dot(H, Y);
    float HdotX2 = HdotX * HdotX;
    float HdotY2 = HdotY * HdotY;

    float tanThetaH2 = (1.0 - NdotH2) / NdotH2;

    float exponent = -((HdotX2 / (alpha_x * alpha_x)) + (HdotY2 / (alpha_y * alpha_y))) * tanThetaH2;
    float denominator = 4.0 * PI * alpha_x * alpha_y * NdotH2 * NdotH2;

    float D = exp(exponent) / denominator;
    return D;
}

float G(vec3 n, vec3 v, vec3 l, vec3 h){
    float a = 2*max(dot(h, n), 0.0) * max(dot(n, v), 0.0) / max(dot(v, h), 0.00001);
    float b = 2*max(dot(h, n), 0.0) * max(dot(n, l), 0.0) / max(dot(v, h), 0.00001);

    return min(1.0, min(a, b));
}

float F(vec3 n, vec3 v, vec3 l, vec3 h){
    float f0 = 0.951;
    float F = f0 + (1 - f0) * pow(max((1 - dot(v, h)), 0.0) , 5);

    return F;
}


vec3 cookeTorranceColor(vec3 norml, vec3 v, float specular, float alpha, float kd){
    float s = specular;
    float d = 1 - s;

    vec3 phColor = vec3(0.0);

    for(int i=0; i<ubo.numberOfLights; i++){
        PointLight light = ubo.pointLights[i];
        float Idi = light.color.w;
        vec3 lPos = vec3(light.position.xyz);
        vec3 lightDir = lPos - fragPosition;
        
        //wektor od powierzchni do źródła światła
        vec3 Li = normalize(lightDir); 

        //wektor połówkowy pomiędzy Li a obserwatorem
        vec3 H = normalize(Li + v);

        //attenuation
        float dist = length(lightDir);
        float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

        float rd = kd;   
        float GDF = G(norml, v, Li, H) * D_Beckmann(norml, H, alpha) * F(norml, v, Li, H);
        float rs = GDF / (4.0 * max(dot(norml, Li), 0.00001) * max(dot(norml, v), 0.00001));


        //jeśli światło pada na powierzchnię, to sumuj kolory ze wszystkich źródeł 
        phColor += light.color.xyz * Idi * max(dot(norml, Li), 0.0) * (d*rd + s*rs) ; 
    };

    return phColor;
}



vec3 torranceSparrowColor(vec3 norml, vec3 v, float specular, float alpha, float c, float kd){
    float s = specular;
    float d = 1 - s;

    vec3 phColor = vec3(0.0);

    for(int i=0; i<ubo.numberOfLights; i++){
        PointLight light = ubo.pointLights[i];
        float Idi = light.color.w;
        vec3 lPos = vec3(light.position.xyz);
        vec3 lightDir = lPos - fragPosition;
        
        //wektor od powierzchni do źródła światła
        vec3 Li = normalize(lightDir); 

        //wektor połówkowy pomiędzy Li a obserwatorem
        vec3 H = normalize(Li + v);

        //attenuation
        float dist = length(lightDir);
        float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

        float rd = kd;   
        float GDF = G(norml, v, Li, H) * D_Gauss(norml, H, alpha, c) * F(norml, v, Li, H);
        float rs = GDF / (4.0 * max(dot(norml, Li), 0.00001) * max(dot(norml, v), 0.00001));


        //jeśli światło pada na powierzchnię, to sumuj kolory ze wszystkich źródeł 
        phColor += light.color.xyz * Idi * max(dot(norml, Li), 0.0) * (d*rd + s*rs) ; 
    };

    return phColor;
}


/*
vec3 wardColor(vec3 norml, vec3 v, float specular, float alphaX, float alphaY, float kd){
    float s = specular;
    float d = 1 - s;

    vec3 phColor = vec3(0.0);

    for(int i=0; i<ubo.numberOfLights; i++){
        PointLight light = ubo.pointLights[i];
        float Idi = light.color.w;
        vec3 lPos = vec3(light.position.xyz);
        vec3 lightDir = lPos - fragPosition;
        
        //wektor od powierzchni do źródła światła
        vec3 Li = normalize(lightDir); 

        //wektor połówkowy pomiędzy Li a obserwatorem
        vec3 H = normalize(Li + v);

        //attenuation
        float dist = length(lightDir);
        float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

        float rd = kd;   
        float GDF = G(norml, v, Li, H) * D_Ward(norml, H, alphaX, alphaY) * F(norml, v, Li, H);
        float rs = GDF / (4.0 * max(dot(norml, Li), 0.00001) * max(dot(norml, v), 0.00001));


        //jeśli światło pada na powierzchnię, to sumuj kolory ze wszystkich źródeł 
        phColor += light.color.xyz * Idi * max(dot(norml, Li), 0.0) * (d*rd + s*rs); 
    };

    return phColor;
} */

vec3 wardIsotropicColor(vec3 norml, vec3 v, float specular, float alpha, float Cs, float Cd){
    float s = specular;
    float d = 1 - s;

    vec3 phColor = vec3(0.0);

    for(int i=0; i<ubo.numberOfLights; i++){
        PointLight light = ubo.pointLights[i];
        float Idi = light.color.w;
        vec3 lPos = vec3(light.position.xyz);
        vec3 lightDir = lPos - fragPosition;
        
        //wektor od powierzchni do źródła światła
        vec3 Li = normalize(lightDir); 

        //wektor połówkowy pomiędzy Li a obserwatorem
        vec3 H = normalize(Li + v);

        //attenuation
        float dist = length(lightDir);
        float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

        float a = 1 / sqrt( max(dot(norml, Li), 0.00001) * max(dot(norml, v), 0.00001) );
        float b = 1 / ( 4 * PI * alpha * alpha );
        float nhcos = max(dot(norml, H), 0.00001); 
        float ex = ( nhcos * nhcos - 1 ) / ( nhcos * alpha * alpha );
        
        float js = Cs * a * b * exp( ex );  
        float jd = Cd;


        //jeśli światło pada na powierzchnię, to sumuj kolory ze wszystkich źródeł 
        phColor += light.color.xyz * Idi * max(dot(norml, Li), 0.0) * (s*js + d*jd) * attenuation; 
    };

    return phColor;
}


vec3 wardAnisotropicColor(vec3 norml, vec3 v, float specular, float alphaT,  float alphaB, float Cs, float Cd){
    float s = specular;
    float d = 1 - s;

    vec3 tangent = computeTangent(norml);
    vec3 bitangent = computeBitangent(norml, tangent);

    vec3 phColor = vec3(0.0);

    for(int i=0; i<ubo.numberOfLights; i++){
        PointLight light = ubo.pointLights[i];
        float Idi = light.color.w;
        vec3 lPos = vec3(light.position.xyz);
        vec3 lightDir = lPos - fragPosition;
        
        //wektor od powierzchni do źródła światła
        vec3 Li = normalize(lightDir); 

        //wektor połówkowy pomiędzy Li a obserwatorem
        vec3 H = normalize(Li + v);

        //attenuation
        float dist = length(lightDir);
        float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist * dist));

        float a = 1 / sqrt( max(dot(norml, Li), 0.00001) * max(dot(norml, v), 0.00001) );
        float b = 1 / ( 4 * PI * alphaT * alphaB );

        float ht = ( max(dot(H, tangent), 0.0) * max(dot(H, tangent), 0.0) ) / ( alphaT * alphaT );
        float hb = ( max(dot(H, bitangent), 0.0) * max(dot(H, bitangent), 0.0) ) / ( alphaB * alphaB );
        float ex = (-2) * ( ht + hb ) / ( 1 + max(dot(norml, H), 0.00001) );
        
        float js = Cs * a * b * exp( ex );  
        float jd = Cd;


        //jeśli światło pada na powierzchnię, to sumuj kolory ze wszystkich źródeł 
        phColor += light.color.xyz * Idi * max(dot(norml, Li), 0.0) * (s*js + d*jd) * attenuation; 
    };

    return phColor;
}



//````````````````````````````````````````````````````````````````````````


void main() {
    //normalizacja normalnej powierzchni
    vec3 nNormal = normalize(normal);

    //pozycja kamery
    vec3 camPos = ubo.cameraPosition;

    //wektor od fragmentu do kamery
    vec3 v = normalize(camPos - fragPosition); 

//vec3 wardIsotropicColor(vec3 norml, vec3 v, float specular, float alpha, float Cs, float Cd){

    vec3 lightColor = wardIsotropicColor(nNormal, v, 0.2, 0.1, 1.0, 1.0); //wardColor(nNormal, v, 0.7, 0.1, 0.01, 0.22); //torranceSparrowColor(nNormal, v, 0.2, 0.3, 0.22); //wardColor(nNormal, v, 0.9, 0.2, 0.2); //cookeTorranceColor(nNormal, v, 0.9);  // wardColor(nNormal, v, 0.99, 0.01, 0.9); //ambientColor() + lambertColor(nNormal); // blinnPhongColor(nNormal, v, 0.9, 23); // + blinnPhongColor(nNormal, v, 0.9, 3);

    outColor =  vec4(lightColor * fragColor, 1.0); //* texture(texSampler, fragTexCoord); //vec4(nNormal, 1.0); //vec4(lightColor * fragColor, 1.0);
}
