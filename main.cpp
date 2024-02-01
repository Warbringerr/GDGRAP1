#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
float x_mod = 0;
float y_mod = 0;
float theta_mod = 0;
float scale_mod = 1;
float z_mod = -2;


void Key_Callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
    if (key == GLFW_KEY_D &&
        action == GLFW_PRESS) {
        //move bunny to right
        x_mod += 0.01f;
    }

    if (key == GLFW_KEY_A &&
        action == GLFW_PRESS) {
        //move bunny to left
        x_mod += -0.01f;
    }

    if (key == GLFW_KEY_S &&
        action == GLFW_PRESS) {
        //move bunny to down
        y_mod += -0.01f;
    }

    if (key == GLFW_KEY_W &&
        action == GLFW_PRESS) {
        //move bunny to down
        y_mod += 0.01f;
    }
    if (key == GLFW_KEY_Y) {
        theta_mod += 2.0f;
    }
    if (key == GLFW_KEY_X &&
        action == GLFW_PRESS) {
        //move bunny to down
        scale_mod += 0.01f;
    }
    if (key == GLFW_KEY_Z)
    {
        z_mod += 0.1;
    }
    
}
int main(void)
{
    float x=0, y=0, z=0, scale_x=1, scale_y=1, scale_z=1,theta=90,axis_x=0,axis_y=1,axis_z=0;
    float window_height = 600.0f;
    float window_width = 600.0f;
    glm::mat4 identity_matrix=glm::mat4(1.0f);
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "Ignite the drive drive in your soul", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();
    
    //glViewport(0,0, 300,600);//temp renderr forcing

    glfwSetKeyCallback(window, Key_Callback);
    std::fstream vertSrc("Shaders/sample.vert");
     std::stringstream vertBuff;

     vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;

    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();
    //Create a vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v , NULL);
    glCompileShader(vertexShader);


//error checker
    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
        std::cout << &errorLog[0];
        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        //glDeleteShader(vertexShader); // Don't leak the shader.
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);


    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(fragShader, maxLength, &maxLength, &errorLog[0]);
        std::cout << &errorLog[0];
        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
       // glDeleteShader(fragShader); // Don't leak the shader.
    }

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);



    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t>shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }
    GLfloat vertices[]{
        //x  y   z
        0.f,0.5f,0.f,
        -0.5f, -0.5f, 0.f,
        0.5f,-0.5f ,0.f
    };

    GLuint indices[]{
        0,1,2
    };
    //create VAO / VBO Variables
    GLuint VAO, VBO, EBO;
    //Initialize the VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Currently Editing VAO = null
    glBindVertexArray(VAO);
    //Currently editing VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Currently editing VBO = VBO

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT)* attributes.vertices.size(),//bytes
        attributes.vertices.data(),// array
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        3, // x y z
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint)*mesh_indices.size(),
       mesh_indices.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    //Currently editing VBO = VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Currently editing VBO = null 
    
    //Currently editing VAO = VAO
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
   /* glm::mat4 projectionMatrix = glm::ortho(-2.0f,
        2.0f,
        -2.0f,
        2.0f,
        -1.f,
        1.0f);*/
   
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.f),
        window_height / window_width,
        0.f,
        100.f
    );
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        /*simple animation
       // x_mod += 0.001f;
       // y_mod += 0.001f;

       x=x_mod

        //get the address of variable x in the shader
        unsigned int xLoc = glGetUniformLocation(shaderProg, "x");
        //assign x by using its address
        glUniform1f(xLoc, x_mod);

        //get the address of variable x in the shader
        unsigned int yLoc = glGetUniformLocation(shaderProg, "y");
        //assign x by using its address
        glUniform1f(yLoc, y_mod);
        */
        //----------------------------------------------------------------------------------------------------------------
        x = x_mod;
        y = y_mod;
        theta = theta_mod;
        scale_x = scale_mod;
        z = z_mod;

        

        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix,
            glm::vec3(x, y, z)
        );

        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
        );


        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(transformation_matrix));
        glUseProgram(shaderProg);
        
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        
        
        //POLYGAN
        /*
        glBegin(GL_POLYGON); 
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //glColor3f(1, 0, 0); // red

        glVertex2f(0, 1);


        glVertex2f(-0.9511, 0.3090);

        //glColor3f(0, 0, 9);

        glVertex2f(-0.5878, -0.8090);

        //glColor3f(9, 5, 1);

        glVertex2f(0.5878, -0.8090);

        //glColor3f(8, 4, 2);

        glVertex2f(0.9511, 0.3090);

        glEnd();
        */
        /* (not included)
        glBegin(GL_POLYGON);

        glVertex2f(1,0);
        glVertex2f(0.309,0.951);
        glVertex2f(-0.809, 0.587);
        glVertex2f(-0.809, -0.587);
        glVertex2f(0.309, -0.951);

        glEnd();
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


/*std::fstream vertSrc("Shaders/sample.vert)
std::stringstream vertBuff;        

        std::string vertS=vertBuff.str();
        const char* v = vertS.c_str();

std::fstream fragSrc("Shaders/sample.vert)
std::stringstream fragBuff;

fragBuff<<fragSrc.rdbuf();

        std::string fragS=fragBuff.str();
        const char* f = fragS.c_str();
        */
