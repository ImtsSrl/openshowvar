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
 *	\brief Variabile generica robot
 *
 *	Contiene tutte le informazioni necessarie per la lettura delle 
 *	variabili dal robot.
 *	Tra le varie informazioni vi e' anche l'ip di provenienza della
 *	variabile, questo perche' possono essere mappate variabili provenienti
 *	da piu' robot presenti sulla rete.
 */

#include "robotvar.h"

/*!	\brief Costruttore
 *
 *	Crea e inizializza la variabile robot
 *
 *	\param varname Nome della variabile da leggere
 *	\param ip Indirizzo robot di provenienza della variabile
 */

RobotVar::RobotVar(const QByteArray varname, const QHostAddress ip) : KukaVar(varname)
{
	variablename=varname;
	robotip=ip;
	readtime=-1;
}

/*!	\brief Distruttore
 *
 */

RobotVar::~RobotVar()
{
	
}

/*!	\brief Impostazione nome variabile
 *
 *	Consente di cambiare al volo il nome della variabile.
 *
 *	\param varname Nome variabile
 */

void RobotVar::setVarName(const QByteArray varname){
	variablename = varname;
}

/*!	\brief Impostazione nuovo valore variabile
 *
 *	Imposta il nuovo valore da scrivere sul robot
 *
 *	\param varvalue Valore variabile
 */

void RobotVar::setNewValue(const QByteArray varvalue)
{
	newvalue=true;
	newvariablevalue = varvalue;
}

/*!	\brief Interrogazione di nuovo valore variabile
 *
 *	Verifica la presenza di un nuovo valore della variabile da
 *	scrivere sul robot
 *
 *	\param varvalue Valore variabile
 *	\return True se c'e' un nuovo valore da scrivere
 */

bool RobotVar::getNewValue(QByteArray* varvalue){
	if(newvalue){
		*varvalue = newvariablevalue;
		newvalue=false;
		return true;
	}
	else{
		varvalue = NULL;
		return false;
	}
}

/*!	\brief Riceve il tempo di lettura
 *
 *	Restituisce il tempo di lettura della variabile in ms
 *
 *	\return Valore tempo di lettura
 */

int RobotVar::getReadTime()
{
	return readtime;
}

/*!	\brief Impostazione tempo di lettura
 *
 *	Imposta il tempo di lettura della variabile in ms
 *
 *	\param varreadtime Valore tempo di lettura
 */

void RobotVar::setReadTime(const int varreadtime)
{
	readtime = varreadtime;
}

/*!	\brief Lettura nome variabile
 *
 *	Restituisce il nome della variabile
 *
 *	\return nome della variabile
 */

QByteArray RobotVar::getVarName()
{
	return variablename;
}

/*!	\brief Lettura indirizzo ip robot
 *
 */

QHostAddress RobotVar::getRobotIP()
{
	return robotip;
}
