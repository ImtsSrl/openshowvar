#include "c3dsmodel.h"

C3DSModel::C3DSModel(const char* file3ds, float mount_x, float mount_y, float mount_z){

    m_mountx = mount_x;
    m_mounty = mount_y;
    m_mountz = mount_z;

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
    m_displayList = 0;
}

void C3DSModel::renderModel(){
    if( m_displayList == 0 ){
        m_displayList = glGenLists(1);
        glNewList( m_displayList, GL_COMPILE );
        buildDisplayList();
        glEndList();
    }else{
        glCallList(m_displayList);
    }
}

void C3DSModel::buildDisplayList(){
    if( !m_loaded )
        return;

    Lib3dsFace* face;
    Lib3dsMesh* mesh;
    int iface = 0;
    int imesh = 0;
    int material = -1;

    GLfloat sambient[4] = { 1.0, 0.4, 0.0, 1.0 };
    GLfloat sdiffuse[4] = { 1.0, 0.4, 0.0, 1.0 };
    GLfloat sspecular[4] = { 0.0, 0.0, 0.0, 0.0 };

    /*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sambient ); // Ambient color
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sspecular ); //specular color
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);*/

    glPushMatrix();

    //glScaled( m_scaleFactor, m_scaleFactor, m_scaleFactor );

    glBegin( GL_TRIANGLES );

    for( imesh = 0; imesh < m_fileModel->nmeshes; imesh++ ){
        mesh = m_fileModel->meshes[imesh];
        material = -1;

        for( iface = 0; iface < mesh->nfaces; iface++ ){

            face = &mesh->faces[iface];

            if( material != face->material ||
                sambient[0] != m_fileModel->materials[material]->ambient[0] ||
                sambient[1] != m_fileModel->materials[material]->ambient[1] ||
                sambient[2] != m_fileModel->materials[material]->ambient[2] ||
                iface == 0 ){
                material = face->material;
                if( face->material > 0 ){
                    sambient[0] = m_fileModel->materials[material]->ambient[0];
                    sambient[1] = m_fileModel->materials[material]->ambient[1];
                    sambient[2] = m_fileModel->materials[material]->ambient[2];
                    sdiffuse[0] = m_fileModel->materials[material]->diffuse[0];
                    sdiffuse[1] = m_fileModel->materials[material]->diffuse[1];
                    sdiffuse[2] = m_fileModel->materials[material]->diffuse[2];
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_fileModel->materials[material]->ambient); // Ambient color
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_fileModel->materials[material]->diffuse); //diffuse color
                    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_fileModel->materials[material]->specular); //specular color
                    /*float shine;
                    shine = pow(2, 10.0 * m_fileModel->materials[material]->shininess);
                    if(shine > 128.0)
                        shine = 128.0;
                    glMaterialf(GL_FRONT, GL_SHININESS, shine);*/
                }else{
                    GLfloat ambient[4] = { 1.0, 0.4, 0.0, 1.0 };
                    GLfloat specular[4] = { 0.0, 0.0, 0.0, 0.0 };

                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambient ); // Ambient color
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular ); //specular color
                    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
                }
            }

            glNormal3f( m_normalsList.at(imesh)[iface][0] ,m_normalsList.at(imesh)[iface][1] , m_normalsList.at(imesh)[iface][2] );
            glVertex3f( (float)mesh->vertices[face->index[0]][0] + m_mountx,
                        (float)mesh->vertices[face->index[0]][1] + m_mounty,
                        (float)mesh->vertices[face->index[0]][2] + m_mountz);

            glNormal3f( m_normalsList.at(imesh)[iface][0] ,m_normalsList.at(imesh)[iface][1] , m_normalsList.at(imesh)[iface][2] );
            glVertex3f( (float)mesh->vertices[face->index[1]][0] + m_mountx,
                        (float)mesh->vertices[face->index[1]][1] + m_mounty,
                        (float)mesh->vertices[face->index[1]][2] + m_mountz);

            glNormal3f( m_normalsList.at(imesh)[iface][0] ,m_normalsList.at(imesh)[iface][1] , m_normalsList.at(imesh)[iface][2] );
            glVertex3f( (float)mesh->vertices[face->index[2]][0] + m_mountx,
                        (float)mesh->vertices[face->index[2]][1] + m_mounty,
                        (float)mesh->vertices[face->index[2]][2] + m_mountz);

        }

    }

    glEnd();

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
