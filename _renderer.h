#ifndef _RENDERER_H
#define _RENDERER_H
#include <vector>
#include "_shader.h"
#include <qopenglextrafunctions.h>
#include <qmatrix4x4.h>
#include <qelapsedtimer.h>//for timer
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//
#include "_sceneentity.h"
/*
 * The Renderer class
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
    void setTexture(char* texBitmap);//takes am image and binds it to object
    void setModelMatrix(QVector3D position, float scale, QQuaternion rotation);//set the model matrix
    void setCamViewMatrix(QVector3D eyePos, QVector3D focalPoint, QVector3D upVector);//sets the Camera matrix
    void setProjectionMatrix(int resW, int resH, float fov, float zFar, float zNear);//sets the projection matrix
    void updateTrasformations(QVector3D pos,QQuaternion, float scale);
	void updateTrasformations(QVector3D pos, QQuaternion rot);
	void updateTrasformations(QVector3D pos);
	void setSceneEntityInRenderer(_SceneEntity s);
	_SceneEntity getSceneEntity();
    void draw();//Draws everything bound in the scene

private:
unsigned int VBO;//vertex buffer object
unsigned int VAO;//attribute buffer object
unsigned int EBO;//index buffer object
//Shader class object sets the shaders and passes
//the program to the current context
_Shader* shdr;
//Matrices for Translation and view
//will be multiplied with the position to set translation
//rotaion ,scaling witrespect to view.
glm::mat4 glm_model4x4;
glm::mat4 glm_projection4x4;
glm::mat4 glm_view4x4;
//Stores the uniform location allocated in the shader
int colorUniform,mvpUniform,modelUnifrom,viewUniform,projectionUniform;
std::vector<_Renderer> sceneObject;
//Holds the vertex and index data
std::vector<float> vertices;//not allocated yet
std::vector<unsigned int> indices;//not allocated yet
//
QElapsedTimer timer;
//
_SceneEntity sceneEntity;

};

#endif // _RENDERER_H
