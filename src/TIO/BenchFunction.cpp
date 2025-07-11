#include "TIO/BenchFunction.h"
#include "GraphicsEngine/GetPointMatrix.h"


BenchFunction::BenchFunction(VulkanEngine* app){
        Transform t{};
        t.position = { 0., 0., 0. };
        t.scale = { 1., 1., 1. };
        t.rotationAxis = { 0., 0., 1. };
        t.rotationAngle = 3.1415f;

        Pipeline pipeline = app->initPipeline({ "../resources/shaders/vert.spv" , "../resources/shaders/frag.spv", VK_PRIMITIVE_TOPOLOGY_POINT_LIST });
        Model wall = getPointMatrix(app, -6, 6, 100); //getPointMatrix(app, 100, .01);
        Texture paper = app->initTexture({ "../resources/textures/whitePaper2.jpg" });

        UniformBuffer uBuffer = app->initUniformBuffer();
        DescriptorSets dSet = app->initDescriptorSet(paper, uBuffer);

        mapBenchFunction(app, &wall);

        drawInfo = {wall, t, pipeline, uBuffer, dSet};
    };

void BenchFunction::himmelBlauTransformation(Vertex& vertex){
        glm::vec3 p = vertex.pos;
        float himmel = float(pow(p.x*p.x + p.z - 11, 2) + pow(p.x + p.z*p.z -7 ,2));
        vertex.pos.y = himmel;
    }

    void BenchFunction::normalize(std::vector<Vertex>& vertices, float scale){
        float yMax = 0;
        float yMin = 0;

        for (int i = 0; i < vertices.size(); i++){
            float y = vertices[i].pos.y; 
            if (y < yMin)
                yMin = y;
            else if (yMax < y)
                yMax = y;
        }

        float range = yMax - yMin;
        float modifier = scale/range;

        for (int i = 0; i < vertices.size(); i++)
            vertices[i].pos.y *= modifier;


        printf("MIN: %.2f  MAX %.2f\n", yMin, yMax);
    }

// Funkcja kolorująca wierzchołki w zależności od głębokości w osi Y
void BenchFunction::colorizeByDepth(std::vector<Vertex>& vertices) {
    // Znajdujemy minimalną i maksymalną głębokość
    float yMin = vertices[0].pos.y;
    float yMax = vertices[0].pos.y;
    for (const auto& vertex : vertices) {
        if (vertex.pos.y < yMin)
            yMin = vertex.pos.y;
        if (vertex.pos.y > yMax)
            yMax = vertex.pos.y;
    }

    // Kolorujemy wierzchołki na podstawie ich głębokości w osi Y
    for (auto& vertex : vertices) {
        // Interpolacja kolorów w przestrzeni HSL
        float t = (vertex.pos.y - yMin) / (yMax - yMin);
        glm::vec3 hslColor(240.0f * (1.0f - t), 1.0f, 0.5f); // Barwa, nasycenie, jasność

        // Konwersja z HSL do RGB
        float hue = hslColor.x;
        float saturation = hslColor.y;
        float lightness = hslColor.z;

        float chroma = (1.0f - fabs(2.0f * lightness - 1.0f)) * saturation;
        float huePrime = hue / 60.0f;
        float x = chroma * (1.0f - fabs(fmod(huePrime, 2.0f) - 1.0f));

        glm::vec3 rgbColor;
        if (0 <= huePrime && huePrime < 1)
            rgbColor = glm::vec3(chroma, x, 0);
        else if (1 <= huePrime && huePrime < 2)
            rgbColor = glm::vec3(x, chroma, 0);
        else if (2 <= huePrime && huePrime < 3)
            rgbColor = glm::vec3(0, chroma, x);
        else if (3 <= huePrime && huePrime < 4)
            rgbColor = glm::vec3(0, x, chroma);
        else if (4 <= huePrime && huePrime < 5)
            rgbColor = glm::vec3(x, 0, chroma);
        else if (5 <= huePrime && huePrime < 6)
            rgbColor = glm::vec3(chroma, 0, x);

        float m = lightness - 0.5f * chroma;
        vertex.color = rgbColor + glm::vec3(m);
    }
}

    
    void BenchFunction::mapBenchFunction(VulkanEngine* app, Model* model) { //jamcolor
        std::vector<Vertex> vertices = model->vertices;

        for (int i = 0; i < vertices.size(); i++)
            himmelBlauTransformation(vertices[i]);
        

        normalize(vertices, 10.);
        colorizeByDepth(vertices);
        
        app->updateModel(model, vertices);
    };

    DrawModelInfo& BenchFunction::getDrawInfo(){
        return drawInfo;
    };



