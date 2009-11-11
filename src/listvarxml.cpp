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
 *	\brief Salvataggio lista variabili in formato xml
 *
 *	Consente di salvare la lista delle variabili attualmente in lettura
 *	su di un file. Questo consente l'inserimento automatico nella lista
 *	alla prossima apertura del programma.
 */

#include "listvarxml.h"

/*!	\brief Costruttore
 *
 *	Inizializza la classe xml
 *
 */

ListVarXml::ListVarXml(){
	
}

ListVarXml::~ListVarXml(){
	
}
/*
/*!	\brief Scrittura lista variabili
 *
 *	Scrive su file la lista delle variabili e i relativi ip di provenienza nel seguente
 *	formato:
 *	<VARLIST>
 *		<variable>
 *   			<name>$OV_PRO</name>
 *   			<robot>192.168.0.1</robot>
 *		</variable>
 *		<variable>
 *   			<name>$POS_ACT</name>
 *   			<robot>192.168.0.1</robot>
 *		</variable>
 *	</VARLIST>
 *
 *	\param tree Lista delle variabili
 */

void ListVarXml::writeList(QTreeWidget *tree){
	QDomDocument doc;
	
	//if(tree->topLevelItemCount()){
	
		QDomElement variablelist = doc.createElement("VARLIST");
		doc.appendChild(variablelist);
		
		for(int row=0;row<tree->topLevelItemCount();row++)
		{
			QDomElement variable = doc.createElement("VARIABLE");
			QDomElement varname = doc.createElement("NAME");
			QDomElement robotip = doc.createElement("ROBOT");
			QDomText var = doc.createTextNode(tree->topLevelItem(row)->text(0));
			QDomText ip = doc.createTextNode(tree->topLevelItem(row)->text(4));
			variablelist.appendChild(variable);
			variable.appendChild(varname);
			variable.appendChild(robotip);
			varname.appendChild(var);
			robotip.appendChild(ip);
		}
		
		QFile file(FILENAME);
		QTextStream out(&file);
		file.open(QIODevice::WriteOnly);
		const int Indent=4;
		doc.save(out, Indent);
		file.close();
	//}
}

void ListVarXml::readList(){
	QDomDocument doc;
	
	QFile file(FILENAME);
	file.open(QIODevice::ReadOnly);
	
	doc.setContent(&file);
	QDomElement root = doc.documentElement();
	
	QDomNode node = root.firstChild();
	
	while (!node.isNull()) {
		QDomElement e = node.toElement(); // try to convert the node to an element.
		if(!e.isNull()) {
			if(e.toElement().tagName()=="VARIABLE")
				estraiVariabile(e.toElement());
		}
		node = node.nextSibling();
	}
	
	file.close();
}

void ListVarXml::estraiVariabile(const QDomElement &element){
	QDomNode node = element.firstChild();
	QString variable, robot;
	
	while(!node.isNull()){
		QDomElement e = node.toElement();
		variable = e.text();
		node = node.nextSibling();
		e = node.toElement();
		robot = e.text();
		node = node.nextSibling();
		emit insertNewVar(variable, robot);
	}
	
}
