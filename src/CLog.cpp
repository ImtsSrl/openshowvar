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
 *	\brief Salvataggio log valori in formato xml
 *
 *	Consente il salvataggio di un log delle variabili lette dal robot
 *	e visualizzate nella lista di openshowvar.
 *	Lo step di archiviazione e' pari all'intervallo di visualizzazione.
 *	
 */

#include "CLog.h"

/*!	\brief Costruttore
 *
 *	Inizializza la classe xml
 *
 */

CLog::CLog(){
	
}

CLog::~CLog(){
	
}

/*!	\brief Scrittura lista variabili
 *
 *	Scrive su file la lista delle variabili e i relativi ip di provenienza nel seguente
 *	formato:
 *	<LOG DATA="sab mar 22 10:49:08 2008" >
 *   		<VAR NAME="$OV_PRO">
 *			<VALORE>50</VALORE>
 *			<TEMPOLETTURA>10 [ms]</TEMPOLETTURA>
 *			<IPROBOT>192.168.0.1</IPROBOT>
 *		</VARNAME>
 *	</LOG>
 *
 *	\param tree Lista delle variabili
 */

void CLog::writeList(QTreeWidget *tree){
	QDomDocument doc;
	
	//if(tree->topLevelItemCount()){
	QDateTime tempolettura;
	
	QDomElement variablelist = doc.createElement("LOG");
	variablelist.setAttribute("DATA", tempolettura.currentDateTime().toString());
	doc.appendChild(variablelist);
		
	for(int row=0;row<tree->topLevelItemCount();row++)
	{
		QDomElement variable = doc.createElement("VAR");
		variable.setAttribute("NAME",tree->topLevelItem(row)->text(0));
		QDomElement varvalue = doc.createElement("VALUE");
		QDomElement readtime = doc.createElement("READTIME");
		QDomElement robotip = doc.createElement("ROBOT");
		QDomText var = doc.createTextNode(tree->topLevelItem(row)->text(1));
		QDomText time = doc.createTextNode(tree->topLevelItem(row)->text(3));
		QDomText ip = doc.createTextNode(tree->topLevelItem(row)->text(4));
		variablelist.appendChild(variable);
		variable.appendChild(varvalue);
		variable.appendChild(readtime);
		variable.appendChild(robotip);
		varvalue.appendChild(var);
		readtime.appendChild(time);
		robotip.appendChild(ip);
	}
		
	QFile file(FILENAMELOG);
	QTextStream out(&file);
	file.open(QIODevice::Append);
	//file.open(QIODevice::WriteOnly);
	const int Indent=4;
	doc.save(out, Indent);
	file.close();
	//}
}
