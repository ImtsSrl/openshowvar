/***************************************************************************
 *   Copyright (C) 2007 by Massimiliano Fago                               *
 *   massimiliano.fago@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CLIENTCROSS_H
#define CLIENTCROSS_H

#include <QObject>
#include <QList>
#include <QThread>
#include <QTcpSocket>
#include <QMutex>
//#include <QWaitCondition>
#include <QTime>

#include "robotvar.h"
#include "showvarconst.h"

class ClientCross : public QThread
{
	Q_OBJECT	
	
	public:
                ClientCross(const QHostAddress* address, int readtime = 1000, quint16 port = 7000);
		~ClientCross();
		
		void stop();
		QByteArray read(int varindex);
		QByteArray read(QByteArray varname);
		void add(RobotVar* newvar);
		void delVar(int index);
		QHostAddress getSocketIP();
		
		void setReadFreq(int* readfreq);
		
		QMutex mutex;
		
	protected:
		void run();
		
	private:
		volatile bool stopped;
		QHostAddress robotip;
		quint16 robotport;
		int readfreq;
		int threadarrayindex;
                unsigned short idmsgtosend,idreadmsg; //ID messaggio da inviare
		QTime readtime;
		
		QList<RobotVar *> listvar;
		QByteArray formatMsg(QByteArray msg);
		QByteArray formatMsg(QByteArray msg, QByteArray value);
		QByteArray clearMsg(QByteArray msg);
		
		bool readConsistency(const QByteArray request, const QByteArray earn);
};
#endif
