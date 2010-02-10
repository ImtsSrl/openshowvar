#ifndef CGLROBOTAXE_H
#define CGLROBOTAXE_H

#include <QtOpenGL>
#include "cglrenderable.h"

#define ROTATE_X 0
#define ROTATE_Y 1
#define ROTATE_Z 2

class CGLRobotAxe:public CGLRenderable{
public:
    CGLRobotAxe( int rotationAxis, QString modelFile, float x, float y, float z, float mount_rx, float mount_ry, float mount_rz, CGLRobotAxe* child, float rx, float ry, float rz, float mount_x=0, float mount_y=0, float mount_z=0 );

    void render( );

    void rotateRelative( float angle );
    void rotate( float angle );

    float angle();

private:
    CGLRobotAxe* m_child;
    int          m_rotationAxis; //0-x 1-y 2-z

};

#endif // CGLROBOTAXE_H
