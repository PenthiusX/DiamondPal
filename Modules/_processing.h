#ifndef _PROCESSING_H
#define _PROCESSING_H

#include <QObject>
#include <QOpenGLExtraFunctions>
#include <QOffscreenSurface>
#include <IO/_configcontrolentity.h>
#include <QOpenGLContext>
#include <QWidget>
#include <Compute/_gpu_compute.h>
#include <Compute/_cpu_compute.h>

//22_04_2019
class _Processing : public QObject , protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit _Processing(QObject *parent = nullptr);

    bool makeCurrent();
    void doneCurrent();
    _GPU_Compute *gpu_compute = nullptr;
    _Cpu_Compute *cpu_compute = nullptr;

    void setApplicationSettings(_ConfigControlEntity *app_sett);

signals:
    void outputImage(char *img, unsigned int iwidth, unsigned int iheight);
    void outputImage2(char *img, unsigned int iwidth, unsigned int iheight);
    void inputImageRecived(char *img, unsigned int iwidth, unsigned int iheight);
    void generatedModelTextureOut(char *img, unsigned int iwidth, unsigned int iheight);
    void stageCenterAngleOut(float center_x,float center_y,float angle);


public slots:
    void inputImage(char *img, unsigned int iwidth, unsigned int iheight);
    void passThroughFrame(char *img, unsigned int iwidth, unsigned int iheight);
    void histogram(char *img, unsigned int iwidth, unsigned int iheight);
    void markLineLaser(char *img, unsigned int iwidth, unsigned int iheight,int rotation_step = 0,glm::vec2 stage_center = glm::vec2(430,466));
    void markStageEdge(char *img, unsigned int iwidth, unsigned int iheight);
    void generateEdgeModel(char *img, unsigned int iwidth, unsigned int iheight,int rotation_step = 0,glm::vec2 stage_center = glm::vec2(430,466));

    void init();
    void setActiveProcess(const char *slot);
    void generateVoxelsModel(char *img, unsigned int iwidth, unsigned int iheight, int rotation_step, glm::vec2 stage_center);
    void generateVoxelsModelCV(char *img, unsigned int iwidth, unsigned int iheight, int rotation_step, glm::vec2 stage_center);

    void passThroughFrameLocal(char *img, unsigned int iwidth, unsigned int iheight);
    void markStageEdgeCV(char *img, unsigned int iwidth, unsigned int iheight,std::vector<float> params={0.0,0.0,0.0,0.0,0.0,0.0});

protected:

    QOpenGLContext *context = nullptr;
    QOffscreenSurface *surface = nullptr;
    char * colorFrame = nullptr;
    char * colorFrame2 = nullptr;

    const char* active_slot = nullptr;
    static _ConfigControlEntity *application_settings;
    cv::Mat _frameOriginal;
    cv::Mat _frameProcessed;
};

#endif // _PROCESSING_H