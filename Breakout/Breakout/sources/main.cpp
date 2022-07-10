#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <filesystem>
#include <future>

#include "Shader.hpp"
#include "Renderer.hpp"
#include "Util.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "SDL.h"
#include "Settings.hpp"
#include "SoundEngine.hpp"
#include "tinyxml/tinyxml2.h"
#undef main

Settings& settings = Settings::getInstance();
SoundEngine& soundEngine = SoundEngine::getInstance();

bool bPaused = false;

double lastX, lastY;

bool firstMouse = true;

Camera* activeCamera;

Object* activeObject;
Object* activeObject2;

void framebuffer_size_callback(GLFWwindow* window, int Width, int Height) {
    settings.width = Width;
    settings.height = Height;

    glViewport(0, 0, Width, Height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // todo pause menu 
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_P && action == GLFW_PRESS) bPaused = !bPaused;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) if (activeObject) activeObject->rotate({0, 45, 0});
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) if (activeObject2) activeObject2->rotate({0, 45, 0});
}

void processInput(GLFWwindow* window, double deltaTime) {
    float speed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) speed *= 5.0f;

    if (glfwGetKey(window, GLFW_KEY_W)) activeCamera->move(activeCamera->getFront() * speed);
    if (glfwGetKey(window, GLFW_KEY_S)) activeCamera->move(-activeCamera->getFront() * speed);
    if (glfwGetKey(window, GLFW_KEY_A)) activeCamera->move(-activeCamera->getRight() * speed);
    if (glfwGetKey(window, GLFW_KEY_D)) activeCamera->move(activeCamera->getRight() * speed);
    if (glfwGetKey(window, GLFW_KEY_Q)) activeCamera->move(activeCamera->getViewUp() * speed);
    if (glfwGetKey(window, GLFW_KEY_E)) activeCamera->move(-activeCamera->getViewUp() * speed);
}

void mouse_callback(GLFWwindow* window, double x, double y) {
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    double xoffset = x - lastX;
    double yoffset = lastY - y;

    lastX = x;
    lastY = y;


    xoffset *= settings.sensitivity;
    yoffset *= settings.sensitivity;

    activeCamera->rotate(glm::vec3(0, yoffset, xoffset));
}

int main(int argc, char* argv[]) {
    GLFWwindow* window;

    glfwInit();
    gladLoadGL();

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(settings.width, settings.height, "Breakout", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to Create OpenGL Context" << std::endl;
        return 1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return 1;
    }
    // todo move this to main menu, toggle when in menu
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.15f, 0.1f, 0.1f, 1);

    glfwSwapInterval(0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;


    settings.runtimePath = std::filesystem::path(argv[0]).parent_path();
    settings.defaultShader = new Shader("phongShading");

    activeCamera = new Camera(glm::vec3(3, 4, 1), glm::vec3(0, 0, 0));

    auto scene = new Scene(activeCamera);
    auto plane = scene->load("plane/plane.obj", settings.defaultShader, false);

    auto shader1 = new Shader("phongShading");
    auto shader2 = new Shader("phongShading");

    auto brickParent = scene->load("brick/brick.obj", shader1, false);
    auto brick = scene->load("brick/brick.obj", shader2, false);

    activeObject = brickParent;
    activeObject2 = brick;

    brick->attachToObject(brickParent);
    brick->setLocation({1, 0, 0});

    brickParent->material->setDiffuse({0, 0, 1});
    brick->material->setDiffuse({1, 0, 0});

    brickParent->setLocation({2, 0, 0});
    //brick->setScale({0.1f, 0.1f, 0.1f});

     brickParent->rotate({0, 90, 0});
    //brick->rotate({0, 35, 0});

    glm::vec3 brickParentGlobalLocation = brickParent->getGlobalLocation();
    glm::vec3 brickGlobalLocation = brick->getGlobalLocation();
    std::cout << "brickParentGlobalLocation: " << brickParentGlobalLocation.x << ", " << brickParentGlobalLocation.y <<
        ", " << brickParentGlobalLocation.z << std::endl;
    std::cout << "brickGlobalLocation: " << brickGlobalLocation.x << ", " << brickGlobalLocation.y << ", " <<
        brickGlobalLocation.z << std::endl;


    plane->material->setTextureMap(DIFFUSE, settings.runtimePath / "resources" / "plane/WoodFineDark004_COL_4K.jpg");

    scene->addLight(new Light({1, 5, 1}, {1, 1, 1}, 10.0f));
    scene->addLight(new Light({5, 2, 4}, {0.5, 0.8, 0.7}, 1.0f));

    auto renderer = new Renderer(scene, activeCamera);

    auto cue = soundEngine.loadSoundCue("audio/177850__motion-s__hit-modulated-fx.wav");
    cue->play(0);

    double lastFrame = 0.0;
    double deltaTime;

    while (glfwWindowShouldClose(window) == false) {
        const double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        processInput(window, deltaTime);

        if (!bPaused) scene->tick(deltaTime);
        renderer->render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings", nullptr, flags);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0 / static_cast<double>(ImGui::GetIO().Framerate),
                    static_cast<double>(ImGui::GetIO().Framerate));
        ImGui::Text("CTRL to release cursor");

        if (ImGui::Button("Play/Pause"))
            bPaused = !bPaused;

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete renderer;

    glfwTerminate();

    return 0;
}
