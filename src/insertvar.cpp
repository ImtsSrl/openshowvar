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
 *	\class InsertVar
 *	\author Massimiliano Fago
 *	\version 1.0
 *	\date 2008
 *	\brief Finestra inserimento nuova variabile
 *
 */

#include "insertvar.h"

/*!	\brief Costruttore
 *	
 *	Costruttore della classe. Definisce l'interfaccia utente.
 *
 */

InsertVar::InsertVar(){
	
    //label di inserimento e lineedit
    varLabel = new QLabel(tr("Var name"));
    lineEdit = new QLineEdit();

    QRegExp regExp("[$]?[_A-Za-z0-9]{1,30}[.]?[_A-Za-z0-9]{1,30}([[][0-9]{0,3}([,]{0,1}[0-9]{1,3}){0,1}([,]{0,1}[0-9]{1,3}){0,1}[]]){0,1}([.]?[_A-Za-z0-9]{1,30}){0,1}");

    lineEdit->setValidator(new QRegExpValidator(regExp, this));

    //lista dei robot presenti
    labelRobot = new QLabel(tr("Robot list (Serial @ IP Address)"));
    comboRobotList = new QComboBox;
    QFont font("Helvetica", 10, QFont::Bold);
    comboRobotList->setFont(font);

    //comandi
    insertButton = new QPushButton(tr("Insert"));
    insertButton->setEnabled(false);
    abortButton = new QPushButton(tr("Abort"));

    /*la finestra è strutturata su tre righe:
	la prima descrizione e inserimento
	la seconda lista dei robot
	la terza comandi
	*/
    QHBoxLayout* insertLayout = new QHBoxLayout;
    QHBoxLayout* robotLayout = new QHBoxLayout;
    QHBoxLayout* commandLayout = new QHBoxLayout;

    insertLayout->addWidget(varLabel);
    insertLayout->addSpacing(20);
    insertLayout->addWidget(lineEdit);

    robotLayout->addWidget(labelRobot);
    robotLayout->addSpacing(20);
    robotLayout->addWidget(comboRobotList);

    commandLayout->addStretch();
    commandLayout->addWidget(insertButton);
    commandLayout->addWidget(abortButton);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addLayout(insertLayout);
    mainLayout->addLayout(robotLayout);
    mainLayout->addLayout(commandLayout);
    setLayout(mainLayout);

//    setMaximumSize(500,200);
//    setWindowTitle(tr("New variable"));
//    resize(430,150);

    broadcast = new Broadcast(this);

    connect(broadcast, SIGNAL(addRobot(QList<QByteArray> &)), this, SLOT(Robot(QList<QByteArray> &)));
    connect(insertButton,SIGNAL(clicked()), this, SLOT(on_insertButton_clicked()));
    connect(lineEdit,SIGNAL(textChanged(const QString &)), this, SLOT(on_lineEdit_textChanged()));
    connect(lineEdit,SIGNAL(returnPressed()), this, SLOT(on_lineEdit_returnPressed()));
    connect(abortButton, SIGNAL(clicked()), this, SLOT(on_abortButton_clicked()));

    connect(broadcast, SIGNAL(newMsg(QString &)), this, SLOT(newMsg(QString &)));

    setWindowIcon(QIcon("openshowvar.png"));

    broadcast->send();
}

InsertVar::~InsertVar(){
	
}

void InsertVar::DropVar(QString varName){
    lineEdit->setText(varName);
}

void InsertVar::on_lineEdit_textChanged()
{
    insertButton->setEnabled(lineEdit->hasAcceptableInput() && !comboRobotList->currentText().isEmpty());
}

void InsertVar::on_lineEdit_returnPressed()
{
    on_insertButton_clicked();
}

void InsertVar::on_insertButton_clicked(){
    QString text=lineEdit->text();
    QList<QString> selectedRobot;
    selectedRobot = comboRobotList->currentText().split("@");
    emit insertNewVar(text, selectedRobot[1]);
//    delete broadcast;
//    accept();
//    delete this;
    lineEdit->clear();
}

void InsertVar::on_abortButton_clicked(){
    emit insertClose();
    delete broadcast;
    reject();
}

/*!	\brief Aggiunta robot alla lista
 *	
 *	Popola la combobox con la lista dei robot trovati
 *
 *	\param datirobot struttura contenente le informazioni sui robot
 */

void InsertVar::Robot(QList<QByteArray> &datirobot){
	comboRobotList->addItem(datirobot[2] + "@" + datirobot[3]);
	//comboRobotList->setEditable(true);
	//comboRobotList->lineEdit()->setAlignment(Qt::AlignHCenter);
	//comboRobotList->setItemData(0,Qt::AlignHCenter, Qt::TextAlignmentRole);
	
	on_lineEdit_textChanged();
}

void InsertVar::closeEvent (QCloseEvent* event)
{
	on_abortButton_clicked();
}

void InsertVar::newMsg(QString &msg){
	this->setWindowTitle(msg);
}
