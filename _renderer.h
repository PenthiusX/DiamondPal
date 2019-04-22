#ifndef _RENDERER_H
#define _RENDERER_H
#include <vector>
#include "_shader.h"
#include "_sceneentity.h"
#include "_texture.h"
//
#include <qopenglextrafunctions.h>
#include <qelapsedtimer.h>//for timer
//GLM math Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
/*
 * Class: _Renderer
 * To create an abstraction for randering data
 * Extends the _glwidget class, will be included in the _glwidget class
 * Created: 8_02_2019
 * Author: Aditya,Saurabh
*/
class _Renderer : protected QOpenGLExtraFunctions
{
public:
    _Renderer();
    ~_Renderer();

    void setShader();//default shatder to load
    void setShader(QString vertexShader, QString fragmentShader);//takes a string literal and passes
    void setModelDataInBuffers(std::vector<float>vertexArray,std::vector<unsigned int> indexArray);//take vertex and index data and binds it to object buffer

    //setting image of existing texture, current context should be active while calling these functions, use makeCurrent() to make context current
    void setTexture(QString pathtoTexture);
    void setTexture(char* texBitmap);//takes am image and binds it to object
    void setTexture(char* texBitmap,unsigned int iwidth,unsigned int iheight);////takes am image and binds it to object\

    //generating new texture from texfile path or default , context should be active to call these function
    void setupTexture();//takes a hardcoded imagedata and binds it to object
    void setupTexture(QString texfile); //initializes texture from file

    void setModelMatrix(QVector3D position, float scale, QVector3D rotation);//set the model matrix
    void setCamViewMatrix(QVector3D eyePos, QVector3D focalPoint, QVector3D upVector);//sets the Camera matrix
    void setProjectionMatrix(int resW, int resH, float fov, float zFar, float zNear);//sets the projection matrix

    void setPosition(QVector3D pos);//resets the positon to the value that is required
    void translate(QVector3D pos);//update the current positon with the value that is set
    void setRotation(QVector3D rot);
    void setscale(float scale);

    void setSceneEntityInRenderer(_SceneEntity s);
    _SceneEntity getSceneEntity();
    void draw();//Draws everything bound in the scene

    //Debug implentation
    void transitionColors(QVector2D mousePos);

private:
    unsigned int VBO;//vertex buffer object
    unsigned int VAO;//attribute buffer object
    unsigned int EBO;//index buffer object

    //Shader class object sets the shaders and passes the program to the current context
    _Shader* shdr;
    GLint colorUniform,mvpUniform,modelUnifrom,viewUniform,projectionUniform,mousePosUniform;
    //Stores the uniform location allocated in the shader
    void setuniformLocations();

    //Matrices for Translation and view will be multiplied with the position to set translation rotaion ,scaling witrespect to view.
    glm::mat4 glm_model4x4,rotationMatrix,translationMatrix,scalingMatrix;
    glm::mat4 glm_projection4x4;
    glm::mat4 glm_view4x4;

    //The local scene object that carries all the prelaoded and updated values for the sceneOject being rendered.
    std::vector<_Renderer> sceneObject;

    //Holds the vertex and index data
    std::vector<float> vertices;//not allocated yet
    std::vector<unsigned int> indices;//not allocated yet

    std::vector<_Texture> textures;//Texture array for tetures in use for the respective renderer object

    _SceneEntity sceneEntity;//the local sceneEntity object for use in the renderer

    QElapsedTimer timer;//timer object to maintain a timer based events and trasformations
    bool isTranfomationLocal;//Sets the flag to determine if tranformations need to be set on a local or world pivot
};
#endif // _RENDERER_H
