//Proyecto Final
//Morales Galicia Angel Uriel
//Elizalde Perez Alan
//Capistran Ponce Manuel Emiliano
//17-05-2025

#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL2/SOIL2.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

const GLuint WIDTH = 1950, HEIGHT = 1000;
int SCREEN_WIDTH, SCREEN_HEIGHT;

Camera  camera(glm::vec3(-17.5f, 5.2f, 8.3f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

bool AnimChange = false;
int step = 0;
float bounceTime = 0.0f;
float alturaRebote = 0.0f;
float alturaCompuNueva = 0.0f;

float scaleSalonN = 0.0f;
float scaleSalonV = 1.0f;

float compuNuevaTime = 0.0f;
float scaleMesa = 1.0f;
float scaleMesaNew = 0.0f;

float scaleCompuNueva = 0.0f;
float scaleCompuVieja = 1.0f;
float scalePantallaVieja = 1.0f;
float scaleTecladoVieja = 1.0f;

float rotateSillaViejaR = 180.0f;
float rotateSillaViejaL = 180.0f;
float scaleSillaVieja = 1.0f;
float rotateSillaNuevaR = 270.0f;
float rotateSillaNuevaL = 270.0f;
float scaleSillaNueva = 0.0f;

// Variables globales para Topos
float topoA = 0;

glm::vec3 posicionesMesasViejas[] = {
    glm::vec3(6.5f,  0.0f, -5.5f),
    glm::vec3(6.5f,  0.0f, -0.5f),
    glm::vec3(6.5f,  0.0f,  4.5f),
    glm::vec3(-6.5f, 0.0f, -5.5f),
    glm::vec3(-6.5f, 0.0f, -0.5f),
    glm::vec3(-6.5f, 0.0f,  4.5f)
};
glm::vec3 posicionesMesasNuevas[] = {
    glm::vec3(6.5f,  2.0f, -5.5f),
    glm::vec3(6.5f,  2.0f, -0.5f),
    glm::vec3(6.5f,  2.0f,  4.5f),
    glm::vec3(-6.5f, 2.0f, -5.5f),
    glm::vec3(-6.5f, 2.0f, -0.5f),
    glm::vec3(-6.5f, 2.0f,  4.5f)
};
glm::vec3 posicionesSillasViejasR[] = {
    glm::vec3(9.0f, 0.0f, -3.5f),
    glm::vec3(6.5f, 0.0f, -3.5f),
    glm::vec3(4.0f, 0.0f, -3.5f),

    glm::vec3(9.0f, 0.0f, 1.5f),
    glm::vec3(6.5f, 0.0f, 1.5f),
    glm::vec3(4.0f, 0.0f, 1.5f),

    glm::vec3(9.0f, 0.0f, 6.5f),
    glm::vec3(6.5f, 0.0f, 6.5f),
    glm::vec3(4.0f, 0.0f, 6.5f)
};
glm::vec3 posicionesSillasViejasL[] = {
    glm::vec3(-9.0f, 0.0f, -3.5f),
    glm::vec3(-6.5f, 0.0f, -3.5f),
    glm::vec3(-4.0f, 0.0f, -3.5f),

    glm::vec3(-9.0f, 0.0f, 1.5f),
    glm::vec3(-6.5f, 0.0f, 1.5f),
    glm::vec3(-4.0f, 0.0f, 1.5f),

    glm::vec3(-9.0f, 0.0f, 6.5f),
    glm::vec3(-6.5f, 0.0f, 6.5f),
    glm::vec3(-4.0f, 0.0f, 6.5f)
};
glm::vec3 posicionesSillasOriginalR[] = {
    glm::vec3(9.0f, 0.0f, -3.5f),
    glm::vec3(6.5f, 0.0f, -3.5f),
    glm::vec3(4.0f, 0.0f, -3.5f),

    glm::vec3(9.0f, 0.0f, 1.5f),
    glm::vec3(6.5f, 0.0f, 1.5f),
    glm::vec3(4.0f, 0.0f, 1.5f),

    glm::vec3(9.0f, 0.0f, 6.5f),
    glm::vec3(6.5f, 0.0f, 6.5f),
    glm::vec3(4.0f, 0.0f, 6.5f)
};
glm::vec3 posicionesSillasOriginalL[] = {
    glm::vec3(-9.0f, 0.0f, -3.5f),
    glm::vec3(-6.5f, 0.0f, -3.5f),
    glm::vec3(-4.0f, 0.0f, -3.5f),

    glm::vec3(-9.0f, 0.0f, 1.5f),
    glm::vec3(-6.5f, 0.0f, 1.5f),
    glm::vec3(-4.0f, 0.0f, 1.5f),

    glm::vec3(-9.0f, 0.0f, 6.5f),
    glm::vec3(-6.5f, 0.0f, 6.5f),
    glm::vec3(-4.0f, 0.0f, 6.5f)
};
glm::vec3 posicionesSillasNuevaR[] = {
    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f),

    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f),

    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f)
};
glm::vec3 posicionesSillasNuevaL[] = {
    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f),

    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f),

    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f),
    glm::vec3(-13.0f, 0.0f, 8.0f)
};
glm::vec3 posicionesCompuVieja[] = {
    glm::vec3(10.3f, 2.5f, -5.5f),
    glm::vec3(7.7f,  2.5f, -5.5f),
    glm::vec3(5.1f,  2.5f, -5.5f),

    glm::vec3(10.3f, 2.5f, -0.5f),
    glm::vec3(7.7f,  2.5f, -0.5f),
    glm::vec3(5.1f,  2.5f, -0.5f),

    glm::vec3(10.3f, 2.5f, 4.5f),
    glm::vec3(7.7f,  2.5f, 4.5f),
    glm::vec3(5.1f,  2.5f, 4.5f),

    glm::vec3(-10.3f, 2.5f, -5.5f),
    glm::vec3(-7.7f,  2.5f, -5.5f),
    glm::vec3(-5.1f,  2.5f, -5.5f),

    glm::vec3(-10.3f, 2.5f, -0.5f),
    glm::vec3(-7.7f,  2.5f, -0.5f),
    glm::vec3(-5.1f,  2.5f, -0.5f),

    glm::vec3(-10.3f, 2.5f, 4.5f),
    glm::vec3(-7.7f,  2.5f, 4.5f),
    glm::vec3(-5.1f,  2.5f, 4.5f),
};
glm::vec3 posicionesPantallaVieja[] = {
    glm::vec3(9.0f,  3.1f, -5.8f),
    glm::vec3(6.4f,  3.1f, -5.8f),
    glm::vec3(3.8f,  3.1f, -5.8f),

    glm::vec3(9.0f,  3.1f, -0.8f),
    glm::vec3(6.4f,  3.1f, -0.8f),
    glm::vec3(3.8f,  3.1f, -0.8f),

    glm::vec3(9.0f,  3.1f, 4.2f),
    glm::vec3(6.4f,  3.1f, 4.2f),
    glm::vec3(3.8f,  3.1f, 4.2f),

    glm::vec3(-9.0f,  3.1f, -5.8f),
    glm::vec3(-6.4f,  3.1f, -5.8f),
    glm::vec3(-3.8f,  3.1f, -5.8f),

    glm::vec3(-9.0f,  3.1f, -0.8f),
    glm::vec3(-6.4f,  3.1f, -0.8f),
    glm::vec3(-3.8f,  3.1f, -0.8f),

    glm::vec3(-9.0f,  3.1f, 4.2f),
    glm::vec3(-6.4f,  3.1f, 4.2f),
    glm::vec3(-3.8f,  3.1f, 4.2f),
};
glm::vec3 posicionesTecladoVieja[] = {
    glm::vec3(8.9f,  2.5f, -5.3f),
    glm::vec3(6.3f,  2.5f, -5.3f),
    glm::vec3(3.7f,  2.5f, -5.3f),

    glm::vec3(9.0f,  2.5f, -0.3f),
    glm::vec3(6.3f,  2.5f, -0.3f),
    glm::vec3(3.7f,  2.5f, -0.3f),

    glm::vec3(9.0f,  2.5f, 4.7f),
    glm::vec3(6.3f,  2.5f, 4.7f),
    glm::vec3(3.7f,  2.5f, 4.7f),

    glm::vec3(-9.1f,  2.5f, -5.3f),
    glm::vec3(-6.5f,  2.5f, -5.3f),
    glm::vec3(-3.9f,  2.5f, -5.3f),

    glm::vec3(-9.1f,  2.5f, -0.3f),
    glm::vec3(-6.5f,  2.5f, -0.3f),
    glm::vec3(-3.9f,  2.5f, -0.3f),

    glm::vec3(-9.1f,  2.5f, 4.7f),
    glm::vec3(-6.5f,  2.5f, 4.7f),
    glm::vec3(-3.9f,  2.5f, 4.7f),
};
glm::vec3 posicionesCompuNueva[] = {
    glm::vec3(9.0f,  2.45f, -5.2f),
    glm::vec3(6.4f,  2.45f, -5.2f),
    glm::vec3(3.8f,  2.45f, -5.2f),

    glm::vec3(9.0f,  2.45f, -0.2f),
    glm::vec3(6.4f,  2.45f, -0.2f),
    glm::vec3(3.8f,  2.45f, -0.2f),

    glm::vec3(9.0f,  2.45f, 4.8f),
    glm::vec3(6.4f,  2.45f, 4.8f),
    glm::vec3(3.8f,  2.45f, 4.8f),

    glm::vec3(-9.0f,  2.45f, -5.2f),
    glm::vec3(-6.4f,  2.45f, -5.2f),
    glm::vec3(-3.8f,  2.45f, -5.2f),

    glm::vec3(-9.0f,  2.45f, -0.2f),
    glm::vec3(-6.4f,  2.45f, -0.2f),
    glm::vec3(-3.8f,  2.45f, -0.2f),

    glm::vec3(-9.0f,  2.45f, 4.8f),
    glm::vec3(-6.4f,  2.45f, 4.8f),
    glm::vec3(-3.8f,  2.45f, 4.8f),
};

glm::vec3 pointLightPositions[] = {
   glm::vec3(0.0f,0.0f, 0.0f),
   glm::vec3(0.0f,0.0f, 0.0f),
   glm::vec3(0.0f,0.0f,  0.0f),
   glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);


GLfloat deltaTime = 0.0f;	
GLfloat lastFrame = 0.0f;  

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    Model Feria((char*)"Models/Feria.obj");
    // Modelos Topos
	Model cajaTopos((char*)"Models/topos/cajaTopos.obj");
	Model toposA((char*)"Models/topos/toposA.obj");
	Model toposB((char*)"Models/topos/toposB.obj");
	Model mazo((char*)"Models/topos/mazo.obj");

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        Animation();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

         lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f); 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.8f, 0.8f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);

        glm::vec3 lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.direction"), lightDir.x, lightDir.y, lightDir.z);

        // Point light 1
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);


        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        glm::mat4 model(1);

        //Carga de camara 
        view = camera.GetViewMatrix();


        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Feria.Draw(lightingShader);
        //Cajon topos
        glm::mat4 modelTempTopos = glm::mat4(1.0f); //Temp
        glm::mat4 modelTempTopos2 = glm::mat4(1.0f); //Temp
        glm::mat4 modelTempTopos3 = glm::mat4(1.0f); //Temp
        
        model = glm::mat4(1);
        modelTempTopos = model = glm::translate(model, glm::vec3(-17.0f, 0.0f, 24.0f));
        modelTempTopos = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTempTopos = model = glm::scale(modelTempTopos, glm::vec3(0.65f, 0.65f, 0.65f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cajaTopos.Draw(lightingShader);
        // ToposA
		model = modelTempTopos;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        toposA.Draw(lightingShader);
        //ToposB
        model = modelTempTopos;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        toposB.Draw(lightingShader);
		//Mazo
        model = modelTempTopos;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        mazo.Draw(lightingShader);

        //Topos izquierda
        // caja topos 2
        modelTempTopos2 = glm::translate(modelTempTopos, glm::vec3(-7.5f, 0.0f, 0.0f));
		model = modelTempTopos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cajaTopos.Draw(lightingShader);
        // ToposA Izquierda
        model = modelTempTopos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        toposA.Draw(lightingShader);
		//ToposB Izquierda
        model = modelTempTopos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        toposB.Draw(lightingShader);
		//Mazo Izquierda
        model = modelTempTopos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        mazo.Draw(lightingShader);

        //Topos derecha
        // caja topos 3
        modelTempTopos3 = glm::translate(modelTempTopos, glm::vec3(7.5f, 0.0f, 0.0f));
        model = modelTempTopos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cajaTopos.Draw(lightingShader);
        // ToposA Izquierda
        model = modelTempTopos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        toposA.Draw(lightingShader);
        //ToposB Izquierda
        model = modelTempTopos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        toposB.Draw(lightingShader);
        //Mazo Izquierda
        model = modelTempTopos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        mazo.Draw(lightingShader);






        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
       
        

        glBindVertexArray(0);



        glfwSwapBuffers(window);
    }


    glfwTerminate();



    return 0;
}

void DoMovement()
{
    
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_1])
    {
        AnimChange = true;

    }

    if (keys[GLFW_KEY_2])
    {
        AnimChange = false;

    }

}

void Animation() {
    if (AnimChange) {

        if (step == 0) {
            scaleSalonV = 0.0f;
            scaleSalonN = 1.0f;
            scaleMesa -= 0.005f;
            if (scaleMesa < 0.0f) {
                scaleMesa = 0.0f;
                scaleMesaNew += 0.005f;
                scaleCompuNueva = scaleMesaNew;

                compuNuevaTime += 0.03f;
                alturaCompuNueva = sin(compuNuevaTime * 10.0f) * exp(-compuNuevaTime * 1.5f);

                if (scaleMesaNew > 1.0f) {
                    scaleMesaNew = 1.0f;
                    step = 1;
                    alturaRebote = 0.0f;

                }
            }
            bounceTime += 0.03f;
            alturaRebote = sin(bounceTime * 10.0f) * exp(-bounceTime * 1.5f);
            scaleCompuVieja = scaleMesa;
            scalePantallaVieja = scaleMesa;
            scaleTecladoVieja = scaleMesa;
        }

        if (step == 1) {
            rotateSillaViejaR += 0.7f;
            if (rotateSillaViejaR > 270.0f) {
                rotateSillaViejaR = 270.0f;
                for (int i = 0; i < 9; ++i) {
                    if (posicionesSillasViejasR[i].x > 0.0f) {
                        posicionesSillasViejasR[i].x -= 0.07f;
                        if (posicionesSillasViejasR[i].x < 0.0f)
                            posicionesSillasViejasR[i].x = 0.0f;
                    }
                }
            }
            rotateSillaViejaL -= 0.7f;
            if (rotateSillaViejaL < 90.0f) {
                rotateSillaViejaL = 90.0f;
                for (int i = 0; i < 9; ++i) {
                    if (posicionesSillasViejasL[i].x < 0.0f) {
                        posicionesSillasViejasL[i].x += 0.07f;
                        if (posicionesSillasViejasL[i].x > 0.0f)
                            posicionesSillasViejasL[i].x = 0.0f;
                    }
                }
            }
            bool todasListas = true;
            for (int i = 0; i < 9; ++i) {
                if (posicionesSillasViejasR[i].x > 0.0f || posicionesSillasViejasL[i].x < 0.0f) {
                    todasListas = false;
                    break;
                }
            }
            if (todasListas) {
                step = 2;
            }
        }

        if (step == 2) {
            rotateSillaViejaR += 0.7f;
            if (rotateSillaViejaR > 360.0f ) {
                rotateSillaViejaR = 360.0f;
                for (int i = 0; i < 9; ++i) {
                    if (posicionesSillasViejasR[i].z < 8.0f) {
                        posicionesSillasViejasR[i].z += 0.07f;
                        if (posicionesSillasViejasR[i].z > 8.0f)
                            posicionesSillasViejasR[i].z = 8.0f;
                    }
                }
            }
            rotateSillaViejaL -= 0.7f;
            if (rotateSillaViejaL < 0.0f) {
                rotateSillaViejaL = 0.0f;
                for (int i = 0; i < 9; ++i) {
                    if (posicionesSillasViejasL[i].z < 8.0f) {
                        posicionesSillasViejasL[i].z += 0.07f;
                        if (posicionesSillasViejasL[i].z > 8.0f)
                            posicionesSillasViejasL[i].z = 8.0f;
                    }
                }
            }
            bool todasListas = true;
            for (int i = 0; i < 9; ++i) {
                if (posicionesSillasViejasR[i].z > 8.0f || posicionesSillasViejasL[i].z < 8.0f) {
                    todasListas = false;
                    break;
                }
            }
            if (todasListas) {
                step = 3;
            }
        }

        if (step == 3) {
            rotateSillaViejaR -= 0.7f;
            if (rotateSillaViejaR < 270.0f) {
                rotateSillaViejaR = 270.0f;
                for (int i = 0; i < 9; ++i) {
                    if (posicionesSillasViejasR[i].x > -13.0f) {
                        posicionesSillasViejasR[i].x -= 0.07f;
                        if (posicionesSillasViejasR[i].x < -13.0f)
                            posicionesSillasViejasR[i].x = -13.0f;
                    }
                }
            }
            rotateSillaViejaL -= 0.7f;
            if (rotateSillaViejaL < -90.0f) {
                rotateSillaViejaL = -90.0f;
                for (int i = 0; i < 9; ++i) {
                    if (posicionesSillasViejasL[i].x > -13.0f) {
                        posicionesSillasViejasL[i].x -= 0.07f;
                        if (posicionesSillasViejasL[i].x < -13.0f)
                            posicionesSillasViejasL[i].x = -13.0f;
                    }
                }
            }
            bool todasListas = true;
            for (int i = 0; i < 9; ++i) {
                if (posicionesSillasViejasR[i].x > -13.0f || posicionesSillasViejasL[i].x > -13.0f) {
                    todasListas = false;
                    break;
                }
            }
            if (todasListas) {
                step = 4;
            }
        }

        if (step == 4) {
            scaleSillaVieja -= 0.008f;
            if (scaleSillaVieja < 0.0f) {
                scaleSillaVieja = 0.0f;
                scaleSillaNueva += 0.008f;
                if (scaleSillaNueva > 1.0f) {
                    scaleSillaNueva = 1.0f;
                    step = 5;
                }
            }
        }

        if (step == 5) {
            for (int i = 0; i < 9; ++i) {
                if (posicionesSillasNuevaR[i].x < 0.0f) {
                    posicionesSillasNuevaR[i].x += 0.07f;
                    if (posicionesSillasNuevaR[i].x > 0.0f)
                        posicionesSillasNuevaR[i].x = 0.0f;
                }
                if (posicionesSillasNuevaL[i].x < 0.0f) {
                    posicionesSillasNuevaL[i].x += 0.07f;
                    if (posicionesSillasNuevaL[i].x > 0.0f)
                        posicionesSillasNuevaL[i].x = 0.0f;
                }
            }

            bool todasListas = true;
            for (int i = 0; i < 9; ++i) {
                if (posicionesSillasNuevaR[i].x < 0.0f || posicionesSillasNuevaL[i].x < 0.0f) {
                    todasListas = false;
                    break;
                }
            }

            if (todasListas) {
                rotateSillaNuevaL += 0.7f;
                rotateSillaNuevaR += 0.7f;
                if (rotateSillaNuevaL > 360.0f && rotateSillaNuevaR > 360.0f) {
                    rotateSillaNuevaL = 360.0f;
                    rotateSillaNuevaR = 360.0f;
                    step = 6;
                }
            }
        }

        if (step == 6) {
            bool todasListas = true;

            for (int i = 0; i < 9; ++i) {
                float& zActualL = posicionesSillasNuevaL[i].z;
                float zDestinoL = posicionesSillasOriginalL[i].z;

                if (std::abs(zActualL - zDestinoL) > 0.01f) {
                    float delta = (zDestinoL > zActualL ? 1 : -1) * 0.07f;
                    zActualL += delta;
                    if ((delta > 0 && zActualL > zDestinoL) || (delta < 0 && zActualL < zDestinoL)) {
                        zActualL = zDestinoL;
                    }
                    todasListas = false;
                }

                float& zActualR = posicionesSillasNuevaR[i].z;
                float zDestinoR = posicionesSillasOriginalR[i].z;

                if (std::abs(zActualR - zDestinoR) > 0.01f) {
                    float delta = (zDestinoR > zActualR ? 1 : -1) * 0.07f;
                    zActualR += delta;
                    if ((delta > 0 && zActualR > zDestinoR) || (delta < 0 && zActualR < zDestinoR)) {
                        zActualR = zDestinoR;
                    }
                    todasListas = false;
                }
            }

            if (todasListas) {
                step = 7;
            }
        }

        if (step == 7) {
            bool rotacionCompletaL = false;
            bool rotacionCompletaR = false;

            if (rotateSillaNuevaL < 450.0f) {
                rotateSillaNuevaL += 0.7f;
                if (rotateSillaNuevaL > 450.0f)
                    rotateSillaNuevaL = 450.0f;
            }
            else {
                rotacionCompletaL = true;
            }

            if (rotateSillaNuevaR > 270.0f) {
                rotateSillaNuevaR -= 0.7f;
                if (rotateSillaNuevaR < 270.0f)
                    rotateSillaNuevaR = 270.0f;
            }
            else {
                rotacionCompletaR = true;
            }

            if (rotacionCompletaL && rotacionCompletaR) {
                step = 8;
            }
        }

        if (step == 8) {
            bool todasListas = true;

            for (int i = 0; i < 9; ++i) {
                float& xActualL = posicionesSillasNuevaL[i].x;
                float xDestinoL = posicionesSillasOriginalL[i].x;

                if (std::abs(xActualL - xDestinoL) > 0.01f) {
                    float delta = (xDestinoL > xActualL ? 1 : -1) * 0.07f;
                    xActualL += delta;
                    if ((delta > 0 && xActualL > xDestinoL) || (delta < 0 && xActualL < xDestinoL)) {
                        xActualL = xDestinoL;
                    }
                    todasListas = false;
                }

                float& xActualR = posicionesSillasNuevaR[i].x;
                float xDestinoR = posicionesSillasOriginalR[i].x;

                if (std::abs(xActualR - xDestinoR) > 0.01f) {
                    float delta = (xDestinoR > xActualR ? 1 : -1) * 0.07f;
                    xActualR += delta;
                    if ((delta > 0 && xActualR > xDestinoR) || (delta < 0 && xActualR < xDestinoR)) {
                        xActualR = xDestinoR;
                    }
                    todasListas = false;
                }
            }

            if (todasListas) {
                step = 9;
            }
        }

        if (step == 9) {
            bool rotacionCompletaL = false;
            bool rotacionCompletaR = false;

            if (rotateSillaNuevaL > 360.0f) {
                rotateSillaNuevaL -= 0.7f;
                if (rotateSillaNuevaL < 360.0f)
                    rotateSillaNuevaL = 360.0f;
            }
            else {
                rotacionCompletaL = true;
            }

            if (rotateSillaNuevaR < 360.0f) {
                rotateSillaNuevaR += 0.7f;
                if (rotateSillaNuevaR > 360.0f)
                    rotateSillaNuevaR = 360.0f;
            }
            else {
                rotacionCompletaR = true;
            }

            if (rotacionCompletaL && rotacionCompletaR) {
                step = 9;
            }
        }


    }
    else{

        if (step == 9) {
            bool rotacionCompletaL = false;
            bool rotacionCompletaR = false;

            if (rotateSillaNuevaL < 450.0f) {
                rotateSillaNuevaL += 0.7f;
                if (rotateSillaNuevaL > 450.0f)
                    rotateSillaNuevaL = 450.0f;
            }
            else {
                rotacionCompletaL = true;
            }

            if (rotateSillaNuevaR > 270.0f) {
                rotateSillaNuevaR -= 0.7f;
                if (rotateSillaNuevaR < 270.0f)
                    rotateSillaNuevaR = 270.0f;
            }
            else {
                rotacionCompletaR = true;
            }

            if (rotacionCompletaL && rotacionCompletaR) {
                step = 8;
            }
        }

        if (step == 8) {
            bool todasListas = true;

            for (int i = 0; i < 9; ++i) {
                float& xActualL = posicionesSillasNuevaL[i].x;
                float xDestinoL = 0.0;

                if (std::abs(xActualL - xDestinoL) > 0.01f) {
                    float delta = (xDestinoL > xActualL ? 1 : -1) * 0.07f;
                    xActualL += delta;
                    if ((delta > 0 && xActualL > xDestinoL) || (delta < 0 && xActualL < xDestinoL)) {
                        xActualL = xDestinoL;
                    }
                    todasListas = false;
                }

                float& xActualR = posicionesSillasNuevaR[i].x;
                float xDestinoR = 0.0f;

                if (std::abs(xActualR - xDestinoR) > 0.01f) {
                    float delta = (xDestinoR > xActualR ? 1 : -1) * 0.07f;
                    xActualR += delta;
                    if ((delta > 0 && xActualR > xDestinoR) || (delta < 0 && xActualR < xDestinoR)) {
                        xActualR = xDestinoR;
                    }
                    todasListas = false;
                }
            }

            if (todasListas) {
                step = 7;
            }
        }

        if (step == 7) {
            bool rotacionCompletaL = false;
            bool rotacionCompletaR = false;

            if (rotateSillaNuevaL > 360.0f) {
                rotateSillaNuevaL -= 0.7f;
                if (rotateSillaNuevaL < 360.0f)
                    rotateSillaNuevaL = 360.0f;
            }
            else {
                rotacionCompletaL = true;
            }

            if (rotateSillaNuevaR < 360.0f) {
                rotateSillaNuevaR += 0.7f;
                if (rotateSillaNuevaR > 360.0f)
                    rotateSillaNuevaR = 360.0f;
            }
            else {
                rotacionCompletaR = true;
            }

            if (rotacionCompletaL && rotacionCompletaR) {
                step = 6;
            }
        }

        if (step == 6) {
            bool todasListas = true;

            for (int i = 0; i < 9; ++i) {
                float& zActualL = posicionesSillasNuevaL[i].z;
                float zDestinoL = 8.0f;

                if (std::abs(zActualL - zDestinoL) > 0.01f) {
                    float delta = (zDestinoL > zActualL ? 1 : -1) * 0.07f;
                    zActualL += delta;
                    if ((delta > 0 && zActualL > zDestinoL) || (delta < 0 && zActualL < zDestinoL)) {
                        zActualL = zDestinoL;
                    }
                    todasListas = false;
                }

                float& zActualR = posicionesSillasNuevaR[i].z;
                float zDestinoR = 8.0f;

                if (std::abs(zActualR - zDestinoR) > 0.01f) {
                    float delta = (zDestinoR > zActualR ? 1 : -1) * 0.07f;
                    zActualR += delta;
                    if ((delta > 0 && zActualR > zDestinoR) || (delta < 0 && zActualR < zDestinoR)) {
                        zActualR = zDestinoR;
                    }
                    todasListas = false;
                }
            }

            if (todasListas) {
                step = 5;
            }
        }

        if (step == 5) {
            rotateSillaNuevaL -= 0.7f;
            rotateSillaNuevaR -= 0.7f;
            if (rotateSillaNuevaL < 270.0f && rotateSillaNuevaR < 270.0f) {
                rotateSillaNuevaL = 270.0f;
                rotateSillaNuevaR = 270.0f;

                for (int i = 0; i < 9; ++i) {
                    if (posicionesSillasNuevaR[i].x > -13.0f) {
                        posicionesSillasNuevaR[i].x -= 0.07f;
                        if (posicionesSillasNuevaR[i].x < -13.0f)
                            posicionesSillasNuevaR[i].x = -13.0f;
                    }
                    if (posicionesSillasNuevaL[i].x > -13.0f) {
                        posicionesSillasNuevaL[i].x -= 0.07f;
                        if (posicionesSillasNuevaL[i].x < -13.0f)
                            posicionesSillasNuevaL[i].x = -13.0f;
                    }
                }

                bool todasListas = true;
                for (int i = 0; i < 9; ++i) {
                    if (posicionesSillasNuevaR[i].x > -13.0f || posicionesSillasNuevaL[i].x > -13.0f) {
                        todasListas = false;
                        break;
                    }
                }
                if (todasListas) {
                    step = 4;
                    
                }
            }
        }

        if (step == 4) {
            scaleSillaNueva -= 0.008f;
            if (scaleSillaNueva < 0.0f) {
                scaleSillaNueva = 0.0f;
                scaleSillaVieja += 0.008f;
                if (scaleSillaVieja > 1.0f) {
                    scaleSillaVieja = 1.0f;
                    step = 3;
                }
            }
        }

        if (step == 3) {
            bool todasListas = true;

            for (int i = 0; i < 9; ++i) {
                float& xR = posicionesSillasViejasR[i].x;
                if (std::abs(xR - 0.0f) > 0.01f) {
                    float delta = 0.07f;
                    xR += delta;
                    if (xR > 0.0f) xR = 0.0f;
                    todasListas = false;
                }

                float& xL = posicionesSillasViejasL[i].x;
                if (std::abs(xL - 0.0f) > 0.01f) {
                    float delta = 0.07f;
                    xL += delta;
                    if (xL > 0.0f) xL = 0.0f;
                    todasListas = false;
                }
            }

            if (todasListas) {
                bool rotacionesCompletas = true;

                if (rotateSillaViejaR < 360.0f) {
                    rotateSillaViejaR += 0.7f;
                    if (rotateSillaViejaR > 360.0f) rotateSillaViejaR = 360.0f;
                    rotacionesCompletas = false;
                }

                if (rotateSillaViejaL < 0.0f) {
                    rotateSillaViejaL += 0.7f;
                    if (rotateSillaViejaL > 0.0f) rotateSillaViejaL = 0.0f;
                    rotacionesCompletas = false;
                }

                if (rotacionesCompletas) {
                    step = 2;
                }
            }
        }

        if (step == 2) { 
            bool todasListas = true;

            for (int i = 0; i < 9; ++i) {
                float& zL = posicionesSillasViejasL[i].z;
                float zDestinoL = posicionesSillasOriginalL[i].z;
                if (std::abs(zL - zDestinoL) > 0.01f) {
                    float delta = (zDestinoL > zL ? 1 : -1) * 0.07f;
                    zL += delta;
                    if ((delta > 0 && zL > zDestinoL) || (delta < 0 && zL < zDestinoL)) {
                        zL = zDestinoL;
                    }
                    todasListas = false;
                }

                float& zR = posicionesSillasViejasR[i].z;
                float zDestinoR = posicionesSillasOriginalL[i].z; 
                if (std::abs(zR - zDestinoR) > 0.01f) {
                    float delta = (zDestinoR > zR ? 1 : -1) * 0.07f;
                    zR += delta;
                    if ((delta > 0 && zR > zDestinoR) || (delta < 0 && zR < zDestinoR)) {
                        zR = zDestinoR;
                    }
                    todasListas = false;
                }
            }

            if (todasListas) {
                bool rotacionesCompletas = true;

                if (rotateSillaViejaL < 90.0f) {
                    rotateSillaViejaL += 0.7f;
                    if (rotateSillaViejaL > 90.0f) rotateSillaViejaL = 90.0f;
                    rotacionesCompletas = false;
                }

                if (rotateSillaViejaR > 270.0f) {
                    rotateSillaViejaR -= 0.7f;
                    if (rotateSillaViejaR < 270.0f) rotateSillaViejaR = 270.0f;
                    rotacionesCompletas = false;
                }

                if (rotacionesCompletas) {
                    step = 1;
                }
            }
        }

        if (step == 1) { 
            bool todasListas = true;

            for (int i = 0; i < 9; ++i) {
                float& xL = posicionesSillasViejasL[i].x;
                float xDestinoL = posicionesSillasOriginalL[i].x;
                if (std::abs(xL - xDestinoL) > 0.01f) {
                    float delta = (xDestinoL > xL ? 1 : -1) * 0.07f;
                    xL += delta;
                    if ((delta > 0 && xL > xDestinoL) || (delta < 0 && xL < xDestinoL)) {
                        xL = xDestinoL;
                    }
                    todasListas = false;
                }

                float& xR = posicionesSillasViejasR[i].x;
                float xDestinoR = posicionesSillasOriginalR[i].x;
                if (std::abs(xR - xDestinoR) > 0.01f) {
                    float delta = (xDestinoR > xR ? 1 : -1) * 0.07f;
                    xR += delta;
                    if ((delta > 0 && xR > xDestinoR) || (delta < 0 && xR < xDestinoR)) {
                        xR = xDestinoR;
                    }
                    todasListas = false;
                }
            }

            if (todasListas) {
                bool rotacionesCompletas = true;

                if (rotateSillaViejaL < 180.0f) {
                    rotateSillaViejaL += 0.7f;
                    if (rotateSillaViejaL > 180.0f) rotateSillaViejaL = 180.0f;
                    rotacionesCompletas = false;
                }

                if (rotateSillaViejaR > 180.0f) {
                    rotateSillaViejaR -= 0.7f;
                    if (rotateSillaViejaR < 180.0f) rotateSillaViejaR = 180.0f;
                    rotacionesCompletas = false;
                }

                if (rotacionesCompletas) {
                    step = 0;
                }
            }
        }

        if (step == 0) { 
            scaleSalonV = 1.0f;
            scaleSalonN = 0.0f;

            if (scaleMesaNew > 0.0f) {
                scaleMesaNew -= 0.005f;
                if (scaleMesaNew < 0.0f) {
                    scaleMesaNew = 0.0f;
                    compuNuevaTime = 0.0f;
                    bounceTime = 0.0f; 
                }

                compuNuevaTime += 0.03f;
                alturaCompuNueva = sin(compuNuevaTime * 10.0f) * exp(-compuNuevaTime * 1.5f);
                scaleCompuNueva = scaleMesaNew;
            }
            else {
                scaleMesa += 0.005f;
                if (scaleMesa > 1.0f) {
                    scaleMesa = 1.0f;
                    step = 0; 
                }

                bounceTime += 0.03f;
                alturaRebote = sin(bounceTime * 10.0f) * exp(-bounceTime * 1.5f);

                scaleCompuVieja = scaleMesa;
                scalePantallaVieja = scaleMesa;
                scaleTecladoVieja = scaleMesa;
            }
        }


    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}