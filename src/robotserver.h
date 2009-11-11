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

#ifndef ROBOTSERVER_H
#define ROBOTSERVER_H

#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QTableWidget>
#include <QHostAddress>
#include <QList>

#include "broadcast.h"

class QString;

class RobotServer : public QWidget
{
	Q_OBJECT

	public:
		RobotServer(QWidget *parent = 0);
		~RobotServer();
		
		void addRobot(const QList<QByteArray> robotName);
		
	private:
		QTableWidget *tableWidget;
		Broadcast* broadcast;
		
	private slots:
		void cellDoubleClicked ( int row, int column );
		void closeEvent ( QCloseEvent * event );
		void Robot(QList<QByteArray> &datirobot);
		
	signals:
		void connectToRobot(QHostAddress serverip);
};

#endif
