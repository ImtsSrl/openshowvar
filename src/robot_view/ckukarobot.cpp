#include "ckukarobot.h"

CKUKARobot::CKUKARobot(QWidget *parent):QWidget(parent){
    m_scene = new CGLScene();

    QWidget* mainContainer = this;//new QWidget( this );
    QVBoxLayout* ml = new QVBoxLayout( mainContainer );
    this->setLayout( ml );

    QWidget* controlsContainer = new QWidget( mainContainer );
    QHBoxLayout* cl = new QHBoxLayout( controlsContainer );
    /*controlsContainer->setLayout( cl );
    cl->addWidget( &m_ax1pos );
    cl->addWidget( &m_ax2pos );
    cl->addWidget( &m_ax3pos );
    cl->addWidget( &m_ax4pos );
    cl->addWidget( &m_ax5pos );
    connect( &m_ax1pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax2pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax3pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax4pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax5pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );*/


    controlsContainer->setFixedWidth( 60 );
    controlsContainer->layout()->setSizeConstraint(QLayout::SetFixedSize);

    mainContainer->layout()->addWidget( controlsContainer );
    mainContainer->layout()->addWidget( m_scene );

    //this->setCentralWidget( mainContainer );
    this->setMinimumSize( 560, 400 );
    m_scene->setMinimumSize( 500, 400 );

    initSceneInteractionControls();
}

CKUKARobot::~CKUKARobot(){
    delete m_scene;
}

void CKUKARobot::initSceneInteractionControls(){
    //this->addToolBar( &m_toolBar );

    QActionGroup* actionGroup = new QActionGroup( this );
    actionGroup->setExclusive( true );

    m_traslateScene = new QAction( QIcon("./icons/traslate.png"),"Traslate scene", this );
    this->addAction( m_traslateScene );
    m_traslateScene->setCheckable(true);
    m_traslateScene->setActionGroup( actionGroup );
    connect( m_traslateScene, SIGNAL(toggled(bool)), this, SLOT(setTraslateSceneMode(bool)) );

    m_rotateScene = new QAction( QIcon("./icons/rotate.png"),"Rotate scene", this );
    this->addAction( m_rotateScene );
    m_rotateScene->setCheckable(true);
    m_rotateScene->setActionGroup( actionGroup );
    connect( m_rotateScene, SIGNAL(toggled(bool)), this, SLOT(setRotateSceneMode(bool)) );

    /*m_traslateObject = new QAction( QIcon("./icons/traslateObj.png"),"Traslate object", this );
    m_toolBar.addAction( m_traslateObject );
    m_traslateObject->setCheckable(true);
    m_traslateObject->setActionGroup( actionGroup );
    connect( m_traslateObject, SIGNAL(toggled(bool)), this, SLOT(setTraslateObjMode(bool)) );

    m_rotateObject = new QAction( QIcon("./icons/rotateObj.png"),"Rotate object", this );
    m_toolBar.addAction( m_rotateObject );
    m_rotateObject->setCheckable(true);
    m_rotateObject->setActionGroup( actionGroup );
    connect( m_rotateObject, SIGNAL(toggled(bool)), this, SLOT(setRotateObjMode(bool)) );*/
}

void CKUKARobot::setTraslateObjMode( bool toggle ){
    if( toggle )
        m_scene->setInteractionMode( INTERACT_TRASLATE_OBJ );
}

void CKUKARobot::setRotateObjMode( bool toggle ){
    if( toggle )
        m_scene->setInteractionMode( INTERACT_ROTATE_OBJ );
}

void CKUKARobot::setTraslateSceneMode( bool toggle ){
    if( toggle )
        m_scene->setInteractionMode( INTERACT_TRASLATE_SCENE );
}

void CKUKARobot::setRotateSceneMode( bool toggle ){
    if( toggle )
        m_scene->setInteractionMode( INTERACT_ROTATE_SCENE );
}

/*void CKUKARobot::inputRobotChanged( QString s ){
    int r1 = m_ax1pos.text().toInt();
    int r2 = m_ax2pos.text().toInt();
    int r3 = m_ax3pos.text().toInt();
    int r4 = m_ax4pos.text().toInt();
    int r5 = m_ax5pos.text().toInt();
    setRobotPosition( r1, r2, r3, r4, r5 );
}*/

void CKUKARobot::setRobotPosition( int r1, int r2, int r3, int r4, int r5 ){
    m_scene->getRobot()->getRobotAxe(0)->setRotationRZ( -r1 );
    m_scene->getRobot()->getRobotAxe(1)->setRotationRX( r2 );
    m_scene->getRobot()->getRobotAxe(2)->setRotationRX( r3 );
    m_scene->getRobot()->getRobotAxe(3)->setRotationRZ( r4 );
    m_scene->getRobot()->getRobotAxe(4)->setRotationRX( r5 );
    m_scene->repaint();
}

