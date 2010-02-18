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

KukaVar::KukaVar(const QByteArray& varname, const QByteArray& varvalue)
{
        this->varname=varname;
        this->varvalue=varvalue;
	
        setValue(this->varvalue);
}

KukaVar::KukaVar(const QByteArray varname)
{
        //qDebug() << "Costruttore KukaVar: " << varname;
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
    //qDebug() << "Variabile " << variabile;
    if(variabile.startsWith("{") && variabile.endsWith("}"))
        return STRUCTURE;

    if((variabile.toLong()) || (variabile.data()==QByteArray("0")))
        return INT;

    if((variabile.toFloat()) || (variabile.data()==QByteArray("0.0")))
        return REAL;

    if((variabile.data()==QByteArray("TRUE")) || (variabile.data()==QByteArray("FALSE")))
        return BOOL;

    if((!variabile.toLong() && variabile.size()>=1)){
        //if(variabile.endsWith("]"))
        return CHAR;
    }

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
        QByteArray *fieldelement = new QByteArray(arrayvalue[fieldposition].right(arrayvalue[fieldposition].length()-arrayvalue[fieldposition].indexOf(' ')));
        //qDebug() << "Vartype: " << fieldelement->trimmed();
        //qDebug() << "Vartype: " << VarType(fieldelement->trimmed());
        datatype = VarType(fieldelement->trimmed());
        return *fieldelement;
}

QByteArray KukaVar::getStructureValue(const QByteArray *fieldname, int &datatype)
{
	if(elements.contains(fieldname->data())){
		datatype = VarType(elements.value(fieldname->data()));
		return elements.value(fieldname->data());
	}else
		return NULL;
}

/*! \brief Contenuto struttura
 *
 *  Ritorna il valore della struttura pulito dal tipo di struttura
 *
 */

QByteArray KukaVar::getStructureValue()
{
        return structurevalue;
}

/*! \brief Prende il nome di un membro della struttura
 *
 *  Imposta il valore della variabile letta da robot
 *
 *  \param fieldposition indice del campo
 *
 *  Riceve l'indice della posizione del campo e restituisce il nome della variabile
 *
 */

QByteArray KukaVar::getStructureMember(const int &fieldposition)
{
    QList<QByteArray> field = arrayvalue[fieldposition].split(' ');
    //qDebug() << "Contenuto di fieldposition " << arrayvalue[fieldposition];
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

/*! \brief Imposta il valore della variabile
 *	
 *  Imposta il valore della variabile letta da robot
 *
 *  \param varvalue Valore variabile
 *
 *  Nel caso di struttura il discorso e' piu' complesso. Le strutture possono prevedere
 *  altre strutture annidate a piu' livelli, per questo il metodo di separazione con split
 *  non puo' andare bene. Questa funzione deve separare i campi della struttura principale
 *  e non intaccare le strutture sottostanti che devono essere riportate alla stessa maniera
 *  Una chiamata ricorsiva di questa struttura provvedera' a dividere le altre strutture
 *  eventualmente presenti
 */

void KukaVar::setValue(QByteArray varvalue)
{
    QByteArray test;

    this->varvalue=varvalue;
	
    intvartype=VarType(varvalue.data());

    //qDebug() << "varvalue in kukavar " << this->varvalue;
    //qDebug() << "lunghezza in kukavar" << this->varvalue.length();

    switch(intvartype){
    case KukaVar::STRUCTURE:
        {
            //verify the start position
            int starttype=varvalue.indexOf("{");
            int stoptype=varvalue.indexOf(":");

            int lenvartype=stoptype-starttype-1;
            structurename=varvalue.mid(starttype+1,lenvartype);

            //Pulizia stringa da parte iniziale e finale, comprese le parentesi graffe
            int lenstructure=varvalue.size()-(stoptype+2)-1;
            test=varvalue.mid(stoptype+2,lenstructure);

            /*
            Se la variabile non e' impostata nel config del robot, ritorna una struttura
            con i campi vuoti, tipo questa:
            {E6POS: }
            con uno o piu' spazi dopo il carattere ":" prima della parentesi graffa di chiusura
            */

            test=test.trimmed();
            structurevalue=structurevalue.append('{').append(test).append('}');

            if(!test.isEmpty()){
                //separazione dei campi e valori in una lista

                int intstruct=0,initpos=0;
                bool finestruttura=false;
                QByteArray varnameandvalue;
                arrayvalue.clear();

                //qDebug() << "valore di test" << test << "lunghezza di test " << test.length();
                for(int charpos=0;charpos<test.length();charpos++)
                {
                    switch(test[charpos])
                    {
                    case ',':
                        {
                            if(intstruct==0){
                                varnameandvalue=test.mid(initpos,charpos-initpos);
                                if(!finestruttura)
                                {
                                    QList<QByteArray> field = varnameandvalue.split(' ');
                                    elements.insert(field[0], field[1]);
                                    initpos=charpos+2;

                                    arrayvalue.append(varnameandvalue);
                                    //qDebug() << "Variabile: " << field[0] << " valore " << field[1];
                                }
                                finestruttura=false;
                            }
                            else{
                                //qDebug() << "Campo interno alla struttura";
                            }
                            break;
                        }
                    case '{':
                        {
                            intstruct++;
                            qDebug() << "Trovata nuova struttura";
                            break;
                        }
                    case '}':
                        {
                            if(intstruct==1){
                                varnameandvalue=test.mid(initpos,charpos-initpos+1);
                                int posstartstr=varnameandvalue.indexOf('{');
                                QList<QByteArray> field;
                                field.append(varnameandvalue.mid(0,posstartstr-1));
                                field.append(varnameandvalue.mid(posstartstr,charpos-posstartstr));
                                elements.insert(field[0], field[1]);
                                initpos=charpos+3;

                                arrayvalue.append(varnameandvalue);
                                finestruttura=true;
                                qDebug() << "Variabile: " << field[0] << " valore " << field[1];
                                qDebug() << "Chiusura struttura aperta";
                            }
                            else
                                qDebug() << "Chiusura struttura non aperta";
                                intstruct--;
                            break;
                        }
                    default:
                        break;
                    }
                }

                if(initpos<test.length()){
                    varnameandvalue=test.mid(initpos,test.length()-initpos);
                    QList<QByteArray> field = varnameandvalue.split(' ');
                    elements.insert(field[0], field[1]);

                    arrayvalue.append(varnameandvalue);
                    qDebug() << "Variabile: " << field[0] << " valore " << field[1];
                }

            }
            else{
                elementsnumber=0;
                //qDebug() << "Struttura vuota";
            }

            newarrayvalue=arrayvalue;
            elementsnumber=newarrayvalue.size();
            //qDebug() << "numero elementi array " << elementsnumber;
            //qDebug() << "Array " << newarrayvalue << " valori " << elementsnumber;
            //qDebug() << "Elements " << elements;

            newvarvalue=this->varvalue;
            break;
        }
    case INT:
        {
            elementsnumber=1;
            vartype="INT";

            newvarvalue=this->varvalue;
            break;
        }
    case REAL:
        {
            elementsnumber=1;
            vartype="REAL";

            newvarvalue=this->varvalue;
            break;
        }
    case BOOL:
        {
            elementsnumber=1;
            vartype="BOOL";

            newvarvalue=this->varvalue;
            break;
        }
    case CHAR:
        {
            elementsnumber=1;
            vartype="CHAR";

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
	
        qDebug() << retvar;
	return retvar;
}
