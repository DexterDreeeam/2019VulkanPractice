
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
#include <cstring>

VkResult CreateDebugReportCallbackEXT(
    VkInstance instance,
    const VkDebugReportCallbackCreateInfoEXT * pCreateInfo,
    const VkAllocationCallbacks * pAllocator,
    VkDebugReportCallbackEXT * pCallback
                                     )
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if(func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugReportCallbackEXT(
    VkInstance instance, 
    VkDebugReportCallbackEXT callback,
    const VkAllocationCallbacks * pAllocator
                                  )
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if(func != nullptr)
    {
        func(instance, callback, pAllocator);
    }
}

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
    static const unsigned int WIDTH = 800;
    static const unsigned int HEIGHT = 600;
    const std::vector<const char *> validationLayers =
    {
        "VK_LAYER_LUNARG_standard_validation"
    };
#ifdef NDEBUG
    static const bool enableValidationLayers = false;
#else
    static const bool enableValidationLayers = true;
#endif

    GLFWwindow * window;
    VkInstance instance;
    VkDebugReportCallbackEXT callback;

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
        setupDebugCallback();
    }

    void createInstance()
    {
        if(enableValidationLayers && !checkValidationLayerSupport())
        {
            throw std::runtime_error("Validation layer requested, but not available.");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);
        appInfo.apiVersion = VK_API_VERSION_1_1;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        std::vector<const char *> extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        if(enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create instance!");
        }
    }

    void setupDebugCallback()
    {
        if(!enableValidationLayers)
            return;
        
        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
        createInfo.pfnCallback = debugCallback;

        if(CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to set up debug callback!");
        }
    }

    std::vector<const char *> getRequiredExtensions()
    {
        std::vector<const char *> extensions;
        unsigned int glfwExtensionCount = 0;
        const char ** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for(unsigned int i = 0; i != glfwExtensionCount; ++i)
        {
            extensions.push_back(glfwExtensions[i]);
        }
        if(enableValidationLayers)
        {
            extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }
        return extensions;
    }

    bool checkValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(const char * layerName : validationLayers)
        {
            bool layerFound = false;
            for(const auto & layerProperties : availableLayers)
            {
                if(strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }
            if(!layerFound)
            {
                return false;
            }
        }
        return true;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objType,
        uint64_t obj,
        size_t location,
        int32_t code,
        const char * layerPrefix,
        const char * msg,
        void * userData
                                                       )
    {
        std::cerr << "Validation layer : " << msg << std::endl;
        return VK_FALSE;
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
        DestroyDebugReportCallbackEXT(instance, callback, nullptr);

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