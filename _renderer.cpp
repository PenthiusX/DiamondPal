#include "_renderer.h"
#include <iostream>
#include <tools.h>
#include <qquaternion.h>
/*
 * The Renderer class
 * Created: 8_02_2019
 * Author: Aditya,Saurabh
*/
/* Constructor: _Renderer Class
 * The "QOpenGLExtraFunctions(QOpenGLContext::currentContext())" is passed by parameter
 * to avoid using initialiseopenglfunction() in the initcallback.
 * Create:11_02_2019
 */
_Renderer::_Renderer() : QOpenGLExtraFunctions(QOpenGLContext::currentContext())
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRONT_AND_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1, 0.1, 0.3, 1.0);//sets the bckground color of the openglContext.
	//
	shdr = new _Shader();//initialising the _shader() class * object.
	setShader();//will run this shader by default.
	timer.start();
	//
	glm_projection4x4 = glm::mat4(1.0f);
	glm_model4x4 = glm::mat4(1.0f);
}
/*
 *Distructor: _Renderer Class
*/
_Renderer::~_Renderer()
{
	shdr = NULL;
	delete shdr;
}
/*
 * Function: setShader(no params)
 * Sets a dafault hardfed shader
 * on the render object
 * Is being used by the _glWidget class
 * Create:11_02_2019 
*/
void _Renderer::setShader()
{
	shdr->attachShaders(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
}
/*
 * Function: setShader(QString vSh, QString fSh)
 * Takes the path to the relative qrc aided directory
 * to set shader paths externaly on the render object.
 * Is being used by the _glWidget class
 * Create:11_02_2019
*/
void _Renderer::setShader(QString vSh, QString fSh)
{
	shdr->attachShaders(vSh,fSh);
}
/*
 * Function: setBuffers(std::vector<float> vertexArray, std::vector<int> indexArray)
 * set Vertex and Index data into
 * the GPU buffers to use for the current model.
 * May have extended implementation for inclusion of UV for texture and Normals for 
 * lighting.
 * Used by: the _glWidget class initializeGL().
 * Created: 11_02_2019
*/
void _Renderer::setModelDataInBuffers(std::vector<float> vertexArray, std::vector<unsigned int> indexArray)
{
	//copy the vertex and index data locally for use in the current drawcall.
	this->vertices = vertexArray;
	this->indices = indexArray;
    //  Initialization code (done once (unless your object frequently changes))
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    //
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof (float), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
    //
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//
	this->colorUniform = shdr->getUniformLocation("aColor");//will be replaced with texture
	this->modelUnifrom = shdr->getUniformLocation("model");
	this->viewUniform  = shdr->getUniformLocation("view");
	this->projectionUniform = shdr->getUniformLocation("projection");
}
/*
 * Function: setTexture(char *texBitmap)
 * Implementation pending
*/
void _Renderer::setTexture(char *texBitmap)
{

}
/*
* Function: setModelMatrix(QVector3D position,float scale,QQuaternion rotation)
* Sets the values matrices for the model matrix 
* works in implementing translation , rotation and scaling
* Used by: the _glWidget class initialiseGl() or paintGl().
* Created: 25_02_2019
*/
void _Renderer::setModelMatrix(QVector3D position,float scale,QQuaternion rotation)
{
	glm_model4x4 = glm::mat4(1.0f);

	float x = rotation.x();
	x = rotation.y();
	x = rotation.z();
	QVector3D q = rotation.toEulerAngles();
	glm_model4x4 = glm::translate(glm_model4x4,glm::vec3(position.x(), position.y(), position.z()));
	glm_model4x4 = glm::rotate(glm_model4x4, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
	glm_model4x4 = glm::scale(glm_model4x4, glm::vec3(scale, scale, scale));
}

/*
* Function: setCamViewMatrix(QVector3D eyePos,QVector3D focalPoint,QVector3D upVector)
* sets the camera view for the scene through this matrix
* helps set the camera , eye positon , rotation, lookat.
* Used by: the _glWidget class initialiseGl() or paintGl().
* depending if the camra needs to update its position in  realtime.
* Created: 25_02_2019
*/
void _Renderer::setCamViewMatrix(QVector3D eyePos,QVector3D focalPoint,QVector3D upVector)
{
	glm_view4x4 = glm::mat4(1.0f);
	glm_view4x4 = glm::lookAt(
	glm::vec3(eyePos.x(), eyePos.y(), eyePos.z()),
	glm::vec3(focalPoint.x(), focalPoint.y(), focalPoint.z()),
	glm::vec3(upVector.x(), upVector.y(), upVector.z()));
}
/*
* Function: setProjectionMatrix(int w, int h)
* takes thew width and height of the window and sets the relative 
* field of view and the aspect ration bindings. will update itself each time the 
* window is resized.and needs to be called in the resizeGl function.
* Used by: the _glWidget class resizeGL().
* Created: 25_02_2019
*/
void _Renderer::setProjectionMatrix(int resW, int resH, float fov, float zFar, float zNear )
{
	// Calculate aspect ratio
	qreal aspect = qreal(resW) / qreal(resH ? resH : 1);
	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
	glm_projection4x4 = glm::perspective(fov, float(aspect), zNear, zFar);
}
/*
* Function: updateTransformations()
* updates the trasformations of the matrices of the individual object per frame.
* Used by: _render class in draw().
* Created: 25_02_2019
*/
float i = 0;
void _Renderer::updateTrasformations()
{
	i += 0.0055;
	glm_model4x4 = glm::translate(glm_model4x4, glm::vec3((sin(timer.elapsed() * 0.005)* 0.3),0.0,0.00 ));
	glm_model4x4 = glm::rotate(glm_model4x4, (0.02f), glm::vec3(0.0f, 1.0f, 1.0f));
}
/*
 * Function: draw()
 * This is your proprietory draw function 
 * Used by: the _glWidget class paintGl().
 * Create:11_02_2019
*/
void _Renderer::_Renderer::draw()
{
	//updates all trasformations each frame
	updateTrasformations();
    //Using the shader program in the current context
    //can be called once in the init or every frame
    //if the shader is switching between objects
	shdr->useShaderProgram();
	//Bind the Buffers data of the respective buffer object
	//in the context each frame.
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	//Setting the uniform each frame.
	//Depends if the need is to update the values
	float r = abs(cos(timer.elapsed() * 0.002));
	float g = abs(sin(timer.elapsed() * 0.003));
	float b = abs(cos(timer.elapsed() * 0.005));
	glUniform4f(colorUniform, r,g,b, 1.0f);//will be replaced by Texture
	//sets the values for the MVP matrix in the vertex shader
	glUniformMatrix4fv(modelUnifrom, 1, GL_FALSE, glm::value_ptr(glm_model4x4));
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(glm_view4x4));
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(glm_projection4x4));
	//The Final draw call for each frame
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}

