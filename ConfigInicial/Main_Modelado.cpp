//Practica 4
//Computacion Grafica eInteraccion Humano Computadora
//Grupo:1
//Misael Ivan Sosa Cortez
//319033515

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

void Inputs(GLFWwindow* window);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -14.0f;   // alejado para ver el bloque completo
float rot = 0.0f;

// ---------- Helper voxel ----------
struct Voxel {
    glm::vec3 p;     // posición en “grid units”
    glm::vec3 s;     // escala (normalmente 1,1,1)
    glm::vec3 color; // color del cubo
};

static void DrawCubeVoxel(const Voxel& v, float grid,
    GLint modelLoc, GLint colorLoc)
{
    // translate primero, scale después (para que la traslación NO se escale)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, v.p * grid);
    model = glm::scale(model, v.s);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(v.color));
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4_Misael Ivan Sosa Cortez", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialise GLEW\n";
        return EXIT_FAILURE;
    }

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Cubo base (solo posiciones)
    float vertices[] = {
        // Front
        -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,-0.5f, 0.5f,
         // Back
         -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
          0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
          // Right
           0.5f,-0.5f, 0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
           0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,  0.5f,-0.5f, 0.5f,
           // Left
           -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f, -0.5f,-0.5f,-0.5f,
           -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
           // Bottom
           -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,
            0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f, -0.5f,-0.5f,-0.5f,
            // Top
            -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,
             0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f),
        (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

    // Colores
    glm::vec3 rosaClaro(0.95f, 0.84f, 0.82f);
    glm::vec3 rosaFuerte(0.89f, 0.35f, 0.63f);
    glm::vec3 negro(0.0f, 0.0f, 0.0f);

    // VOXELS:CABEZA 
    const float GRID = 1.05f;

    std::vector<Voxel> head;
    std::vector<Voxel> body;

    float headBaseY = 3.0f;   
    float headZ = 0.0f;       

    for (int iy = 0; iy < 3; iy++)
    {
        for (int iz = -1; iz <= 1; iz++)
        {
            for (int ix = -2; ix <= 2; ix++)
            {
                head.push_back({
                    glm::vec3((float)ix, headBaseY + (float)iy, headZ + (float)iz),
                    glm::vec3(1.0f, 1.0f, 1.0f),
                    rosaClaro
                    });
            }
        }
    }

// NARIZ 

    float noseY = headBaseY + 1.0f;   
    float noseZ = 2.0f;               

    head.push_back({ glm::vec3(0.0f, noseY, noseZ), glm::vec3(1.0f), rosaFuerte });

// OJOS NEGROS

    float eyeY = headBaseY + 1.0f;
    float eyeZ = 2.0f;  

    head.push_back({ glm::vec3(-2.0f, eyeY, eyeZ), glm::vec3(1.0f), negro });
    head.push_back({ glm::vec3(2.0f, eyeY, eyeZ), glm::vec3(1.0f), negro });

// CUERPO BEIGE

    float bodyTopY = headBaseY - 1.0f;  
    float bodyZ = 0.0f;                

    for (int iy = 0; iy < 5; iy++)        
    {
        for (int ix = -1; ix <= 1; ix++)      
        {
            body.push_back({
                glm::vec3((float)ix, bodyTopY - (float)iy, bodyZ),
                glm::vec3(1.0f, 1.0f, 1.0f),
                rosaClaro
                });
        }
    }
   
// Patas
    float zFront = 1.0f;

    float yBottom = bodyTopY - 4.0f;  
    float ySecond = bodyTopY - 3.0f;   

    body.push_back({ glm::vec3(-1.0f, yBottom, zFront), glm::vec3(1.0f), rosaClaro });
    body.push_back({ glm::vec3(1.0f, yBottom, zFront), glm::vec3(1.0f), rosaClaro });
    body.push_back({ glm::vec3(-1.0f, yBottom, zFront), glm::vec3(1.0f), rosaClaro });
    body.push_back({ glm::vec3(1.0f, yBottom, zFront), glm::vec3(1.0f), rosaClaro });

    body.push_back({ glm::vec3(-1.0f, yBottom + 0.0f, zFront + 1.0f), glm::vec3(1.0f), rosaFuerte });
    body.push_back({ glm::vec3(1.0f, yBottom + 0.0f, zFront + 1.0f), glm::vec3(1.0f), rosaFuerte });
  
    // BRAZOS
  
    float armY = bodyTopY - 1.0f;
    float armZ = 0.0f;

    // Izquierda

    body.push_back({ glm::vec3(-2.0f, armY, armZ), glm::vec3(1.0f), rosaClaro });
    body.push_back({ glm::vec3(-3.0f, armY, armZ), glm::vec3(1.0f), rosaFuerte });

    // Derecha

    body.push_back({ glm::vec3(2.0f, armY, armZ), glm::vec3(1.0f), rosaClaro });
    body.push_back({ glm::vec3(3.0f, armY, armZ), glm::vec3(1.0f), rosaFuerte });
 
//REJAS SUPERIORES
    float yTop = headBaseY + 2.0f;   
    float hornZ = 0.0f;             

    // LADO IZQUIERDO
    head.push_back({ glm::vec3(-2.0f, yTop + 1.0f, hornZ), glm::vec3(1.0f), rosaFuerte });
    head.push_back({ glm::vec3(-2.0f, yTop + 2.0f, hornZ), glm::vec3(1.0f), rosaFuerte });
    head.push_back({ glm::vec3(-1.0f, yTop + 1.0f, hornZ), glm::vec3(1.0f), rosaFuerte });


    // LADO DERECHO
    head.push_back({ glm::vec3(2.0f, yTop + 1.0f, hornZ), glm::vec3(1.0f), rosaFuerte });
    head.push_back({ glm::vec3(2.0f, yTop + 2.0f, hornZ), glm::vec3(1.0f), rosaFuerte });
    head.push_back({ glm::vec3(1.0f, yTop + 1.0f, hornZ), glm::vec3(1.0f), rosaFuerte });

// BRANQUIAS LATERALES
    float y0 = headBaseY + 0.0f;
    float y1 = headBaseY + 1.0f;
    float y2 = headBaseY + 2.0f;

    float zSide = 1.0f;

    // IZQUIERDA
    head.push_back({ glm::vec3(-4.0f, y2, zSide), glm::vec3(1.0f), rosaFuerte }); // arriba-afuera
    head.push_back({ glm::vec3(-3.0f, y1, zSide), glm::vec3(1.0f), rosaFuerte }); // medio-pegado
    head.push_back({ glm::vec3(-4.0f, y1, zSide), glm::vec3(1.0f), rosaFuerte }); // medio-afuera
    head.push_back({ glm::vec3(-3.0f, y0, zSide), glm::vec3(1.0f), rosaFuerte }); // abajo-pegado

    // DERECHA
    head.push_back({ glm::vec3(4.0f, y2, zSide), glm::vec3(1.0f), rosaFuerte }); // arriba-afuera
    head.push_back({ glm::vec3(3.0f, y1, zSide), glm::vec3(1.0f), rosaFuerte }); // medio-pegado
    head.push_back({ glm::vec3(4.0f, y1, zSide), glm::vec3(1.0f), rosaFuerte }); // medio-afuera
    head.push_back({ glm::vec3(3.0f, y0, zSide), glm::vec3(1.0f), rosaFuerte }); // abajo-pegado
    // ---------------- Render loop ----------------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Inputs(window);
        glfwPollEvents();

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0, 1, 0));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint colorLoc = glGetUniformLocation(ourShader.Program, "objectColor");

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // Dibuja SOLO la cabeza
        for (const auto& v : head)
            DrawCubeVoxel(v, GRID, modelLoc, colorLoc);
        for (const auto& v : body)
            DrawCubeVoxel(v, GRID, modelLoc, colorLoc);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window)
{
    float speed = 6.0f * deltaTime;
    float rotSpeed = 90.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movX += speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movX -= speed;

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)   movY += speed;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) movY -= speed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movZ -= speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movZ += speed;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rot += rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  rot -= rotSpeed;
}