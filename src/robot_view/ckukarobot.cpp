#include "ckukarobot.h"

CKUKARobot::CKUKARobot( VariableDB* databaseVar, int updateInterval, QWidget *parent ):QDockWidget(parent){
    m_databaseVar = databaseVar;
    m_scene = new CGLScene();

    this->setAllowedAreas(Qt::AllDockWidgetAreas);

    mainContainer = new QWidget( this );
    QVBoxLayout* ml = new QVBoxLayout( mainContainer );
    mainContainer->setLayout( ml );
    this->setWidget( mainContainer );

    /*cl->addWidget( &m_ax1pos );
    cl->addWidget( &m_ax2pos );
    cl->addWidget( &m_ax3pos );
    cl->addWidget( &m_ax4pos );
    cl->addWidget( &m_ax5pos );
    connect( &m_ax1pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax2pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax3pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax4pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );
    connect( &m_ax5pos, SIGNAL(textChanged(QString)), this, SLOT(inputRobotChanged(QString)) );*/

    //m_controlsContainer->layout()->setSizeConstraint(QLayout::SetFixedSize);

    m_controlsContainer = new QWidget( );
    m_controlsContainer->setFixedHeight( 60 );
    QHBoxLayout* cl = new QHBoxLayout( m_controlsContainer );
    m_controlsContainer->setLayout( cl );
    initSceneInteractionControls();
    mainContainer->layout()->addWidget( m_controlsContainer );
    cl->addWidget( &m_robotList );
    connect( &m_robotList, SIGNAL(currentIndexChanged(QString)), this, SLOT(robotListSelection(QString)) );

    mainContainer->layout()->addWidget( m_scene );


    connect( &m_pulsar, SIGNAL(timeout()), this, SLOT(updatePulsar()));
    m_pulsar.start( updateInterval );
}

CKUKARobot::~CKUKARobot(){
    delete m_scene;
}

void CKUKARobot::robotListSelection( QString ip ){
    m_robotIP = ip;
}

void CKUKARobot::initSceneInteractionControls(){
    //this->addToolBar( &m_toolBar );

    QActionGroup* actionGroup = new QActionGroup( m_controlsContainer );
    actionGroup->setExclusive( true );

    //QMenuBar* mnuBar = new QMenuBar( m_controlsContainer );
    QToolBar* mnuBar = new QToolBar( m_controlsContainer );
    m_controlsContainer->layout()->addWidget( mnuBar );

    m_traslateScene = new QAction( QIcon(":traslate"),"Traslate scene", mnuBar );
    mnuBar->addAction( m_traslateScene );
    m_traslateScene->setCheckable(true);
    m_traslateScene->setActionGroup( actionGroup );
    connect( m_traslateScene, SIGNAL(toggled(bool)), this, SLOT(setTraslateSceneMode(bool)) );

    m_rotateScene = new QAction( QIcon(":rotate"),"Rotate scene", mnuBar );
    mnuBar->addAction( m_rotateScene );
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

void CKUKARobot::setRobotPosition( float r1, float r2, float r3, float r4, float r5, float r6 ){
    m_scene->getRobot()->getRobotAxe(0)->setRotationRZ( -r1 );
    m_scene->getRobot()->getRobotAxe(1)->setRotationRY( r2 );
    m_scene->getRobot()->getRobotAxe(2)->setRotationRY( r3 );
    m_scene->getRobot()->getRobotAxe(3)->setRotationRX( -r4 );
    m_scene->getRobot()->getRobotAxe(4)->setRotationRY( r5 );
    m_scene->getRobot()->getRobotAxe(5)->setRotationRX ( -r6 );
    m_scene->repaint();
}

void CKUKARobot::updatePulsar(){
    if( this->isVisible() ){
        //m_scene->setFixedSize( this->width()-20 , this->height() - 110 );
        //m_scene->setMinimumSize( this->width()-20 , this->height() - 110 );

        QStringList ipRobotList = m_databaseVar->getRobotIPList();
        if( ipRobotList.count() != m_robotList.count() ){
            m_robotList.clear();
            m_robotList.addItems( ipRobotList );
        }

        int readTime = 0;
        QByteArray value;
        if( m_databaseVar->readVar( "$AXIS_ACT", QHostAddress( m_robotIP ), &value, &readTime  ) ){
            int i = 0;
            int t = KukaVar::INT;
            KukaVar var( "$AXIS_ACT", ""+ value );
            //qDebug() << "__________________________-------------------------" << var.getStructureValue(0, t) << var.getStructureValue(1, t) << value;
            if( var.getVarType() == KukaVar::STRUCTURE ){
                if( var.getElementsNumber() > 4 ){
                    float r1 = var.getStructureValue(0, t).toFloat();
                    float r2 = var.getStructureValue(1, t).toFloat();
                    float r3 = var.getStructureValue(2, t).toFloat();
                    float r4 = var.getStructureValue(3, t).toFloat();
                    float r5 = var.getStructureValue(4, t).toFloat();
                    float r6 = var.getStructureValue(5, t).toFloat();
                    this->setRobotPosition( r1, r2, r3, r4, r5, r6 );
                }
            }
        }else{
            float r1 = 0;
            float r2 = 0;
            float r3 = 0;
            float r4 = 0;
            float r5 = 0;
            float r6 = 0;
            this->setRobotPosition( r1, r2, r3, r4, r5, r6 );
        }
    }
}
