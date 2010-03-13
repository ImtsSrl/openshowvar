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
 *	\class ClientCross
 *	\author Massimiliano Fago
 *	\version 1.0
 *	\date 2007
 *	\brief Comunicazione TCP robot
 *
 *	Gestisce una connessione socket per robot.
 *	La classe Variabledb genera n istanze di questa classe, una per
 *	ciascun robot. Questa classe si occupa della comunicazione TCP
 *	in un thread.
 *
 */

#include "clientcross.h"

//QMutex ClientCross::mutex;

/*!	\brief Costruttore
 *
 *	Inizializza il thread di lettura delle variabili
 *
 *	\param address Indirizzo del robot a cui connettersi
 *	\param readtime Frequenza di aggiornamento delle variabili
 *	\param port Porta a cui connettersi
 */

ClientCross::ClientCross(const QHostAddress* address, int readtime, quint16 port) : QThread(), stopped(false)
{
	//QMutexLocker locker(&mutex);
	this->robotip = *address;
	this->readfreq = readtime;
	this->robotport = port;
}

/*!	\brief Distruttore
 *
 *	
 *
 */

ClientCross::~ClientCross()
{
	mutex.lock();
	stopped=true;
	wait();
	mutex.unlock();
}

/*!	\brief Processo thread
 *
 *	
 *
 */

void ClientCross::run()
{
	mutex.lock();
	QHostAddress serverName = robotip;
	quint16 serverPort = robotport;
	mutex.unlock();
	
	bool err;
	QByteArray vartowrite;
	QTcpSocket socket;
	
	const int Timeout = 5 * 1000;
        idmsgtosend=0;
	
	qDebug() << "Thread lettura partito";
	while (!stopped) {
		
		err=false;
		switch(socket.state()){
			case QAbstractSocket::UnconnectedState:
			{
				socket.connectToHost(serverName, serverPort);
				if (!socket.waitForConnected(Timeout)) {
                                        //qDebug() << "Timeout!";
					err=true;
				}
				break;
			}
			case QAbstractSocket::ConnectedState:
			{
				mutex.lock();
				for(int index=0;index<listvar.count();index++){
					readtime.start();
					
                                        idmsgtosend++;
                                        if(listvar[index]->getNewValue(&vartowrite))
                                                socket.write(formatMsg(listvar[index]->getVarName(),vartowrite,idmsgtosend));
                                        else
                                                socket.write(formatMsg(listvar[index]->getVarName(),idmsgtosend));

                                        if(!socket.waitForBytesWritten(Timeout))
                                        {
                                                qDebug() << "Timeout invio";
                                                err=true;
                                        }

					
					while (socket.bytesAvailable() == 0) {
						if (!socket.waitForReadyRead(Timeout)) {
							qDebug() << "Errore timeout attesa dati";
							err=true;
							break;
						}
					}
					
					//mutex.lock();
					if(!err){
                                                QByteArray result = socket.read(socket.bytesAvailable());
                                                unsigned char ok = result.right(1).at(0);
                                                QByteArray value;
                                                unsigned short idmsg = clearMsg(result,value);
                                                //qDebug() << "id msg inviato: " << idmsgtosend << " id msg ricevuto: " << idmsg;
                                                if (ok && clearMsg(result,value)==idmsgtosend) {
                                                    listvar[index]->setValue(value);
                                                    listvar[index]->setReadTime(readtime.elapsed());
                                                } else {
                                                    qDebug() << "Errore sincronizzazione messaggio";
                                                    listvar[index]->setReadTime(-1);
                                                }
					}
					else{
						listvar[index]->setReadTime(-1);
					}
					//mutex.unlock();
				}
				mutex.unlock();
				break;
			}
		}
		
		//QMutexLocker locker(&mutex);
		
		msleep(readfreq);
	}
	
	qDebug() << "Thread terminato";
	socket.disconnectFromHost();
	if (socket.state() == QAbstractSocket::UnconnectedState || socket.waitForDisconnected(Timeout))
		qDebug("Disconnected!");
	
	stopped=false;
}

/*!	\brief Processo thread
 *
 *	Termina il thread
 *
 */

void ClientCross::stop()
{
	stopped=true;
}

/*!	\brief Lettura variabile da indice
 *
 *	Consente la lettura di una variabile specificandone l'indice
 *
 *	\param varindex Indice variabile
 *	\return Valore della variabile
 */

QByteArray ClientCross::read(int varindex)
{
	return listvar[varindex]->getValue();
}

/*!	\brief Lettura variabile da indice
 *
 *	Consente la lettura di una variabile specificandone il nome
 *
 *	\param varname Nome variabile
 *	\return Valore della variabile
 */

QByteArray ClientCross::read(QByteArray varname)
{
	QByteArray varvalue = QByteArray();
	for(int i=0;i<listvar.count();i++){
		//qDebug() << "indice: (" << i << ") " << listvar[i]->getVarName() << " varname: " << varname;
		if(listvar[i]->getVarName() == varname){
			varvalue = listvar[i]->getValue();
			break;
		}
	}
	return varvalue;
}

/*!	\brief Aggiunta di una nuova variabile
 *
 *
 */

void ClientCross::add(RobotVar* newvar)
{
	listvar.append(newvar);
}

/*!	\brief Rimuove una variabile
 *
 *
 */

void ClientCross::delVar(int index)
{
    qDebug() << "Indice var " << index;
    qDebug() << "Numero di variabili nella lista: " << listvar.count();
//    mutex.lock();
//    delete listvar[index];
//    listvar[index]=NULL;
//    listvar.removeAt(index);
//    mutex.unlock();
}

/*!	\brief Rimuove una variabile
 *
 *
 */

void ClientCross::delVar(QByteArray varname)
{
    //qDebug() << "Numero di variabili nella lista: " << listvar.count();
    mutex.lock();
    for(int i=0;i<listvar.count();i++){
        if(listvar[i]->getVarName()==varname)
            //qDebug() << "Nome: " << listvar[i]->getVarName();
            delete listvar[i];
            listvar[i]=NULL;
            listvar.removeAt(i);
    }

    mutex.unlock();
}

/*!	\brief Formattazione messaggio per lettura variabile
 *
 *	Formatta il messaggio secondo lo standard di invio/ricezione definito da
 *	OPENCROSSCOMM
 *
 *	Per la lettura di una variabile sono necessari due parametri:
 *	il tipo di funzione da esguire e il nome della variabile.
 *	Nel caso di lettura la funzione da richiedere e' 0
 *	
 *      I primi due byte identificano l'ID del messaggio da inviare, mentre
 *      i secondi due la lunghezza del messaggio che segue.
 *      Ogni informazione, ad eccezione della funzione, e' preceduta da due byte
 *      che contengono la lunghezza del messaggio che segue.
 *      Per esempio, se si vuole leggere la variabile
 *	$OV_PRO verra' inviata la richiesta:
 *      0009007$OV_PRO che e' vettore di byte.
 *	00      ID messaggio
 *      09      Lunghezza messaggio seguente
 *      0       Funzione richiesta
 *      07      Lunghezza pacchetto seguente
 *      $OV_PRO Variabile da leggere
 *	Quindi la richiesta finale sul socket sara': 009007$OV_PRO
 *
 *	\param msg Variabile da richiedere
 *	\return Messaggio formattato
 */

QByteArray ClientCross::formatMsg(QByteArray msg, unsigned short idmsg){
	
    QByteArray header, block;
    int lunghezza,varnamelen;
    unsigned char hbyte, lbyte;
    unsigned char hbytemsg,lbytemsg;

    varnamelen=msg.size();
    hbyte=(varnamelen & 0xff00) >> 8;
    lbyte=(varnamelen & 0x00ff);

    block.append(READVARIABLE).append(hbyte).append(lbyte).append(msg);
    lunghezza=block.size();

    hbyte=(lunghezza & 0xff00) >> 8;
    lbyte=(lunghezza & 0x00ff);

    hbytemsg=(idmsg & 0xff00) >> 8;
    lbytemsg=(idmsg & 0x00ff);

    header.append(hbytemsg).append(lbytemsg).append(hbyte).append(lbyte);
    block.prepend(header);
    //qDebug() << block.toHex();

    return block;
}

/*!	\brief Formattazione messaggio per scrittura variabile
 *
 *	Formatta il messaggio secondo lo standard di invio/ricezione definito da
 *	OPENCROSSCOMM
 *
 *	Per la scrittura di una variabile sono necessari tre parametri:
 *	-#il tipo di funzione da esguire;
 *	-#il nome della variabile;
 *	-#il valore da scrivere;
 *	
 *	Nel caso di scrittura la funzione da richiedere e' 1
 *	
 *	I primi tre caratteri identificano sempre la lunghezza della
 *	stringa da inviare. Per esempio, se si vuole scrivere la variabile
 *	$OV_PRO verra' inviata la richiesta 1|$OV_PRO|100 che e' vettore di byte
 *	formato da 12 byte.
 *	Quindi la richiesta finale sul socket sara': 0121|$OV_PRO|100
 *
 *	\param msg Variabile da richiedere
 *	\param value Valore da scrivere
 *	\return Messaggio formattato
 */

QByteArray ClientCross::formatMsg(QByteArray msg, QByteArray value, unsigned short idmsg){

    QByteArray header, block;
    short lunghezza,varnamelen,varvaluelen;
    unsigned char hbyte, lbyte;
    unsigned char hbytemsg,lbytemsg;

    varnamelen=msg.size();
    hbyte=(varnamelen & 0xff00) >> 8;
    lbyte=(varnamelen & 0x00ff);

    block.append(WRITEVARIABLE).append(hbyte).append(lbyte).append(msg);

    varvaluelen=value.size();
    hbyte=(varvaluelen & 0xff00) >> 8;
    lbyte=(varvaluelen & 0x00ff);

    block.append(hbyte).append(lbyte).append(value);

    lunghezza=block.size();

    hbyte=(lunghezza & 0xff00) >> 8;
    lbyte=(lunghezza & 0x00ff);

    hbytemsg=(idmsg & 0xff00) >> 8;
    lbytemsg=(idmsg & 0x00ff);

    header.append(hbytemsg).append(lbytemsg).append(hbyte).append(lbyte);
    block.prepend(header);
    //qDebug() << "Scrittura: " << block.toHex();

    return block;
}

/*!	\brief Pulizia messaggio
 *
 *	Riceve un messaggio da OPENCROSSCOMM formattato secondo la specifica del protocollo
 *	e si occupa di controllarne l'esattezza e di pulirlo dalle informazioni necessarie
 *	alla trasmissione/ricezione.
 *
 *	\param msg Variabile da pulire
 *	\return Messaggio pulito
 */

QByteArray ClientCross::clearMsg(QByteArray msg){
    short lenmsg,func,lenmsg1;
	if(msg.length() > 0){
            //ID Messaggio
            idreadmsg=((unsigned char)msg[0])<<8 | ((unsigned char)msg[1]);
            //qDebug() << "ID Messaggio: " << idreadmsg;

            //Lunghezza messaggio
            lenmsg=((unsigned char)msg[2])<<8 | ((unsigned char)msg[3]);
            //qDebug() << "Lunghezza messaggio: " << lenmsg;

            //Funzione
            func=((int)msg[4]);
            //qDebug() << "Funzione: " << func;

            //Lunghezza messaggio 1
            lenmsg1=((unsigned char)msg[5])<<8 | ((unsigned char)msg[6]);
            //qDebug() << "Lunghezza messaggio 1: " << lenmsg1;
		
            //qDebug() << "Valore ricevuto " << msg.toHex();

            return msg.mid(7,lenmsg1);
	}
	else{
            return QByteArray("");
	}
}

/*!	\brief Pulizia messaggio
 *
 *	Riceve un messaggio da OPENCROSSCOMM formattato secondo la specifica del protocollo
 *	e si occupa di controllarne l'esattezza e di pulirlo dalle informazioni necessarie
 *	alla trasmissione/ricezione.
 *
 *	\param msg Variabile da pulire
 *      \param value Valore variabile pulito
 *	\return id messaggio
 */

unsigned short ClientCross::clearMsg(QByteArray msg, QByteArray &value){
    short lenmsg,func,lenmsg1;
    unsigned short idreadmsg;
    if(msg.length() > 0){
        //ID Messaggio
        idreadmsg=((unsigned char)msg[0])<<8 | ((unsigned char)msg[1]);
        //qDebug() << "ID Messaggio: " << idreadmsg;

        //Lunghezza messaggio
        lenmsg=((unsigned char)msg[2])<<8 | ((unsigned char)msg[3]);
        //qDebug() << "Lunghezza messaggio: " << lenmsg;

        //Funzione
        func=((int)msg[4]);
        //qDebug() << "Funzione: " << func;

        //Lunghezza messaggio 1
        lenmsg1=((unsigned char)msg[5])<<8 | ((unsigned char)msg[6]);
        //qDebug() << "Lunghezza messaggio 1: " << lenmsg1;

        //qDebug() << "Valore ricevuto " << msg.toHex();

        value = msg.mid(7,lenmsg1);
        return idreadmsg;
    }
    else{
        value = QByteArray("");
        return 0;
    }
}

/*!	\brief IP su cui e' attivo il socket
 *
 *	Restituisce l'ip su cui e' attivo il socket
 *
 *	\return Ip socket
 */

QHostAddress ClientCross::getSocketIP(){
	return robotip;
}

/*!	\brief Tempo aggiornamento variabili da robot
 *
 *	Imposta la frequenza di aggiornamento delle variabili dal robot
 *
 *	\param readfreq Frequenza di lettura in millisecondi
 */

void ClientCross::setReadFreq(int* readfreq){
	mutex.lock();
	this->readfreq=*readfreq;
	mutex.unlock();
}
