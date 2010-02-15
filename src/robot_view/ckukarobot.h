#ifndef CKUKAROBOT_H
#define CKUKAROBOT_H

#include <QtGui>
#include "cglscene.h"


class CKUKARobot : public QWidget{
    Q_OBJECT

public:
    CKUKARobot(QWidget *parent = 0);
    ~CKUKARobot();

    void setRobotPosition( int r1, int r2, int r3, int r4, int r5 );

private:
    void initSceneInteractionControls();

private:
    CGLScene*   m_scene;

private://scene interaction
    QWidget*    m_controlsContainer;
    QToolBar    m_toolBar;
    QAction*    m_traslateObject;
    QAction*    m_rotateObject;

    QAction*    m_traslateScene;
    QAction*    m_rotateScene;

    /*QLineEdit  m_ax1pos;
    QLineEdit  m_ax2pos;
    QLineEdit  m_ax3pos;
    QLineEdit  m_ax4pos;
    QLineEdit  m_ax5pos;*/

public slots:
    void setTraslateObjMode( bool toggle );
    void setRotateObjMode( bool toggle );

    void setTraslateSceneMode( bool toggle );
    void setRotateSceneMode( bool toggle );

    //void inputRobotChanged( QString s );

};

#endif // CKUKAROBOT_H
