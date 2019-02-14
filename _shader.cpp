#include "_shader.h"
#include "tools.h"
#include <iostream>

_Shader::_Shader()
{
    vertexShader = 0;
    fragmentShader = 0;
}
_Shader::~_Shader(){}
/*
 *
*/
uint _Shader::getShaderProgram()
{
    return this->shaderProgram;
}
/*
 *
*/
void _Shader::setFragmentShader(QString f)
{
    QByteArray v_source_utf = ReadStringFromQrc(f).toLocal8Bit(); // get shader source from qrc file
    const char *fshader = v_source_utf.data(); //convert to const char*

    //Vertex Shader
    fragmentShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(fragmentShader, 1, &fshader, NULL);
    glCompileShader(fragmentShader);

    //check for compile success
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}
/*
 *
*/
void _Shader::setVertexShader(QString v)
{
    QByteArray v_source_utf = ReadStringFromQrc(v).toLocal8Bit(); // get shader source from qrc file
    const char *vshader = v_source_utf.data(); //convert to const char*

    //Vertex Shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vshader, NULL);
    glCompileShader(vertexShader);

    //check for compile success
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}
/*
 *
*/
void _Shader::attachShaders()
{
    if(vertexShader > 0 && fragmentShader > 0)
    {
        //shader program is a uint in the header
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    else
    {
       std::cerr << "ERROR::SHADERCLASS::PLEASE_COMPILE_AND_ATTACH_SHADERS_FIRST" << std::endl;
       exit(0);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
/*
 *
*/
void _Shader::attachShaders(QString v,QString f)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0,0.1,0.1,1.0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //shader string literals
    QByteArray v_source_utf = ReadStringFromQrc(v).toLocal8Bit(); // get shader source from qrc file
    QByteArray f_source_utf = ReadStringFromQrc(f).toLocal8Bit(); // get shader source from qrc file

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
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShader, NULL);
    glCompileShader(fragmentShader);\
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!fragmentShader)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //shader program is a uint in the header
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //delete shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
/*
 * Needs to be called before draw
 * everyloop for multiple
 * sets which shader needs to be used in the
 * current context
 */
void _Shader::useShaderProgram()
{
    glUseProgram(this->shaderProgram);
}
