#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f
};
const GLfloat colors[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

const GLchar* vertex120 = R"END(
#version 120
attribute vec4 inColor;
attribute vec4 inPosition;
uniform mat4 matrix;
varying vec4 outColor;
void main()
{
    outColor = inColor;
    gl_Position = inPosition; // * matrix;
}
)END";

const GLchar* raster120 = R"END(
#version 120
varying vec4 outColor;
void main()
{
    gl_FragColor = outColor;
}
)END";

const int steps = 100;

int main(void) {
    GLFWwindow* window;

    if (!glfwInit()) {
        std::cout << "Init Error";
        exit(-1);
    }

    window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);

    if (!window) {
        std::cout << "Window creation error";
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    std::cout << "Init :: checking OpenGL version:\n";
    const unsigned char* msg;
    msg = glGetString(GL_VERSION);
    std::cout << msg << "\n Shader language version: \n";
    msg = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << msg << "\n";

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    std::cerr << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

    GLuint shaderVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderVertex, 1, &vertex120, 0);
    glCompileShader(shaderVertex);

    GLint compilationStatus;

    glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &compilationStatus);
    if (compilationStatus == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderVertex, sizeof(messages), 0, &messages[0]);
        std::cout << messages;
        exit(1);
    }

    GLuint shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderFragment, 1, &raster120, 0);
    glCompileShader(shaderFragment);

    glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &compilationStatus);
    if (compilationStatus == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderFragment, sizeof(messages), 0, &messages[0]);
        std::cout << messages;
        exit(1);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, shaderVertex);
    glAttachShader(shaderProgram, shaderFragment);
    glLinkProgram(shaderProgram);

    GLint linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(shaderProgram, sizeof(messages), 0, &messages[0]);
        std::cout << messages;
        exit(1);
    }

    glUseProgram(shaderProgram);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    GLuint colorsBuffer;
    glGenBuffers(1, &colorsBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLint attribPosition = glGetAttribLocation(shaderProgram, "inPosition");
    glEnableVertexAttribArray(attribPosition);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLint attribColor = glGetAttribLocation(shaderProgram, "inColor");
    glEnableVertexAttribArray(attribColor);
    glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer);
    glVertexAttribPointer(attribColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void drawCircle(float red, float green, float blue)
{
    float radius = 1.;
    const float angle = 3.1415926 * 2 / steps;
    float oldX = 0; float oldY = 1;
    for (int i = 0; i <= steps; i++) {
        float newX = radius * sin(angle * i);
        float newY = -radius * cos(angle * i);

        glColor3f(red, green, blue);
        glBegin(GL_TRIANGLES);
        glVertex3f(0.f, 0.f, 0.f);
        glVertex3f(oldX, oldY, 0.f);
        glVertex3f(newX, newY, 0.f);
        glEnd();

        oldX = newX;
        oldY = newY;
    }
}


