
#define private_mem private
#define private_fun private
#define public_mem public
#define public_fun public

//#define TEST_ENVIRONMENT

#define GLFW_INCLUDE_VULKAN //#include <vulkan/vulkan.h>
#include <glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>

class HelloTriangleApplication
{
public_mem:

public_fun:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private_mem:
    GLFWwindow * window;
    static const unsigned int WIDTH = 800;
    static const unsigned int HEIGHT = 600;

    VkInstance instance;

private_fun:
    void initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Practice", nullptr, nullptr);
    }

    void initVulkan()
    {
        createInstance();
    }

    void createInstance()
    {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);
        appInfo.apiVersion = VK_API_VERSION_1_1;

        unsigned int glfwExtensionCount = 0;
        const char ** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create instance!");
        }
    }

    void mainLoop()
    {
        while(!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }

    void cleanup()
    {
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main(void)
{
#ifdef TEST_ENVIRONMENT
    int mainTest(void);
    return mainTest();

#else
    HelloTriangleApplication app;

    try
    {
        app.run();
    }
    catch(const std::runtime_error & e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
#endif
}