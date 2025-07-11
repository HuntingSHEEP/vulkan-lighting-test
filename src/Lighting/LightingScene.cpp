#include "Lighting/LightingScene.h"

LightingScene::LightingScene(VulkanEngine* vkEngine){
    float PI = 3.141592f;

    //Utworzenie obiektów przechowujących tekstury
    Texture stone = vkEngine->initTexture({ "../resources/textures/BaseStone_Basecolor.jpeg" });
    Texture paper = vkEngine->initTexture({ "../resources/textures/whitePaper2.jpg" });

    Texture sphereTexture = vkEngine->initTexture({ "../resources/textures/fire_profile2.png" });
    Texture monkeyTexture = vkEngine->initTexture({ "../resources/textures/monkeyTexture.png" });

    //Utworzenie modeli na podstawie plików xx.obj
    Model plane = vkEngine->initModel({ "../resources/models/plane.obj" });
    Model wall = vkEngine->initModel({ "../resources/models/wall.obj" });
    Model sphere = vkEngine->initModel({ "../resources/models/sphere.obj" });
    Model monkey = vkEngine->initModel({ "../resources/models/monkey.obj" });
    Model cube = vkEngine->initModel({ "../resources/models/cube.obj" });

    //Utworzenie pipelinów. W Vulkanie shadery są przypisane do pipeline na stałe, stąd jedynym sposobem 
    //na obsługę wielu shaderów jest osobny pipeline dla każdego zestawu.
    Pipeline pipelineSphere = vkEngine->initPipeline({ "../resources/shaders/vertSphere.spv" , "../resources/shaders/fragSphere.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST});
    Pipeline pipelineMonkey = vkEngine->initPipeline({ "../resources/shaders/vertMonkey.spv" , "../resources/shaders/fragMonkey.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST});
    Pipeline pipelineColors = vkEngine->initPipeline({ "../resources/shaders/vertColors.spv" , "../resources/shaders/fragColors.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST});

    Pipeline pipeline2 = vkEngine->initPipeline({ "../resources/shaders/vert.spv" , "../resources/shaders/frag.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST});
    Pipeline lightPipeline= vkEngine->initPipeline({ "../resources/shaders/pointLight.vert.spv", "../resources/shaders/pointLight.frag.spv" });
        
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Lista do przechowywania wszystki modeli
    std::vector<DrawModelInfo> allModels{};

    
    //SPHERE__#########################################################################
    Transform transform = {{ 0., 0., 0. }, { 1., 1., 1. }, { 0., 0., 1. },  0.f};
    allModels.push_back(createModel(vkEngine, sphere, pipeline2, paper, transform));

    //MONKEY__#########################################################################
    transform = {{ -4., 0., 0. }, { 1., 1., 1. }, { 0., 0., 1. },  0.f};
    allModels.push_back(createModel(vkEngine, monkey, pipeline2, stone, transform));

    //CUBE__###########################################################################
    transform = {{ 4., 0., 0. }, { 1., 1., 1. }, { 0., 0., 1. },  0.f};
    allModels.push_back(createModel(vkEngine, cube, pipeline2, stone, transform));



    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Lista do przechowywania wszystkich źródeł światła
    std::vector<DrawPointLightInfo> allLights{};

    //OGNIK0#########################################################################
    transform = {{ -2., -4., 3. }, { 1., 1., 1. }, { 0., 0., 1. }};
    allLights.push_back(createPointLight(vkEngine, lightPipeline, stone, transform, {0., 1., 1., 1.}));

    //OGNIK1#########################################################################
    transform = {{ 2., -3., 1. }, { 1., 1., 1. }, { 0., 0., 1. }};
    allLights.push_back(createPointLight(vkEngine, lightPipeline, stone, transform, {1., 1., 0., 1.}));


    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    drawAllOfThem = { allModels, allLights };
};

DrawAllOfThem LightingScene::getModels(){
    return drawAllOfThem;
}


//Funkcja przygotowująca strukturę pojedynczego modelu, która jest potem wysysłana do silnika renderującego
DrawModelInfo LightingScene::createModel(VulkanEngine* vkEngine, Model model, Pipeline pipeline, Texture texture, Transform transform){
    UniformBuffer wallBufferS = vkEngine->initUniformBuffer();
    DescriptorSets wallDescriptorS = vkEngine->initDescriptorSet(texture, wallBufferS);

    DrawModelInfo drawWallS{};
    drawWallS.model = model;
    drawWallS.transform = transform;
    drawWallS.pipeline = pipeline;
    drawWallS.buffer = wallBufferS;
    drawWallS.descriptorSets = wallDescriptorS;

    return drawWallS;
};

DrawPointLightInfo LightingScene::createPointLight(VulkanEngine* vkEngine, Pipeline pipeline, Texture texture, Transform transform, glm::vec4 color){
    UniformBuffer ognikBuffer = vkEngine->initUniformBuffer();
    DescriptorSets ognikDescriptorSet = vkEngine->initDescriptorSet(texture, ognikBuffer); //TODO tekstura może być opcjonalna,

    DrawPointLightInfo ognik{};
    ognik.transform = transform;
    ognik.pipeline = pipeline;
    ognik.buffer = ognikBuffer;
    ognik.descriptorSets = ognikDescriptorSet;
    ognik.color = color;

    return ognik;
};
