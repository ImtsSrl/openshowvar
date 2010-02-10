#ifndef CGLRENDERABLE_H
#define CGLRENDERABLE_H

#include <QtOpenGL>
#include "c3dsmodel.h"


class CGLRenderable{
public:
    CGLRenderable(QString modelFile, float x, float y, float z, float mount_rx, float mount_ry, float mount_rz, float rx, float ry, float rz );

    void setSelectIndex( int index );

    void setPositionX( float x ){ m_x = x; };
    void setPositionY( float y ){ m_y = y; };
    void setPositionZ( float z ){ m_z = z; };

    float positionX( ){ return m_x; };
    float positionY( ){ return m_y; };
    float positionZ( ){ return m_z; };

    void setRotationRX( float rx ){ m_rx = rx; };
    void setRotationRY( float ry ){ m_ry = ry; };
    void setRotationRZ( float rz ){ m_rz = rz; };

    float rotationRX( ){ return m_rx; };
    float rotationRY( ){ return m_ry; };
    float rotationRZ( ){ return m_rz; };

    void setColor( float r, float g, float b );

    void render();
    void renderBoundedBox();

    C3DSModel* model();

    void select();
    void unselect();
    bool selected();

private:
    void calculateBoundedBox();

protected:
    C3DSModel*  m_3dsModel;

    float   m_x;
    float   m_y;
    float   m_z;

    float   m_mountrx;
    float   m_mountry;
    float   m_mountrz;

    float   m_rx;
    float   m_ry;
    float   m_rz;

    float   m_color[3];

    int     m_selectIndex;  //indice per la selezione
    bool    m_selected;

    C3DSModel::boundedBox*  m_boundedBox;
};

#endif // CGLRENDERABLE_H
