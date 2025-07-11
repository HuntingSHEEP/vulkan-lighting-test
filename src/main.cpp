#include <iostream>

#include "GraphicsEngine/VulkanEngine.h"
#include "GraphicsEngine/HelperStructures.h"
#include "Lighting/LightingScene.h"

struct Statistics{
    double currentTime;
    double lastUpdateFPS = glfwGetTime();
    int licznik = 0;

    void update(double cTime){
        currentTime = cTime;
    };
    
    void printFPS(){
        double deltaTimeFPS = currentTime - lastUpdateFPS;

        if (deltaTimeFPS > 1) {
            printf("FPS: %10.2f\n", licznik / deltaTimeFPS);
            licznik = 0;
            lastUpdateFPS = currentTime;
        }
        else
            licznik++;
    };
};


int main(){
    try{
        //Inicjowanie silnika renderującego
        VulkanEngine* vkEngine = new VulkanEngine();
        vkEngine->setupCallbacks(vkEngine->window);

        //Przygotowanie sceny
        LightingScene* lightingScene = new LightingScene(vkEngine);

        //Pobranie ze sceny wszystkich modeli do rysowania
        DrawAllOfThem models = lightingScene->getModels();

        //Tworzenie struktury odpowiedzialnej za rejestrowanie statystyk
        Statistics stats;

        //Główna pętla rysująca
        while(!glfwWindowShouldClose(vkEngine->window)){
            glfwPollEvents();
            vkEngine->updateTime();
            vkEngine->updateCamera(2.);
            stats.update(vkEngine->currentTime);

            //Rysowanie modeli
            vkEngine->drawFrameAnother(models);

            stats.printFPS();
        }

        vkDeviceWaitIdle(vkEngine->device);
        vkEngine->cleanup();
    }
    catch(const std::exception& e){std::cout<< "ERROR: "<<e.what()<<std::endl;}
    catch(...){std::cout<< "Unknown error"<<std::endl;}

    return 0;
}
