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
	
	qDebug() << "Thread lettura partito";
	while (!stopped) {
		
		err=false;
		switch(socket.state()){
			case QAbstractSocket::UnconnectedState:
			{
				socket.connectToHost(serverName, serverPort);
				if (!socket.waitForConnected(Timeout)) {
					qDebug() << "Timeout!";
					err=true;
				}
				break;
			}
			case QAbstractSocket::ConnectedState:
			{
				mutex.lock();
				for(int index=0;index<listvar.count();index++){
					readtime.start();
					
					if(listvar[index]->getNewValue(&vartowrite))
						socket.write(formatMsg(listvar[index]->getVarName(),vartowrite));
					else
						socket.write(formatMsg(listvar[index]->getVarName()));
					
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
                                                if (ok) {
                                                    //listvar[index]->setValue(clearMsg(socket.read(socket.bytesAvailable())));
                                                    listvar[index]->setValue(clearMsg(result));
                                                    listvar[index]->setReadTime(readtime.elapsed());
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
	mutex.lock();
	delete listvar[index];
	listvar[index]=NULL;
	listvar.removeAt(index);
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
 *	I primi tre caratteri identificano sempre la lunghezza della
 *	stringa da inviare. Per esempio, se si vuole leggere la variabile
 *	$OV_PRO verra' inviata la richiesta 0|$OV_PRO che e' vettore di byte
 *	formato da 9 byte.
 *	Quindi la richiesta finale sul socket sara': 0090|$OV_PRO
 *
 *	\param msg Variabile da richiedere
 *	\return Messaggio formattato
 */

QByteArray ClientCross::formatMsg(QByteArray msg){
	
        QByteArray header, block;
        int lunghezza;
        unsigned char hbyte, lbyte;
	
	//inserisco il primo byte in testa al messaggio: 0 lettura variabile
        //block = READVARIABLE + "|" + msg;
        //blocksize.setNum(block.size());  //inserisco in blocksize la lunghezza in byte di block
	//3 byte format
        //QString blocktosend = QString("%1").arg(block.size(), NUMBYTEINT, 'f',0, '0');

        block.append(READVARIABLE).append("|").append(msg);
        lunghezza=block.size();

        hbyte=(lunghezza & 0xff00) >> 8;
        lbyte=(lunghezza & 0x00ff);

        header.append((char)0).append((char)0).append(hbyte).append(lbyte);
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

QByteArray ClientCross::formatMsg(QByteArray msg, QByteArray value){
        /*
	QByteArray blocksize, block;

	//inserisco il primo byte in testa al messaggio: 0 lettura variabile
	block = WRITEVARIABLE + "|" + msg + "|" + value;
	blocksize.setNum(block.size());  //inserisco in blocksize la lunghezza in byte di block
	//3 byte format
	QString blocktosend = QString("%1").arg(block.size(), NUMBYTEINT, 'f',0, '0');
	
	return blocktosend.toAscii() + block;
        */

        QByteArray header, block;
        int lunghezza;
        unsigned char hbyte, lbyte;

        block.append(WRITEVARIABLE).append("|").append(msg).append("|").append(value);
        lunghezza=block.size();

        hbyte=(lunghezza & 0xff00) >> 8;
        lbyte=(lunghezza & 0x00ff);

        header.append((char)0).append((char)0).append(hbyte).append(lbyte);
        block.prepend(header);
        //qDebug() << block.toHex();

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
	if(msg.length() > 0){
		QByteArray QBLunghezzaBlocco = msg.mid(0,NUMBYTEINT);
		//int nLunghezzaBlocco=QBLunghezzaBlocco.toInt();
			
		//separazione del messaggio ricevuto in tre campi
		//rimuovo i primi due byte
		msg=msg.remove(0,NUMBYTEINT);
		//viene generato un array di nome list
		QList<QByteArray> lista=msg.split('|');
		
		return lista[1];
	}
	else{
		return QByteArray("");
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

/*!	\brief controllo coerenza
 *	
 *	A volte puo' capitare che a seguito di un ritardo in lettura, la stessa
 *	funzione di lettura vada in timeout. Questo provoca uno sfasamento
 *	con il valore della variabile successiva e cosi' via, causando una serie
 *	di letture errate a catena.
 *	
 *	Lo scopo di questa funzione e' di verificare che la risposta ottenuta dal
 *	socket sia coerente con la richiesta.
 *	Dato che la risposta di lettura porta con se anche il nome della variabile
 *	letta, sara' facile effettuare questa verifica.
 *
 *	\param request nome della variabile da scrivere
 *	\param earn valore da scrivere
 *	\return true se la variabile e' coerente con la richiesta
 */

bool ClientCross::readConsistency(const QByteArray request, const QByteArray earn)
{
	return false;
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
