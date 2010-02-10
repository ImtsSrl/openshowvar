#ifndef CKUKAMODEL_H
#define CKUKAMODEL_H

#include <QtOpenGL>

#include "lib3ds/lib3ds.h"

struct str_boundedBox{
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;
};


class C3DSModel{
public:
    typedef struct str_boundedBox boundedBox;

public:
    C3DSModel(const char* file3ds, float mount_x=0, float mount_y=0, float mount_z=0);
    ~C3DSModel();

    void renderModel();

    void* getBoundedBox();

    float absoluteSize();

    float scaleTo( float size );
    float scaleFactor( );

    void setSelfIlluminate();

    void buildDisplayList();

private:
    bool            m_loaded;

    Lib3dsFile*     m_fileModel;

    QString         m_fileName;

    QList<float(*)[3]>  m_normalsList;

    double          m_scaleFactor;

    bool            m_selfIlluminate;

    float   m_mountx;
    float   m_mounty;
    float   m_mountz;

    GLuint  m_displayList;
};

#endif // CKUKAMODEL_H
