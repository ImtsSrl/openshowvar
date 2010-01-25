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
 *	\class RobotServer
 *	\author Massimiliano Fago
 *	\version 1.0
 *	\date 2007
 *	\brief Ricerca robot attivi
 *
 *	Gestisce una connessione socket per robot.
 *	La classe Variabledb genera n istanze di questa classe, una per
 *	ciascun robot. Questa classe si occupa della comunicazione TCP
 *	in un thread.
 *
 */

#include "robotserver.h"

/*!	\brief Costruttore
 *
 */

RobotServer::RobotServer(QWidget *parent)
	: QWidget(parent)
{
	tableWidget = new QTableWidget;
	
	tableWidget->setColumnCount(3);
	tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Robot model") << tr("Serial number") << tr("IP address"));
	
	tableWidget->setColumnWidth(0,250);
	tableWidget->setColumnWidth(1,120);
	
	tableWidget->setAlternatingRowColors(true);
	//tableWidget->setSelectionMode(QAbstractItemView::SelectRows);
	tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tableWidget);
	setLayout(mainLayout);
	
	setWindowFlags(Qt::WindowMaximizeButtonHint);
	
	setWindowIcon(QIcon("bender.png"));
	
	setWindowTitle(tr("ROBOT List"));
	resize(500,150);
	
	connect(tableWidget,SIGNAL(cellDoubleClicked(int, int)),this,SLOT(cellDoubleClicked(int, int)));
	
	broadcast = new Broadcast(this);
	connect(broadcast, SIGNAL(addRobot(QList<QByteArray> &)), this, SLOT(Robot(QList<QByteArray> &)));
	
	broadcast->send();
}

RobotServer::~RobotServer()
{
	qDebug() << "Chiamato distruttore RobotServer";
	delete broadcast;
}

void RobotServer::closeEvent ( QCloseEvent * event )
{
	qDebug() << "chiusa finestra";
}

/*!	\brief Aggiunta di un nuovo robot alla lista
 *	
 *	Questa funzione membro viene chiamata per ogni messaggio di broadcast
 *	ricevuto da clientcrosscomm.
 *	Quando il pulsante find robot viene premuto, sulla rete viene emesso ad
 *	intervalli regolari un messaggio di broadcast per cercare eventuali robot
 *	attivi sulla sottorete.
 *	Se sulla rete e' presente un robot con CROSSCOMM attivo, esso rispondera'
 *	con un messaggio del tipo:
 *
 *	"KUKA|#KR2100P_2 S C2 FLR ZH150/18|944288|192.168.0.1"
 *
 *	Questo messaggio viene opportunamente trattato da openshowvardock e viene
 *	inserito in una QList (parametro della funzione).
 *
 *	La variabile robotName contiene le seguenti informazioni in questa sequenza:
 *	- 0 KUKA
 *	- 1 \#KR2100P_2 S C2 FLR ZH150/18
 *	- 2 944288
 *	- 3 IP
 *
 *	Se nella tablewidget non ci sono altri robot con lo stesso numero di serie,
 *	questo viene inserito.
 *
 *	\param robotName contiene tutte le informazioni sul robot selezionato
 *			nella lista.
 */

void RobotServer::addRobot(const QList<QByteArray> robotName)
{	
	QList<QTableWidgetItem *> item = tableWidget->findItems(robotName[2],Qt::MatchExactly);
	if(item.count()==0){
		int row = tableWidget->rowCount();
		tableWidget->insertRow(row);
		QTableWidgetItem *item0 = new QTableWidgetItem;
		QTableWidgetItem *item1 = new QTableWidgetItem;
		QTableWidgetItem *item2 = new QTableWidgetItem;
		item0->setTextAlignment((Qt::AlignLeft | Qt::AlignVCenter));
		item1->setTextAlignment((Qt::AlignRight | Qt::AlignVCenter));
		item2->setTextAlignment((Qt::AlignRight | Qt::AlignVCenter));
		item0->setFlags((Qt::ItemIsEnabled | Qt::ItemIsSelectable));
		item1->setFlags((Qt::ItemIsEnabled | Qt::ItemIsSelectable));
		item2->setFlags((Qt::ItemIsEnabled | Qt::ItemIsSelectable));
		tableWidget->setItem(row,0,item0);
		tableWidget->setItem(row,1,item1);
		tableWidget->setItem(row,2,item2);
		tableWidget->item(row,0)->setText(robotName[1]);
		tableWidget->item(row,1)->setText(robotName[2]);
		tableWidget->item(row,2)->setText(robotName[3]);
		
		tableWidget->resizeColumnToContents(0);
	}
}

/*!	\brief connessione con robot
 *	
 *	Connette openshowvar al robot selezionato nella lista.
 *
 *	Dopo aver effettuato il doppio click sul robot che si vuole collegare
 *	viene emesso il segnale 
 *
 *	connectToRobot.
 *
 *	Come parametro viene passato
 *	l'indirizzo IP del robot selezionato.
 *
 *	\param row riga su cui e' avvenuto il click
 *	\param column colonna su cui e' avvenuto il click
 */

void RobotServer::cellDoubleClicked ( int row, int column )
{
	QHostAddress robotIP(tableWidget->item(row,2)->text());
	emit connectToRobot(robotIP);
}

void RobotServer::Robot(QList<QByteArray> &datirobot){
	addRobot(datirobot);
}

