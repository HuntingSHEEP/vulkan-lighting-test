#pragma once 
#include "GraphicsEngine/HelperStructures.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <unordered_map>





class VulkanEngine {
public:
    static bool rmbIsPressed;
    static glm::vec2 rmbPressed;

    static glm::vec3 eyePosition;
    static glm::vec3 lookDirection;
    static double currentTime;
    static double deltaTime;

    static Key keyQ;
    static Key keyE;
    static Key keyC;
    static Key keyW;
    static Key keyS;
    static Key keyA;
    static Key keyD;
    static Key keySPACE;
    static Key keyCTRL_LEFT;
    static Key keySHIFT_LEFT;
    static Key keyF11;

    

    GLFWwindow* window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;

    VkCommandPool commandPool;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    std::vector<VkImage> textureImageALL;
    std::vector<VkDeviceMemory> textureMemoryALL;
    std::vector<VkImageView> textureImageViewALL;
    std::vector<VkSampler> textureSamplerALL;

    std::vector<VkBuffer> buffersALL;
    std::vector<VkDeviceMemory> buffersMemoryALL;

    std::vector<VkPipeline> pipelineALL;
    std::vector<VkPipelineLayout> pipelineLayoutALL;

    std::vector<VkBuffer> uniformBuffersALL;
    std::vector<VkDeviceMemory> uniformBuffersMemoryALL;

    std::vector<VkDescriptorPool> descriptorPoolALL;
    std::vector<VkDescriptorSet> descriptorSetsALL;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    bool framebufferResized = false;

    // Funkcja do generowania losowej liczby z zakresu [min, max]

    VulkanEngine();
    ~VulkanEngine();

    void setupCallbacks(GLFWwindow* window);
    void updateTime();
    

    float randomFloat(float min, float max);

    void initWindow();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    void initVulkan();

    Model initModel(ModelCreateInfo createInfo);


    Model initModel(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

    void updateModel(Model* model, std::vector<Vertex> vertices);

    void updateModel(Model* model, std::vector<Vertex> vertices, std::vector<uint32_t> indices);

    Pipeline initPipeline(PipelineCreateInfo createInfo);

    Texture initTexture(TextureCreateInfo createInfo);

    UniformBuffer initUniformBuffer();

    DescriptorSets initDescriptorSet(Texture texture, UniformBuffer buffer);



    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    template <typename T> static int sgn(T val);

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);



    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void updateCamera(float movementSpeed);

    void lerpTime(glm::vec3 x1, glm::vec3 x2, glm::vec3 * x, glm::vec3 v1, glm::vec3 v2, glm::vec3* v, float t1, float t2, float t);

    double f(double x, double a);

    void sequenceTransformation(Model* model, glm::vec3 p0, float a, float b);


    void mainLoop(DrawAllOfThem models);

    void cleanupSwapChain();
    void cleanup();

    void recreateSwapChain();

    void createInstance();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    void setupDebugMessenger();

    void createSurface();

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSwapChain();

    void createImageViews();

    void createRenderPass();

    void createDescriptorSetLayout();

    void createGraphicsPipeline(VkPipeline* graphicsPipeline, VkPipelineLayout* pipelineLayout, const char* vertShaderPath, const char* fragShaderPath, VkPrimitiveTopology primitiveTopology);

    void createFramebuffers();

    void createCommandPool();
    void createDepthResources();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    VkFormat findDepthFormat();

    bool hasStencilComponent(VkFormat format);
    void createTextureImage(const char* filePath, VkImage* textureImage, VkDeviceMemory* textureImageMemory);

    void createTextureImageView(VkImageView* textureImageView, VkImage* textureImage);

    void  createTextureSampler(VkSampler* textureSampler);

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createVertexBuffer(std::vector<Vertex> vertices, VkBuffer* vertexBuffer, VkDeviceMemory* vertexBufferMemory);

    void updateVertexBuffer(std::vector<Vertex> vertices, VkBuffer* vertexBuffer);

    void updateIndexBuffer(std::vector<uint32_t> indices, VkBuffer* indexBuffer, VkDeviceMemory* indexBufferMemory);

    void createIndexBuffer(std::vector<uint32_t> indices, VkBuffer* indexBuffer, VkDeviceMemory* indexBufferMemory);

    void createUniformBuffers(std::vector<VkBuffer>& uniformBuffers, std::vector<VkDeviceMemory>& uniformBuffersMemory, std::vector<void*>& uniformBuffersMapped);

    void createDescriptorPool(VkDescriptorPool* descriptorPool);

    void createDescriptorSets(VkDescriptorPool* descriptorPool, std::vector<VkDescriptorSet>& descriptorSets, std::vector<VkBuffer>& uniformBuffers, VkImageView* textureImageView, VkSampler* textureSampler);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createCommandBuffers();

    void recordCommandBufferAnother(VkCommandBuffer commandBuffer, uint32_t imageIndex, DrawAllOfThem drawInfo);

    void createSyncObjects();

    void updateUniformBuffer(uint32_t currentImage, std::vector<void*>& uniformBuffersMapped, float deg, Transform transform, DrawAllOfThem models);
    
    
    void drawFrameAnother(DrawAllOfThem models);

    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) ;

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    bool isDeviceSuitable(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    std::vector<const char*> getRequiredExtensions();

    bool checkValidationLayerSupport();
    
    static std::vector<char> readFile(const std::string& filename);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    void piszRozmiar();

    void robObiekty();
};

