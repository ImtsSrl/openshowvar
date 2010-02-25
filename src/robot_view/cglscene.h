#ifndef CGLSCENE_H
#define CGLSCENE_H

#include <QtOpenGL>

#include "cmodelmanager.h"

#include "cglrobot.h"


#define INTERACT_TRASLATE_SCENE         0
#define INTERACT_ROTATE_SCENE           1
#define INTERACT_TRASLATE_OBJ           2
#define INTERACT_ROTATE_OBJ             3


class CGLScene:public QGLWidget, CGLRenderable{
    Q_OBJECT
public:
    CGLScene();
    ~CGLScene();

    CGLRobot* getRobot(){ return m_robot; };

    void    setInteractionMode( int mode );

    void    paintGL();

    void    resizeGL(int width, int height);

private:
    int     m_interactionMode;

protected:
    void initializeGL();

    void wheelEvent( QWheelEvent * event );

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void keyPressEvent ( QKeyEvent * event );
    void keyReleaseEvent ( QKeyEvent * event );

    void renderScene();

    void renderSceneToSelect( int x, int y );

    void resetRenderingViewParameters();

    void processHits (GLint hits, GLuint buffer[]);

private:
    float       m_aspectRatio;
    float       m_fov;

    CGLRobot*       m_robot;
    //CGLRenderable*  m_ucs;

    QPoint      m_lastPos;  //per i controlli del mouse

    CModelManager*  m_modelManager;
    CGLRenderable*  m_world;

};

#endif // CGLSCENE_H
