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
 *	\class KukaVar
 *	\author Massimiliano Fago
 *	\version 1.1
 *	\date 2009
 *	\brief Determinazione tipo variabile
 *
 *	Sui robot kuka possiamo trovare 4 tipi fondamentali:
 *	- BOOL
 *	- INT
 *	- REAL
 *	- CHAR
 *
 *	e un tipo composto:
 *	- STRUCTURE
 *
 *	il tipo STRUCTURE e' un tipo composto dai tipi fondamentali.
 *	Qui di seguito riporto un esempio di struttura, in particolare la struttura E6POS del
 *	robot KUKA
 *	
 *	{E6POS: X -39.86226, Y 1601.820, Z 39.84885, A -104.2217, B 89.98643, C 75.77992, S 2, 
 *	T 3, E1 0.0, E2 0.0, E3 0.0, E4 0.0, E5 0.0, E6 0.0}
 *
 *      Un tipo struttura puo' contenere altre strutture. Questo avviene per esempio per la variabile
 *      $PRO_IP
 */

#include "kukavar.h"

/*!	\brief Costruttore
*	
*	Determina il tipo di dato contenuto nella variabile.
*
*	\param varvalue Contenuto della variabile
*	\param varname Nome della variabile
*/

KukaVar::KukaVar(const QByteArray* varname, const QByteArray* varvalue)
{	
	this->varname=*varname;
	this->varvalue=*varvalue;
	
	setValue(this->varvalue);
}

KukaVar::KukaVar(const QByteArray varname)
{
	qDebug() << "Costruttore KukaVar: " << varname;
}

KukaVar::~KukaVar()
{

}

/*!	\brief Restituisce il tipo di dato
 *	
 *	Determina il tipo di dato contenuto nella variabile
 *
 *	\param variabile Contenuto della variabile da identificare
 *	\return Una variabile di tipo intero contenente la codifica
 *		del tipo di dato
 */

int KukaVar::VarType(const QByteArray variabile)
{
	if(variabile.startsWith("{") && variabile.endsWith("}"))
		return STRUCTURE;
	
	if((variabile.toLong()) || (variabile.data()==QByteArray("0")))
		return INT;
	
	if((variabile.toFloat()) || (variabile.data()==QByteArray("0.0")))
		return REAL;
	
	if((variabile.data()==QByteArray("TRUE")) || (variabile.data()==QByteArray("FALSE")))
		return BOOL;
	
	//if((!variabile.toLong() && variabile.size()==1))
	if(variabile.endsWith("]"))
		return CHAR;
	
	return ERRTYPE;
}

int KukaVar::getVarType()
{
	return intvartype;
}

QByteArray KukaVar::getVarTypeName()
{
	return vartype;
}

int KukaVar::getElementsNumber()
{
	return elementsnumber;
}

QByteArray KukaVar::getVarName()
{
	return varname;
}

QByteArray KukaVar::getStructureName()
{
	return structurename;
}

//TODO Restituire per valore anche il tipo di dato
QByteArray KukaVar::getStructureValue(const int &fieldposition, int &datatype)
{
	QList<QByteArray> field = arrayvalue[fieldposition].split(' ');
	QByteArray *fieldelement = new QByteArray(field[1]);
	//qDebug() << "Vartype: " << VarType(fieldelement->trimmed());
	datatype = VarType(fieldelement->trimmed());
	return fieldelement->trimmed();
}

QByteArray KukaVar::getStructureValue(const QByteArray *fieldname, int &datatype)
{
	if(elements.contains(fieldname->data())){
		datatype = VarType(elements.value(fieldname->data()));
		return elements.value(fieldname->data());
	}else
		return NULL;
}

QByteArray KukaVar::getStructureMember(const int &fieldposition)
{
	QList<QByteArray> field = arrayvalue[fieldposition].split(' ');
        qDebug() << "Contenuto di fieldposition " << arrayvalue[fieldposition];
	QByteArray fieldelement(field[0]);
	return fieldelement.trimmed();
}

QByteArray KukaVar::getStructureElement(const int &elementposition)
{
	return arrayvalue[elementposition];
}

QByteArray KukaVar::getValue()
{
	return varvalue;
}

QByteArray KukaVar::getNewValue()
{
	return newvarvalue;
}

void KukaVar::setFieldValue(const QByteArray &value,const int &fieldposition)
{
	newarrayvalue[fieldposition]=getStructureMember(fieldposition) + " " + value;
}

/*!	\brief Imposta il valore della variabile
 *	
 *	Imposta il valore della variabile letta da robot
 *
 *	\param varvalue Valore variabile
 *
 */

void KukaVar::setValue(QByteArray varvalue)
{
	this->varvalue=varvalue;
	
	intvartype=VarType(varvalue.data());
	
	switch(intvartype){
		case STRUCTURE:
		{
			//verify the start position
			int starttype=varvalue.indexOf("{");
			int stoptype=varvalue.indexOf(":");
		
			int lenvartype=stoptype-starttype-1;
			structurename=varvalue.mid(starttype+1,lenvartype);
		
			//Pulizia stringa da parte iniziale e finale, comprese le parentesi graffe
			int lenstructure=varvalue.size()-(stoptype+2)-1;
			value=varvalue.mid(stoptype+2,lenstructure);
			
			/*
			Se la variabile non e' impostata nel config del robot, ritorna una struttura
			con i campi vuoti, tipo questa:
			{E6POS: }
			con uno o più spazi dopo il carattere ":" prima della parentesi graffa di chiusura
			*/
			value.trimmed();
			if(!value.isEmpty()){
				//separazione dei campi e valori in una lista
				arrayvalue=value.split(',');
				elementsnumber=arrayvalue.count();
				for(int i=0;i<arrayvalue.count();i++)
				{
					QByteArray trimelement(arrayvalue[i]);
					arrayvalue[i] = trimelement.trimmed();
					QList<QByteArray> field = arrayvalue[i].split(' ');
					elements.insert(field[0], field[1]);
				}
			
				newarrayvalue=arrayvalue;
			}
			else{
				elementsnumber=0;
			}
			//MFA 01/02/2008
			newvarvalue=this->varvalue;
			break;
		}		
		case INT:
		{
			elementsnumber=1;
			
			newvarvalue=this->varvalue;
			break;
		}		
		case REAL:
		{
			elementsnumber=1;
			
			newvarvalue=this->varvalue;
			break;
		}		
		case BOOL:
		{
			elementsnumber=1;
		
			newvarvalue=this->varvalue;
			break;
		}
		case CHAR:
		{
			elementsnumber=1;
		
			newvarvalue=this->varvalue;
			break;
		}
		default:
		{
			break;
		}
	}
}

QByteArray KukaVar::createStructure()
{
	QByteArray retvar;
	
	retvar += "{" + structurename + ": " + newarrayvalue[0];
	for(int i=1;i<newarrayvalue.size();i++)
		retvar += ", " + newarrayvalue[i];
	retvar += "}";
	
	return retvar;
}
