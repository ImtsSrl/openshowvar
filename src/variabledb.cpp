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
 *	\class VariableDB
 *	\author Massimiliano Fago
 *	\version 1.0
 *	\date 2007
 *	\brief Database variabili
 *
 *	Genera e mantiene aggionato un database di variabili lette dai 
 *	robot.
 *	Crea n istanze della classe clientcross, una per robot.
 *	Ogni variabile contiene anche il tempo di lettura della stessa.
 *
 */

#include "variabledb.h"

/*!	\brief Costruttore
 *
 *	Inizializza il thread di lettura delle variabili
 *
 */

VariableDB::VariableDB()
{
	memset(robotServer,0,MAXROBOTSERVER * sizeof(ClientCross*));
}

/*!	\brief Distruttore
 *
 *	Arresta tutti i thread in esecuzione
 *
 */

VariableDB::~VariableDB()
{
	for(int i=0;i<MAXROBOTSERVER;i++)
		if(robotServer[i]!=NULL){
			robotServer[i]->stop();
			delete robotServer[i];
		}
}

/*!	\brief Lettura di una variabile
 *
 *	Restituisce il valore attuale della variabile.
 *
 *	\param varname Nome della Variabile
 *	\param robotip Indirizzo ip del robot
 *	\param varvalue Valore della variabile
 *	\param readtime Tempo di lettura variabile
 *	\return True o False a seconda se la variabile e' stata trovata o meno nella lista
 */

bool VariableDB::readVar(QByteArray varname, QHostAddress robotip, QByteArray* varvalue, int* readtime)
{
    for(int i=0;i<listvar.count();i++){
        //qDebug() << "Indice: " << i << " robotip: " << listvar[i]->getRobotIP() << " nome variabile: " << listvar[i]->getVarName() << " valore variabile: " << listvar[i]->getValue();
        if(listvar[i]->getRobotIP() == robotip && listvar[i]->getVarName() == varname){
            int threadid=getThreadIndex(robotip);
            robotServer[threadid]->mutex.lock();
            *readtime = listvar[i]->getReadTime();
            *varvalue = listvar[i]->getValue();
            robotServer[threadid]->mutex.unlock();
            return true;
        }
    }
    return false;
}

/*!	\brief Scrittura di una variabile
 *
 *	Restituisce il valore attuale della variabile.
 *
 *	\param varname Nome della Variabile
 *	\param robotip Indirizzo ip del robot
 *	\param varvalue Valore della variabile
 *	\param writetime Tempo di scrittura
 *	\return True o False a seconda se la variabile e' stata trovata o meno nella lista
 */

bool VariableDB::writeVar(QByteArray varname, QHostAddress robotip, QByteArray varvalue, int* writetime)
{
    for(int i=0;i<listvar.count();i++){
        if(listvar[i]->getRobotIP() == robotip && listvar[i]->getVarName() == varname){
            int threadid=getThreadIndex(robotip);
            robotServer[threadid]->mutex.lock();
            listvar[i]->setNewValue(varvalue);
            *writetime = listvar[i]->getReadTime();
            robotServer[threadid]->mutex.unlock();
            return true;
        }
    }
    return false;
}

/*!	\brief Aggiunta variabile da leggere
 *
 *	Questo metodo si occupa di aggiungere una nuova variabile alla lista delle
 *	variabili lette dai robot.
 *	Dopo aver aggiunto la variabile alla lista, verifica che ci sia gia' un thread
 *	in esecuzione per la lettura delle variabili dal rispettivo indirizzo. Se il
 *	thread e' gia' in esecuzione, la variabile viene aggiunta alla rispettiva lista
 *	in caso contrario viene creato un nuovo thread verso e aggiunta la variabile.
 *
 *	\param varname nome della variabile da leggere
 *	\param robotip indirizzo ip del robot
 */

void VariableDB::addVar(QByteArray varname, QHostAddress robotip){
	
	enum inserimentovariabile {NOACTION, ROBOTNONPRESENTE, VARIABILENONPRESENTE};
	int azione=0;
	
	RobotVar* newvar = new RobotVar(varname, robotip);
	
	int numvariabili=listvar.count();
	
	if(listvar.count()){
		for(int i=0;i<numvariabili;i++){
			if(listvar[i]->getRobotIP() == robotip && listvar[i]->getVarName() == varname){
				//variabile e ip presente
				azione=NOACTION;
				break;
			}
			else if(listvar[i]->getRobotIP() == robotip && listvar[i]->getVarName() != varname){
				//ip presente ma variabile no
				azione=VARIABILENONPRESENTE;
			}
			else{
				//ip non presente. Creo un socket con il robot e aggiungo
				//la variabile.
				azione=ROBOTNONPRESENTE;
			}
		}
	}
	else{
		//non è presente alcun elemento
		azione=ROBOTNONPRESENTE;
	}
	
	switch(azione){
		case ROBOTNONPRESENTE:
		{
			qDebug() << "Partito nuovo thread aggiunta nuova variabile";
			listvar.append(newvar);
			startNewThread(robotip);
			robotServer[getThreadIndex(robotip)]->add(newvar);
			break;
		}
		case VARIABILENONPRESENTE:
		{
			qDebug() << "Aggiunta nuova variabile";
			listvar.append(newvar);
			robotServer[getThreadIndex(robotip)]->add(newvar);
			break;
		}
		default:
		{
			qDebug() << "Nessuna azione";
			break;
		}
	}
}

/*!	\brief Rimuove una variabile dalla lista
 *
 *	Questo metodo si occupa di rimuovere una nuova variabile alla lista delle
 *	variabili lette dai robot.
 *	Dopo aver rimosso la variabile alla lista, bisogna verificare che il corrispondente
 *	thread contenga ancora qualche variabile. In caso contrario bisogna distruggere il
 *	thread.
 *
 *	\param varname nome della variabile da leggere
 *	\param robotip indirizzo ip del robot
 *
 */

void VariableDB::deleteVar(QByteArray varname, QHostAddress robotip){
    for(int varindex=0;varindex<listvar.count();varindex++){
        if(listvar[varindex]->getRobotIP() == robotip && listvar[varindex]->getVarName() == varname){

            /*listvar.removeAt(i);
			robotServer[getThreadIndex(robotip)]->delVar(i);
			*/
            qDebug() << "Rimozione variabile: " << varname << " " << robotip;

            //Verifico se il thread gestisce altre variabili
            bool ippresent=false;
            for(int i=0;i<listvar.count();i++){
                if(listvar[i]->getRobotIP() == robotip){
                    ippresent=true;
                }
            }
            if(!ippresent){
                int threadid = getThreadIndex(robotip);
                robotServer[threadid]->stop();
                delete robotServer[threadid];
                robotServer[threadid]=NULL;
            }

            listvar.removeAt(varindex);
            robotServer[getThreadIndex(robotip)]->delVar(varindex);

        }
    }
}

/*!	\brief Inizializzazione di un nuovo thread
 *
 *	Se ancora non e' presente alcun thread di lettura delle variabili verso questo
 *	indirizzo ip, la seguente funzione ne inizializza uno. aggiunta la variabile.
 *
 *	\param robotip nome della variabile da leggere
 */

void VariableDB::startNewThread(QHostAddress robotip)
{
    for(int i=0;i<MAXROBOTSERVER;i++){
        if(robotServer[i]==NULL){
            robotServer[i] = new ClientCross(&robotip,REFRESHTIME);
            robotServer[i]->start();
            break;
        }
    }
}

/*!	\brief Inizializzazione di un nuovo thread
 *
 *	Restituisce l'indice dell'array che gestisce la comunicazione per il dato ip.
 *
 *	\param robotip Indirizzo ip del robot
 *	\return Indice del thread
 */

int VariableDB::getThreadIndex(QHostAddress robotip){
	int threadindex=-1;
	for(int i=0;i<MAXROBOTSERVER;i++){
		if(robotServer[i]->getSocketIP()==robotip){
			threadindex = i;
			break;
		}
	}
	return threadindex;
}

/*!	\brief Rinomina una variabile
 *
 *	Consente di cambiare al volo il nome di una variabile.
 *
 *	\param varname Nome variabile
 *	\param newvarname Nuovo nome da assegnare alla variabile
 *	\param robotip Indirizzo ip del robot
 */

void VariableDB::renameVar(QByteArray varname, QByteArray newvarname, QHostAddress robotip){
	for(int i=0;i<listvar.count();i++){
		if(listvar[i]->getRobotIP() == robotip && listvar[i]->getVarName() == varname){
			listvar[i]->setVarName(newvarname);
		}
	}
}

/*!	\brief Tempo di aggiornamento variabili
 *
 *	Imposta il tempo di aggiornamento delle variabili per il robot specificato
 *
 *	\param robotip Indirizzo ip del robot
 *	\param readtime Nuovo tempo di lettura
 */

void VariableDB::setReadTime(QHostAddress robotip, int* readtime){
    robotServer[getThreadIndex(robotip)]->setReadFreq(readtime);
}

/*!	\brief Tempo di aggiornamento globale variabili
 *
 *	Imposta il tempo di aggiornamento delle variabili per tutti i robot
 *
 *	\param readtime Nuovo tempo di lettura
 */

void VariableDB::setAllReadTime(int* readtime)
{
    for(int i=0;i<MAXROBOTSERVER;i++)
        if(robotServer[i]!=NULL)
            robotServer[i]->setReadFreq(readtime);
}

/*!	\brief Numero di thread attivi
 *
 *	Restituisce il numero di thread attivi
 *
 */

int VariableDB::getRobotNumber()
{
    return 0;
}
