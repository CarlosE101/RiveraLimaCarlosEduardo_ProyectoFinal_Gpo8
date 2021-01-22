// El siguiente Código se fundamente en la experiencia del ING. CARLOS ALDAIR ROMAN BALBUENA
//Y la implemtanción del ALUMNO: RIVERA LIMA CARLOS EDUARDO


#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

void DoMovementP();//Personaje
//void DoMovement1();//mariposa
void animacion();
void animacion1();//mariposa
void porton();//PUERTA

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-100.0f, 2.0f, -45.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
//glm::vec3 PosIni(-95.0f, 0.0f, -45.0f);
glm::vec3 PosIni(-95.0f, 1.0f, -45.0f);//puerta
glm::vec3 PosIni1(-95.0f, 0.0f, -45.0f);//para la mariposa
bool active;



//Animación del coche
float movKitX = 0.0;
float movKitZ = 0.0;
float rotKit = 0.0;

float movKitX1 = 0.0;//mariposa
float movKitZ1 = 0.0;//mariposa
float rotKit1 = 0.0;//mariposa

bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;


bool circuito1 = false;//mariposa
bool recorrido11 = true;//mariposa
bool recorrido21 = false;//mariposa
bool recorrido31 = false;//mariposa
bool recorrido41 = false;//mariposa
bool recorrido51 = false;//mariposa
bool puerta = false;//puerta

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


//Animacion SOY UN BOBOT
// Keyframes															profe 
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotRodIzq = 0, rotRodDer, rotBraDer = 0, rotBraIzq = 0;//ya con brazos
#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotRodDer;//profe
	float rotInc;
	float rotInc2;//profe
	float rotBraIzq;//brazo
	float rotBraDer;//brazo
	float rotIncDer;//brazoinc
	float rotIncBraIzq;//brazoinc
	float rotIncBraDer;//brazoinc

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	//Personaje
	glm::vec3(posX,posY,posZ),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),

};

glm::vec3 LightP1;




float rotPuerta = 0;//puerta

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto_Final", nullptr, nullptr);

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
	printf("%f", glfwGetTime());

	// GLFW Options
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

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	Model Carroseria((char*)"Models/Carro/12150_Christmas_Tree_V2_L2.obj");
	Model Piso((char*)"Models/Carro/Piso.obj");
	Model Cabana((char*)"Models/Carro/WoodenCabinObj.obj");
	Model Tortuga((char*)"Models/Carro/10042_Sea_Turtle_V2_iterations-2.obj");
	Model Muneco((char*)"Models/Carro/LowResOBjNightMare.obj");
	Model Boat((char*)"Models/Carro/boat.obj");
	Model Sofa((char*)"Models/Carro/sofa.obj");
	Model Comedor((char*)"Models/Carro/3dstylish-fdb001.obj");
	Model Estufa((char*)"Models/Carro/fkc.obj");
	Model Refri((char*)"Models/Carro/B_Daily_R_St_N_Electric_0002.obj");
	Model Arbusto((char*)"Models/Carro/Low Grass.obj");
	Model Bocho((char*)"Models/Carro/bocho.obj");
	Model Bano((char*)"Models/Carro/Toilet.obj");
	Model CuartoBano((char*)"Models/Carro/banocomplete.obj");
	Model Banera((char*)"Models/Carro/bath_obj.obj");
	Model Cama((char*)"Models/Carro/Old_bed.obj");
	Model Camacom((char*)"Models/Carro/almandcobi.obj");
	Model Arbolamarillo((char*)"Models/Carro/cgaxis_models_115_37_obj.obj");
	Model Tronco((char*)"Models/Carro/TREE_TRUNK_26_10K.obj");
	Model Muelle((char*)"Models/Carro/woodenpier.obj");
	Model Arbolnavi((char*)"Models/Carro/12150_Christmas_Tree_V2_L2.obj");
	
	//Animaciones
	Model Mariposa((char*)"Models/Carro/butterfly.obj");
	Model Carusel((char*)"Models/Carro/carusel_caballos.obj");
	Model Carusel_base((char*)"Models/Carro/carousel_base.obj");
	Model Rueda_Molino((char*)"Models/Carro/rueda_molino.obj");
	Model Torre_Molino((char*)"Models/Carro/torre_molino.obj");
	Model Marco_Puerta((char*)"Models/Carro/Marco_Puerta.obj");
	Model puerta((char*)"Models/Carro/Puerta_sola.obj");
	
	//////Personaje Soy un BOBOT
	Model BotaDer((char*)"Models/Carro/bota.obj");
	Model PiernaDer((char*)"Models/Carro/piernader.obj");
	Model PiernaIzq((char*)"Models/Carro/piernaizq.obj");
	Model Torso((char*)"Models/Carro/torso.obj");
	Model BrazoDer((char*)"Models/Carro/brazoder.obj");
	Model BrazoIzq((char*)"Models/Carro/brazoizq.obj");
	Model Cabeza((char*)"Models/Carro/bombman.obj");
	
	// Build and compile our shader program

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
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

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Load textures
	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	image = stbi_load("images/TexturesCom_GravelCobble0019_7_S.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	// Specular map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/piso.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		DoMovementP();//personaje
		animacion();
		animacion1();//mariposa
		porton();//puerta

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// Point light 5 Mariposa//////////////////////////////////////
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

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

		// Bind diffuse map
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		glm::mat4 model(1);


		//Animaciones----------------------------------------------------------------

		//Carga de modelo 
		//tortuga Completa
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-56.0f, 0.0f, 11.5f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Tortuga.Draw(lightingShader);

		
		//Carga de modelo 
		//Marco_Puerta
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
																//19 adelanta al frente
		model = glm::translate(model, glm::vec3(-23.6f, 1.0f, 18.4f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.38f, 0.34f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Marco_Puerta.Draw(lightingShader);


		//Carga de modelo 
		//Puerta
		glm::mat4 tmp = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		model = glm::mat4(1.0);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
																
		//model = glm::translate(model, glm::vec3(-23.6f, 1.0f, 18.4f));
		//model = glm::translate(tmp, glm::vec3(4195.0f, 895.0f, -2012.f));
		
		model = glm::translate(tmp, glm::vec3(-23.6f, 1.0f, 18.4f));
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-0.18f, 0.0f, 0.7f));
		model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.38f, 0.34f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		puerta.Draw(lightingShader);
		


		//Carga de modelo 
		//boat
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX, -1.0f, movKitZ));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(-76.0f, 0.1f, -11.5f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Boat.Draw(lightingShader);


		//Carga de modelo 
		//Mariposa
		view = camera.GetViewMatrix();
		model = glm::mat4(1);                                     //altura
		model = glm::translate(model, PosIni1 + glm::vec3(movKitX1, 4.0f, movKitZ1));
		model = glm::rotate(model, glm::radians(rotKit1), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotKit1), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Mariposa.Draw(lightingShader);


		//Carga de modelo 
		//Carusel solo con caballos 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);                                     //altura
		model = glm::translate(model, glm::vec3(-32.0f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(rotKit1), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));//rotación de rosa
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Carusel.Draw(lightingShader);


		//Carga de modelo 
	   //Carusel solo su base 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);                                     //altura
		model = glm::translate(model, glm::vec3(-32.0f, 0.0f, -5.0f));
		//model = glm::rotate(model, glm::radians(rotKit1), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));//rotación de rosa
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Carusel_base.Draw(lightingShader);

		//Carga de modelo 
		//Rueda de Molino
		view = camera.GetViewMatrix();
		model = glm::mat4(1);                                     //altura
		model = glm::translate(model, glm::vec3(-21.0f, 35.0f, -20.0f));
		//model = glm::rotate(model, glm::radians(rotKit1), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));//rotación de rosa
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Rueda_Molino.Draw(lightingShader);



		//Carga de modelo 
		//Torre de Molino
		view = camera.GetViewMatrix();
		model = glm::mat4(1);                                     //altura
		model = glm::translate(model, glm::vec3(-21.0f, 0.0f, -20.0f));
		//model = glm::rotate(model, glm::radians(rotKit1), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Torre_Molino.Draw(lightingShader);

		//glBindVertexArray(0);

		////////////Personaje SOY UN BOBOT/////Animación

		//Brazo derecho
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY=4.0f, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(-rotBraDer), glm::vec3(1.0f, 0.0f, 0.0f));//brazo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoDer.Draw(lightingShader);

		//Brazo Izquierdo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY = 4.0f, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(-rotBraIzq), glm::vec3(1.0f, 0.0f, 0.0f));//brazo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoIzq.Draw(lightingShader);

		//Cabeza
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY = 3.0f, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cabeza.Draw(lightingShader);


		glBindVertexArray(0);

		///////////////////////////////////fin de animaciones///////////////////////////////////


/////////////////////////////////////////////Modelo sin animación/////////////////////////////////////////////////////////
		////Carga de modelo 
		////Muelle
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(14.2f, -1.3f, 61.7f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Muelle.Draw(lightingShader);


		//Carga de modelo 
		//Tronco 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -17.7f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Tronco.Draw(lightingShader);


		//Carga de modelo 
		//Tronco 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -37.7f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Tronco.Draw(lightingShader);


		//Carga de modelo 
		//cabaña
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-22.0f, 0.0f, 11.5f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Cabana.Draw(lightingShader);

		//Carga de modelo 
		//Arbol navideño
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-20.0f, 1.05f, 16.5f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolnavi.Draw(lightingShader);


		//Carga de modelo 
		//muneco de nieve
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-46.0f, 0.1f, 11.5f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.004f, 0.004f, 0.004f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Muneco.Draw(lightingShader);



		//Carga de modelo 
		//Sofa
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-23.7f, 2.1f, 6.1f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.016f, 0.016f, 0.016f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Sofa.Draw(lightingShader);


		//Carga de modelo 
		//Comedor
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-20.0f, 1.12f, 13.3f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Comedor.Draw(lightingShader);

		//Carga de modelo 
	   //Estufa
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-18.5f, 1.13f, 9.3f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Estufa.Draw(lightingShader);

		//Carga de modelo 
	   //Refrigerador 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-21.5f, 1.13f, 5.7f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Refri.Draw(lightingShader);

		//Carga de modelo 
		//arbusto
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-16.2f, 0.0f, 5.7f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbusto.Draw(lightingShader);

		//Carga de modelo 
		//arbusto
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-14.2f, 0.0f, 5.7f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbusto.Draw(lightingShader);


		//Carga de modelo 
		//arbusto
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-12.2f, 0.0f, 5.7f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbusto.Draw(lightingShader);

		//Carga de modelo 
		//arbusto
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-10.2f, 0.0f, 5.7f));;
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbusto.Draw(lightingShader);

		//Carga de modelo 
		//arbusto
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-8.2f, 0.0f, 5.7f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbusto.Draw(lightingShader);


		//Carga de modelo 
		//arbusto
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-6.2f, 0.0f, 5.7f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbusto.Draw(lightingShader);


		//Carga de modelo 
		//arbusto
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-4.2f, 0.0f, 5.7f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbusto.Draw(lightingShader);


		//Carga de modelo 
		//arbusto
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-2.2f, 0.0f, 5.7f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbusto.Draw(lightingShader);


		//Carga de modelo 
		//bocho
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-8.2f, 0.0f, 7.7f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Bocho.Draw(lightingShader);

		//Carga de modelo 
		//baño
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-27.5f, 0.0f, 6.7f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Bano.Draw(lightingShader);

		//Carga de modelo 
		//Cuartobaño
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-27.2f, 0.0f, 7.7f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.5f, 1.9f, 1.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		CuartoBano.Draw(lightingShader);


		//Carga de modelo 
		//Bañera
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-29.0f, 0.0f, 9.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Banera.Draw(lightingShader);


		//Carga de modelo 
		//Cobija y almohada
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-24.0f, 2.06f, 9.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Camacom.Draw(lightingShader);




		//Carga de modelo 
		//Cama
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-24.0f, 1.1f, 9.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Cama.Draw(lightingShader);



		//Carga de modelo 
		//Arbol amarillo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-84.0f, 0.0f, 25.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolamarillo.Draw(lightingShader);



		//Carga de modelo 
		//Arbol amarillo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-84.0f, 0.0f, 35.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolamarillo.Draw(lightingShader);


		//Carga de modelo 
		//Arbol amarillo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-84.0f, 0.0f, 45.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolamarillo.Draw(lightingShader);


		//Carga de modelo 
		//Arbol amarillo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-94.0f, 0.0f, 45.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolamarillo.Draw(lightingShader);

		//Carga de modelo 
		//Arbol amarillo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-94.0f, 0.0f, 25.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolamarillo.Draw(lightingShader);

		//Carga de modelo 
		//Arbol amarillo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-74.0f, 0.0f, 45.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolamarillo.Draw(lightingShader);


		//Carga de modelo 
		//Arbol amarillo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-64.0f, 0.0f, 25.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolamarillo.Draw(lightingShader);


		//Carga de modelo 
		//Arbol amarillo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-44.0f, 0.0f, 25.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolamarillo.Draw(lightingShader);

		//Carga de modelo 
		//Arbol amarillo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-34.0f, 0.0f, 25.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolamarillo.Draw(lightingShader);


		//Carga de modelo 
		//Arbol amarillo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-34.0f, 0.0f, 35.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Arbolamarillo.Draw(lightingShader);

		//Carga de modelo 
		//Tronco 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-74.0f, 0.0f, 25.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Tronco.Draw(lightingShader);


		//Carga de modelo 
		//Tronco 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-14.0f, 0.0f, 25.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Tronco.Draw(lightingShader);

		//Carga de modelo 
		//Tronco 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, PosIni + glm::vec3(movKitX, 0, movKitZ));
		model = glm::translate(model, glm::vec3(-44.0f, 0.0f, 45.5f));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Tronco.Draw(lightingShader);


		//////////////////////////////Terreno de la granja(Piso)/////////////////////////////////////////////////////////////////////////////////Inicio

		//Piso
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);



		glBindVertexArray(0);

		//////////////////////////////Terreno de la granja(Piso)///////////////////////////////////////////////////////////////////////////////Fin


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}

/////////////////////////////Control de las teclas////////////////////////////////////////////////Inicio
// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{
		range += 0.1;
		rot += 1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_2])
	{
		range -= 0.1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_3])
	{
		range += 0.1;
		printf("El spotangle es %f\n", range);
	}

	if (keys[GLFW_KEY_4])
	{
		range -= 0.1;
		printf("El spotangle es %f\n", range);
	}

	// Camera controls
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

	if (keys[GLFW_KEY_T])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z += 0.1f;
	}
	if (keys[GLFW_KEY_G])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z -= 0.1f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[1].x += 0.1f;
		pointLightPositions[1].y += 0.1f;
		pointLightPositions[1].z += 0.1f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[1].x -= 0.1f;
		pointLightPositions[1].y -= 0.1f;
		pointLightPositions[1].z -= 0.1f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[2].x += 0.1f;
		pointLightPositions[2].y += 0.1f;
		pointLightPositions[2].z += 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[2].x -= 0.1f;
		pointLightPositions[2].y -= 0.1f;
		pointLightPositions[2].z -= 0.1f;
	}
	if (keys[GLFW_KEY_I])
	{
		pointLightPositions[3].x += 0.1f;
		pointLightPositions[3].y += 0.1f;
		pointLightPositions[3].z += 0.1f;
		circuito = true;
	}

	if (keys[GLFW_KEY_O])
	{
		circuito = false;
	}
	if (keys[GLFW_KEY_K])
	{
		pointLightPositions[3].x -= 0.1f;
		pointLightPositions[3].y -= 0.1f;
		pointLightPositions[3].z -= 0.1f;
	}

	if (keys[GLFW_KEY_L])//puerta
	{
		//puerta

		puerta = true;

	}

	if (keys[GLFW_KEY_K])//puerta
	{
		puerta = false;
	}


	///////nuevas teclas


	if (keys[GLFW_KEY_B])//mariposa
	{
		pointLightPositions[3].x += 0.1f;//mariposa
		pointLightPositions[3].y += 0.1f;//mariposa
		pointLightPositions[3].z += 0.1f;//mariposa
		circuito1 = true;//mariposa
	}

	if (keys[GLFW_KEY_N])//mariposa
	{
		circuito1 = false;//mariposa
	}
	if (keys[GLFW_KEY_M])//mariposa
	{
		pointLightPositions[3].x -= 0.1f;//mariposa
		pointLightPositions[3].y -= 0.1f;//mariposa
		pointLightPositions[3].z -= 0.1f;//mariposa
	}


}

/////////////////////////////Control de las teclas////////////////////////////////////////////////FIN


//////////////Código del profe//Recorrido de la Mariposa
void animacion1()
{

	//Movimiento del coche
	if (circuito1)
	{
		if (recorrido11)
		{
			movKitZ1 += 0.1f;
			if (movKitZ1 > 90)
			{
				recorrido11 = false;
				recorrido21 = true;
			}
		}
		if (recorrido21)
		{
			rotKit1 = 90;
			movKitX1 += 0.1f;
			if (movKitX1 > 90)
			{
				recorrido21 = false;
				recorrido31 = true;

			}
		}

		if (recorrido31)
		{
			rotKit1 = 180;
			movKitZ1 -= 0.1f;
			if (movKitZ1 < 0)
			{
				recorrido31 = false;
				recorrido41 = true;
			}
		}

		if (recorrido41)
		{
			rotKit1 = 270;
			movKitX1 -= 0.1f;
			if (movKitX1 < 0)
			{
				recorrido41 = false;
				recorrido51 = true;
			}
		}
		if (recorrido51)
		{
			rotKit1 = 0;
			movKitZ1 += 0.1f;
			if (movKitZ1 > 0)
			{
				recorrido51 = false;
				recorrido11 = true;
			}
		}


	}
}

/////////////////////////////////////////////////////////////////////////////////recorrido con pendiente BOTE///////////////////Inicio


void animacion()
{
	if (circuito) {

		if (recorrido1)
		{
			//rotKit = 90;//original
			rotKit = 0;//rotación inicial//antes 90
			movKitX += 0.1f;
			//if (movKitX > 90)//original
			if (movKitX > 45)//mantiene el sentido del plano
			{
				recorrido1 = false;
				recorrido2 = true;
			}

		}

		if (recorrido2)
		{
			//rotKit = -45;//original
			rotKit = -45;
			movKitX -= 0.1f;
			movKitZ += 0.1f;
			//if (movKitZ > 90)//original
			if (movKitZ > 45)//permitre regresar al origen
			{
				recorrido2 = false;
				recorrido3 = true;
			}

		}

		if (recorrido3)
		{
			rotKit = -180;
			movKitZ -= 0.1f;
			//if (movKitZ < 0)//original
			if (movKitZ < 0)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4)
		{
			rotKit = 90;
			movKitZ += 0.1f;
			if (movKitZ > 0)
			{
				recorrido4 = false;
				recorrido1 = true;
			}
		}

	}
}


/////////////////////////////////////////////////////////////////////////Fin recorrido con pendiente
/////////////////////////////////////////////////////////////////////////////////recorrido con pendiente BOTE///////////////////FIN

/////////////Animación PUERTA//////Inicio
void porton() {

	if (puerta) {

		if (rotPuerta < 416)//336

			for (int i = 0; i <= 198; i++)//335
				rotPuerta += 5.0f;


	}
	else

		rotPuerta = 0;




}
///FIN DE PUERTA


/////////////Animación PUERTA//////FIN

///Animación SOY UN BOBOT(HOMBRE BOMBA)///////////Inicio

// Moves / alters the camera positions based on user input
void DoMovementP()
{

	if (keys[GLFW_KEY_1])
	{

		rot += 1;

	}

	if (keys[GLFW_KEY_2])
	{
		if (rotRodIzq < 80.0f)
			rotRodIzq += 1.0f;

	}

	if (keys[GLFW_KEY_3])
	{
		if (rotRodIzq > -45)
			rotRodIzq -= 1.0f;

	}

	if (keys[GLFW_KEY_4])//profe
	{
		if (rotRodDer < 80.0f)
			rotRodDer += 1.0f;

	}

	if (keys[GLFW_KEY_5])//profe
	{
		if (rotRodDer > -45)
			rotRodDer -= 1.0f;

	}

	//brazo izquierdo
	if (keys[GLFW_KEY_6])
	{
		if (rotBraIzq < 90.0f)
			rotBraIzq += 1.0f;

	}

	if (keys[GLFW_KEY_7])
	{
		if (rotBraIzq > -60)
			rotBraIzq -= 1.0f;

	}
	//brazo derecho
	if (keys[GLFW_KEY_8])
	{
		if (rotBraDer < 90.0f)
			rotBraDer += 1.0f;

	}

	if (keys[GLFW_KEY_9])
	{
		if (rotBraDer > -60)
			rotBraDer -= 1.0f;

	}

	//Mov Personaje
	if (keys[GLFW_KEY_H])
	{
		posZ += 1;
	}

	if (keys[GLFW_KEY_Y])
	{
		posZ -= 1;
	}

	if (keys[GLFW_KEY_G])
	{
		posX -= 1;
	}

	if (keys[GLFW_KEY_J])
	{
		posX += 1;
	}


	///Animación SOY UN BOBOT(HOMBRE BOMBA)///////////Inicio

	// Camera controls
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



// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
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

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
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


