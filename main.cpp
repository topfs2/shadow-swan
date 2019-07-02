#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/scenes/SceneFactory.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Shadow Swan"

IScene *scene = NULL;

glm::vec2 last((float)WINDOW_WIDTH * 0.5f, (float)WINDOW_HEIGHT * 0.5f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main(int argc, const char **argv)
{
    stbi_set_flip_vertically_on_load(false);

    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        last = glm::vec2((float)x, (float)y);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);


    if(!gladLoadGL()) {
        std::cerr << "GLAD Failed to load GL" << std::endl;
        return -1;
    }
    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    scene = SceneFactory::create(argc > 1 ? argv[1] : "pbr");
    scene->OnResize(WINDOW_WIDTH, WINDOW_HEIGHT);

    GLfloat lastFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        GLfloat deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        scene->OnRender(currentFrame, deltaTime);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else {
        scene->OnKey(key, scancode, action, mode);
    }
}

void mouse_callback(GLFWwindow* window, double x, double y)
{
    glm::vec2 position((float)x, float(y));
    glm::vec2 offset(position.x - last.x, last.y - position.y); // Reversed Y since y-coordinates go from bottom to left

    scene->OnMouseMove(position, offset);
    last = position;
}
