#include "c3dsmodel.h"

C3DSModel::C3DSModel(const char* file3ds){

    m_selfIlluminate = false;

    m_fileName = QString( file3ds );
    m_fileModel = lib3ds_file_open( file3ds );

    m_loaded = false;

    if( QFile::exists( m_fileName ) && m_fileModel != NULL ){

        for( int imesh = 0; imesh < m_fileModel->nmeshes; imesh++ ){
            Lib3dsMesh* mesh = m_fileModel->meshes[imesh];

            m_normalsList.append( (float(*)[3])malloc(sizeof(float) * 3 * mesh->nfaces) );

            lib3ds_mesh_calculate_face_normals( mesh, m_normalsList.at( imesh ) );
        }

        m_loaded = true;
    }

    m_scaleFactor = 1.0;
}


void C3DSModel::renderModel(){

    if( !m_loaded )
        return;

    Lib3dsFace* face;
    Lib3dsMesh* mesh;
    int iface = 0;
    int imesh = 0;
    int material = -1;

    glPushMatrix();

    glScaled( m_scaleFactor, m_scaleFactor, m_scaleFactor );

    glBegin( GL_TRIANGLES );

    //qDebug() << m_scaleFactor;
    //qDebug() << m_fileModel->nmeshes;
    for( imesh = 0; imesh < m_fileModel->nmeshes; imesh++ ){
        mesh = m_fileModel->meshes[imesh];

        //glLoadMatrixf( (float*)mesh->matrix );//glMultMatrixf( (float*)mesh->matrix );

        for( iface = 0; iface < mesh->nfaces; iface++ ){

            face = &mesh->faces[iface];

            if( material != face->material ){
                material = face->material;
                if( material > -1 ){
                    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, m_fileModel->materials[material]->diffuse );
                    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, m_fileModel->materials[material]->ambient );
                    //glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, m_fileModel->materials[material]->specular );

                    /*
                    float lux[3];
                    lux[0] = m_fileModel->materials[material]->diffuse[0]*m_fileModel->materials[material]->self_illum;
                    lux[1] = m_fileModel->materials[material]->diffuse[1]*m_fileModel->materials[material]->self_illum;
                    lux[2] = m_fileModel->materials[material]->diffuse[2]*m_fileModel->materials[material]->self_illum;
                    */
                    if( m_selfIlluminate )
                        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, m_fileModel->materials[material]->ambient );
                }
            }

            glNormal3f( m_normalsList.at(imesh)[iface][0] ,m_normalsList.at(imesh)[iface][1] , m_normalsList.at(imesh)[iface][2] );
            glVertex3f( (float)mesh->vertices[face->index[0]][0],
                        (float)mesh->vertices[face->index[0]][1],
                        (float)mesh->vertices[face->index[0]][2]);

            glNormal3f( m_normalsList.at(imesh)[iface][0] ,m_normalsList.at(imesh)[iface][1] , m_normalsList.at(imesh)[iface][2] );
            glVertex3f( (float)mesh->vertices[face->index[1]][0],
                        (float)mesh->vertices[face->index[1]][1],
                        (float)mesh->vertices[face->index[1]][2]);

            glNormal3f( m_normalsList.at(imesh)[iface][0] ,m_normalsList.at(imesh)[iface][1] , m_normalsList.at(imesh)[iface][2] );
            glVertex3f( (float)mesh->vertices[face->index[2]][0],
                        (float)mesh->vertices[face->index[2]][1],
                        (float)mesh->vertices[face->index[2]][2]);

        }

    }

    glEnd();

    //disable emission
    if( m_selfIlluminate ){
        float lux[3] = {0,0,0};
        glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, lux );
    }

    glPopMatrix();
}


void* C3DSModel::getBoundedBox(){
    boundedBox* box = new boundedBox;

    box->maxX = -65535.0;
    box->maxY = -65535.0;
    box->maxZ = -65535.0;
    box->minX = 65535.0;
    box->minY = 65535.0;
    box->minZ = 65535.0;

    if( !m_loaded )
        return box;

    for( int imesh = 0; imesh < m_fileModel->nmeshes; imesh++ ){
        Lib3dsMesh* mesh = m_fileModel->meshes[imesh];

        for(int iface = 0; iface < mesh->nfaces; iface++ ){

            Lib3dsFace* face = &mesh->faces[iface];

            for( int ivert = 0; ivert < 3; ivert++){
                float v[3];
                v[0]=mesh->vertices[face->index[ivert]][0];
                v[1]=mesh->vertices[face->index[ivert]][1];
                v[2]=mesh->vertices[face->index[ivert]][2];

                box->maxX = qMax( v[0], box->maxX );
                box->minX = qMin( v[0], box->minX );
                box->maxY = qMax( v[1], box->maxY );
                box->minY = qMin( v[1], box->minY );
                box->maxZ = qMax( v[2], box->maxZ );
                box->minZ = qMin( v[2], box->minZ );
            }

        }
    }

    return box;
}

C3DSModel::~C3DSModel(){
    lib3ds_file_free( m_fileModel );
}

float C3DSModel::absoluteSize(){
    boundedBox* box = (boundedBox*)getBoundedBox();
    float size = qMax( qMax( box->maxX, box->maxY ), box->maxZ );
    delete box;
    return size;
}

float C3DSModel::scaleTo( float size ){
    float me_size = absoluteSize();
    m_scaleFactor = (double)size / me_size;
    return m_scaleFactor;
}

float C3DSModel::scaleFactor( ){
    return m_scaleFactor;
}

void C3DSModel::setSelfIlluminate(){
    m_selfIlluminate = true;
}
