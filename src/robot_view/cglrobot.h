#ifndef CGLROBOT_H
#define CGLROBOT_H

#include <QtOpenGL>
#include "cglrobotaxe.h"

class CGLRobot{
public:
    CGLRobot();

    CGLRobotAxe* getRobotAxe( int index );

    void render();

private:
    CGLRobotAxe* m_axis[6];
    CGLRobotAxe* m_base;    

};

#endif // CGLROBOT_H
