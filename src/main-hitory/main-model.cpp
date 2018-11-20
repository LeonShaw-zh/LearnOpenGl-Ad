#include <iostream>
#include <stdlib.h>
#include <math.h>
#define GLFW_DLL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
using namespace std;

GLFWwindow* window;
unsigned int screenWidth = 800;
unsigned int screenHeight = 600;
// 计时器相关
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
// 创建摄像机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;
// 光源位置
glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

int initialize();
int initializeGLFW();
int initializeGLAD();
void makeCube(unsigned int VAO);
void makeLight(unsigned int VAO);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int main()
{
    if(!initialize())
        return -1;

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    Shader colorShader("../src/shader/3.1.color.vs", "../src/shader/3.1.color.fs");
    Shader lampShader("../src/shader/2.1.lamp.vs", "../src/shader/2.1.lamp.fs");

    unsigned int VAOLight;
    glGenVertexArrays(1, &VAOLight);
    makeLight(VAOLight);
    Model nanosuit = Model("../src/model/nanosuit/nanosuit.obj");

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)){
        processInput(window);
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // 生成观察矩阵和投影矩阵
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / screenHeight, 0.1f, 100.0f);

        // 绘制物体
        colorShader.use();
        // 片段着色器所需的参数
        colorShader.setVec3("viewPos", camera.Position);
        // dir light
        colorShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        colorShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        colorShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        colorShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        colorShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        colorShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        colorShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        colorShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        colorShader.setFloat("pointLights[0].constant", 1.0f);
        colorShader.setFloat("pointLights[0].linear", 0.09);
        colorShader.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        colorShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        colorShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        colorShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        colorShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        colorShader.setFloat("pointLights[1].constant", 1.0f);
        colorShader.setFloat("pointLights[1].linear", 0.09);
        colorShader.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        colorShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        colorShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        colorShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        colorShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        colorShader.setFloat("pointLights[2].constant", 1.0f);
        colorShader.setFloat("pointLights[2].linear", 0.09);
        colorShader.setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        colorShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        colorShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        colorShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        colorShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        colorShader.setFloat("pointLights[3].constant", 1.0f);
        colorShader.setFloat("pointLights[3].linear", 0.09);
        colorShader.setFloat("pointLights[3].quadratic", 0.032);
        // spotLight
        colorShader.setVec3("spotLight.position", camera.Position);
        colorShader.setVec3("spotLight.direction", camera.Front);
        colorShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        colorShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        colorShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        colorShader.setFloat("spotLight.constant", 1.0f);
        colorShader.setFloat("spotLight.linear", 0.09);
        colorShader.setFloat("spotLight.quadratic", 0.032);
        colorShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        colorShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        // 顶点着色器需要的参数
        model = glm::scale(model, glm::vec3(0.15f));
        colorShader.setMat4("model", model);
        colorShader.setMat4("view", view);
        colorShader.setMat4("projection", projection);
        glm::mat4 normalMat = glm::transpose(glm::inverse(model)); // 法线矩阵
        colorShader.setMat4("normalMat", normalMat);
        colorShader.setFloat("material.shininess", 32.0f);
        nanosuit.Draw(colorShader);

        // 绘制光源
        lampShader.use();
        // 顶点着色器需要的参数
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        for(unsigned int i = 0; i < 4; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lampShader.setMat4("model", model);
            glBindVertexArray(VAOLight);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 应用程序采用着双缓冲模式，一张在前面显示，一张在后面慢慢加载
        // Swap交换缓冲，完成立刻刷新
        glfwSwapBuffers(window);
    }

    // 释放glfw的资源
    glfwTerminate();

    return 0;
}

int initialize(){
    // 初始化GLFW
    if(!initializeGLFW())
        return 0;
    // 初始化GLAD
    if(!initializeGLAD())
        return 0;
    return 1;
}

int initializeGLFW(){
    // 初始GLFW，设置版本号为3.3，使用核心模式
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // 创建一个窗口，将窗口的上下文应用到当前的主上下文
    window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return 0;
    }   
    glfwMakeContextCurrent(window);
    return 1;
}

int initializeGLAD(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return 0;
    }
    return 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void makeCube(unsigned int VAO){
    glBindVertexArray(VAO);
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void makeLight(unsigned int VAO){
    makeCube(VAO);
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // 获得渲染时间，保证速度相同
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // 键盘移动
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // y的坐标是上面小，下面大
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(yoffset);
}