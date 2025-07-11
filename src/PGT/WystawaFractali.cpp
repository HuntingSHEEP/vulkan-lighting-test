#include "PGT/WystawaFractali.h"
#include "PGT/Maze.h"

//Funkcja przygotowująca strukturę pojedynczego modelu, która jest potem wysysłana do silnika renderującego
DrawModelInfo WystawaFractali::createModel(VulkanEngine* vkEngine, Model model, Pipeline pipeline, Texture texture, Transform transform){
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

WystawaFractali::WystawaFractali(VulkanEngine* vkEngine){
    float PI = 3.141592f;

    //Utworzenie obiektów przechowujących tekstury
    Texture stone = vkEngine->initTexture({ "../resources/textures/BaseStone_Basecolor.jpeg" });
    Texture paper = vkEngine->initTexture({ "../resources/textures/whitePaper2.jpg" });
    Texture mandelbrot = vkEngine->initTexture({ "../resources/textures/mandelbrot.bmp" });
    Texture julia = vkEngine->initTexture({ "../resources/textures/julia.bmp" });
    Texture julia2 = vkEngine->initTexture({ "../resources/textures/julia_2.bmp" });
    Texture orbitTrap = vkEngine->initTexture({ "../resources/textures/orbit_trap.bmp" });

    Texture sphereTexture = vkEngine->initTexture({ "../resources/textures/fire_profile2.png" });
    Texture monkeyTexture = vkEngine->initTexture({ "../resources/textures/monkeyTexture.png" });

    //Utworzenie modeli na podstawie plików xx.obj
    Model plane = vkEngine->initModel({ "../resources/models/plane.obj" });
    Model wall = vkEngine->initModel({ "../resources/models/wall.obj" });
    Model sphere = vkEngine->initModel({ "../resources/models/sphere.obj" });
    Model monkey = vkEngine->initModel({ "../resources/models/monkey.obj" });

    //Utworzenie pipelinów. W Vulkanie shadery są przypisane do pipeline na stałe, stąd jedynym sposobem 
    //na obsługę wielu shaderów jest osobny pipeline dla każdego zestawu.
    Pipeline pipelineSphere = vkEngine->initPipeline({ "../resources/shaders/vertSphere.spv" , "../resources/shaders/fragSphere.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST});
    Pipeline pipelineMonkey = vkEngine->initPipeline({ "../resources/shaders/vertMonkey.spv" , "../resources/shaders/fragMonkey.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST});
    Pipeline pipelineColors = vkEngine->initPipeline({ "../resources/shaders/vertColors.spv" , "../resources/shaders/fragColors.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST});

    Pipeline pipeline = vkEngine->initPipeline({ "../resources/shaders/vert.spv" , "../resources/shaders/frag.spv", VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST});
   
    
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Lista do przechowywania wszystki modeli
    std::vector<DrawModelInfo> allModels{};

    //mandelbrot#########################################################################
    Transform wallTransformS{{ -24., 15., 0. }, { 1., 1., 0.7 }, { 0., 0., 1. },  0.f};
    allModels.push_back(createModel(vkEngine, wall, pipeline, paper, wallTransformS));

    Transform plotnoTransformS{{ -24., 14., 6. }, { 1., 1., 1.22 }, { 0., 0., 1. }, PI};
    allModels.push_back(createModel(vkEngine, plane, pipeline, mandelbrot, plotnoTransformS));

    //julia#########################################################################
    wallTransformS = {{ -12., 15., 0. }, { 1., 1., 0.7 }, { 0., 0., 1. },  0.f};
    allModels.push_back(createModel(vkEngine, wall, pipeline, paper, wallTransformS));

    plotnoTransformS = {{ -12., 14., 6. }, { 1., 1., 1.22 }, { 0., 0., 1. }, PI};
    allModels.push_back(createModel(vkEngine, plane, pipeline, julia, plotnoTransformS));

     //julia2#########################################################################
    wallTransformS = {{ 0., 15., 0. }, { 1., 1., 0.7 }, { 0., 0., 1. },  0.f};
    allModels.push_back(createModel(vkEngine, wall, pipeline, paper, wallTransformS));

    plotnoTransformS = {{ 0., 14., 6. }, { 1., 1., 1.22 }, { 0., 0., 1. }, PI};
    allModels.push_back(createModel(vkEngine, plane, pipeline, julia2, plotnoTransformS));

    //orbitTrap#########################################################################
    wallTransformS = {{ 12., 15., 0. }, { 1., 1., 0.7 }, { 0., 0., 1. },  0.f};
    allModels.push_back(createModel(vkEngine, wall, pipeline, paper, wallTransformS));

    plotnoTransformS = {{ 12, 14., 6. }, { 1., 1., 1.22 }, { 0., 0., 1. }, PI};
    allModels.push_back(createModel(vkEngine, plane, pipeline, orbitTrap, plotnoTransformS));


    //SPHERE#########################################################################
    wallTransformS = {{ 30., 50., 0. }, { 10., 10., 10. }, { 0., 0., 1. },  PI/2.f};
    allModels.push_back(createModel(vkEngine, sphere, pipelineSphere, sphereTexture, wallTransformS));

    //MONKEY#########################################################################
    wallTransformS = {{ 60., 50., 0. }, { 10., 10., 10. }, { 0., 0., 1. },  PI};
    allModels.push_back(createModel(vkEngine, monkey, pipelineMonkey, monkeyTexture, wallTransformS));


    //COLORS#########################################################################
    plotnoTransformS = {{ 70., 10., 0. }, { 10., 10., 10. }, { 0., 0., 1. }, PI};
    allModels.push_back(createModel(vkEngine, sphere, pipelineColors, paper, plotnoTransformS));


    //MAZE BINARY TREE########################################################################
    Maze* mazeBinaryTree = new Maze(vkEngine, 20, 30, 0.4f, MAZE_BUILD_ALGORITHM_BINARY_TREE, {-40, 0, 0});
    allModels.push_back(mazeBinaryTree->getDrawInfo());

    //MAZE SIDEWINDER#########################################################################
    Maze* mazeSidewinder = new Maze(vkEngine, 20, 30, 0.4f, MAZE_BUILD_ALGORITHM_SIDEWINDER, {-50, 0, 0});
    allModels.push_back(mazeSidewinder->getDrawInfo());

    
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    std::vector<DrawPointLightInfo> allLights{};
    drawAllOfThem = { allModels, allLights };
};

DrawAllOfThem WystawaFractali::getModels(){
    return drawAllOfThem;
}