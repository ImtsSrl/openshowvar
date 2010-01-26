/***************************************************************************
 *   Copyright (C) 2007 by Massimiliano Fago   *
 *   massimiliano.fago@gmail.com   *
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
 
#ifndef ROBOTVAREDIT_H
#define ROBOTVAREDIT_H

#include <QtGui>
#include <QHostAddress>

#include "kukavar.h"

#define MAX_WIDGET_IN_EDIT_VAR 40

class RobotVarEdit : public QDialog
{
	Q_OBJECT
			
	public:
                RobotVarEdit(const QByteArray& variabile, const QByteArray& varname, const QHostAddress varip, QWidget *parent = 0);
		~RobotVarEdit();
		
	private:

		KukaVar *robotvar;
                QWidget *widget[MAX_WIDGET_IN_EDIT_VAR];
                QLabel *label[MAX_WIDGET_IN_EDIT_VAR];
                QSignalMapper* mapper[MAX_WIDGET_IN_EDIT_VAR];
                QHostAddress varip;
		
		void on_Accept();
                void addInt(int widgetindex, QByteArray varName, int value);
                void addBool(int widgetindex, QByteArray varName, QByteArray value);
                void addReal(int widgetindex, QByteArray varName, double value);
                void addChar(int widgetindex, QByteArray varName, QByteArray value);
                void addStructure(int widgetindex,int value);

                void addMap(int widgetIndex, int mapIndex,int tipodato);
		
	private slots:
		void on_okButton();
		void on_abortButton();
		void on_namedLineEdit_textChanged(const QString &text);
		void on_Changed(int index);
		
	signals:
		void writevalue(const QByteArray &, const QByteArray &, const QHostAddress &);
};

#endif
