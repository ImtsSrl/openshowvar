#include "cglrobotaxe.h"

CGLRobotAxe::CGLRobotAxe( int rotationAxis, QString modelFile, float x, float y, float z, float mount_rx, float mount_ry, float mount_rz, CGLRobotAxe* child, float rx, float ry, float rz )
         : CGLRenderable( modelFile, x, y, z, mount_rx, mount_ry, mount_rz, rx, ry, rz ){

    m_rotationAxis = rotationAxis;
    m_child = child;

}

float CGLRobotAxe::angle(){
    switch( m_rotationAxis ){
    case ROTATE_X:
        return m_rx;
        break;
    case ROTATE_Y:
        return m_ry;
        break;
    case ROTATE_Z:
        return m_rz;
        break;
    }
    return 0.0;
}

void CGLRobotAxe::rotateRelative( float ang ){
    rotate( this->angle() + ang );
}

void CGLRobotAxe::rotate( float angle ){
    switch( m_rotationAxis ){
    case ROTATE_X:
        m_rx = angle;
        break;
    case ROTATE_Y:
        m_ry = angle;
        break;
    case ROTATE_Z:
        m_rz = angle;
        break;
    }
}

void CGLRobotAxe::render( ){
    glPushMatrix();

        if( m_selectIndex > 0 )
            glLoadName( m_selectIndex );

        glTranslatef( m_x, m_y, m_z );
        glRotatef( m_mountrx + m_rx, 1.0, 0.0, 0.0 );
        glRotatef( m_mountry + m_ry, 0.0, 1.0, 0.0 );
        glRotatef( m_mountrz + m_rz, 0.0, 0.0, 1.0 );

        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_color );

        m_3dsModel->renderModel();

        if( m_selected )
            this->renderBoundedBox();

        if( m_child != NULL )
            ((CGLRobotAxe*)m_child)->render();

    glPopMatrix();
}
