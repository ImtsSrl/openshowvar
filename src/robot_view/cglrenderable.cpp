#include "cglrenderable.h"

CGLRenderable::CGLRenderable(QString modelFile, float x, float y, float z, float mount_rx, float mount_ry, float mount_rz, float rx, float ry, float rz, float mount_x, float mount_y, float mount_z ){
    m_x = x;
    m_y = y;
    m_z = z;

    m_mountrx = mount_rx;
    m_mountry = mount_ry;
    m_mountrz = mount_rz;

    m_rx = rx;
    m_ry = ry;
    m_rz = rz;

    m_mountx = mount_x;
    m_mounty = mount_y;
    m_mountz = mount_z;

    m_3dsModel = new C3DSModel( modelFile.toAscii().data(), m_mountx, m_mounty, m_mountz );

    m_selected = false;

    calculateBoundedBox();
}

void CGLRenderable::calculateBoundedBox(){
    m_boundedBox = (C3DSModel::boundedBox*)m_3dsModel->getBoundedBox();
}


void CGLRenderable::renderBoundedBox(){

    glPushMatrix();

    float color[3];
    color[0]=1.0;
    color[1]=1.0;
    color[2]=1.0;
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, color );
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, color );

    C3DSModel::boundedBox box;
    box.minX = m_boundedBox->minX * m_3dsModel->scaleFactor();
    box.minY = m_boundedBox->minY * m_3dsModel->scaleFactor();
    box.minZ = m_boundedBox->minZ * m_3dsModel->scaleFactor();
    box.maxX = m_boundedBox->maxX * m_3dsModel->scaleFactor();
    box.maxY = m_boundedBox->maxY * m_3dsModel->scaleFactor();
    box.maxZ = m_boundedBox->maxZ * m_3dsModel->scaleFactor();

    glBegin( GL_LINES );
        //front face
        glVertex3f( box.minX, box.minY, box.minZ );
        glVertex3f( box.maxX, box.minY, box.minZ );

        glVertex3f( box.maxX, box.minY, box.minZ );
        glVertex3f( box.maxX, box.maxY, box.minZ );

        glVertex3f( box.maxX, box.maxY, box.minZ );
        glVertex3f( box.minX, box.maxY, box.minZ );

        glVertex3f( box.minX, box.maxY, box.minZ );
        glVertex3f( box.minX, box.minY, box.minZ );

        //back face
        glVertex3f( box.minX, box.minY, box.maxZ );
        glVertex3f( box.maxX, box.minY, box.maxZ );

        glVertex3f( box.maxX, box.minY, box.maxZ );
        glVertex3f( box.maxX, box.maxY, box.maxZ );

        glVertex3f( box.maxX, box.maxY, box.maxZ );
        glVertex3f( box.minX, box.maxY, box.maxZ );

        glVertex3f( box.minX, box.maxY, box.maxZ );
        glVertex3f( box.minX, box.minY, box.maxZ );

        //left face
        glVertex3f( box.minX, box.minY, box.minZ );
        glVertex3f( box.minX, box.minY, box.maxZ );

        glVertex3f( box.minX, box.maxY, box.minZ );
        glVertex3f( box.minX, box.maxY, box.maxZ );

        //right face
        glVertex3f( box.maxX, box.minY, box.minZ );
        glVertex3f( box.maxX, box.minY, box.maxZ );

        glVertex3f( box.maxX, box.maxY, box.minZ );
        glVertex3f( box.maxX, box.maxY, box.maxZ );

    glEnd();

    color[0]=0.0;
    color[1]=0.0;
    color[2]=0.0;
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, color );
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, color );

    glPopMatrix();
}


void CGLRenderable::render(){
    glLoadName( m_selectIndex );
    glPushMatrix();
        glTranslatef( m_x, m_y, m_z );
        glRotatef( m_mountrx + m_rx, 1.0, 0.0, 0.0 );
        glRotatef( m_mountry + m_ry, 0.0, 1.0, 0.0 );
        glRotatef( m_mountrz + m_rz, 0.0, 0.0, 1.0 );
        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_color );

        m_3dsModel->renderModel();

        if( m_selected )
            this->renderBoundedBox();

    glPopMatrix();
}

void CGLRenderable::setSelectIndex( int index ){
    m_selectIndex = index;
}

void CGLRenderable::setColor( float r, float g, float b ){
    m_color[0] = r;
    m_color[1] = g;
    m_color[2] = b;
}

void CGLRenderable::select(){
    m_selected = true;
}
void CGLRenderable::unselect(){
    m_selected = false;
}
bool CGLRenderable::selected(){
    return m_selected;
}

C3DSModel* CGLRenderable::model(){
    return m_3dsModel;
}
