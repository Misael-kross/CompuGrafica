// Practica 9
// Computacion Grafica e Interaccion Humano Computadora
// Grupo: 1
// Misael Ivan Sosa Cortez
// 319033515

#include <cmath>
#include <algorithm>

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

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 1.5f, 8.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024] = { false };
bool firstMouse = true;

// ---------------------------------------
// Luces
// ---------------------------------------
bool spotLightEnabled = true;

// Posiciones de las point lights
// Luz 1: al frente del perro y a ras de suelo
// Luz 2: posicion lateral/diagonal a ras de suelo
// Luz 3: elevada
// Luz 4: apagada
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, -0.3f, 1.0f),
	glm::vec3(2.1f, -0.3f, 1.2f),
	glm::vec3(-3.0f, 1.5f, 2.5f),
	glm::vec3(0.0f, 0.0f, 0.0f)
};

// Estado de cada luz
bool pointLightEnabled[3] = { true, true, true };

// Intensidad independiente
float pointLightIntensity[3] = { 1.2f, 1.0f, 1.4f };

// Color base de cada luz
glm::vec3 pointLightBaseColor[3] = {
	glm::vec3(1.0f, 0.65f, 0.25f),  // roja
	glm::vec3(1.0f, 1.0f, 1.0f),  // verde
	glm::vec3(1.0f, 1.0f, 1.0f)   // azul
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

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	// Init GLFW
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9_Misael Ivan Sosa Cortez", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	Model Dog((char*)"Models/RedDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Campfire((char*)"Models/Campfire/Campfire.obj");
	Model Lantern((char*)"Models/Old Lantern Iridesum/Old Lantern Model.obj");
	Model Moon((char*)"Models/Moon/Moon.obj");

	// Si luego quieres poner modelos sobre las fuentes, solo cambias nombres/rutas y descomentas Draw
	// Model LightModel1((char*)"Models/LightModel1.obj");
	// Model LightModel2((char*)"Models/LightModel2.obj");
	// Model LightModel3((char*)"Models/LightModel3.obj");

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(
		camera.GetZoom(),
		(GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
		0.1f,
		100.0f
	);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use corresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// ---------------------------------------
		// Directional light apagada
		// ---------------------------------------
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.0f, 0.0f, 0.0f);

		// ---------------------------------------
		// Point lights 1, 2 y 3
		// ---------------------------------------
		for (int i = 0; i < 3; i++)
		{
			glm::vec3 finalColor(0.0f);

			if (pointLightEnabled[i])
			{
				finalColor = pointLightBaseColor[i] * pointLightIntensity[i];
			}

			std::string index = "pointLights[" + std::to_string(i) + "].";

			glUniform3f(glGetUniformLocation(lightingShader.Program, (index + "position").c_str()),
				pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);

			glUniform3f(glGetUniformLocation(lightingShader.Program, (index + "ambient").c_str()),
				finalColor.x * 0.15f, finalColor.y * 0.15f, finalColor.z * 0.15f);

			glUniform3f(glGetUniformLocation(lightingShader.Program, (index + "diffuse").c_str()),
				finalColor.x, finalColor.y, finalColor.z);

			glUniform3f(glGetUniformLocation(lightingShader.Program, (index + "specular").c_str()),
				finalColor.x * 1.2f, finalColor.y * 1.2f, finalColor.z * 1.2f);

			glUniform1f(glGetUniformLocation(lightingShader.Program, (index + "constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (index + "linear").c_str()), 0.045f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (index + "quadratic").c_str()), 0.0075f);
		}

		// Point light 4 apagada
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"),
			pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);

		// ---------------------------------------
		// SpotLight / linterna de camara
		// Tecla L para prender/apagar
		// ---------------------------------------
		glm::vec3 spotAmbient(0.0f);
		glm::vec3 spotDiffuse(0.0f);
		glm::vec3 spotSpecular(0.0f);

		if (spotLightEnabled)
		{
			spotAmbient = glm::vec3(0.15f, 0.15f, 0.15f);
			spotDiffuse = glm::vec3(0.9f, 0.9f, 0.9f);
			spotSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
		}

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
			camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
			camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"),
			spotAmbient.x, spotAmbient.y, spotAmbient.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"),
			spotDiffuse.x, spotDiffuse.y, spotDiffuse.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),
			spotSpecular.x, spotSpecular.y, spotSpecular.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.0075f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(10.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(14.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1.0f);

		// ---------------------------------------
		// Piso
		// ---------------------------------------
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		// ---------------------------------------
		// Perro
		// ---------------------------------------
		model = glm::mat4(1.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Dog.Draw(lightingShader);

// Modelo de la luz 1: Campfire
// ---------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0055f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Campfire.Draw(lightingShader);

		// ---------------------------
		// Modelo de la luz 2: Lantern
		// ---------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[1]);
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lantern.Draw(lightingShader);

		// ---------------------------
		// Modelo de la luz 3: Moon
		// ---------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[2]);
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0035f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Moon.Draw(lightingShader);

		glBindVertexArray(0);

		// ---------------------------------------
		// Dibujar cubos de referencia de las luces
		// ---------------------------------------
		lampShader.Use();

		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//for (GLuint i = 0; i < 3; i++)
		//{
		//	model = glm::mat4(1.0f);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f));
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glBindVertexArray(VAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
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

	// Intensidad luz 1
	if (keys[GLFW_KEY_R])
	{
		pointLightIntensity[0] = std::min(pointLightIntensity[0] + 0.01f, 3.0f);
	}
	if (keys[GLFW_KEY_F])
	{
		pointLightIntensity[0] = std::max(pointLightIntensity[0] - 0.01f, 0.0f);
	}

	// Intensidad luz 2
	if (keys[GLFW_KEY_T])
	{
		pointLightIntensity[1] = std::min(pointLightIntensity[1] + 0.01f, 3.0f);
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightIntensity[1] = std::max(pointLightIntensity[1] - 0.01f, 0.0f);
	}

	// Intensidad luz 3
	if (keys[GLFW_KEY_Y])
	{
		pointLightIntensity[2] = std::min(pointLightIntensity[2] + 0.01f, 3.0f);
	}
	if (keys[GLFW_KEY_H])
	{
		pointLightIntensity[2] = std::max(pointLightIntensity[2] - 0.01f, 0.0f);
	}
}

// Is called whenever a key is pressed/released via GLFW
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

	// Toggles al presionar una vez
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_Z)
		{
			pointLightEnabled[0] = !pointLightEnabled[0];
		}

		if (key == GLFW_KEY_X)
		{
			pointLightEnabled[1] = !pointLightEnabled[1];
		}

		if (key == GLFW_KEY_C)
		{
			pointLightEnabled[2] = !pointLightEnabled[2];
		}

		if (key == GLFW_KEY_L)
		{
			spotLightEnabled = !spotLightEnabled;
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xPos;
		lastY = (GLfloat)yPos;
		firstMouse = false;
	}

	GLfloat xOffset = (GLfloat)xPos - lastX;
	GLfloat yOffset = lastY - (GLfloat)yPos;  // Reversed since y-coordinates go from bottom to top

	lastX = (GLfloat)xPos;
	lastY = (GLfloat)yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}