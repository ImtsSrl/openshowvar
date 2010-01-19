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
 
#ifndef BROADCAST_H
#define BROADCAST_H

#include <QUdpSocket>
#include <QNetworkInterface>

#include <QSignalMapper>

#include "CDebug.h"
#include "showvarconst.h"

class QNetworkAddressEntry;

class Broadcast : public QUdpSocket
{
	Q_OBJECT

	public:
		Broadcast(QWidget *parent = 0);
		~Broadcast();
		
		void send();
		
#ifdef DEBUGWIN
		CDebug* debugWindow;
#endif
		
	private:
                QNetworkInterface networkInterface;
                QList<QNetworkAddressEntry> address;
		QUdpSocket* privateBind[MAXBROADCASTNETWORK];
                QNetworkAddressEntry broadcastAddress[MAXBROADCASTNETWORK];
                int interfaceCount;
                int initBroadcastNetwork();
		
	private slots:
		void processPending(int index);
		
	signals:
		void addRobot(QList<QByteArray> &);
		void newMsg(QString &);
};

#endif
