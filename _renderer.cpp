#include "_renderer.h"
#include <iostream>
#include <_tools.h>
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
//    glRenderMode(GL_SELECT);
    glClearColor(0.1f, 0.1f, 0.3f, 1.0);//sets the bckground color of the openglContext.

    shdr = new _Shader();//initialising the _shader() class * object.
    setShader();//will run this shader by default.
    timer.start();

    glm_projection4x4 = glm::mat4(1.0f);
    glm_model4x4 = glm::mat4(1.0f);
    glm_view4x4 = glm::mat4(1.0f);
    isTranfomationLocal = false;

    qDebug() << "render initialised ";
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
 * Sets a dafault hard-fed shader
 * on the render object
 * Is being used by the _glWidget class
 * Create:11_02_2019
*/
void _Renderer::setShader()
{
    shdr->attachShaders(":/shaders/vshader.glsl", ":/shaders/fshader.glsl");
    qDebug() << "default Shader attached for entity" << this->sceneEntity.getId();
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
    qDebug() << "setShader(QString"<<vSh<<", QString"<<fSh<<")" << this->sceneEntity.getId();
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
    // Copy the vertex and index data locally for use in the current drawcall.
    this->vertices = vertexArray;
    this->indices = indexArray;
    // Initialization code (done once (unless your object frequently changes))
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    setuniformLocations();//sets all uniform locations to respective variables.
    qDebug() << "setModelDataInBuffers() for entity" << this->sceneEntity.getId();
}
/*
 *
*/
void _Renderer::setuniformLocations()
{
    this->colorUniform = shdr->getUniformLocation("aColor");//will be replaced with texture and UVs
    this->modelUnifrom = shdr->getUniformLocation("model");
    this->viewUniform  = shdr->getUniformLocation("view");
    this->projectionUniform = shdr->getUniformLocation("projection");
    this->mousePosUniform = shdr->getUniformLocation("mousePos");
}

/*
 * Function: setupTexture()
 * Contributor : Saurabh
 * creates new texture and adds into list(vector) of textures
 * current context should be active while calling these functions
 * use makeCurrent() to make context current
 * set a default 8bit single color texture of size 1360 x 1024
 * Created: 2_3_2019
 */
void _Renderer::setupTexture()
{
    char* img = new char[1360*1024];
    _Texture texture(img,1360,1024);
    texture.load(GL_RED,GL_UNSIGNED_BYTE);
    textures.push_back(texture);
    qDebug() << "setupTexture() on entity" << this->sceneEntity.getId();
}
void _Renderer::setupTexture(QString texfile)
{
    QImage img = QImage(texfile);
    _Texture texture(img);
    texture.load(GL_RGBA,GL_UNSIGNED_BYTE);
    textures.push_back(texture);
    qDebug() << "setupTexture(QString texfile) on entity" << this->sceneEntity.getId();
}

/*
 * Function: setTexture()
 * Contributor : saurabh
 * updates the first texture image from char pointer array
 * resolution of previous image is used
 * current context should be active while calling this function
 * Created: 2_3_2019
 */
void _Renderer::setTexture(char* texBitmap)
{
    if(!textures.empty())
        textures[0].setImage(texBitmap);
    qDebug() << "setTexture(char* texBitmap) on entity" << this->sceneEntity.getId();
}
void _Renderer::setTexture(char* texBitmap,unsigned int iwidth,unsigned int iheight)
{
    if(!textures.empty())
        textures[0].setImage(texBitmap,iwidth,iheight);
    qDebug() << "setTexture(char* texBitmap,unsigned int iwidth,unsigned int iheight) on entity" << this->sceneEntity.getId();
}
void _Renderer::setTexture(QString pathtoTexture)
{
    if(!textures.empty())
        textures[0].setImage(pathtoTexture);
    qDebug() << "setTexture(QString pathtoTexture) on entity" << this->sceneEntity.getId();
}
/*
* Function: setModelMatrix(QVector3D position,float scale,QQuaternion rotation)
* Sets the values matrices for the model matrix 
* works in implementing translation , rotation and scaling
* Used by: the _glWidget class initialiseGl() or paintGl().
* Created: 25_02_2019
*/
void _Renderer::setModelMatrix(QVector3D position,float scale,QVector3D rotation)
{ 
    glm_model4x4 = glm::mat4(1.0f);
    translationMatrix = glm::mat4(1.f);
    rotationMatrix = glm::mat4(1.f);
    scalingMatrix = glm::mat4(1.f);

    scalingMatrix = glm::scale(scalingMatrix, glm::vec3(scale, scale, scale));//scale equally on all sides
    glm::vec3 EulerAngles(rotation.x(),rotation.y(),rotation.z());
    glm::quat quat = glm::quat(EulerAngles);
    rotationMatrix = glm::mat4_cast(quat);
    translationMatrix = glm::translate(translationMatrix,glm::vec3(position.x(), position.y(), position.z()));

    glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
    qDebug() << "setModelMatrix() on entity" << this->sceneEntity.getId();
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
    //qDebug() << "setCamViewMatrix() on entity" << this->sceneEntity.getId();
}
/*
* Function: setProjectionMatrix(int w, int h)
* takes thew width and height of the window and sets the relative 
* field of view and the aspect ration bindings. will update itself each time the 
* window is resized.and needs to be called in the resizeGl function.
* Used by: the _glWidget class resizeGL().
* Created: 25_02_2019
*/
void _Renderer::setProjectionMatrix(int resW, int resH, float fov, float zNear, float zFar )
{
    // Calculate aspect ratio
    float aspect = float(resW) / float(resH ? resH : 1);
    glm_projection4x4 = glm::perspective(glm::radians(fov), float(aspect), zNear, zFar);
    qDebug() << "setProjectionMatrix() on entity" << this->sceneEntity.getId();
}
/*
 * Function: setPosition(QVector3D pos)\translate(QVector3D pos)
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the positions
 * Used by: _render class in draw()
 * Created: 1_03_2019
*/
void _Renderer::setPosition(QVector3D pos)
{
    this->isTranfomationLocal = this->sceneEntity.getIsTransfomationLocal();
    this->sceneEntity.setPosition(pos);
    if(isTranfomationLocal)
    {
        glm_model4x4 = glm::mat4(1.0f);
        glm_model4x4 = glm::translate(glm_model4x4,glm::vec3(pos.x(), pos.y(), pos.z()));
    }
    else if(!isTranfomationLocal)
    {
        translationMatrix = glm::mat4(1.f);
        translationMatrix = glm::translate(translationMatrix,glm::vec3( this->sceneEntity.getPostion().x(),
                                                                        this->sceneEntity.getPostion().y(),
                                                                        this->sceneEntity.getPostion().z()));
        glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
    }
}
void _Renderer::translate(QVector3D pos)
{
    this->isTranfomationLocal = this->sceneEntity.getIsTransfomationLocal();
    if(isTranfomationLocal)
    {
        glm_model4x4 = glm::translate(glm_model4x4,glm::vec3(pos.x(), pos.y(), pos.z()));
    }
    else if(!isTranfomationLocal)
    {
        translationMatrix = glm::translate(translationMatrix,glm::vec3(pos.x(), pos.y(), pos.z()));
        glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
    }
    this->sceneEntity.setPosition(QVector3D(translationMatrix[3][0], //sets the actual matrix positons to the Entity
            translationMatrix[3][1], // an alternate implemtation to the norm.
            translationMatrix[3][2]));
}
/*
 * Function: setRotation(QVector3D pos)
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the rotation
 * Used by: _render class in draw()
 * Created: 1_03_2019
*/
void _Renderer::setRotation(QVector3D rot)
{
    this->sceneEntity.setRotation(rot);
    this->isTranfomationLocal = this->sceneEntity.getIsTransfomationLocal();
    if(isTranfomationLocal)
    {//still buggy
        glm::vec3 EulerAngles(this->sceneEntity.getRotation().x(), 
							  this->sceneEntity.getRotation().y(), 
							  this->sceneEntity.getRotation().z());
        glm::quat quat = glm::quat(EulerAngles);
        glm_model4x4 *= glm::mat4_cast(quat);
    }
    else if(!isTranfomationLocal)
    {
        rotationMatrix = glm::mat4x4(1.f);
        glm::vec3 EulerAngles(this->sceneEntity.getRotation().x(),
                              this->sceneEntity.getRotation().y(),
                              this->sceneEntity.getRotation().z());
        glm::quat quat = glm::quat(EulerAngles);
        rotationMatrix = glm::mat4_cast(quat);
        glm_model4x4 =  translationMatrix * rotationMatrix * scalingMatrix;
    }
}
/*
 * Function: setscale(float scale)
 * updates the specific trasformations that affect the model matrix
 * of the matrices of the individual object.In this case the scale
 * Used by: _render class in draw()
 * Created: 1_03_2019
*/
void _Renderer::setscale(float scale)
{
    this->sceneEntity.setScale(scale);//reimplemnt
    scalingMatrix = glm::mat4(1.f);
    scalingMatrix = glm::scale(scalingMatrix, glm::vec3(this->sceneEntity.getScale(),//scale eqally on all axis(dont need respective sclaing)
                                                        this->sceneEntity.getScale(),
                                                        this->sceneEntity.getScale()));
    glm_model4x4 = translationMatrix * rotationMatrix * scalingMatrix;
}
/*
* Function: setSceneEntity(_SceneEntity s)
* Sets the sceen entity object locally and sets the 
* shader ,Model data , projection matrix, texture,
* and initialises the modelMatrix.
* Created: 1_03_2019
*/
void _Renderer::setSceneEntityInRenderer(_SceneEntity s)
{
    this->sceneEntity = s;
    this->isTranfomationLocal = s.getIsTransfomationLocal();
    setShader(s.getVertexShaderPath(), s.getFragmentShaderPath());
    setupTexture(s.getTexturePath());
    setModelDataInBuffers(s.getvertexData(), s.getIndexData());
    setModelMatrix(s.getPostion(), s.getScale(), s.getRotation());
}

/*
* Function: getSceneEntity()
* returns the current scene entity object.
* Created:11_02_2019
*/
_SceneEntity _Renderer::getSceneEntity()
{
    return this->sceneEntity;
}

/*
 * Function: draw()
 * This is your proprietory draw function
 * Draws frames on a avg of 60frames per second(is subjective and changes with hardware)
 * Used by: the _glWidget class paintGl().
 * Created:11_02_2019
*/
void _Renderer::_Renderer::draw()
{
    //use cased during debuging the color and Depth buffer
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Using the shader program in the current context
    shdr->useShaderProgram();
    //Setting the uniform for color
    transitionColors(QVector2D( 0.3,0.1));
    //Bind Textures
    for(unsigned int t=0;t<textures.size();t++){
        textures[t].bind();}
    //Bind the Buffers data of the respective buffer object
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //sets the values for the MVP matrix in the vertex shader
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(glm_view4x4));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(glm_projection4x4));
    glUniformMatrix4fv(modelUnifrom, 1, GL_FALSE, glm::value_ptr(glm_model4x4));
    //The Final draw call for each frame
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
}

/*
 * Temporary debugging implemetation, trasitions colors of object
 * hot reloding of shaders also needs to be implemented
*/
void _Renderer::transitionColors(QVector2D mousePos)
{
    float x = mousePos.x();
    float y = mousePos.y();
    x = x / 824;//limiting the value between 0 and 1
    y = y / 375;//limiting the value between 0 and 1

    //qDebug() << x << "." << y;

    double r = abs(cos(timer.elapsed() * 0.002));
    double g = abs(sin(timer.elapsed() * 0.003));
    double b = abs(cos(timer.elapsed() * 0.005));
    glUniform4f(colorUniform, r, g, b, 1.0f);//will be replaced by Texture
    glUniform2f(mousePosUniform,x,y);//passing mouse value to shader
}
