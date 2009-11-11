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

/*!	\file
 *	\brief Finestra di debug
 *
 *	Serve a visualizzare eventuali informazioni di debug. Utile per scovare
 *	i problemi su windows
 *
 */

#include "CDebug.h"

/*!	\brief Costruttore
 *
 *	Inizializza la finestra di debug
 *
 */

CDebug::CDebug(QWidget *parent)
	: QWidget()
{	
	setWindowTitle("Finestra di debug");
	resize(300,200);
	
	layout = new QVBoxLayout;
}

CDebug::~CDebug()
{
}

void CDebug::addDebug(QString debugMsg){
	QLabel* labelDebug = new QLabel(debugMsg);
	
	layout->addWidget(labelDebug);
	
	setLayout(layout);
}
