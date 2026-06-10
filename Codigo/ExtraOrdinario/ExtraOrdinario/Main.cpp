#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//C++ standard libraries
#include <iostream>
#include <fstream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "SOIL2/SOIL2.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Animation.h"
#include "Animator.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void AnimationKeys();

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1000;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(1.5f, 0.5f,3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 270.0f);
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

//Luces
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f)
};

#define MAX_FRAMES 20
typedef struct _frame {
	float rotGato;
	float rotGatoInc;
	float gatoPosX;
	float gatoPosY;
	float gatoPosZ;
	float incX;
	float incY;
	float incZ;
	float headGato;
	float headIncGato;
	float FLegsGato;
	float FLegsIncGato;
	float BLegsGato;
	float BLegsIncGato;
	float tailGato;
	float tailIncGato;
	float inclinacionGato;
	float inclinacionIncGato;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

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

// Variables de Animacion del Zorro
float zorroPosX = -3.0f;
float zorroBaseY = 0.35f;
float zorroPosY = zorroBaseY;
float zorroPosZ = -2.0f; // Posicion inicial (para no chocar)
float rotPataDelDer = 0.0f;
float rotPataDelIzq = 0.0f;
float rotPataTrasDer = 0.0f;
float rotPataTrasIzq = 0.0f;
bool animCaminarZorro = false;
bool animSaltarZorro = false;
float tiempoAnimZorro = 0.0f;

// Variables Animacion Bola (animación en bucle continuo)
float anguloBola = 0.0f;
float radioBola = 2.0f;     // Distancia a la que girará del zorro
float velocidadBola = 2.0f;

// Variables de Animación del Perro (Máquina de Estados)
enum EstadoPerro { CAMINANDO, PARADO };
EstadoPerro estadoActualPerro = CAMINANDO;
float perroPosX = 0.0f;
float perroBaseY = 0.35f;
float perroPosY = perroBaseY;
float perroPosZ = -8.0f;
float perroVelocidad = 1.5f;
float perroRotCabeza = 0.0f;
float perroRotCola = 0.0f;
float perroRotPatas = 0.0f;
float tiempoPerro = 0.0f;

// === Animacion ciclo dia-noche ===
float sunAngle = 0.0f;
float sunSpeed = 0.2f;
float sunRadius = 10.0f;
bool dayNightCycle = true;
glm::vec3 sunPosition(0.0f);

// Intensidad luz puntual, se cambia con SPACE
glm::vec3 Light1 = glm::vec3(0);


// === Animaciones FBX (por esqueleto) ===
bool playFBX = false;
Animation* g_SteveAnim = nullptr;
Animator* g_SteveAnimator = nullptr;
float steveSpeed = 1.0f;
int steveEstado = 0;
float steveDir = 1.0f;
float stevePosX = 0.0f, stevePosY = 0.0f, stevePosZ = 0.0f;




#define MAX_FRAMES 16
int i_max_steps = 140;
int i_curr_steps = 0;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Inicializacion de GLFW y creacion de ventana
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Extraordinario CG", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// === Carga de shaders ===
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader skyboxShader("Shader/SkyBox.vs", "Shader/SkyBox.frag");


	// === Carga de modelos ===

	//Model Aldea
	Model Aldea((char*)"ModelsExtra/FloorUpdate.obj");


	Model tree((char*)"ModelsExtra/Tree/Tree.obj");

	//Model Steve
	Model Steve((char*)"ModelsExtra/Steve/steveAnimated.fbx");
	g_SteveAnim = new Animation("ModelsExtra/Steve/steveAnimated.fbx", &Steve);
	g_SteveAnimator = new Animator(g_SteveAnim);
	g_SteveAnimator->Reset();

	//Model Fox
	Model zorroCuerpo((char*)"ModelsExtra/Zorro/cuerpo.obj");
	Model zorroCabeza((char*)"ModelsExtra/Zorro/cabeza.obj");
	Model zorroCola((char*)"ModelsExtra/Zorro/cola.obj");
	Model zorroPDD((char*)"ModelsExtra/Zorro/pataDerDelante.obj");
	Model zorroPID((char*)"ModelsExtra/Zorro/pataIzqDelante.obj");
	Model zorroPDT((char*)"ModelsExtra/Zorro/pataDerDetras.obj");
	Model zorroPIT((char*)"ModelsExtra/Zorro/pataIzqDetras.obj");

	// Modelos Estáticos extra (Minecraft)
	Model modelLow((char*)"ModelsExtra/model_low.obj");
	Model Grass_block((char*)"ModelsExtra/Grass_block.obj");

	//Modelos de animación
	Model bola((char*)"ModelsExtra/ball.obj"); //animacion en bucle sin pausas
	// Modelos del Perro Rojo
	Model dogCuerpo((char*)"ModelsExtra/DogBody.obj");
	Model dogCabeza((char*)"ModelsExtra/HeadDog.obj");
	Model dogCola((char*)"ModelsExtra/TailDog.obj");
	Model dogPDD((char*)"ModelsExtra/F_RightLegDog.obj");
	Model dogPID((char*)"ModelsExtra/F_LeftLegDog.obj");
	Model dogPTD((char*)"ModelsExtra/B_RightLegDog.obj");
	Model dogPTI((char*)"ModelsExtra/B_LeftLegDog.obj");

	// Vertices skybox
	GLfloat skyboxVertices[] = {
		// Positions
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
	{  // Note that we start from 0!
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

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	//Skybox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector < const GLchar*> faces;
	faces.push_back("SkyBox/right.jpg");
	faces.push_back("SkyBox/left.jpg");
	faces.push_back("SkyBox/top.jpg");
	faces.push_back("SkyBox/bottom.jpg");
	faces.push_back("SkyBox/back.jpg");
	faces.push_back("SkyBox/front.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// === Loop para renderizar la escena ===
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		sunPosition = glm::vec3(0.0f, 10.0f, 0.0f);
		float dayFactor = 0.5f;

		// Dirección de la luz hacia abajo
		glm::vec3 sunDir = glm::vec3(0.0f, -1.0f, 0.0f);

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		AnimationKeys();


		// Clear the colorbuffer

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp

		
		// Use corresponding shader when setting uniforms/drawing objects
		lightingShader.Use();


		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// === Directional light (sun/moon) ===
		// Sunlight color shifts: warm orange near horizon, white at noon
		glm::vec3 noonColor = glm::vec3(1.0f, 1.0f, 1.0f);   // bright daylight

		glm::vec3 sunColor = noonColor;

		// Night ambient/diffuse values (cool, dim moonlight)
		glm::vec3 nightAmbient = glm::vec3(0.1f, 0.1f, 0.2f);
		glm::vec3 nightDiffuse = glm::vec3(0.08f, 0.08f, 0.2f);

		// Interpolate between night and day depending on dayFactor
		glm::vec3 ambient = glm::mix(nightAmbient, sunColor * 0.4f, dayFactor);
		glm::vec3 diffuse = glm::mix(nightDiffuse, sunColor * 0.8f, dayFactor);
		glm::vec3 specular = glm::mix(glm::vec3(0.1f), sunColor * 0.5f, dayFactor);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), sunDir.x, sunDir.y, sunDir.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), ambient.x, ambient.y, ambient.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), specular.x, specular.y, specular.z);


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);


		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Aldea.Draw(lightingShader);

		model = modelTemp;
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0, 0, 8));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tree.Draw(lightingShader);

		model = modelTemp;
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8, 0, 0));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tree.Draw(lightingShader);

		model = modelTemp;
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-8, 0, 0));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tree.Draw(lightingShader);

		model = modelTemp;
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0, 0, -10));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tree.Draw(lightingShader);

		model = modelTemp;
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3, 0, -15));
		model = glm::scale(model, glm::vec3(2.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		tree.Draw(lightingShader);

		// === RENDERIZADO DEL ZORRO ===
		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		glm::mat4 modelZorroBase = glm::mat4(1.0f);
		modelZorroBase = glm::translate(modelZorroBase, glm::vec3(zorroPosX, zorroPosY, zorroPosZ));
		modelZorroBase = glm::scale(modelZorroBase, glm::vec3(0.3f)); // Escalar para que no sea un zorro gigante

		// 1. Cuerpo, Cabeza y Cola (Heredan directo de la base)
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelZorroBase));
		zorroCuerpo.Draw(lightingShader);
		zorroCabeza.Draw(lightingShader);
		zorroCola.Draw(lightingShader);

		// 2. Patas Delanteras
		glm::mat4 modelPDD = modelZorroBase;
		modelPDD = glm::rotate(modelPDD, glm::radians(rotPataDelDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPDD));
		zorroPDD.Draw(lightingShader);

		glm::mat4 modelPDI = modelZorroBase;
		modelPDI = glm::rotate(modelPDI, glm::radians(rotPataDelIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPDI));
		zorroPID.Draw(lightingShader);

		// 3. Patas Traseras
		glm::mat4 modelPTD = modelZorroBase;
		modelPTD = glm::rotate(modelPTD, glm::radians(rotPataTrasDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPTD));
		zorroPDT.Draw(lightingShader);

		glm::mat4 modelPTI = modelZorroBase;
		modelPTI = glm::rotate(modelPTI, glm::radians(rotPataTrasIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPTI));
		zorroPIT.Draw(lightingShader);


		// === Animacion de Steve caminando ===
		model = modelTemp;
		if (playFBX)
		{
			// Maquina estados
			if (steveEstado == 0) // Estado 0: Caminando hacia adelante
			{
				g_SteveAnimator->UpdateAnimation(deltaTime);
				stevePosZ += steveDir * steveSpeed * deltaTime;

				if (stevePosZ >= 2.8f)
				{
					stevePosZ = 2.8f;   
					steveDir = -1.0f;
					steveEstado = 1;    
				}
			}
			else if (steveEstado == 1) // Estado 1: Regresando al origen
			{
				g_SteveAnimator->UpdateAnimation(deltaTime);
				stevePosZ += steveDir * steveSpeed * deltaTime;

				if (stevePosZ <= 0.0f)
				{
					stevePosZ = 0.0f;    
					steveEstado = 2;     
					playFBX = false;
				}
			}

			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 1);
			model = glm::translate(model, glm::vec3(2.0f + stevePosX, 0.1f + stevePosY, 0.0f + stevePosZ));
			if (steveEstado == 1)
				model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.05f));
			const auto& transforms = g_SteveAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < (int)transforms.size(); i++)
			{
				string uniformName = "finalBonesMatrices[" + to_string(i) + "]";
				glUniformMatrix4fv(
					glGetUniformLocation(lightingShader.Program, uniformName.c_str()),
					1, GL_FALSE, glm::value_ptr(transforms[i])
				);
			}
		}
		else
		{
			glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);
			model = glm::translate(model, glm::vec3(2.0f + stevePosX, 0.1f + stevePosY, 0.0f + stevePosZ));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.05f));
		}

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Steve.Draw(lightingShader);

		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		// === RENDERIZADO DE MODELOS MINECRAFT DESCARGADOS ===
		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		// 1. Model_Low
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		modelLow.Draw(lightingShader);

		// 2. Grass Block (Sin textura, solo geometría)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Grass_block.Draw(lightingShader);

		// === RENDERIZADO DE LA BOLA ORBITANDO ===
		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);
		glm::mat4 modelBola = glm::mat4(1.0f);

		modelBola = glm::translate(modelBola, glm::vec3(zorroPosX, zorroPosY + 0.5f, zorroPosZ));
		modelBola = glm::rotate(modelBola, anguloBola, glm::vec3(0.0f, 1.0f, 0.0f));
		modelBola = glm::translate(modelBola, glm::vec3(radioBola, 0.0f, 0.0f));
		modelBola = glm::scale(modelBola, glm::vec3(1.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBola));
		bola.Draw(lightingShader);

		// === RENDERIZADO DEL PERRO ROJO CON AJUSTE DE PIVOTE JERÁRQUICO ===
		glUniform1i(glGetUniformLocation(lightingShader.Program, "useBones"), 0);

		glm::mat4 modelPerroBase = glm::mat4(1.0f);
		modelPerroBase = glm::translate(modelPerroBase, glm::vec3(perroPosX, perroPosY, perroPosZ));
		modelPerroBase = glm::scale(modelPerroBase, glm::vec3(0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPerroBase));
		dogCuerpo.Draw(lightingShader);

		glm::mat4 modelDogCabeza = modelPerroBase;
		modelDogCabeza = glm::translate(modelDogCabeza, glm::vec3(0.0f, 0.08f, 0.08f));
		modelDogCabeza = glm::rotate(modelDogCabeza, glm::radians(perroRotCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		modelDogCabeza = glm::translate(modelDogCabeza, glm::vec3(0.0f, -0.08f, -0.08f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDogCabeza));
		dogCabeza.Draw(lightingShader);

		glm::mat4 modelDogCola = modelPerroBase;
		modelDogCola = glm::translate(modelDogCola, glm::vec3(0.0f, 0.05f, -0.20f));
		modelDogCola = glm::rotate(modelDogCola, glm::radians(perroRotCola), glm::vec3(0.0f, 1.0f, 0.0f));
		modelDogCola = glm::translate(modelDogCola, glm::vec3(0.0f, -0.05f, 0.20f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDogCola));
		dogCola.Draw(lightingShader);

		glm::mat4 modelDogPDD = modelPerroBase;
		modelDogPDD = glm::translate(modelDogPDD, glm::vec3(-0.03f, 0.0f, 0.05f));
		modelDogPDD = glm::rotate(modelDogPDD, glm::radians(perroRotPatas), glm::vec3(1.0f, 0.0f, 0.0f));
		modelDogPDD = glm::translate(modelDogPDD, glm::vec3(0.03f, 0.0f, -0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDogPDD));
		dogPDD.Draw(lightingShader);

		glm::mat4 modelDogPID = modelPerroBase;
		modelDogPID = glm::translate(modelDogPID, glm::vec3(0.03f, 0.0f, 0.05f));
		modelDogPID = glm::rotate(modelDogPID, glm::radians(-perroRotPatas), glm::vec3(1.0f, 0.0f, 0.0f));
		modelDogPID = glm::translate(modelDogPID, glm::vec3(-0.03f, 0.0f, -0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDogPID));
		dogPID.Draw(lightingShader);

		glm::mat4 modelDogPTD = modelPerroBase;
		modelDogPTD = glm::translate(modelDogPTD, glm::vec3(-0.03f, 0.0f, -0.15f));
		modelDogPTD = glm::rotate(modelDogPTD, glm::radians(-perroRotPatas), glm::vec3(1.0f, 0.0f, 0.0f));
		modelDogPTD = glm::translate(modelDogPTD, glm::vec3(0.03f, 0.0f, 0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDogPTD));
		dogPTD.Draw(lightingShader);

		glm::mat4 modelDogPTI = modelPerroBase;
		modelDogPTI = glm::translate(modelDogPTI, glm::vec3(0.03f, 0.0f, -0.15f));
		modelDogPTI = glm::rotate(modelDogPTI, glm::radians(perroRotPatas), glm::vec3(1.0f, 0.0f, 0.0f));
		modelDogPTI = glm::translate(modelDogPTI, glm::vec3(-0.03f, 0.0f, 0.15f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDogPTI));
		dogPTI.Draw(lightingShader);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(0);

		// === Dibujo del skybox ===
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);
	delete g_SteveAnim;
	delete g_SteveAnimator;
	glfwTerminate();
	return 0;
}


void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		stevePosZ = 0.0f;
		steveEstado = 0;
		steveDir = 1.0f;

		g_SteveAnimator->PlayAnimation(g_SteveAnim);
		playFBX = true;

		printf("Animaci�n de Steve iniciada\n");

	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		animCaminarZorro = !animCaminarZorro;
		animSaltarZorro = false;
		tiempoAnimZorro = 0.0f;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		animSaltarZorro = true;
		animCaminarZorro = false;
		tiempoAnimZorro = 0.0f;
	}

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		estadoActualPerro = CAMINANDO;
		printf("Perro reanudado\n");
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		estadoActualPerro = PARADO;
		printf("Perro pausado\n");
	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

// Function to handle keyframe animation logic
void AnimationKeys() {
	if (animCaminarZorro) {
		tiempoAnimZorro += deltaTime;
		zorroPosZ += 1.5f * deltaTime;

		// Limitar para que el zorro no salga de la escena y evitar choques
		if (zorroPosZ > 6.0f) {
			zorroPosZ = -10.0f;
		}

		// Movimiento alternado de patitas
		rotPataDelDer = sin(tiempoAnimZorro * 8.0f) * 25.0f;
		rotPataDelIzq = -sin(tiempoAnimZorro * 8.0f) * 25.0f;
		rotPataTrasDer = -sin(tiempoAnimZorro * 8.0f) * 25.0f;
		rotPataTrasIzq = sin(tiempoAnimZorro * 8.0f) * 25.0f;
	}
	else if (animSaltarZorro) {
		tiempoAnimZorro += deltaTime * 2.0f;

		// Movimiento salto hacia adelante
		zorroPosZ += 2.0f * deltaTime;

		// SUMA LA ALTURA BASE A LA PARÁBOLA DEL SALTO
		zorroPosY = zorroBaseY + (sin(tiempoAnimZorro * 3.1416f) * 1.5f);

		// Evitar salir de los limites durante el salto
		if (zorroPosZ > 6.0f) {
			zorroPosZ = -10.0f;
		}

		rotPataDelDer = -20.0f; rotPataDelIzq = -20.0f;
		rotPataTrasDer = 30.0f; rotPataTrasIzq = 30.0f;

		if (tiempoAnimZorro >= 1.0f) {
			animSaltarZorro = false;
			zorroPosY = zorroBaseY;

			rotPataDelDer = 0.0f; rotPataDelIzq = 0.0f;
			rotPataTrasDer = 0.0f; rotPataTrasIzq = 0.0f;
		}
	}
	else {
		rotPataDelDer = 0.0f; rotPataDelIzq = 0.0f;
		rotPataTrasDer = 0.0f; rotPataTrasIzq = 0.0f;
	}

	// === Animación continua de la bola orbitando al zorrito ===
	anguloBola += velocidadBola * deltaTime;

	if (anguloBola > 360.0f) {
		anguloBola -= 360.0f;
	}

	// === Lógica del Perro Rojo (Máquina de Estados) ===
	if (estadoActualPerro == CAMINANDO) {
		tiempoPerro += deltaTime;
		perroPosZ += perroVelocidad * deltaTime;

		if (perroPosZ > 6.0f) {
			perroPosZ = -8.0f;
		}

		perroRotPatas = sin(tiempoPerro * 8.0f) * 25.0f;

		perroRotCola = sin(tiempoPerro * 12.0f) * 20.0f;

		perroRotCabeza = sin(tiempoPerro * 2.0f) * 5.0f;
	}

}

// Is called whenever the mouse moves
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}


void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].gatoPosX - KeyFrame[playIndex].gatoPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].gatoPosY - KeyFrame[playIndex].gatoPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].gatoPosZ - KeyFrame[playIndex].gatoPosZ) / i_max_steps;
	KeyFrame[playIndex].headIncGato = (KeyFrame[playIndex + 1].headGato - KeyFrame[playIndex].headGato) / i_max_steps;
	KeyFrame[playIndex].FLegsIncGato = (KeyFrame[playIndex + 1].FLegsGato - KeyFrame[playIndex].FLegsGato) / i_max_steps;
	KeyFrame[playIndex].BLegsIncGato = (KeyFrame[playIndex + 1].BLegsGato - KeyFrame[playIndex].BLegsGato) / i_max_steps;
	KeyFrame[playIndex].tailIncGato = (KeyFrame[playIndex + 1].tailGato - KeyFrame[playIndex].tailGato) / i_max_steps;

	KeyFrame[playIndex].rotGatoInc = (KeyFrame[playIndex + 1].rotGato - KeyFrame[playIndex].rotGato) / i_max_steps;
	KeyFrame[playIndex].inclinacionIncGato = (KeyFrame[playIndex + 1].inclinacionGato - KeyFrame[playIndex].inclinacionGato) / i_max_steps;
}