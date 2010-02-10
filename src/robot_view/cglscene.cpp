#include "cglscene.h"

CGLScene::CGLScene()
         : CGLRenderable( "", 0.0, -1500.0, -8000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ){

    m_robot = new CGLRobot();
    /*m_ucs = new CGLRenderable( "./model/ucs_rot2.3DS", 0,0,1000,0,0,0,0,0,0);
    m_ucs->model()->scaleTo( m_robot->getRobotAxe(0)->model()->absoluteSize() );
    m_ucs->model()->setSelfIlluminate();*/


    m_modelManager = new CModelManager();
    m_modelManager->addModel( m_robot->getRobotAxe(0) );
    m_modelManager->addModel( m_robot->getRobotAxe(1) );
    m_modelManager->addModel( m_robot->getRobotAxe(2) );
    m_modelManager->addModel( m_robot->getRobotAxe(3) );
    m_modelManager->addModel( m_robot->getRobotAxe(4) );
    //m_modelManager->addModel( m_ucs );


    this->setMouseTracking( true );
    this->setFocusPolicy(Qt::StrongFocus);

    m_interactionMode = INTERACT_ROTATE_SCENE;
}

CGLScene::~CGLScene(){
    delete m_robot;
    delete m_modelManager;
}

void CGLScene::setInteractionMode( int mode ){
    m_interactionMode = mode;
}

void CGLScene::initializeGL(){
    qglClearColor( Qt::lightGray );

    //glEnable(GL_DOUBLEBUFFER);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    static GLfloat lightPosition[4] = { -2000.0, -2000.0, 3000.0, 1.0 }; //{ -0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glDepthFunc(GL_LEQUAL);							// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


void CGLScene::renderScene(){
    float params[3];

    params[0]= 0.5;
    params[1]= 0.5;
    params[2]= 0.5;
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, params );

    glBegin( GL_QUADS );

        float plateWidth = 5000.0;
        float h = 0.0;
        int nx = 30;
        int ny = 30;

        float xstep = plateWidth/nx;
        float ystep = plateWidth/ny;
        for( float x = -plateWidth/2; x < plateWidth/2; x += xstep ){
            for( float y = -plateWidth/2; y < plateWidth/2; y += ystep ){
                glNormal3f( 0.0f, 1.0f, 0.0f);
                glVertex3f( x, h, y );
                glNormal3f( 0.0f, 1.0f, 0.0f);
                glVertex3f( x+xstep, h, y );
                glNormal3f( 0.0f, 1.0f, 0.0f);
                glVertex3f( x+xstep, h, y+ystep );
                glNormal3f( 0.0f, 1.0f, 0.0f);
                glVertex3f( x, h, y+ystep );
            }
        }

    glEnd();

    m_robot->render();
    //m_ucs->render();

    glFinish();

}

void CGLScene::processHits (GLint hits, GLuint buffer[]){
    unsigned int bestIndex, i, j; //l'oggetto piu vicino
    float currZ, bestZ;
    bestZ = 65535.0;


    GLuint names, *ptr;
    qDebug() << "hits = %d\n"<< hits;
    ptr = (GLuint *) buffer;
    for (i = 0; i < hits; i++) { /* for each hit */
        names = *ptr;
        qDebug() << " number of names for this hit = %d\n" << names;
        ptr++;
        qDebug() << " z1 is;" <<  (float) *ptr/0x7fffffff;
        ptr++;
        qDebug() << " z2 is" << (float) *ptr/0x7fffffff;
        currZ = (float)*ptr/0x7fffffff;
        ptr++;
        qDebug() << "names are ";
        for (j = 0; j < names; j++) {
            if( currZ < bestZ ){
                bestIndex = *ptr;
                bestZ = currZ;
            }
            qDebug() << "%d "<< *ptr;
            ptr++;
        }
    }

    m_modelManager->unselectAll();
    if( m_modelManager->getModelFromKey( bestIndex ) != NULL ){
        //m_modelManager->getModelFromKey( bestIndex )->setColor( 0.5, 1.0, 0.0 );
        m_modelManager->getModelFromKey( bestIndex )->select();
    }
}



void CGLScene::renderSceneToSelect( int x, int y ){
GLuint selectBuf[512];
GLint hits;
GLint viewport[4];

    glSelectBuffer( 512, selectBuf );

    glGetIntegerv( GL_VIEWPORT, viewport );

    glRenderMode( GL_SELECT );

    glInitNames();
    glPushName( 0 );

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix( x, viewport[3] - y, 1.0, 1.0, viewport);
    gluPerspective( m_fov, m_aspectRatio, 1.0, 25000.0);
    glMatrixMode(GL_MODELVIEW);

    glBegin( GL_QUADS );
        float plateWidth = 5000.0;
        float h = 0.0;
        int nx = 30;
        int ny = 30;
        float xstep = plateWidth/nx;
        float ystep = plateWidth/ny;
        for( float x = -plateWidth/2; x < plateWidth/2; x += xstep ){
            for( float y = -plateWidth/2; y < plateWidth/2; y += ystep ){
                glNormal3f( 0.0f, 1.0f, 0.0f);glVertex3f( x, h, y );glNormal3f( 0.0f, 1.0f, 0.0f);glVertex3f( x+xstep, h, y );glNormal3f( 0.0f, 1.0f, 0.0f);glVertex3f( x+xstep, h, y+ystep );glNormal3f( 0.0f, 1.0f, 0.0f);glVertex3f( x, h, y+ystep );
            }
        }
    glEnd();

    m_robot->render();
    //m_ucs->render();

    glPopMatrix();
    glFlush();


    hits = glRenderMode( GL_RENDER );

    processHits( hits, selectBuf );

}


void CGLScene::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef( m_x, m_y, m_z );

    glRotatef( m_rx, 1.0, 0.0, 0.0);
    glRotatef( m_ry, 0.0, 1.0, 0.0);
    glRotatef( m_rz, 0.0, 0.0, 1.0);

    renderScene();
}

void CGLScene::resetRenderingViewParameters(){
    resizeGL( this->width(), this->height() );
}

void CGLScene::resizeGL(int width, int height){
int side = qMin(width, height);
    //glViewport((width - side) / 2, (height - side) / 2, side, side);
    glViewport( 0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    m_aspectRatio = (float)width/height;
    m_fov = 45.0;
    gluPerspective( m_fov, m_aspectRatio, 1.0, 25000.0);//60?
    glMatrixMode(GL_MODELVIEW);
}


void CGLScene::mousePressEvent(QMouseEvent *event){
    m_lastPos = event->pos();
}

void CGLScene::mouseMoveEvent(QMouseEvent *event){
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();


    CGLRenderable* obj = m_modelManager->getSelectedObject();
    if( m_interactionMode == INTERACT_ROTATE_SCENE || m_interactionMode == INTERACT_TRASLATE_SCENE )
        obj = this;
    if( obj == NULL )
        return;

    if( m_interactionMode == INTERACT_ROTATE_SCENE ){
        float mul = 0.1; //moltiplicatore
        if (event->buttons() & Qt::LeftButton) {
            obj->setRotationRX( obj->rotationRX() + mul * dy );
            obj->setRotationRY( obj->rotationRY() + mul * dx );
        } else if (event->buttons() & Qt::RightButton) {
            obj->setRotationRX( obj->rotationRX() + mul * dy );
            obj->setRotationRZ( obj->rotationRZ() + mul * dx );
        }
    }else{
        float mul = 3.0; //moltiplicatore
        if (event->buttons() & Qt::LeftButton) {
            obj->setPositionX( obj->positionX() + mul * dx );
            obj->setPositionZ( obj->positionZ() + mul * dy );
        } else if (event->buttons() & Qt::RightButton) {
            obj->setPositionX( obj->positionX() + mul * dx );
            obj->setPositionY( obj->positionY() + mul * dy * -1 );
        }
    }

    m_lastPos = event->pos();

    repaint();
}

void CGLScene::mouseReleaseEvent(QMouseEvent *event){
    renderSceneToSelect( event->x(), event->y() );
    resetRenderingViewParameters();
    repaint();
}

void CGLScene::keyPressEvent ( QKeyEvent * event ){
    CGLRenderable* obj = m_modelManager->getSelectedObject();
    if( m_interactionMode == INTERACT_ROTATE_SCENE || m_interactionMode == INTERACT_TRASLATE_SCENE )
        obj = this;
    if( obj == NULL )
        return;

    int value = 10;

    if( m_interactionMode == INTERACT_TRASLATE_SCENE || m_interactionMode == INTERACT_TRASLATE_OBJ ){

        if( event->key() == Qt::Key_Up ){
            obj->setPositionZ( obj->positionZ() + value );
        }else if( event->key() == Qt::Key_Down ){
            obj->setPositionZ( obj->positionZ() - value );
        }
        if( event->key() == Qt::Key_Left ){
            obj->setPositionX( obj->positionX() + value );
        }else if( event->key() == Qt::Key_Right ){
            obj->setPositionX( obj->positionX() - value );
        }
        if( event->key() == Qt::Key_A ){
            obj->setPositionX( obj->positionX() + value );
        }else if( event->key() == Qt::Key_D ){
            obj->setPositionX( obj->positionX() - value );
        }
        if( event->key() == Qt::Key_W ){
            obj->setPositionY( obj->positionY() + value );
        }else if( event->key() == Qt::Key_S ){
            obj->setPositionY( obj->positionY() - value );
        }

    }else if( m_interactionMode == INTERACT_ROTATE_SCENE || m_interactionMode == INTERACT_ROTATE_OBJ ){
        if( event->key() == Qt::Key_Up ){
            obj->setRotationRZ( obj->rotationRZ() + value );
        }else if( event->key() == Qt::Key_Down ){
            obj->setRotationRZ( obj->rotationRZ() - value );
        }

        if( event->key() == Qt::Key_Left ){
            obj->setRotationRX( obj->rotationRX() + value );
        }else if( event->key() == Qt::Key_Right ){
            obj->setRotationRX( obj->rotationRX() - value );
        }

        if( event->key() == Qt::Key_A ){
            obj->setRotationRX( obj->rotationRX() + value );
        }else if( event->key() == Qt::Key_D ){
            obj->setRotationRX( obj->rotationRX() - value );
        }

        if( event->key() == Qt::Key_W ){
            obj->setRotationRY( obj->rotationRY() - value );
        }else if( event->key() == Qt::Key_S ){
            obj->setRotationRY( obj->rotationRY() + value );
        }
    }
    repaint();
}

void CGLScene::keyReleaseEvent ( QKeyEvent * event ){
    CGLRenderable* obj = m_modelManager->getSelectedObject();
    if( m_interactionMode == INTERACT_ROTATE_SCENE || m_interactionMode == INTERACT_TRASLATE_SCENE )
        obj = this;
    if( obj == NULL )
        return;

    if( event->key() == Qt::Key_Space ){

        if( m_interactionMode == INTERACT_ROTATE_SCENE || m_interactionMode == INTERACT_ROTATE_OBJ ){
            obj->setRotationRX( 0.0 );
            obj->setRotationRY( 0.0 );
            obj->setRotationRZ( 0.0 );
        }

        if( m_interactionMode == INTERACT_TRASLATE_SCENE || m_interactionMode == INTERACT_TRASLATE_OBJ ){
            if( m_interactionMode == INTERACT_TRASLATE_SCENE ){
                obj->setPositionX( 0.0 );
                obj->setPositionY( -1500.0 );
                obj->setPositionZ( -6000.0 );
            }else{
                obj->setPositionX( 0.0 );
                obj->setPositionY( 0.0 );
                obj->setPositionZ( 0.0 );
            }
        }

        repaint();
    }
}
