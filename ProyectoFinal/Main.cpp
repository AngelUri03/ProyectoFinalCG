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
void ResetMousePosition(GLFWwindow* window);
void DoMovement();
void Animation();

const GLuint WIDTH = 1950, HEIGHT = 1000;
int SCREEN_WIDTH, SCREEN_HEIGHT;

Camera  camera(glm::vec3(0.0f, 0.0f, 0.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
bool vistaTerceraPersona = true;
bool camaraCambiada = false;

enum TipoCamara {
    CAMARA_LIBRE,
    CAMARA_TERCERA_PERSONA,
    CAMARA_VISTA_AEREA,
    CAMARA_FIJA_1,
    CAMARA_FIJA_2,
};

TipoCamara tipoCamara = CAMARA_TERCERA_PERSONA;

glm::vec3 dronePos = glm::vec3(0.0f, 60.0f, 0.0f);
float droneSpeed = 5.0f;
float droneDistance = 0.0f;
float maxArmLength = 50.0f; 
int droneDir = 0; 
bool goingOut = true;


bool AnimRick = false;
int stepRick = 0;

float rotateRickCabeza = 0.0f;
bool rotateCabezaSentido = true;
float rotateRickBrazos = 0.0f;
bool rotateRickBrazosSentido = true;
float rotateRickPiernas = 0.0f;
bool rotateRickPiernasSentido = true;
glm::vec3 translateRick = glm::vec3(0.0f, 0.0f, 45.0f) ;
float rotateRick = 90.0f;
bool caminando = false;

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
    Model RickCuerpo((char*)"Models/RickCuerpo.obj");
    Model RickCabeza((char*)"Models/RickCabeza.obj");
    Model RickBrazoD((char*)"Models/RickBrazoD.obj");
    Model RickBrazoI((char*)"Models/RickBrazoI.obj");
    Model RickPiernaD((char*)"Models/RickPiernaD.obj");
    Model RickPiernaI((char*)"Models/RickPiernaI.obj");

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


        if (tipoCamara == CAMARA_TERCERA_PERSONA) {
            glm::vec3 offset(-10.0f, 5.0f, 0.0f);
            float radians = glm::radians(rotateRick);
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::vec3 rotatedOffset = glm::vec3(rotation * glm::vec4(offset, 1.0f));
            glm::vec3 rickPos = translateRick;
            glm::vec3 cameraPos = rickPos + rotatedOffset;
            glm::vec3 target = rickPos;
            camera.SetPosition(cameraPos);
            camera.SetFront(glm::normalize(target - cameraPos));

        }
        if (tipoCamara == CAMARA_VISTA_AEREA) {
            float velocity = droneSpeed * deltaTime;
            if (goingOut) {
                switch (droneDir) {
                case 0: dronePos.x += velocity; break; 
                case 1: dronePos.z -= velocity; break;
                case 2: dronePos.x -= velocity; break; 
                case 3: dronePos.z += velocity; break;
                }
                droneDistance += velocity;
                if (droneDistance >= maxArmLength) {
                    goingOut = false;
                }
            }
            else {
                switch (droneDir) {
                case 0: dronePos.x -= velocity; break; 
                case 1: dronePos.z += velocity; break; 
                case 2: dronePos.x += velocity; break;
                case 3: dronePos.z -= velocity; break;
                }
                droneDistance -= velocity;
                if (droneDistance <= 0.0f) {
                    goingOut = true;
                    droneDistance = 0.0f;
                    droneDir = (droneDir + 1) % 4;
                }
            }
            camera.SetPosition(dronePos);
            camera.SetFront(glm::vec3(0.0f, -1.0f, 0.0f));
            camera.SetPitch(-90.0f);
            camera.SetYaw(0.0f);
            camera.UpdateVectors();
        }




        glm::mat4 view;
        view = camera.GetViewMatrix();

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        glm::mat4 model(1);

        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Feria.Draw(lightingShader);
        
        model = glm::mat4(1);
        model = glm::translate(model, translateRick);
        model = glm::rotate(model, glm::radians(rotateRick), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 1.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        RickCuerpo.Draw(lightingShader);

        glm::mat4 modelCabeza = model;
        modelCabeza = glm::translate(modelCabeza, glm::vec3(-0.1f, 1.62f, 0.0f));
        modelCabeza = glm::rotate(modelCabeza, glm::radians(rotateRickCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCabeza));
        RickCabeza.Draw(lightingShader);

        glm::mat4 modelBrazoD = model;
        modelBrazoD = glm::translate(modelBrazoD, glm::vec3(-0.1f, 1.34f, 0.58f)); 
        modelBrazoD = glm::rotate(modelBrazoD, glm::radians(rotateRickBrazos), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBrazoD));
        RickBrazoD.Draw(lightingShader);

        glm::mat4 modelBrazoI = model;
        modelBrazoI = glm::translate(modelBrazoI, glm::vec3(-0.1f, 1.22f, -0.7f));
        modelBrazoI = glm::rotate(modelBrazoI, glm::radians(rotateRickBrazos), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBrazoI));
        RickBrazoI.Draw(lightingShader);

        glm::mat4 modelPiernaD = model;
        modelPiernaD = glm::translate(modelPiernaD, glm::vec3(-0.1f, -1.2f, 0.2f)); 
        modelPiernaD = glm::rotate(modelPiernaD, glm::radians(rotateRickPiernas * -1.0f) , glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPiernaD));
        RickPiernaD.Draw(lightingShader);

        glm::mat4 modelPiernaI = model;
        modelPiernaI = glm::translate(modelPiernaI, glm::vec3(-0.1f, -1.2f, -0.2f));
        modelPiernaI = glm::rotate(modelPiernaI, glm::radians(rotateRickPiernas), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPiernaI));
        RickPiernaI.Draw(lightingShader);


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
    
    if (tipoCamara == CAMARA_LIBRE) {
        if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, deltaTime);
        if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, deltaTime);
        if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (tipoCamara == CAMARA_TERCERA_PERSONA) {

        caminando = false;

        if (keys[GLFW_KEY_UP])
        {
            translateRick.z += sin(glm::radians(-rotateRick)) * 0.02f;
            translateRick.x += cos(glm::radians(-rotateRick)) * 0.02f;
            caminando = true;
        }

        if (keys[GLFW_KEY_DOWN])
        {
            translateRick.z -= sin(glm::radians(-rotateRick)) * 0.02f;
            translateRick.x -= cos(glm::radians(-rotateRick)) * 0.02f;
            caminando = true;
        }

        if (keys[GLFW_KEY_LEFT])
        {
            rotateRick += 0.5f;
        }

        if (keys[GLFW_KEY_RIGHT])
        {
            rotateRick -= 0.5f;
        }
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

    if (keys[GLFW_KEY_C]) {
        if (!camaraCambiada) {
            ResetMousePosition(window);
            camaraCambiada = true;
            tipoCamara = static_cast<TipoCamara>((tipoCamara + 1) % 3); 
        }
    }
    else {
        camaraCambiada = false;
    }


}

void Animation() {

    if (rotateCabezaSentido) {
        rotateRickCabeza += 0.1f;
        if (rotateRickCabeza >= 20.0f)
            rotateCabezaSentido = false;
    }
    else {
        rotateRickCabeza -= 0.1f;
        if (rotateRickCabeza <= -20.0f)
            rotateCabezaSentido = true;
    }

    if (caminando) {
        if (rotateRickPiernasSentido) {
            rotateRickPiernas += 0.5f;
            if (rotateRickPiernas >= 25.0f)
                rotateRickPiernasSentido = false;
        }
        else {
            rotateRickPiernas -= 0.5f;
            if (rotateRickPiernas <= -25.0f)
                rotateRickPiernasSentido = true;
        }
        if (rotateRickBrazosSentido) {
            rotateRickBrazos += 0.9f;
            if (rotateRickBrazos >= 70.0f)
                rotateRickBrazosSentido = false;
        }
        else {
            rotateRickBrazos -= 0.9f;
            if (rotateRickBrazos <= -70.0f)
                rotateRickBrazosSentido = true;
        }
    }
    else {
        const float epsilon = 0.5f;
        if (rotateRickPiernas > epsilon)
            rotateRickPiernas -= 0.4f;
        else if (rotateRickPiernas < -epsilon)
            rotateRickPiernas += 0.4f;
        else
            rotateRickPiernas = 0.0f;
        if (rotateRickBrazos > epsilon)
            rotateRickBrazos -= 0.5f;
        else if (rotateRickBrazos < -epsilon)
            rotateRickBrazos += 0.5f;
        else
            rotateRickBrazos = 0.0f;
    }

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{

    if (tipoCamara == CAMARA_LIBRE) {
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
}

void ResetMousePosition(GLFWwindow* window) {
    camera.SetYaw(0.0f);
    camera.SetPitch(0.0f);
    camera.UpdateVectors();
}