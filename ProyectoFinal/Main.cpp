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
#include <fmod.hpp>
#include <fmod_errors.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL2/SOIL2.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ResetMousePosition(GLFWwindow* window);
void DoMovement();
void Animation();

const GLuint WIDTH = 1950, HEIGHT = 1000;
int SCREEN_WIDTH, SCREEN_HEIGHT;

//Camara
Camera  camera(glm::vec3(0.0f, 0.0f, 0.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool camaraCambiada = false;
enum TipoCamara {
    CAMARA_LIBRE,
    CAMARA_TERCERA_PERSONA,
    CAMARA_VISTA_AEREA,
    CAMARA_JUEGO_TOPOS,
    CAMARA_JUEGO_BATEO,
    CAMARA_JUEGO_BOLOS,
    CAMARA_JUEGO_DADOS,
    CAMARA_JUEGO_HACHA,
    CAMARA_JUEGO_GLOBOS
};
TipoCamara tipoCamara = CAMARA_TERCERA_PERSONA;
glm::vec3 dronePos = glm::vec3(0.0f, 60.0f, 0.0f);
float droneSpeed = 5.0f;
float droneDistance = 0.0f;
float maxArmLength = 50.0f;
int droneDir = 0;
bool goingOut = true;

//Sonido
FMOD::System* fmodSystem = nullptr;
FMOD::Sound* sonidoFeria = nullptr;
FMOD::Sound* sonidoTopos = nullptr;
FMOD::Sound* sonidoBateo = nullptr;
FMOD::Sound* sonidoDados = nullptr;
FMOD::Sound* sonidoHacha = nullptr;
FMOD::Sound* sonidoBolos = nullptr;
FMOD::Sound* sonidoGlobos = nullptr;
FMOD::Channel* canalFeria = nullptr;
FMOD::Channel* canalTopos = nullptr;
FMOD::Channel* canalBateo = nullptr;
FMOD::Channel* canalDados = nullptr;
FMOD::Channel* canalHacha = nullptr;
FMOD::Channel* canalBolos = nullptr;
FMOD::Channel* canalGlobos = nullptr;

//Rick
float rotateRickCabeza = 0.0f;
bool rotateCabezaSentido = true;
float rotateRickBrazos = 0.0f;
bool rotateRickBrazosSentido = true;
float rotateRickPiernas = 0.0f;
bool rotateRickPiernasSentido = true;
glm::vec3 translateRick = glm::vec3(0.0f, 0.0f, 45.0f);
float rotateRick = 90.0f;
bool caminando = false;

struct Topo {
    float altura = 0.0f;
    float tiempo = 0.0f;
    bool subiendo = true;
    bool retrasoInicial = true;
};
std::vector<Topo> topos(2);

bool juegoToposActivo = false;
bool juegoBatearActivo = false;
bool juegoBolosActivo = false;
bool juegoDadosActivo = false;
bool juegoHachaActivo = false;
bool juegoGlobosActivo = false;
bool insertCoinTopos = false;
bool insertCoinBatear = false;
bool insertCoinBolos = false;
bool insertCoinDados = false;
bool insertCoinHacha = false;
bool insertCoinGlobos = false;

bool puedeTeclear = true;
bool esDeNoche = false;
float tiempoAnimacion = 0.0f;
float duracionAnimacion = 10.0f;

float anguloMazoX = 0.0f;
float anguloMazoY = 0.0f;
glm::vec3 masoTranslate = glm::vec3(-17.8f, 1.55f, 25.75f);
bool bajando = true;
bool izquierda = true;
float maxAnguloGolpe = 65.0f;
float minAnguloGolpe = 0.0f;

float anguloBrazoRick = 0.0f;
bool brazoSubiendo = true;
bool pausaInsertCoin = false;
float tiempoPausa = 0.0f;

//Animacion de hacha
bool animarHacha = false;
bool hachaDesplazando = false;
float rotacionHacha = 0.0f;
float desplazamientoHacha = 0.0f; 
float tiempoEsperaHacha = 0.0f;
bool enEsperaHacha = false;
float tiempoAnimacionHacha = 0.0f;

//Animacion de globos
bool animarDardos = false;
bool animarDardo1 = false;
bool animarDardo2 = false;
bool animarDardo3 = false;
float posDardo1Z = -2.5f;
float posDardo2Z = -2.5f;
float posDardo3Z = -2.5f;
bool globo1Visible = false;
bool globo2Visible = false;
bool globo3Visible = false;
float tiempoAnimacionDardos = 0.0f;
float tiempoEsperaDardos = 0.0f;
bool enEsperaDardos = false;

// Animación de dados
bool animarDados = false;
float tiempoAnimacionDados = 0.0f;
float duracionDados = 2.0f;
bool enEsperaDados = false;
float tiempoEsperaDados = 0.0f;

float rotX1 = 0.0f, rotY1 = 0.0f, rotZ1 = 0.0f;
float rotX2 = 0.0f, rotY2 = 0.0f, rotZ2 = 0.0f;
float rotX3 = 0.0f, rotY3 = 0.0f, rotZ3 = 0.0f;
float rotX4 = 0.0f, rotY4 = 0.0f, rotZ4 = 0.0f;
float rotX5 = 0.0f, rotY5 = 0.0f, rotZ5 = 0.0f;

//Variables animacion bateo
bool animarPelota = false;
float posPelotaX = -31.5f;
float posPelotaY = 1.3f;
float posPelotaZ = -0.0f;
float velocidadPelota = 0.1f;
bool pelotaGolpeada = false;
float velocidadGolpe = 0.25f;
float rotacionBate = -30.0f;
bool animarBate = false;
bool bateGolpeo = false;
bool pelotaRegresando = false;

//Variables animacion bolos
bool lanzarCaparazon = false;
float posCaparazonX = 32.0f;
float velocidadCaparazon = 0.02f;
float impactoRotacion = 0.0f;
bool pinosCaidos = false;
float rotacionPinoAnimado = 0.0f;
float pinoPosY = 0.5f;
std::vector<float> carrilesZ = { -4.6f, -3.0f, -1.4f, 1.8f, 3.4f, 5.0f };
std::vector<glm::vec3> trianguloRelativo = {
    glm::vec3(0.0f, 0.5f, 0.0f),
    glm::vec3(0.15f, 0.5f, -0.1f),
    glm::vec3(-0.15f, 0.5f, -0.1f),
    glm::vec3(0.3f, 0.5f, -0.2f),
    glm::vec3(0.0f, 0.5f, -0.2f),
    glm::vec3(-0.3f, 0.5f, -0.2f),
    glm::vec3(0.45f, 0.5f, -0.3f),
    glm::vec3(0.15f, 0.5f, -0.3f),
    glm::vec3(-0.15f, 0.5f, -0.3f),
    glm::vec3(-0.45f, 0.5f, -0.3f)
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

GLfloat skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

GLuint indices[] =
{
    0,1,2,3,
    4,5,6,7,
    8,9,10,11,
    12,13,14,15,
    16,17,18,19,
    20,21,22,23,
    24,25,26,27,
    28,29,30,31,
    32,33,34,35
};

const int NUM_LUCES = 72;

struct Luz {
    glm::vec3 posicion;
    glm::vec3 color;
    float alcance;
};

Luz luces[NUM_LUCES] = {
    //Rojo
    {glm::vec3(8.7f, 4.3f, 8.0f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(16.0f, 5.7f, 18.9f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(17.2f, 5.8f, 20.7f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(9.5f, 4.5f, -8.8f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(16.4f, 5.9f, -19.8f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(17.6f, 6.0f, -21.7f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(-7.9f, 4.3f, -5.9f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(-9.3f, 4.4f, -8.0f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(-16.1f, 5.5f, -19.2f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(-17.2f, 5.7f, -21.0f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(-6.7f, 4.1f, 4.1f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(-8.3f, 4.2f, 6.3f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(-15.0f, 5.2f, 17.2f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    {glm::vec3(-16.2f, 5.5f, 19.2f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f},
    
    // Verde
    {glm::vec3(7.3f, 4.1f, 5.5f), glm::vec3(0.0f, 1.0f, 0.0f), 20.0f},
    {glm::vec3(14.7f, 5.2f, 17.0f), glm::vec3(0.0f, 1.0f, 0.0f), 20.0f},
    {glm::vec3(7.9f, 4.2f, -6.3f), glm::vec3(0.0f, 1.0f, 0.0f), 20.0f},
    {glm::vec3(15.2f, 5.3f, -17.9f), glm::vec3(0.0f, 1.0f, 0.0f), 20.0f},
    {glm::vec3(-14.9f, 5.0f, -17.3f), glm::vec3(0.0f, 1.0f, 0.0f), 20.0f},
    {glm::vec3(-13.9f, 5.0f, 15.5f), glm::vec3(0.0f, 1.0f, 0.0f), 20.0f},

    //Amarillo
    {glm::vec3(10.5f, 4.4f, 10.5f), glm::vec3(1.0f, 1.0f, 0.0f), 20.0f},
    {glm::vec3(11.2f, 4.6f, -11.3f), glm::vec3(1.0f, 1.0f, 0.0f), 20.0f},
    {glm::vec3(-10.9f, 4.5f, -10.9f), glm::vec3(1.0f, 1.0f, 0.0f), 20.0f},
    {glm::vec3(-9.9f, 4.3f, 9.1f), glm::vec3(1.0f, 1.0f, 0.0f), 20.0f},

    //Azul
    {glm::vec3(11.9f, 4.7f, 12.9f), glm::vec3(0.0f, 0.0f, 1.0f), 20.0f},
    {glm::vec3(12.7f, 4.9f, -13.5f), glm::vec3(0.0f, 0.0f, 1.0f), 20.0f},
    {glm::vec3(-12.5f, 4.8f, -13.2f), glm::vec3(0.0f, 0.0f, 1.0f), 20.0f},
    {glm::vec3(-11.4f, 4.6f, 11.4f), glm::vec3(0.0f, 0.0f, 1.0f), 20.0f},

    //Morado
    {glm::vec3(13.3f, 4.9f, 15.0f), glm::vec3(0.5f, 0.0f, 0.5f), 20.0f},
    {glm::vec3(14.0f, 5.1f, -15.6f), glm::vec3(0.5f, 0.0f, 0.5f), 20.0f},
    {glm::vec3(-13.7f, 4.9f, -15.2f), glm::vec3(0.5f, 0.0f, 0.5f), 20.0f},
    {glm::vec3(-12.5f, 4.7f, 13.3f), glm::vec3(0.5f, 0.0f, 0.5f), 20.0f},

    //Blanco
    {glm::vec3(-19.7f, 7.0f, 24.8f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(-14.5f, 7.1f, 24.9f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(14.3f, 7.2f, 25.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(19.9f, 6.9f, 25.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(19.5f, 7.0f, -24.8f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(14.0f, 7.1f, -25.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(-13.9f, 7.0f, -25.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(-20.0f, 7.0f, -25.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(38.1f, 7.0f, -2.7f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(38.0f, 7.2f, 2.8f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(-38.0f, 7.0f, 2.7f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(-38.0f, 6.8f, -2.9f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},

    {glm::vec3(-21.0f, 1.6f, -8.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(-21.0f, 1.6f, 8.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(21.0f, 1.6f, 8.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},
    {glm::vec3(21.0f, 1.6f, -8.0f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f},

    {glm::vec3(-49.8f, 0.05f,-49.8), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f},
    {glm::vec3(-49.8f, 0.05f,49.8), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f},
    {glm::vec3(49.8f, 0.05f,-49.8), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f},
    {glm::vec3(49.8f, 0.05f,49.8), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f},
    {glm::vec3(49.8f, 0.05f,-39.9), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f},
    {glm::vec3(49.8f, 0.05f,39.9), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f},
    {glm::vec3(5.0f, 0.05f,40.0), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f},
    {glm::vec3(-5.0f, 0.05f,40.0), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f},

    {glm::vec3(-38.4f, 2.2f, -0.4f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(-38.4f, 2.2f, 4.5f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(-38.4f, 2.2f, -4.4f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},

    {glm::vec3(-21.6f, 1.1f, -24.8f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(-17.0f, 0.8f, -25.0f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(-12.3f, 0.7f, -25.1f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},

    {glm::vec3(12.4f, 3.0f, -25.6f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(17.3f, 3.0f, -25.6f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(22.0f, 3.0f, -25.6f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},

    {glm::vec3(37.2f, 0.2f, -4.5f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(37.2f, 0.2f, -2.96f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(37.2f, 0.2f, -1.4f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(37.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(37.2f, 0.2f, 1.8f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(37.2f, 0.2f, 3.4f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},
    {glm::vec3(37.2f, 0.2f, 4.95f), glm::vec3(1.0f, 1.0f, 1.0f), 13.0f},

};

glm::vec3 Light1 = glm::vec3(0);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    glfwInit();

    FMOD_RESULT resultado;
    resultado = FMOD::System_Create(&fmodSystem);
    if (resultado != FMOD_OK) {
        std::cerr << "FMOD error: " << FMOD_ErrorString(resultado) << std::endl;
        return EXIT_FAILURE;
    }

    resultado = fmodSystem->init(512, FMOD_INIT_NORMAL, 0);
    if (resultado != FMOD_OK) {
        std::cerr << "FMOD error: " << FMOD_ErrorString(resultado) << std::endl;
        return EXIT_FAILURE;
    }

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
    Shader skyboxshader("Shader/SkyBox.vs", "Shader/SkyBox.frag");

    //Modelo Feria
    Model Feria((char*)"Models/Feria.obj");
    Model Finn((char*)"Models/texturas_finn/finn.obj");
    Model Luigi((char*)"Models/Luigi/luigi.obj");


    //Modelo Rick
    Model RickCuerpo((char*)"Models/RickCuerpo.obj");
    Model RickCabeza((char*)"Models/RickCabeza.obj");
    Model RickBrazoD((char*)"Models/RickBrazoD.obj");
    Model RickBrazoI((char*)"Models/RickBrazoI.obj");
    Model RickPiernaD((char*)"Models/RickPiernaD.obj");
    Model RickPiernaI((char*)"Models/RickPiernaI.obj");

    //Modelos Topos
    Model cajaTopos((char*)"Models/topos/cajaTopos.obj");
    Model toposA((char*)"Models/topos/toposA.obj");
    Model toposB((char*)"Models/topos/toposB.obj");
    Model mazo((char*)"Models/topos/mazo.obj");
    Model mazo2((char*)"Models/topos/mazo2.obj");

    //Modelos Globos
    Model cajaGlobos((char*)"Models/globos/cajaGlobos.obj");
    Model dardo1((char*)"Models/globos/dardo1.obj");
    Model dardo2((char*)"Models/globos/dardo2.obj");
    Model dardo3((char*)"Models/globos/dardo3.obj");
    Model globo1((char*)"Models/globos/globo1.obj");
    Model globo2((char*)"Models/globos/globo2.obj");
    Model globo3((char*)"Models/globos/globo3.obj");

    //Modelos Hacha
    Model cajaHacha((char*)"Models/hacha/cajaHacha.obj");
    Model hacha((char*)"Models/hacha/hachaAnim.obj");
    Model reja((char*)"Models/hacha/reja.obj");

    //Modelos Dados
    Model tarro((char*)"Models/dados/tarro.obj");
    Model dado1((char*)"Models/dados/dadoAnim.obj");
    Model dado2((char*)"Models/dados/dadoAnim.obj");
    Model dado3((char*)"Models/dados/dadoAnim.obj");
    Model dado4((char*)"Models/dados/dadoAnim.obj");
    Model dado5((char*)"Models/dados/dadoAnim.obj");
    Model dadosA((char*)"Models/dados/dadosA.obj");
    Model dadosB((char*)"Models/dados/dadosB.obj");
    Model mesaDados((char*)"Models/dados/mesaDados.obj");

    //Modelos Boliche 
    Model boliche((char*)"Models/boliche.obj");
    Model caparazon((char*)"Models/caparazon.obj");
    Model pino((char*)"Models/pino.obj");

    //Modelos Bateo
    Model bateo((char*)"Models/zona_bateo.obj");
    Model bate((char*)"Models/Bate.obj");
    Model pelota((char*)"Models/Pelota.obj");

    //Modelos Maquina Monedas
    Model Maquina((char*)"Models/monedas/maquina.obj");

    fmodSystem->createSound("sonidos/feria.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sonidoFeria);
    fmodSystem->playSound(sonidoFeria, 0, false, &canalFeria);

    fmodSystem->createSound("sonidos/topos.mp3", FMOD_DEFAULT, 0, &sonidoTopos);
    fmodSystem->createSound("sonidos/bateo.mp3", FMOD_DEFAULT, 0, &sonidoBateo);
    fmodSystem->createSound("sonidos/dados.mp3", FMOD_DEFAULT, 0, &sonidoDados);
    fmodSystem->createSound("sonidos/hacha.mp3", FMOD_DEFAULT, 0, &sonidoHacha);
    fmodSystem->createSound("sonidos/bolos.mp3", FMOD_DEFAULT, 0, &sonidoBolos);
    fmodSystem->createSound("sonidos/globos.mp3", FMOD_DEFAULT, 0, &sonidoGlobos);

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Skybox
    GLuint skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    //Load textures
    vector < const GLchar*> faces;
    faces.push_back("skyBox/HDAright.jpg");
    faces.push_back("skyBox/HDAleft.jpg");
    faces.push_back("skyBox/HDAtop.jpg");
    faces.push_back("skyBox/HDAbottom.jpg");
    faces.push_back("skyBox/HDAback.jpg");
    faces.push_back("skyBox/HDAfront.jpg");

    GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

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

        fmodSystem->update();
        glfwPollEvents();
        DoMovement();
        Animation();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        if (!esDeNoche) {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.4f, 0.4f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.8f, 0.8f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);
        }
        else {
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.1f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.1f, 0.1f, 0.2f);
        }

        for (int i = 0; i < NUM_LUCES; i++) {
            std::string index = "pointLights[" + std::to_string(i) + "]";
            glm::vec3 pos = luces[i].posicion;
            glm::vec3 col = luces[i].color;
            float alcance = luces[i].alcance;
            float intensidadFija = esDeNoche ? 2.0f : 0.0f;

            glUniform3f(glGetUniformLocation(lightingShader.Program, (index + ".position").c_str()), pos.x, pos.y, pos.z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (index + ".ambient").c_str()), col.x * 0.1f * intensidadFija, col.y * 0.1f * intensidadFija, col.z * 0.1f * intensidadFija);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (index + ".diffuse").c_str()), col.x * intensidadFija, col.y * intensidadFija, col.z * intensidadFija);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (index + ".specular").c_str()), intensidadFija, intensidadFija, intensidadFija);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (index + ".constant").c_str()), 1.0f);

            float linear, quadratic;
            if (alcance <= 7.0f) {
                linear = 0.7f; quadratic = 1.8f;
            }
            else if (alcance <= 13.0f) {
                linear = 0.35f; quadratic = 0.44f;
            }
            else if (alcance <= 20.0f) {
                linear = 0.22f; quadratic = 0.20f;
            }
            else if (alcance <= 50.0f) {
                linear = 0.09f; quadratic = 0.032f;
            }
            else if (alcance <= 100.0f) {
                linear = 0.045f; quadratic = 0.0075f;
            }
            else {
                linear = 0.02f; quadratic = 0.001f;
            }

            glUniform1f(glGetUniformLocation(lightingShader.Program, (index + ".linear").c_str()), linear);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (index + ".quadratic").c_str()), quadratic);
        }

        glm::vec3 lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.direction"), lightDir.x, lightDir.y, lightDir.z);

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
        if (tipoCamara == CAMARA_JUEGO_TOPOS) {
            camera.SetPosition(glm::vec3(-17.0f, 3.5f, 20.0f));
            camera.SetYaw(90.0f);
            camera.SetPitch(-15.0f);
            camera.UpdateVectors();
        }
        if (tipoCamara == CAMARA_JUEGO_BATEO) {
            camera.SetPosition(glm::vec3(-28.0f, 3.2f, 0.0f));
            camera.SetYaw(180.0f);
            camera.SetPitch(-15.0f);
            camera.UpdateVectors();
        }
        if (tipoCamara == CAMARA_JUEGO_BOLOS) {
            camera.SetPosition(glm::vec3(28.0f, 3.2f, 0.0f));
            camera.SetYaw(0.0f);
            camera.SetPitch(-15.0f);
            camera.UpdateVectors();
        }
        if (tipoCamara == CAMARA_JUEGO_DADOS) {
            camera.SetPosition(glm::vec3(-17.0f, 3.5f, -20.0f));
            camera.SetYaw(270.0f);
            camera.SetPitch(-15.0f);
            camera.UpdateVectors();
        }
        if (tipoCamara == CAMARA_JUEGO_HACHA) {
            camera.SetPosition(glm::vec3(17.0f, 3.5f, -20.0f));
            camera.SetYaw(270.0f);
            camera.SetPitch(-15.0f);
            camera.UpdateVectors();
        }
        if (tipoCamara == CAMARA_JUEGO_GLOBOS) {
            camera.SetPosition(glm::vec3(17.0f, 3.5f, 20.0f));
            camera.SetYaw(90.0f);
            camera.SetPitch(-15.0f);
            camera.UpdateVectors();
        }
        glm::mat4 view;
        view = camera.GetViewMatrix();

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //FERIA

        glm::mat4 model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Feria.Draw(lightingShader);


        //RICK

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
        modelBrazoD = glm::rotate(modelBrazoD, glm::radians(anguloBrazoRick), glm::vec3(0.0f, 0.0f, 1.0f));
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
        modelPiernaD = glm::rotate(modelPiernaD, glm::radians(rotateRickPiernas * -1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPiernaD));
        RickPiernaD.Draw(lightingShader);

        glm::mat4 modelPiernaI = model;
        modelPiernaI = glm::translate(modelPiernaI, glm::vec3(-0.1f, -1.2f, -0.2f));
        modelPiernaI = glm::rotate(modelPiernaI, glm::radians(rotateRickPiernas), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPiernaI));
        RickPiernaI.Draw(lightingShader);

        //NPC

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(10.0f, 0.1f, 10.0f));
        model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Finn.Draw(lightingShader);


        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(30.0f, 0.1f, 8.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.019f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Luigi.Draw(lightingShader);

        //TOPOS

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-17.0f, 0.0f, 24.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cajaTopos.Draw(lightingShader);
        glm::mat4 modelTopoA = model;
        modelTopoA = glm::translate(modelTopoA, glm::vec3(0.0f, topos[0].altura, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTopoA));
        toposA.Draw(lightingShader);
        glm::mat4 modelTopoB = model;
        modelTopoB = glm::translate(modelTopoB, glm::vec3(0.0f, topos[1].altura, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTopoB));
        toposB.Draw(lightingShader);
        glm::mat4 modelMazo = glm::mat4(1.0f);;
        modelMazo = glm::translate(modelMazo, masoTranslate);
        modelMazo = glm::rotate(modelMazo, glm::radians(anguloMazoY), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMazo = glm::rotate(modelMazo, glm::radians(anguloMazoX), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMazo = glm::scale(modelMazo, glm::vec3(0.65f, 0.65f, 0.65f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMazo));
        mazo2.Draw(lightingShader);

        glm::mat4 model2 = model;
        model2 = glm::translate(model2, glm::vec3(-7.5f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        cajaTopos.Draw(lightingShader);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        toposA.Draw(lightingShader);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        toposB.Draw(lightingShader);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        mazo.Draw(lightingShader);

        glm::mat4 model3 = model;
        model3 = glm::translate(model3, glm::vec3(7.5f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        cajaTopos.Draw(lightingShader);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        toposA.Draw(lightingShader);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        toposB.Draw(lightingShader);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        mazo.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-22.0f, 0.0f, 21.0f));
        model = glm::rotate(model, glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Maquina.Draw(lightingShader);

        //BATEO

        glm::mat4 modelTempBateo = glm::mat4(1.0f);
        glm::mat4 modelTempBateo2 = glm::mat4(1.0f);
        glm::mat4 modelTempBateo3 = glm::mat4(1.0f);

        model = glm::mat4(1.0f);
        modelTempBateo = model = glm::translate(model, glm::vec3(-35.0f, 0.1f, 0.0f));
        modelTempBateo = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        modelTempBateo = model = glm::scale(model, glm::vec3(0.35f, 0.24f, 0.40f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        bateo.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-31.5f, 0.3f, 4.3f));
        model = glm::scale(model, glm::vec3(0.06f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pelota.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-31.0f, 0.3f, 4.3f));
        modelTempBateo = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.06f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        bate.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-31.5f, 0.3f, -4.3f));
        model = glm::scale(model, glm::vec3(0.06f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pelota.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-31.0f, 0.3f, -4.3f));
        modelTempBateo = model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.06f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        bate.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(posPelotaX, posPelotaY, posPelotaZ));
        model = glm::scale(model, glm::vec3(0.06f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pelota.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-31.0f, 1.3f, 0.7f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotacionBate), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.06f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        bate.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -6.0f));
        model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Maquina.Draw(lightingShader);

        //DADOS

        glm::mat4 modelTempDado = glm::mat4(1.0f);
        modelTempDado = model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-17.0f, 0.0f, -22.0f));
        model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        tarro.Draw(lightingShader);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dadosA.Draw(lightingShader);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dadosB.Draw(lightingShader);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        mesaDados.Draw(lightingShader);

        glm::mat4 modelD1 = glm::mat4(1.0f);
        modelD1 = glm::translate(modelD1, glm::vec3(-17.0f, 2.1f, -23.0f));
        modelD1 = glm::rotate(modelD1, glm::radians(0.0f), glm::vec3(1, 0, 0));
        modelD1 = glm::rotate(modelD1, glm::radians(rotX1), glm::vec3(1, 0, 0));
        modelD1 = glm::rotate(modelD1, glm::radians(rotY1), glm::vec3(0, 1, 0));
        modelD1 = glm::rotate(modelD1, glm::radians(rotZ1), glm::vec3(0, 0, 1));
        modelD1 = glm::scale(modelD1, glm::vec3(0.26f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD1));
        dado1.Draw(lightingShader);

        glm::mat4 modelD2 = glm::mat4(1.0f);
        modelD2 = glm::translate(modelD2, glm::vec3(-16.5f, 2.1f, -22.5f));
        modelD2 = glm::rotate(modelD2, glm::radians(90.0f), glm::vec3(0, 0, 1));
        modelD2 = glm::rotate(modelD2, glm::radians(rotX2), glm::vec3(1, 0, 0));
        modelD2 = glm::rotate(modelD2, glm::radians(rotY2), glm::vec3(0, 1, 0));
        modelD2 = glm::rotate(modelD2, glm::radians(rotZ2), glm::vec3(0, 0, 1));
        modelD2 = glm::scale(modelD2, glm::vec3(0.26f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD2));
        dado2.Draw(lightingShader);

        glm::mat4 modelD3 = glm::mat4(1.0f);
        modelD3 = glm::translate(modelD3, glm::vec3(-17.5f, 2.1f, -24.0f));
        modelD3 = glm::rotate(modelD3, glm::radians(90.0f), glm::vec3(1, 0, 0));
        modelD3 = glm::rotate(modelD3, glm::radians(rotX3), glm::vec3(1, 0, 0));
        modelD3 = glm::rotate(modelD3, glm::radians(rotY3), glm::vec3(0, 1, 0));
        modelD3 = glm::rotate(modelD3, glm::radians(rotZ3), glm::vec3(0, 0, 1));
        modelD3 = glm::scale(modelD3, glm::vec3(0.26f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD3));
        dado3.Draw(lightingShader);

        glm::mat4 modelD4 = glm::mat4(1.0f);
        modelD4 = glm::translate(modelD4, glm::vec3(-16.8f, 2.1f, -24.5f));
        modelD4 = glm::rotate(modelD4, glm::radians(-90.0f), glm::vec3(1, 0, 0));
        modelD4 = glm::rotate(modelD4, glm::radians(rotX4), glm::vec3(1, 0, 0));
        modelD4 = glm::rotate(modelD4, glm::radians(rotY4), glm::vec3(0, 1, 0));
        modelD4 = glm::rotate(modelD4, glm::radians(rotZ4), glm::vec3(0, 0, 1));
        modelD4 = glm::scale(modelD4, glm::vec3(0.26f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD4));
        dado4.Draw(lightingShader);

        glm::mat4 modelD5 = glm::mat4(1.0f);
        modelD5 = glm::translate(modelD5, glm::vec3(-17.3f, 2.1f, -25.5f));
        modelD5 = glm::rotate(modelD5, glm::radians(180.0f), glm::vec3(0, 0, 1));
        modelD5 = glm::rotate(modelD5, glm::radians(rotX5), glm::vec3(1, 0, 0));
        modelD5 = glm::rotate(modelD5, glm::radians(rotY5), glm::vec3(0, 1, 0));
        modelD5 = glm::rotate(modelD5, glm::radians(rotZ5), glm::vec3(0, 0, 1));
        modelD5 = glm::scale(modelD5, glm::vec3(0.26f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelD5));
        dado5.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-12.0f, 0.0f, -20.0f));
        model = glm::rotate(model, glm::radians(280.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Maquina.Draw(lightingShader);

        //HACHA

        glm::mat4 modelTempHacha = glm::mat4(1.0f);
        modelTempHacha = model = glm::mat4(1.0f);
        modelTempHacha = model = glm::translate(model, glm::vec3(17.0f, 0.15f, -22.0f));
        modelTempHacha = model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cajaHacha.Draw(lightingShader);
        modelTempHacha = glm::translate(glm::mat4(1.0f), glm::vec3(17.0f, 2.3f, -22.0f - desplazamientoHacha));
        modelTempHacha = glm::rotate(modelTempHacha, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, -0.4f));
        modelTempHacha = glm::scale(modelTempHacha, glm::vec3(0.08f));
        glm::mat4 modelHacha = modelTempHacha;
        modelHacha = glm::translate(modelHacha, glm::vec3(0.0f, 0.5f, 0.0f));
        modelHacha = glm::rotate(modelHacha, glm::radians(rotacionHacha), glm::vec3(0.0f, 0.0f, 1.0f));
        modelHacha = glm::translate(modelHacha, glm::vec3(0.0f, -0.5f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHacha));
        hacha.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(17.0f, 0.15f, -22.0f));
        model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        reja.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(22.0f, 0.0f, -21.0f));
        model = glm::rotate(model, glm::radians(280.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Maquina.Draw(lightingShader);

        //BOLOS

        glm::mat4 modelTempBolos = glm::mat4(1.0f);
        glm::mat4 modelTempBolos2 = glm::mat4(1.0f);
        glm::mat4 modelTempBolos3 = glm::mat4(1.0f);
        model = glm::mat4(1.0f);
        modelTempBolos = model = glm::translate(model, glm::vec3(35.0f, 0.1f, 0.0f));
        modelTempBolos = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        modelTempBolos = model = glm::scale(model, glm::vec3(0.35f, 0.24f, 0.40f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        boliche.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(posCaparazonX, 0.1f, 0.2f));
        model = glm::rotate(model, glm::radians(impactoRotacion), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        caparazon.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(32.0f, 0.1f, 1.8f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        caparazon.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(32.0f, 0.1f, 3.4f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        caparazon.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(32.0f, 0.1f, 5.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        caparazon.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(32.0f, 0.1f, -1.4f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        caparazon.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(32.0f, 0.1f, -3.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        caparazon.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(32.0f, 0.1f, -4.6f));
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        caparazon.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(37.1f, 0.0f, 0.2f));
        model = glm::translate(model, glm::vec3(0.0f, pinoPosY, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f - rotacionPinoAnimado), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(37.25f, 0.0f, 0.4f));
        model = glm::translate(model, glm::vec3(0.0f, pinoPosY, 0.0f));
        model = glm::rotate(model, glm::radians(-rotacionPinoAnimado), glm::vec3(0.0f, 0.5f, 1.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(37.25f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, pinoPosY, 0.0f));
        model = glm::rotate(model, glm::radians(-rotacionPinoAnimado), glm::vec3(0.0f, -0.5f, 1.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(37.4f, 0.0f, 0.5f));
        model = glm::translate(model, glm::vec3(0.0f, pinoPosY, 0.0f));
        model = glm::rotate(model, glm::radians(-rotacionPinoAnimado), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(37.4f, 0.0f, -0.1f));
        model = glm::translate(model, glm::vec3(0.0f, pinoPosY, 0.0f));
        model = glm::rotate(model, glm::radians(-rotacionPinoAnimado), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(37.4f, 0.0f, 0.2f));
        model = glm::translate(model, glm::vec3(0.0f, pinoPosY, 0.0f));
        model = glm::rotate(model, glm::radians(-rotacionPinoAnimado), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(37.55f, 0.0f, 0.7f));
        model = glm::translate(model, glm::vec3(0.0f, pinoPosY, 0.0f));
        model = glm::rotate(model, glm::radians(-rotacionPinoAnimado), glm::vec3(0.0f, 0.5f, 1.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(37.55f, 0.0f, 0.4f));
        model = glm::translate(model, glm::vec3(0.0f, pinoPosY, 0.0f));
        model = glm::rotate(model, glm::radians(-rotacionPinoAnimado), glm::vec3(0.0f, -0.5f, 1.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(37.55f, 0.0f, -0.0f));
        model = glm::translate(model, glm::vec3(0.0f, pinoPosY, 0.0f));
        model = glm::rotate(model, glm::radians(-rotacionPinoAnimado), glm::vec3(0.0f, 0.5f, 1.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(37.55f, 0.0f, -0.3f));
        model = glm::translate(model, glm::vec3(0.0f, pinoPosY, 0.0f));
        model = glm::rotate(model, glm::radians(-rotacionPinoAnimado), glm::vec3(0.0f, -0.5f, 1.0f));
        model = glm::scale(model, glm::vec3(0.08f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pino.Draw(lightingShader);

        for (float zCarril : carrilesZ) {
            for (const auto& rel : trianguloRelativo) {
                glm::vec3 posicionPino = glm::vec3(37.1f - rel.z, rel.y, zCarril + rel.x);
                model = glm::mat4(1.0f);
                model = glm::translate(model, posicionPino);
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.08f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                pino.Draw(lightingShader);
            }
        }

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(30.0f, 0.0f, 6.0f));
        model = glm::rotate(model, glm::radians(190.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Maquina.Draw(lightingShader);

        //GLOBOS

        glm::mat4 modelTempGlobos = glm::mat4(1.0f);
        glm::mat4 modelTempGlobos2 = glm::mat4(1.0f);
        glm::mat4 modelTempGlobos3 = glm::mat4(1.0f);
        model = glm::mat4(1);
        modelTempGlobos = model = glm::translate(model, glm::vec3(17.0f, 0.0f, 24.0f));
        modelTempGlobos = model = glm::scale(modelTempGlobos, glm::vec3(0.40f, 0.40f, 0.40f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cajaGlobos.Draw(lightingShader);

        glm::mat4 modelG1 = modelTempGlobos;
        modelG1 = glm::translate(modelG1, glm::vec3(6.0f, 9.0f, posDardo1Z));
        modelG1 = glm::rotate(modelG1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelG1));
        dardo1.Draw(lightingShader);

        glm::mat4 modelG2 = modelTempGlobos;
        modelG2 = glm::translate(modelG2, glm::vec3(2.85f, 4.5f, posDardo2Z));
        modelG2 = glm::rotate(modelG2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelG2));
        dardo2.Draw(lightingShader);

        glm::mat4 modelG3 = modelTempGlobos;
        modelG3 = glm::translate(modelG3, glm::vec3(3.7f, 4.2f, posDardo3Z));
        modelG3 = glm::rotate(modelG3, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelG3));
        dardo3.Draw(lightingShader);

        if (globo1Visible) {
            model = modelTempGlobos;
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            globo1.Draw(lightingShader);
        }
        if (globo2Visible) {
            model = modelTempGlobos;
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            globo2.Draw(lightingShader);
        }
        if (globo3Visible) {
            model = modelTempGlobos;
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            globo3.Draw(lightingShader);
        }

        modelTempGlobos2 = glm::translate(modelTempGlobos, glm::vec3(-12.0f, 0.0f, 0.0f));
        model = modelTempGlobos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cajaGlobos.Draw(lightingShader);
        model = modelTempGlobos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dardo1.Draw(lightingShader);
        model = modelTempGlobos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dardo2.Draw(lightingShader);
        model = modelTempGlobos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dardo3.Draw(lightingShader);

        model = modelTempGlobos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        globo1.Draw(lightingShader);
        model = modelTempGlobos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        globo2.Draw(lightingShader);
        model = modelTempGlobos2;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        globo3.Draw(lightingShader);

        modelTempGlobos3 = glm::translate(modelTempGlobos, glm::vec3(12.0f, 0.0f, 0.0f));
        model = modelTempGlobos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cajaGlobos.Draw(lightingShader);
        model = modelTempGlobos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dardo1.Draw(lightingShader);
        model = modelTempGlobos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dardo2.Draw(lightingShader);
        model = modelTempGlobos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dardo3.Draw(lightingShader);

        model = modelTempGlobos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        globo1.Draw(lightingShader);
        model = modelTempGlobos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        globo2.Draw(lightingShader);
        model = modelTempGlobos3;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        globo3.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(12.0f, 0.0f, 21.0f));
        model = glm::rotate(model, glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Maquina.Draw(lightingShader);

        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(0);

        //SKYBOX

        glDepthFunc(GL_LEQUAL);
        skyboxshader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(glGetUniformLocation(skyboxshader.Program, "skyboxIntensity"), esDeNoche ? 0.2f : 1.0f);


        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVAO);

    sonidoFeria->release();
    sonidoTopos->release();
    sonidoBateo->release();
    sonidoDados->release();
    sonidoHacha->release();
    sonidoBolos->release();
    sonidoGlobos->release();
    fmodSystem->close();
    fmodSystem->release();

    glfwTerminate();



    return 0;
}

void DoMovement()
{
    if (!puedeTeclear) return;

    if (tipoCamara == CAMARA_LIBRE) {
        if (keys[GLFW_KEY_W]) {
            camera.ProcessKeyboard(FORWARD, deltaTime);
            glm::vec3 pos = camera.GetPosition();
            std::cout << "Camara: X=" << pos.x << " Y=" << pos.y << " Z=" << pos.z << std::endl;
        }
        if (keys[GLFW_KEY_S]) {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
            glm::vec3 pos = camera.GetPosition();
            std::cout << "Camara: X=" << pos.x << " Y=" << pos.y << " Z=" << pos.z << std::endl;
        }
        if (keys[GLFW_KEY_A]) {
            camera.ProcessKeyboard(LEFT, deltaTime);
            glm::vec3 pos = camera.GetPosition();
            std::cout << "Camara: X=" << pos.x << " Y=" << pos.y << " Z=" << pos.z << std::endl;
        }
        if (keys[GLFW_KEY_D]) {
            camera.ProcessKeyboard(RIGHT, deltaTime);
            glm::vec3 pos = camera.GetPosition();
            std::cout << "Camara: X=" << pos.x << " Y=" << pos.y << " Z=" << pos.z << std::endl;
        }
    }


    if (tipoCamara == CAMARA_TERCERA_PERSONA) {

        caminando = false;

        if (keys[GLFW_KEY_UP])
        {
            float deltaZ = sin(glm::radians(-rotateRick)) * 0.1f;
            float deltaX = cos(glm::radians(-rotateRick)) * 0.1f;

            if (translateRick.z + deltaZ <= 50.0f && translateRick.z + deltaZ >= -50.0f &&
                translateRick.x + deltaX <= 50.0f && translateRick.x + deltaX >= -50.0f)
            {
                translateRick.z += deltaZ;
                translateRick.x += deltaX;
                caminando = true;
            }
        }

        if (keys[GLFW_KEY_DOWN])
        {
            float deltaZ = -sin(glm::radians(-rotateRick)) * 0.1f;
            float deltaX = -cos(glm::radians(-rotateRick)) * 0.1f;

            if (translateRick.z + deltaZ <= 50.0f && translateRick.z + deltaZ >= -50.0f &&
                translateRick.x + deltaX <= 50.0f && translateRick.x + deltaX >= -50.0f)
            {
                translateRick.z += deltaZ;
                translateRick.x += deltaX;
                caminando = true;
            }
        }


        if (keys[GLFW_KEY_LEFT])
        {
            rotateRick += 1.0f;
        }

        if (keys[GLFW_KEY_RIGHT])
        {
            rotateRick -= 1.0f;
        }
    }

}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

    if (!puedeTeclear) return;

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

    if (keys[GLFW_KEY_N]) {
        esDeNoche = !esDeNoche;
    }

    if (key == GLFW_KEY_1) {
        fmodSystem->playSound(sonidoTopos, 0, false, &canalTopos);
        tipoCamara = CAMARA_TERCERA_PERSONA;
        camera.SetYaw(0.0f);
        camera.SetPitch(0.0f);
        camera.UpdateVectors();
        puedeTeclear = false;
        insertCoinTopos = true;
        tiempoAnimacion = 0.0f;
    }

    if (key == GLFW_KEY_2) {
        fmodSystem->playSound(sonidoBateo, 0, false, &canalBateo);
        tipoCamara = CAMARA_TERCERA_PERSONA;
        camera.SetYaw(0.0f);
        camera.SetPitch(0.0f);
        camera.UpdateVectors();
        puedeTeclear = false;
        insertCoinBatear = true;
        tiempoAnimacion = 0.0f;
        anguloBrazoRick = 0.0f;
        animarPelota = true;
        pelotaGolpeada = false;
        pelotaRegresando = false;
        posPelotaX = -37.1f;
        posPelotaZ = 0.0f;
        posPelotaY = 0.3f;
        rotacionBate = 0.0f;
        animarBate = false;
        bateGolpeo = false;
    }
    if (key == GLFW_KEY_3) {
        fmodSystem->playSound(sonidoDados, 0, false, &canalDados);
        tipoCamara = CAMARA_TERCERA_PERSONA;
        camera.SetYaw(0.0f);
        camera.SetPitch(0.0f);
        camera.UpdateVectors();
        tiempoAnimacionDados = 0.0f;
        tiempoAnimacion = 0.0f;
        insertCoinDados = true;
        puedeTeclear = false;
        rotX1 = rotY1 = rotZ1 = 0.0f;
        rotX2 = rotY2 = rotZ2 = 0.0f;
        rotX3 = rotY3 = rotZ3 = 0.0f;
        rotX4 = rotY4 = rotZ4 = 0.0f;
        rotX5 = rotY5 = rotZ5 = 0.0f;
    }

    if (key == GLFW_KEY_4) {
        fmodSystem->playSound(sonidoHacha, 0, false, &canalHacha);
        tipoCamara = CAMARA_TERCERA_PERSONA;
        camera.SetYaw(0.0f);
        camera.SetPitch(0.0f);
        camera.UpdateVectors();
        puedeTeclear = false;
        insertCoinHacha = true;
        hachaDesplazando = false;
        rotacionHacha = 0.0f;
        desplazamientoHacha = 0.0f;
    }


    if (key == GLFW_KEY_5) {
        fmodSystem->playSound(sonidoBolos, 0, false, &canalBolos);
        tipoCamara = CAMARA_TERCERA_PERSONA;
        camera.SetYaw(0.0f);
        camera.SetPitch(0.0f);
        camera.UpdateVectors();
        puedeTeclear = false;
        insertCoinBolos = true;
        tiempoAnimacion = 0.0f;

        lanzarCaparazon = true;
        posCaparazonX = 32.0f;
        impactoRotacion = 0.0f;
        rotacionPinoAnimado = 0.0f;
        pinoPosY = 0.5f;
        pinosCaidos = false;
    }


    if (key == GLFW_KEY_6) {

        fmodSystem->playSound(sonidoGlobos, 0, false, &canalGlobos);

        tipoCamara = CAMARA_TERCERA_PERSONA;
        camera.SetYaw(0.0f);
        camera.SetPitch(0.0f);
        camera.UpdateVectors();
        puedeTeclear = false;
        insertCoinGlobos = true;
        tiempoAnimacion = 0.0f;

        animarDardos = true;
        animarDardo1 = true;
        animarDardo2 = false;
        animarDardo3 = false;

        posDardo1Z = posDardo2Z = posDardo3Z = -13.0f;

        globo1Visible = globo2Visible = globo3Visible = true;
    }


}

void Animation() {

    if (rotateCabezaSentido) {
        rotateRickCabeza += 0.4f;
        if (rotateRickCabeza >= 20.0f)
            rotateCabezaSentido = false;
    }
    else {
        rotateRickCabeza -= 0.4f;
        if (rotateRickCabeza <= -20.0f)
            rotateCabezaSentido = true;
    }

    if (caminando) {
        if (rotateRickPiernasSentido) {
            rotateRickPiernas += 1.0f;
            if (rotateRickPiernas >= 25.0f)
                rotateRickPiernasSentido = false;
        }
        else {
            rotateRickPiernas -= 1.0f;
            if (rotateRickPiernas <= -25.0f)
                rotateRickPiernasSentido = true;
        }
        if (rotateRickBrazosSentido) {
            rotateRickBrazos += 1.5f;
            if (rotateRickBrazos >= 70.0f)
                rotateRickBrazosSentido = false;
        }
        else {
            rotateRickBrazos -= 1.5f;
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

    if (insertCoinTopos) {
        translateRick = glm::vec3(-19.5f, 0.0f, 21.0f);
        rotateRick = 202.0f;

        if (brazoSubiendo) {
            anguloBrazoRick += 2.0f;
            if (anguloBrazoRick >= 90.0f) {
                anguloBrazoRick = 90.0f;
                brazoSubiendo = false;
                pausaInsertCoin = true;
                tiempoPausa = 0.0f;
            }
        }
        else if (pausaInsertCoin) {
            tiempoPausa += deltaTime;
            if (tiempoPausa >= 0.3f) {
                pausaInsertCoin = false;
            }
        }
        else {
            anguloBrazoRick -= 2.0f;
            if (anguloBrazoRick <= 0.0f) {
                anguloBrazoRick = 0.0f;
                tiempoPausa = 0.0f;
                insertCoinTopos = false;
                brazoSubiendo = true;
                juegoToposActivo = true;
                masoTranslate = glm::vec3(-17.8f, 1.35f, 23.75f);
                tipoCamara = CAMARA_JUEGO_TOPOS;
            }
        }
    }

    if (insertCoinBatear) {
        translateRick = glm::vec3(-30.0f, 0.0f, -3.0f);
        rotateRick = 102.0f;

        if (brazoSubiendo) {
            anguloBrazoRick += 2.0f;
            if (anguloBrazoRick >= 90.0f) {
                anguloBrazoRick = 90.0f;
                brazoSubiendo = false;
                pausaInsertCoin = true;
                tiempoPausa = 0.0f;
            }
        }
        else if (pausaInsertCoin) {
            tiempoPausa += deltaTime;
            if (tiempoPausa >= 0.3f) {
                pausaInsertCoin = false;
            }
        }
        else {
            anguloBrazoRick -= 2.0f;
            if (anguloBrazoRick <= 0.0f) {
                anguloBrazoRick = 0.0f;
                tiempoPausa = 0.0f;
                insertCoinBatear = false;
                brazoSubiendo = true;
                juegoBatearActivo = true;
                tipoCamara = CAMARA_JUEGO_BATEO;
            }
        }
    }

    if (insertCoinBolos) {
        translateRick = glm::vec3(30.0f, 0.0f, 3.0f);
        rotateRick = 290.0f;

        if (brazoSubiendo) {
            anguloBrazoRick += 2.0f;
            if (anguloBrazoRick >= 90.0f) {
                anguloBrazoRick = 90.0f;
                brazoSubiendo = false;
                pausaInsertCoin = true;
                tiempoPausa = 0.0f;
            }
        }
        else if (pausaInsertCoin) {
            tiempoPausa += deltaTime;
            if (tiempoPausa >= 0.3f) {
                pausaInsertCoin = false;
            }
        }
        else {
            anguloBrazoRick -= 2.0f;
            if (anguloBrazoRick <= 0.0f) {
                anguloBrazoRick = 0.0f;
                tiempoPausa = 0.0f;
                insertCoinBolos = false;
                brazoSubiendo = true;
                juegoBolosActivo = true;
                tipoCamara = CAMARA_JUEGO_BOLOS;
            }
        }
    }

    if (insertCoinDados) {
        translateRick = glm::vec3(-14.8f, 0.0f, -20.0f);
        rotateRick = 17.2f;

        if (brazoSubiendo) {
            anguloBrazoRick += 2.0f;
            if (anguloBrazoRick >= 90.0f) {
                anguloBrazoRick = 90.0f;
                brazoSubiendo = false;
                pausaInsertCoin = true;
                tiempoPausa = 0.0f;
            }
        }
        else if (pausaInsertCoin) {
            tiempoPausa += deltaTime;
            if (tiempoPausa >= 0.3f) {
                pausaInsertCoin = false;
            }
        }
        else {
            anguloBrazoRick -= 2.0f;
            if (anguloBrazoRick <= 0.0f) {
                anguloBrazoRick = 0.0f;
                tiempoPausa = 0.0f;
                insertCoinDados = false;
                brazoSubiendo = true;
                juegoDadosActivo = true;
                tipoCamara = CAMARA_JUEGO_DADOS;
            }
        }
    }

    //Animacion hacha 
    if (insertCoinHacha) {
        translateRick = glm::vec3(19.3f, 0.0f, -21.0f);
        rotateRick = 17.2f;

        if (brazoSubiendo) {
            anguloBrazoRick += 2.0f;
            if (anguloBrazoRick >= 90.0f) {
                anguloBrazoRick = 90.0f;
                brazoSubiendo = false;
                pausaInsertCoin = true;
                tiempoPausa = 0.0f;
            }
        }
        else if (pausaInsertCoin) {
            tiempoPausa += deltaTime;
            if (tiempoPausa >= 0.3f) {
                pausaInsertCoin = false;
            }
        }
        else {
            anguloBrazoRick -= 2.0f;
            if (anguloBrazoRick <= 0.0f) {
                anguloBrazoRick = 0.0f;
                tiempoPausa = 0.0f;
                insertCoinHacha = false;
                brazoSubiendo = true;
                juegoHachaActivo = true;
                tipoCamara = CAMARA_JUEGO_HACHA;
            }
        }
    }


    if (insertCoinGlobos) {
        translateRick = glm::vec3(14.0f, 0.0f, 21.2f);
        rotateRick = -164.4f;

        if (brazoSubiendo) {
            anguloBrazoRick += 2.0f;
            if (anguloBrazoRick >= 90.0f) {
                anguloBrazoRick = 90.0f;
                brazoSubiendo = false;
                pausaInsertCoin = true;
                tiempoPausa = 0.0f;
            }
        }
        else if (pausaInsertCoin) {
            tiempoPausa += deltaTime;
            if (tiempoPausa >= 0.3f) {
                pausaInsertCoin = false;
            }
        }
        else {
            anguloBrazoRick -= 2.0f;
            if (anguloBrazoRick <= 0.0f) {
                anguloBrazoRick = 0.0f;
                tiempoPausa = 0.0f;
                insertCoinGlobos = false;
                brazoSubiendo = true;
                juegoGlobosActivo = true;
                tipoCamara = CAMARA_JUEGO_GLOBOS;
            }
        }
    }

    if (juegoToposActivo) {

        float velocidad = 2.5f;
        tiempoAnimacion += deltaTime;

        anguloMazoY = 80.0f;

        if (bajando) {
            anguloMazoX += 1.5f;
            if (anguloMazoX >= maxAnguloGolpe) {
                anguloMazoX = maxAnguloGolpe;
                bajando = false;
            }
        }
        else {
            anguloMazoX -= 1.5f;
            if (anguloMazoX <= minAnguloGolpe) {
                anguloMazoX = minAnguloGolpe;
                bajando = true;
            }
        }

        if (izquierda) {
            masoTranslate.x -= 0.03f;
            if (masoTranslate.x <= -18.3f) {
                masoTranslate.x = -18.3f;
                izquierda = false;
            }
        }
        else {
            masoTranslate.x += 0.03f;
            if (masoTranslate.x >= -15.8f) {
                masoTranslate.x = -15.8f;
                izquierda = true;
            }
        }

        for (int i = 0; i < topos.size(); ++i) {
            Topo& topo = topos[i];
            topo.tiempo += deltaTime;

            if (i == 1 && topo.retrasoInicial) {
                if (topo.tiempo < 0.4f) {
                    continue;
                }
                else {
                    topo.retrasoInicial = false;
                    topo.tiempo = 0.0f;
                }
            }

            if (topo.subiendo) {
                if (topo.tiempo < 0.5f) {
                    continue;
                }

                topo.altura += velocidad * deltaTime;
                if (topo.altura >= 0.0f) {
                    topo.altura = 0.0f;
                    topo.tiempo = 0.0f;
                    topo.subiendo = false;
                }
            }
            else {
                if (topo.tiempo < 0.5f) {
                    continue;
                }
                topo.altura -= velocidad * deltaTime;
                if (topo.altura <= -0.5f) {
                    topo.altura = -0.5f;
                    topo.tiempo = 0.0f;
                    topo.subiendo = true;
                }
            }
        }

        if (tiempoAnimacion >= duracionAnimacion) {
            juegoToposActivo = false;
            puedeTeclear = true;
            anguloMazoX = 0.0f;
            anguloMazoY = 0.0f;
            masoTranslate = glm::vec3(-17.8f, 1.55f, 25.75f);
            topos[0].altura = 0.0f; ;
            topos[1].altura = 0.0f; ;
            tipoCamara = CAMARA_TERCERA_PERSONA;
            camera.SetYaw(0.0f);
            camera.SetPitch(0.0f);
            camera.UpdateVectors();
            canalTopos->stop();
        }
    }

    if (juegoBatearActivo) {
        tiempoAnimacion += deltaTime;

        if (animarPelota && !pelotaGolpeada) {
            posPelotaX += velocidadPelota;
            posPelotaY = 1.3f;

            if (posPelotaX >= -31.5f) {
                pelotaGolpeada = true;
                animarBate = true;
            }
        }
        else if (animarPelota && pelotaGolpeada && !pelotaRegresando) {
            posPelotaZ += velocidadGolpe;

            if (posPelotaZ >= -1.6f) {
                pelotaRegresando = true;
            }
        }

        if (animarBate) {
            rotacionBate += 6.0f;
            if (rotacionBate >= 30.0f) {
                animarBate = false;
            }
        }
        else if (animarPelota && pelotaRegresando) {
            posPelotaX -= velocidadPelota;
            posPelotaY += 0.02f;

            if (posPelotaX <= -37.9f) {
                animarPelota = false;
                pelotaGolpeada = false;
                pelotaRegresando = false;
                posPelotaX = -37.9f;

                if (tiempoAnimacion < duracionAnimacion) {
                    posPelotaX = -37.1f;
                    posPelotaZ = 0.0f;
                    posPelotaY = 0.3f;
                    rotacionBate = 0.0f;
                    animarPelota = true;
                    animarBate = false;
                    bateGolpeo = false;
                }
            }
        }
        else if (rotacionBate > -30.0f) {
            rotacionBate -= 3.0f;
            if (rotacionBate < -30.0f) rotacionBate = -30.0f;
        }

        if (tiempoAnimacion >= duracionAnimacion) {
            juegoBatearActivo = false;
            puedeTeclear = true;
            posPelotaX = -37.1f;
            posPelotaZ = 0.0f;
            posPelotaY = 0.3f;
            rotacionBate = 0.0f;
            animarPelota = true;
            animarBate = false;
            bateGolpeo = false;
            tipoCamara = CAMARA_TERCERA_PERSONA;
            camera.SetYaw(0.0f);
            camera.SetPitch(0.0f);
            camera.UpdateVectors();
            canalBateo->stop();
        }
    }

    if (juegoBolosActivo) {
        tiempoAnimacion += deltaTime;

        if (lanzarCaparazon && posCaparazonX < 39.0f) {
            posCaparazonX += velocidadCaparazon;
            impactoRotacion += 4.0f;

            if (!pinosCaidos && posCaparazonX >= 36.8f) {
                pinosCaidos = true;
            }
        }
        else if (lanzarCaparazon) {
            lanzarCaparazon = false;
        }

        if (pinosCaidos && rotacionPinoAnimado < 90.0f) {
            rotacionPinoAnimado += 2.0f;
            pinoPosY -= 0.01f;
            if (pinoPosY < 0.2f) pinoPosY = 0.2f;
        }

        if (pinosCaidos && rotacionPinoAnimado >= 90.0f && tiempoAnimacion < duracionAnimacion) {
            insertCoinBolos = true;
            lanzarCaparazon = true;

            posCaparazonX = 32.0f;
            impactoRotacion = 0.0f;
            rotacionPinoAnimado = 0.0f;
            pinoPosY = 0.5f;
            pinosCaidos = false;
        }

        if (tiempoAnimacion >= duracionAnimacion) {
            juegoBolosActivo = false;
            puedeTeclear = true;
            lanzarCaparazon = false;
            posCaparazonX = 32.0f;
            tipoCamara = CAMARA_TERCERA_PERSONA;
            camera.SetYaw(0.0f);
            camera.SetPitch(0.0f);
            camera.UpdateVectors();
            canalBolos->stop();
        }
    }

    if (juegoDadosActivo) {
        tiempoAnimacion += deltaTime;

        if (enEsperaDados) {
            tiempoEsperaDados += deltaTime;
            if (tiempoEsperaDados >= 1.0f) {
                enEsperaDados = false;
                tiempoEsperaDados = 0.0f;
                tiempoAnimacionDados = 0.0f;
            }
        }
        else {
            tiempoAnimacionDados += deltaTime;

            rotX1 += 4.1f; rotY1 += 3.3f; rotZ1 += 2.7f;
            rotX2 += 2.8f; rotY2 += 4.5f; rotZ2 += 1.9f;
            rotX3 += 3.1f; rotY3 += 3.7f; rotZ3 += 3.0f;
            rotX4 += 3.6f; rotY4 += 2.9f; rotZ4 += 2.5f;
            rotX5 += 4.0f; rotY5 += 3.2f; rotZ5 += 2.3f;

            if (tiempoAnimacionDados >= duracionDados) {
                enEsperaDados = true;

                rotX1 = rotY1 = rotZ1 = 0.0f;
                rotX2 = rotY2 = rotZ2 = 0.0f;
                rotX3 = rotY3 = rotZ3 = 0.0f;
                rotX4 = rotY4 = rotZ4 = 0.0f;
                rotX5 = rotY5 = rotZ5 = 0.0f;
            }
        }

        if (tiempoAnimacion >= duracionAnimacion) {
            juegoDadosActivo = false;
            puedeTeclear = true;
            tiempoAnimacion = 0.0f;
            tiempoAnimacionDados = 0.0f;
            tiempoEsperaDados = 0.0f;
            enEsperaDados = false;

            tipoCamara = CAMARA_TERCERA_PERSONA;
            camera.SetYaw(0.0f);
            camera.SetPitch(0.0f);
            camera.UpdateVectors();

            rotX1 = rotY1 = rotZ1 = 0.0f;
            rotX2 = rotY2 = rotZ2 = 0.0f;
            rotX3 = rotY3 = rotZ3 = 0.0f;
            rotX4 = rotY4 = rotZ4 = 0.0f;
            rotX5 = rotY5 = rotZ5 = 0.0f;
            canalDados->stop();
        }
    }

    if (juegoHachaActivo) {
        tiempoAnimacionHacha += deltaTime;

        if (!enEsperaHacha && !animarHacha) {
            animarHacha = true;
            rotacionHacha = 0.0f;
            desplazamientoHacha = 0.0f;
        }

        if (animarHacha) {
            rotacionHacha -= 3.0f;
            desplazamientoHacha += 0.02f;

            if (rotacionHacha <= -360.0f) {
                rotacionHacha = -360.0f;
            }

            if (desplazamientoHacha >= 2.5f) {
                desplazamientoHacha = 2.5f;
                animarHacha = false;
                enEsperaHacha = true;
                tiempoEsperaHacha = 0.0f;
            }
        }

        if (enEsperaHacha) {
            tiempoEsperaHacha += deltaTime;
            if (tiempoEsperaHacha >= 0.5f) {
                enEsperaHacha = false;
            }
        }

        if (tiempoAnimacionHacha >= duracionAnimacion) {
            juegoHachaActivo = false;
            puedeTeclear = true;
            tiempoAnimacionHacha = 0.0f;
            tiempoEsperaHacha = 0.0f;
            enEsperaHacha = false;
            animarHacha = false;

            tipoCamara = CAMARA_TERCERA_PERSONA;
            camera.SetYaw(0.0f);
            camera.SetPitch(0.0f);
            camera.UpdateVectors();

            rotacionHacha = 0.0f;
            desplazamientoHacha = 2.5f;
            canalHacha->stop();
        }
    }

    if (juegoGlobosActivo) {
        tiempoAnimacion += deltaTime;

        if (animarDardos) {
            float velocidad = 0.1f;
            float impactoZ = -3.0f;
            float limiteZ = -2.5f;

            if (animarDardo1) {
                if (globo1Visible && posDardo1Z + velocidad >= impactoZ)
                    globo1Visible = false;

                if (posDardo1Z < limiteZ)
                    posDardo1Z += velocidad;
                else {
                    animarDardo1 = false;
                    animarDardo2 = true;
                }
            }

            else if (animarDardo2) {
                if (globo3Visible && posDardo2Z + velocidad >= impactoZ)
                    globo3Visible = false;

                if (posDardo2Z < limiteZ)
                    posDardo2Z += velocidad;
                else {
                    animarDardo2 = false;
                    animarDardo3 = true;
                }
            }

            else if (animarDardo3) {
                if (globo2Visible && posDardo3Z + velocidad >= impactoZ)
                    globo2Visible = false;

                if (posDardo3Z < limiteZ)
                    posDardo3Z += velocidad;
                else {
                    animarDardo3 = false;
                    animarDardos = false;

                    enEsperaDardos = true;
                    tiempoEsperaDardos = 0.0f;
                }
            }
        }

        if (enEsperaDardos) {
            tiempoEsperaDardos += deltaTime;

            if (tiempoEsperaDardos >= 0.5f ) {
                enEsperaDardos = false;
                animarDardos = true;
                animarDardo1 = true;
                animarDardo2 = false;
                animarDardo3 = false;
                posDardo1Z = posDardo2Z = posDardo3Z = -13.0f;
                globo1Visible = true;
                globo2Visible = true;
                globo3Visible = true;
            }
        }

        if (tiempoAnimacion >= duracionAnimacion) {
            juegoGlobosActivo = false;
            puedeTeclear = true;

            tipoCamara = CAMARA_TERCERA_PERSONA;
            camera.SetYaw(0.0f);
            camera.SetPitch(0.0f);
            camera.UpdateVectors();

            canalGlobos->stop();
            tiempoAnimacion = 0.0f;
            tiempoEsperaDardos = 0.0f;
            enEsperaDardos = false;
            animarDardos = false;
            animarDardo1 = false;
            animarDardo2 = false;
            animarDardo3 = false;

            posDardo1Z = posDardo2Z = posDardo3Z = -2.5f;
            globo1Visible = globo2Visible = globo3Visible = false;
        }
    }



}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{

    if (!puedeTeclear) return;

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
