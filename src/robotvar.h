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

#ifndef ROBOTVAR_H
#define ROBOTVAR_H

#include <QObject>
#include <QHostAddress>

#include "kukavar.h"

class RobotVar : public KukaVar
{
	Q_OBJECT
			
	friend class ClientCross;
			
	public:
		RobotVar(const QByteArray varname, const QHostAddress ip);
		~RobotVar();
		
		void setVarName(const QByteArray varname);
		void setNewValue(const QByteArray varvalue);
		bool getNewValue(QByteArray* varvalue);
		
		QByteArray getVarName();
		//QByteArray getValue();
		QHostAddress getRobotIP();
		
		int getReadTime();
		
	protected:
		//void setValue(const QByteArray* varvalue);
		void setReadTime(const int varreadtime);
		
	private:
		QByteArray variablename,variablevalue,newvariablevalue;
		QHostAddress robotip;
		
		int readtime;
		bool newvalue;
};

#endif
