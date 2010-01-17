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

#ifndef VARIABLEDB_H
#define VARIABLEDB_H

#include <QObject>
#include <QList>

#include "showvarconst.h"
#include "clientcross.h"

class KukaVar;

class VariableDB : public QObject
{
	Q_OBJECT
	
	public:
		VariableDB();
		~VariableDB();
		
		bool readVar(QByteArray varname, QHostAddress robotip, QByteArray* varvalue, int* readtime);
		bool writeVar(QByteArray varname, QHostAddress robotip, QByteArray varvalue, int* writetime);
		void addVar(QByteArray varname, QHostAddress robotip);
		void deleteVar(QByteArray varname, QHostAddress robotip);
		void renameVar(QByteArray varname, QByteArray newvarname, QHostAddress robotip);
		void setReadTime(QHostAddress robotip, int* readtime);
                int  getRobotNumber();
                void setAllReadTime(int* readtime);
		
	private:
		QList<RobotVar *> listvar;
		ClientCross* robotServer[MAXROBOTSERVER];
		
		void startNewThread(QHostAddress robotip);
		int getThreadIndex(QHostAddress robotip);
};
#endif
