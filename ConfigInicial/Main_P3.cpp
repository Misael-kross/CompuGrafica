// Practica 3 
// Misael Ivan Sosa Cortez
// 319033515
// Computacion Grafica G.1

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to init GLFW\n";
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 3_Misael Ivan Sosa Cortez", nullptr, nullptr);
    if (window == nullptr)
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
        glfwTerminate();
        return EXIT_FAILURE;
    }

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Cubo: 36 vértices, cada uno con (x,y,z,r,g,b)
    float vertices[] = {
        // Front (rojo)
        -0.5f,-0.5f, 0.5f, 1.0f,0.0f,0.0f,
         0.5f,-0.5f, 0.5f, 1.0f,0.0f,0.0f,
         0.5f, 0.5f, 0.5f, 1.0f,0.0f,0.0f,
         0.5f, 0.5f, 0.5f, 1.0f,0.0f,0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f,0.0f,0.0f,
        -0.5f,-0.5f, 0.5f, 1.0f,0.0f,0.0f,

        // Back (verde)
        -0.5f,-0.5f,-0.5f, 0.0f,1.0f,0.0f,
         0.5f,-0.5f,-0.5f, 0.0f,1.0f,0.0f,
         0.5f, 0.5f,-0.5f, 0.0f,1.0f,0.0f,
         0.5f, 0.5f,-0.5f, 0.0f,1.0f,0.0f,
        -0.5f, 0.5f,-0.5f, 0.0f,1.0f,0.0f,
        -0.5f,-0.5f,-0.5f, 0.0f,1.0f,0.0f,

        // Right (azul)
         0.5f,-0.5f, 0.5f, 0.0f,0.0f,1.0f,
         0.5f,-0.5f,-0.5f, 0.0f,0.0f,1.0f,
         0.5f, 0.5f,-0.5f, 0.0f,0.0f,1.0f,
         0.5f, 0.5f,-0.5f, 0.0f,0.0f,1.0f,
         0.5f, 0.5f, 0.5f, 0.0f,0.0f,1.0f,
         0.5f,-0.5f, 0.5f, 0.0f,0.0f,1.0f,

         // Left (amarillo)
         -0.5f, 0.5f, 0.5f, 1.0f,1.0f,0.0f,
         -0.5f, 0.5f,-0.5f, 1.0f,1.0f,0.0f,
         -0.5f,-0.5f,-0.5f, 1.0f,1.0f,0.0f,
         -0.5f,-0.5f,-0.5f, 1.0f,1.0f,0.0f,
         -0.5f,-0.5f, 0.5f, 1.0f,1.0f,0.0f,
         -0.5f, 0.5f, 0.5f, 1.0f,1.0f,0.0f,

         // Bottom (cian)
         -0.5f,-0.5f,-0.5f, 0.0f,1.0f,1.0f,
          0.5f,-0.5f,-0.5f, 0.0f,1.0f,1.0f,
          0.5f,-0.5f, 0.5f, 0.0f,1.0f,1.0f,
          0.5f,-0.5f, 0.5f, 0.0f,1.0f,1.0f,
         -0.5f,-0.5f, 0.5f, 0.0f,1.0f,1.0f,
         -0.5f,-0.5f,-0.5f, 0.0f,1.0f,1.0f,

         // Top (morado)
         -0.5f, 0.5f,-0.5f, 1.0f,0.2f,0.5f,
          0.5f, 0.5f,-0.5f, 1.0f,0.2f,0.5f,
          0.5f, 0.5f, 0.5f, 1.0f,0.2f,0.5f,
          0.5f, 0.5f, 0.5f, 1.0f,0.2f,0.5f,
         -0.5f, 0.5f, 0.5f, 1.0f,0.2f,0.5f,
         -0.5f, 0.5f,-0.5f, 1.0f,0.2f,0.5f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Projection (igual al final)
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f),
        (GLfloat)screenWidth / (GLfloat)screenHeight,
        0.1f, 200.0f);

    // ---------------------------------------------
    // “Serpiente” final: mismos valores del final
    // (posiciones, rotaciones Y y escalas)
    // ---------------------------------------------
    glm::vec3 pos[6] = {
        glm::vec3(0.2f, 0.35f,  0.0f),
        glm::vec3(0.6f, 1.15f, -0.25f),
        glm::vec3(1.0f, 2.35f, -0.50f),
        glm::vec3(1.35f, 4.00f, -0.75f),
        glm::vec3(1.70f, 6.30f, -1.05f),
        glm::vec3(2.10f, 9.55f, -1.40f)
    };

    float rotY[6] = { 15.0f, -20.0f, 30.0f, -10.0f, 25.0f, -35.0f };
    float scale[6] = { 0.7f,  1.0f,   1.4f,  2.0f,   2.7f,  3.6f };

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        // View (igual al final)
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(-4.0f, -5.5f, -15.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // Dibujo estilo intermedio: for + model a mano
        for (int i = 0; i < 6; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos[i]);
            model = glm::rotate(model, glm::radians(rotY[i]), glm::vec3(0.0f, 1.0f, 0.0f)); // SOLO Y
            model = glm::scale(model, glm::vec3(scale[i], scale[i], scale[i]));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return EXIT_SUCCESS;
}