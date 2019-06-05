#include "_scene.h"
/*
 * Class: _Scene()
 * This class define the scene manager , manages what needs to be rendered and what propertes need to be
 * set inside via a sceneentity object. essentially sets values in the scene entity object into the Renderer for drawing
 * Sets up Delegation to the class _Framebuffer,_Render and _Physics to work in one scene instance in cohision.
 * Autor: Aditya Mattoo
 * Created:26_02_2019
*/

/*
 * Constructor: _Scene class
*/
_Scene::_Scene()
{
    isCamera = false;
    fboObject = new _FrameBuffer();
    //preLoad all models in the Qrc File into memory for this scene

}
_Scene::~_Scene()
{
    renderObjects.clear();
    delete r;
}
/*
 * Function: getSceneObjects()
 * returns the vector array of sceneObjects.
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
std::vector<_Renderer*> _Scene::getSceneObjects()
{
    return this->renderObjects;
}
//  ▪   ▐ ▄ ▪  ▄▄▄▄▄▪   ▄▄▄· ▄▄▌  ▪  ·▄▄▄▄•▄▄▄ .
//  ██ •█▌▐███ •██  ██ ▐█ ▀█ ██•  ██ ▪▀·.█▌▀▄.▀·
//  ▐█·▐█▐▐▌▐█· ▐█.▪▐█·▄█▀▀█ ██▪  ▐█·▄█▀▀▀•▐▀▀▪▄
//  ▐█▌██▐█▌▐█▌ ▐█▌·▐█▌▐█ ▪▐▌▐█▌▐▌▐█▌█▌▪▄█▀▐█▄▄▌
//  ▀▀▀▀▀ █▪▀▀▀ ▀▀▀ ▀▀▀ ▀  ▀ .▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀
/*
* Function: addSceneObject(_SceneEntity s)
* binds the propertes set by the scene objectes into the 
* renderer instace for rendering in the scene
* this is being called by the _GlWidget class.
* Created:26_02_2019
*/
void _Scene::addSceneObject(_SceneEntity s)
{
    // Only sets the scene object if the camera has been set already and scene object is active
    if (s.getIsActive() == true)
    {
        if (isCamera)
        {
            r = new _Renderer();
            r->setCamViewMatrix(cam.getEyePosition(), cam.getFocalPoint(), cam.getUpVector());
            r->setProjectionMatrix(this->resW,this->resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
            r->initSceneEntityInRenderer(s);
            renderObjects.push_back(r);
            if(s.getIsPhysicsObject())
            {   _Physics phys;
                phys.setSceneEntity(s);
                physVector.push_back(phys);}
        }
        else if(!isCamera) //use default values for camera if no camera set.
        {
            r = new _Renderer();
            r->setCamViewMatrix(QVector3D(0.0, 0.0, -10.0), QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 0.0, 0.0));//set a default camera value
            r->setProjectionMatrix(this->resW,this->resH,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
            r->initSceneEntityInRenderer(s);
            renderObjects.push_back(r);
            if(s.getIsPhysicsObject())
            {   _Physics phys;
                phys.setSceneEntity(s);
                physVector.push_back(phys);}
        }
    }
    else
    {
        qDebug() << "scene object has not been set Properly";
    }
}
/*
 * Function: addCamera(_Camera c)
 * function checks if the cmaera is attached and sets the local
 * camera object with the camera object passed via parameter for use in rendering
 * and setting the view matrix in the renderer
 * Created:26_02_2019
*/
void _Scene::addCamera(_Camera c)
{
    isCamera = true;
    cam = c;
}

//         ▐ ▄     ▄▄▄  ▄▄▄ ..▄▄ · ▪  ·▄▄▄▄•▄▄▄ .
//  ▪     •█▌▐█    ▀▄ █·▀▄.▀·▐█ ▀. ██ ▪▀·.█▌▀▄.▀·
//   ▄█▀▄ ▐█▐▐▌    ▐▀▀▄ ▐▀▀▪▄▄▀▀▀█▄▐█·▄█▀▀▀•▐▀▀▪▄
//  ▐█▌.▐▌██▐█▌    ▐█•█▌▐█▄▄▌▐█▄▪▐█▐█▌█▌▪▄█▀▐█▄▄▌
//   ▀█▄▀▪▀▀ █▪    .▀  ▀ ▀▀▀  ▀▀▀▀ ▀▀▀·▀▀▀ • ▀▀▀
/*
 * Function: onResize(int w,int h)
 * gets called on resize and all operations will run when the windows is resized
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
void _Scene::onResize(int w,int h)
{
    this->resW = w;
    this->resH = h;
    for (unsigned int i = 0; i < renderObjects.size(); i++)
    {
        renderObjects[i]->setProjectionMatrix(w,h,cam.getFOV(),cam.getNearClipDistance(),cam.getFarClipDistance());
    }
    //FBO init and updateTexture on Resize
    fboObject->initialise();//initialised here buecause this is the closest function that runs right after the openglContext is initialised in _glwidgetclass
    fboObject->setupFramebuffer(w,h);//FBO buffer and textures getSetup here.
}

//  ▄• ▄▌ ▄▄▄··▄▄▄▄   ▄▄▄· ▄▄▄▄▄▄▄▄ .
//  █▪██▌▐█ ▄███▪ ██ ▐█ ▀█ •██  ▀▄.▀·
//  █▌▐█▌ ██▀·▐█· ▐█▌▄█▀▀█  ▐█.▪▐▀▀▪▄
//  ▐█▄█▌▐█▪·•██. ██ ▐█ ▪▐▌ ▐█▌·▐█▄▄▌
//   ▀▀▀ .▀   ▀▀▀▀▀•  ▀  ▀  ▀▀▀  ▀▀▀

/*
 * Function: render()
 * This function is render function that will call the glDraw fuinction in
 * the render final draw of all sceneEntity objects attached to scene.
 * this is being called by the _GlWidget class.
 * Created:26_02_2019
*/
void _Scene::render()
{
    //sets the Frame for the framebufferObject.
    fboObject->setUpdatedFrame();// Rhe frames are being bound underneath in the draw() function below
    //--------------------------------------
    //Frame to render is below
    for (unsigned int i = 0; i < renderObjects.size(); i++)
    {
        //Physics update--
        //update Physics for all the sceneObject with property enabled
        if(renderObjects[i]->getSceneEntity().getIsPhysicsObject())//if the sceneEntity has physics body attached
        {   //Passing some essentials into the updateLoop for physics
            updatePhysics(glm::vec2(this->mousePositionL.x(),//Mouse position
                                    this->mousePositionL.y()),
                          glm::vec3(cam.getEyePosition().x(),//Camera Position
                                    cam.getEyePosition().y(),
                                    cam.getEyePosition().z()),
                          glm::vec2(this->resW,this->resH),//Current Resolution
                          renderObjects[i]->getSceneEntity(),//Selected sceneEntity
                          i);//Selected Index
        }

        //Frame update----
        //Render all objects that are active.
        renderObjects[i]->draw();//calls the draw function unique to each renderObject
    }
    //-----------------------------------------
    //Frame above is loaded in buffers and rendered on FBOquad below
    fboObject->setMousePos(this->mousePositionR); //sets the mouse pointervalues for the shader applied on the FBOquad
    fboObject->renderFrameOnQuad(); // sets the frame on the Quad that has been hardcoded into the function
}

/*
 * Function: updateCamera(_Camera c)
 * sets the camera updated values to every render entity matrix
 * Created:26_02_2019
*/
void _Scene::updateCamera(_Camera c)
{
    cam = c;
    if(isCamera == true)
    {
        for (unsigned int i = 0; i < renderObjects.size(); i++)
        {
            renderObjects[i]->setCamViewMatrix(c.getEyePosition(),c.getFocalPoint(),c.getUpVector());
        }
    }
}

void _Scene::setMousePositionInScene(QVector2D mousePos,Qt::MouseButton m)
{
    if(m == Qt::RightButton)
        this->mousePositionR = mousePos;
    else if(m == Qt::LeftButton)
        this->mousePositionL = mousePos;
}

/*
     ▄▄▄· ▄ .▄ ▄· ▄▌.▄▄ · ▪   ▄▄· .▄▄ ·
    ▐█ ▄███▪▐█▐█▪██▌▐█ ▀. ██ ▐█ ▌▪▐█ ▀.
     ██▀·██▀▐█▐█▌▐█▪▄▀▀▀█▄▐█·██ ▄▄▄▀▀▀█▄
    ▐█▪·•██ ▐▀ ▐█▀·.▐█▄▪▐█▐█▌▐███▌▐█▄▪▐█
    .▀   ▀▀  ·  ▀ •  ▀▀▀▀ ▀▀▀·▀▀▀  ▀▀▀▀
*/
/*
 * Function: updatePhysics(glm::vec2 mousePos,glm::vec3 camPos)
 * update the physcs variables realtime and
 * is called in the _scene class's render() function.
 * Created: 22_05_2019
 */
void _Scene::updatePhysics(glm::vec2 mousePos,glm::vec3 camPos,glm::vec2 screenRes,_SceneEntity s,unsigned int index)
{
    for(int p = 0; p < physVector.size(); p++)
    {
        //updates the physics object instance and runs the main physics updateOperations.
        physVector[p].updatePhysics(mousePos,camPos,screenRes,renderObjects[index]->getSceneEntity());
        //updates the status of scneEntity,for vars like 'isHitByRay' and on collision eventChanges to the object.
        //this is needed if we need to see changes to the sceneEntity in the main render as well.
        renderObjects[index]->setSceneEntityInRenderer(physVector[p].getSceneEntity());
    }
}
