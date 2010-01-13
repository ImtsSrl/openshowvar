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

#ifndef KUKAVAR_H
#define KUKAVAR_H

#include <QByteArray>
#include <QList>
#include <QObject>
#include <QHash>
#include <QDebug>

#include "showvarconst.h"

class KukaVar : public QObject
{
	Q_OBJECT

	friend class RobotVarEdit;
	
	public:
		KukaVar(const QByteArray* varname, const QByteArray* varvalue);
		KukaVar(const QByteArray varname);
		~KukaVar();
		
		int getVarType();
		int getElementsNumber();
		QByteArray getVarName();
		QByteArray getStructureName();
		QByteArray getStructureValue(const int &fieldposition,int &datatype);
		QByteArray getStructureValue(const QByteArray *fieldname, int &datatype);
                QByteArray getStructureValue();
		QByteArray getStructureMember(const int &fieldposition);
		QByteArray getStructureElement(const int &fieldposition);
		QByteArray getNewValue();
		QByteArray getVarTypeName();
		QByteArray createStructure();
		
		void setFieldValue(const QByteArray &value, const int &fieldposition);
		
		QByteArray getValue();

                static const char ERRTYPE=0,STRUCTURE=1,INT=2,REAL=3,BOOL=4,CHAR=5;

	protected:
                void setValue(QByteArray varvalue);
		
	private:
		int elementsnumber, intvartype;
                QByteArray vartype,value,varname,varvalue,structurename,newvarvalue,structurevalue;
		QList<QByteArray> arrayvalue,newarrayvalue;
		QHash<QByteArray, QByteArray> elements;
		
		int VarType(const QByteArray variabile);
};

#endif
