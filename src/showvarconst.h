//
// C++ Interface: showvarconst
//
// Description: 
//
//
// Author: Massimiliano Fago <massimiliano.fago@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QByteArray>

#ifndef SHOWVARCONST_H
#define SHOWVARCONST_H
const int BROADCASTPORT=6999;
const int UDPBROADCASTPORTCROSS=7000;
const int TCPCROSSCOMMPORT=7000;
const int BROADCASTMESSAGETIME=3000;

const int READTIMEOUT=1000; //ms
const int WRITETIMEOUT=200; //ms
const int REFRESHTIME=1000; //ms

const int MAXROBOTSERVER=10; //numero massimo di robot concessi sulla rete
const int MAXBROADCASTNETWORK=9; //numero massimo di schede di rete su cui effettuare il broadcast

const char READVARIABLE=0;
const char WRITEVARIABLE=1;

const int NUMBYTEINT=3;

enum RobotVarType {
	ERRTYPE,	/*!< Tipo di dato errato. */
	STRUCTURE,	/*!< Tipo di dato struttura. */
	INT,		/*!< Tipo di dato intero. */
	REAL,		/*!< Tipo di dato reale. */
	BOOL,		/*!< Tipo di dato booleano. */
	CHAR		/*!< Tipo di dato char. */
};

#define DEBUGWIN 0;

#endif
