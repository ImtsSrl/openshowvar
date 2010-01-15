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
 *	\brief Comunicazione UDP ricerca robot
 *
 *	Invia dei messaggi UDP sulla porta 6999 all'indirizzo di broadcast
 *	della rete. I server OPENSHOWVAR presenti sulla rete rispondono
 *	all'indirizzo UDP 7000 annunciando chi sono e a che indirizzo si trovano.
 *
 */

#include "broadcast.h"

/*!	\brief Costruttore
 *
 *	Inizializza il BIND sulla porta UDPBROADCASTPORTCROSS
 *
 */

Broadcast::Broadcast(QWidget *parent)
	: QUdpSocket()
{	
	memset(privateBind,0,MAXBROADCASTNETWORK * sizeof(QUdpSocket*));
}

Broadcast::~Broadcast()
{
    for(int i=0;i<MAXBROADCASTNETWORK;i++)
        if(privateBind[i]!=NULL){
        privateBind[i]->disconnectFromHost();
        delete privateBind[i];
    }
}

/*!	\brief Invio messaggio di broadcas
 *
 *	Invia il messaggio di broadcast sulla porta BROADCASTPORT
 *
 */

void Broadcast::send()
{
    QByteArray datagram = "WHEREAREYOU?";
    /*
#ifdef DEBUGWIN
	debugWindow = new CDebug;
	debugWindow->show();
#endif
*/
    QList<QNetworkInterface> interface = QNetworkInterface::allInterfaces();
    /*
#ifdef DEBUGWIN
	debugWindow->addDebug(QString("Schede di rete: %1").arg(interface.count()));
#endif
*/	

    qDebug() << "Inviato send...";

    for(int i=0;i<interface.count();i++){
        if((interface[i].flags() & QNetworkInterface::CanBroadcast) && (interface[i].flags() & QNetworkInterface::IsRunning)){
            //qDebug() << "Interface ip: " << interface[i].allAddresses();
            //qDebug() << "Interface name: " << interface[i].name();
            //qDebug() << "Interface name: " << interface[i].flags();


            if((interface[i].flags() & QNetworkInterface::CanBroadcast)){
                QList<QNetworkAddressEntry> address = interface[i].addressEntries();
                //qDebug() << "Flag " << interface[i].flags() << " nome " << interface[i].name();
                //qDebug() << address.count();
                if(address.count()>0 && address[0].ip()!=QHostAddress::LocalHost){
                    //this->bind(address[0].ip(),UDPBROADCASTPORTCROSS);

                    QSignalMapper* mapper[MAXBROADCASTNETWORK];
                    privateBind[i] = new QUdpSocket(this);
                    mapper[i] = new QSignalMapper(this);
                    connect(privateBind[i], SIGNAL(readyRead()),mapper[i], SLOT(map()));
                    mapper[i]->setMapping(privateBind[i], i);
                    connect(mapper[i], SIGNAL(mapped(int)), this, SLOT(processPending(int)));
                    privateBind[i]->bind(address[0].ip(),UDPBROADCASTPORTCROSS);
                    privateBind[i]->writeDatagram(datagram.data(), datagram.size(),address[0].broadcast(), BROADCASTPORT);

                }
            }
        }
    }
}

/*!	\brief Ricezione messaggio di broadcast
 *
 *	Riceve e gestisce il messaggio di broadcast.
 *	OPENCROSSCOMM risponde con un messaggio tipo il successivo:
 *
 *	KUKA|SERIALNO|ROBOTTYPE
 *	SERIALNO=$KR_SERIALNO
 *	ROBOTTYPE=$MODEL_NAME[] --> array 32 elements
 *	KUKA|"#KR2100P_2 S C2 FLR ZH150/180"|944288
 *
 */

//!\todo Trasformare datiRobot in classe

void Broadcast::processPending(int index)
{
/*
#ifdef DEBUGWIN
	debugWindow->addDebug(QString("Ricevuta risposta su indice %1").arg(index));
#endif
*/
    qDebug() << "Ricevuto nuovo robot";
    
    QByteArray datagram;
    QHostAddress sender;
    while (privateBind[index]->hasPendingDatagrams()) {
        datagram.resize(privateBind[index]->pendingDatagramSize());
        privateBind[index]->readDatagram(datagram.data(), datagram.size(),&sender);
        datagram.replace('"',"");
        datagram.append('|').append(sender.toString());
        QList<QByteArray> datiRobot = datagram.split('|');
        if(datiRobot.count()==4)
            emit addRobot(datiRobot);
    }
}
