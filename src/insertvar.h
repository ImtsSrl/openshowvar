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
 
#ifndef INSERTVAR_H
#define INSERTVAR_H

#include <QtGui>
#include <QList>

#include "broadcast.h"

class QPushButton;
class QByteArray;
class QComboBox;

class InsertVar : public QDialog
{
	Q_OBJECT
	
public:
	InsertVar();
	~InsertVar();
	void DropVar(QString varName);
	
	QLabel* varLabel, *labelRobot;
	QLineEdit* lineEdit;
	QPushButton* insertButton;
	QPushButton* abortButton;
	
private:
	QComboBox* comboRobotList;
	Broadcast* broadcast;

        QTimer qtimeBroadcast;
	
private slots:
	void on_insertButton_clicked();
	void on_lineEdit_textChanged();
        void on_lineEdit_returnPressed();
	void on_abortButton_clicked();
        void on_Broadcast();
	void Robot(QList<QByteArray> &datirobot);
	void closeEvent(QCloseEvent * event);
	
	void newMsg(QString & msg);
	
signals:
	void insertNewVar(const QString &str, const QString &iprobot);
	void insertClose();
	
};

#endif
