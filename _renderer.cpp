#include "_renderer.h"
#include <tools.h>
#include <iostream>
/*
 * The Renderer class
 * To create an abstraction for randering data
 * Extends the _glwidget class, will be included in the _glwidget class.
 * Created: 8_02_2019
 * Author: Aditya,Saurabh
*/

/* Constructor for the renderer class
 * The "QOpenGLExtraFunctions(QOpenGLContext::currentContext())" is passed by parameter
 * to avoid using initialiseopenglfunction() in the initcallback.
 * Create:11_02_2019
 */
_Renderer::_Renderer() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{

}
_Renderer::~_Renderer(){}
/*
 *
*/
void _Renderer::setShader()
{
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //---------------------------------
    //shader string literals
    QByteArray v_source_utf = ReadStringFromQrc(":/shaders/vshader.glsl").toLocal8Bit(); // get shader source from qrc file
    QByteArray f_source_utf = ReadStringFromQrc(":/shaders/fshader.glsl").toLocal8Bit(); // get shader source from qrc file

    const char *vshader = v_source_utf.data(); //convert to const char*
    const char *fShader = f_source_utf.data(); //convert to const char*

    //Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vshader, NULL);
    glCompileShader(vertexShader);
    //check for compile success
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShader, NULL);
    glCompileShader(fragmentShader);

    //shader program is a uint in the header
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //------------------------------------
    glUseProgram(shaderProgram);
    //------------------------------------
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //------------------------------------
}
/*
 * SetBUffers set Vertex and Index data into
 * the GPU buffers to use for the current model.
 * The functions takes two parameters
 * Created: 11_02_2019
*/
void _Renderer::setBuffers(std::vector<float> vertexArray, std::vector<int> indexArray)
{
    //  Initialization code (done once (unless your object frequently changes))
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    std::cout << VBO << std::endl;
    //
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //
    glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof (float), &vertexArray[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size()* sizeof(int), &indexArray[0], GL_STATIC_DRAW);
    //
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}
/*
 *
*/
void _Renderer::setTexture(char *texBitmap)
{

}
/*
 *
*/
void _Renderer::draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
