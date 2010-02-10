#include "cglrobot.h"

CGLRobot::CGLRobot(){

    m_axis[5] = new CGLRobotAxe( ROTATE_X, "./model/ax6.3ds", 0.0, 0.0, 0.0, 0.0, 0.0, 180.0, NULL, 0.0, 0.0, 0.0, -160.0, 0.0, -50.0  );
    m_axis[4] = new CGLRobotAxe( ROTATE_Y, "./model/ax5.3ds", 280.0, 0.0, 0.0, 0.0, 0.0, 0.0, m_axis[5], 0.0, 0.0, 0.0, -1330.0-280, 0.0, -640.0  );
    m_axis[3] = new CGLRobotAxe( ROTATE_X, "./model/ax4.3ds", 390.0, 0.0, -35.0, 0.0, 0.0, 0.0, m_axis[4], 0.0, 0.0, 0.0, -940.0 -390 , 0.0, -640.0 );
    m_axis[2] = new CGLRobotAxe( ROTATE_Y, "./model/ax3.3ds", 680.0, 0.0, 0.0, 0.0, 0.0, 0.0, m_axis[3], 0.0, 0.0, 0.0, -260 -680 , 0.0, -675.0 );
    m_axis[1] = new CGLRobotAxe( ROTATE_Y, "./model/ax2.3ds", 260.0, 0.0, 275.0, 0.0, 0.0, 0.0, m_axis[2], 0.0, 0.0, 0.0, -260.0, 0.0, -675.0 );
    m_axis[0] = new CGLRobotAxe( ROTATE_X, "./model/ax1.3ds", 0.0, 0.0, 400.0, 0.0, 0.0, 0.0, m_axis[1], 0.0, 0.0, 0.0, 0.0, 0.0, -400.0 );


    //kr100-2p
    /*m_axis[4] = new CGLRobotAxe( ROTATE_X, "./model/ax56.3DS", 25.0, 0.0, -550.0, 0.0, 0.0, 0.0, NULL, 0.0, 0.0, 0.0 );
    m_axis[3] = new CGLRobotAxe( ROTATE_Z, "./model/ax4.3DS", 145.0, 52.5, -950.0, 0.0, 0.0, 0.0, m_axis[4], 0.0, 0.0, 0.0 );
    m_axis[2] = new CGLRobotAxe( ROTATE_X, "./model/ax3.3DS", -120.0, -1250.0, -0.0, -90.0, 0.0, 0.0, m_axis[3], 0.0, 0.0, 0.0 );
    m_axis[1] = new CGLRobotAxe( ROTATE_X, "./model/ax2.3DS", 100.0, -800.0, 400.0, 0.0, 0.0, 0.0, m_axis[2], 0.0, 0.0, 0.0 );
    m_axis[0] = new CGLRobotAxe( ROTATE_Z, "./model/ax1.3DS", 10.0, 30.0, 325.0, 0.0, 0.0, 0.0, m_axis[1], 0.0, 0.0, 0.0 );*/

    m_base = new CGLRobotAxe( ROTATE_X, "./model/base.3DS", 0.0, 0.0, 0.0, -90.0, 0.0, 90.0, m_axis[0], 0.0, 0.0, 0.0 );

    m_base->setColor( 0.9, 0.25, 0.0 );
    m_axis[0]->setColor( 1.0, 0.35, 0.0 );
    m_axis[1]->setColor( 1.0, 0.35, 0.0 );
    m_axis[2]->setColor( 1.0, 0.35, 0.0 );
    m_axis[3]->setColor( 1.0, 0.35, 0.0 );
    m_axis[4]->setColor( 1.0, 0.35, 0.0 );
    m_axis[5]->setColor( 0.0, 0.0, 0.0 );
}

CGLRobotAxe* CGLRobot::getRobotAxe( int index ){
    return m_axis[index];
}

void CGLRobot::render(){
    glPushMatrix();

        m_base->render();
        //m_axis[0]->render();

    glPopMatrix();
}

