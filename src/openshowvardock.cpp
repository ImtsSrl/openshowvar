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
 *	\class OpenShowVarDock
 *	\author Massimiliano Fago
 *	\version 0.1
 *	\date 2010
 *	\brief Finestra principale
 *
 */

#include <QtGui>

#include "openshowvardock.h"

OpenShowVarDock::OpenShowVarDock()
{
    insertVar=NULL;

    treeWidget = new CTreeVar();
    setCentralWidget(treeWidget);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    setUnifiedTitleAndToolBarOnMac(true);

    database = new VariableDB();

    connect(&qtimeLettura, SIGNAL(timeout()), this, SLOT(lettura()));
    qtimeLettura.start(REFRESHTIME);

    connect(&timeUpdateGraph, SIGNAL(timeout()), this, SLOT(updateGraph()));
    timeUpdateGraph.start(500);

    setWindowIcon(QIcon(":openshowvar"));
    resize(700,500);
}

void OpenShowVarDock::newVar()
{
    QString varName="";
    on_insertVar(&varName);
}

void OpenShowVarDock::addGraph()
{
    QDockWidget *dock = new QDockWidget(tr("New robot variable"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    CVarsGrid* g = new CVarsGrid( database );
    m_gridList.append( g );

    dock->setWidget(g);
    addDockWidget(Qt::RightDockWidgetArea, dock);

//    statusBar()->showMessage(tr("Ready"), 2000);
}

void OpenShowVarDock::about()
{
   QMessageBox::about(this, tr("About OpenShowVar"),
            tr("<b>OpenShowVar</b> è un editor di variabili "
               "per robot KUKA realizzato da: "
               "massimiliano.fago@gmail.com "
               "dddomodossola@gmail.com "
               "cyberpro4@gmail.com"));
}

void OpenShowVarDock::createActions()
{
    newVarAct = new QAction(QIcon(":/images/add.png"), tr("&Add Robot Var..."),this);
    //newVarAct->setShortcuts(QKeySequence::New);
    newVarAct->setStatusTip(tr("Insert a new robot var"));
    connect(newVarAct, SIGNAL(triggered()), this, SLOT(newVar()));

    deleteVarAct = new QAction(QIcon(":delete"), tr("&Delete a robot var"), this);
    deleteVarAct->setStatusTip(tr("Delete a robot var from var list"));
    connect(deleteVarAct, SIGNAL(triggered()), this, SLOT(deleteVar()));

    addGraphAct = new QAction(QIcon(":addGraph"), tr("&Show graph"), this);
    addGraphAct->setStatusTip(tr("Show var graph"));
    connect(addGraphAct, SIGNAL(triggered()), this, SLOT(addGraph()));

    refVarAct = new QComboBox(this);
    QStringList scaleTime;
    scaleTime << "100" << "500" << "1000" << "2000";
    refVarAct->addItems(scaleTime);
    refVarAct->setCurrentIndex(refVarAct->findText("1000"));
    connect(refVarAct,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(on_refVarAct(const QString &)));

    refVarDatabaseAct = new QSpinBox(this);
    refVarDatabaseAct->setRange(10,2000);
    refVarDatabaseAct->setSingleStep(50);
    refVarDatabaseAct->setSuffix(" [ms]");
    connect(refVarDatabaseAct,SIGNAL(valueChanged(int)),this,SLOT(on_refVarDatabaseAct(int)));

    editVarAct = new QAction(QIcon(":editvar"), tr("&Edit Var..."), this);
    editVarAct->setStatusTip(tr("Edit variable value"));
    connect(editVarAct, SIGNAL(triggered()), this, SLOT(on_editVar()));

    saveVarAct = new QAction(QIcon(":saveVar"), tr("&Save var list"), this);
    saveVarAct->setStatusTip(tr("Save var list"));
    connect(saveVarAct, SIGNAL(triggered()), this, SLOT(on_saveVar()));

    openVarAct = new QAction(QIcon(":openVar"), tr("&Open var list"), this);
    openVarAct->setStatusTip(tr("Save var list"));
    connect(openVarAct, SIGNAL(triggered()), this, SLOT(on_openVar()));

    clearListAct = new QAction(QIcon(":clearList"), tr("&Delete all variables"), this);
    clearListAct->setStatusTip(tr("Delete all"));
    connect(clearListAct, SIGNAL(triggered()), this, SLOT(on_clearList()));

//    quitAct = new QAction(tr("&Quit"), this);
//    quitAct->setShortcuts(QKeySequence::Quit);
//    quitAct->setStatusTip(tr("Quit the application"));
//    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
//
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void OpenShowVarDock::createMenus()
{
    robotMenu = menuBar()->addMenu(tr("&Robot"));
    robotMenu->addAction(newVarAct);
    robotMenu->addAction(deleteVarAct);
    robotMenu->addSeparator();
    robotMenu->addAction(addGraphAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
//    editMenu->addAction(undoAct);
//
//    viewMenu = menuBar()->addMenu(tr("&View"));
//
    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void OpenShowVarDock::createToolBars()
{
    robotToolBar = addToolBar(tr("Robot"));
    robotToolBar->addAction(newVarAct);
    robotToolBar->addAction(deleteVarAct);
    robotToolBar->addAction(addGraphAct);
    robotToolBar->addSeparator();
    robotToolBar->addWidget(refVarAct);
    robotToolBar->addWidget(refVarDatabaseAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(editVarAct);
    editToolBar->addAction(clearListAct);
    editToolBar->addAction(openVarAct);
    editToolBar->addAction(saveVarAct);
}

void OpenShowVarDock::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void OpenShowVarDock::on_insertVar(const QString *varName)
{
    if(insertVar==NULL)
    {
        QDockWidget *dock = new QDockWidget(tr("New robot variable"), this);
        dock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

        insertVar = new InsertVar();

        dock->setWidget(insertVar);
        addDockWidget(Qt::BottomDockWidgetArea, dock);

        connect(insertVar,SIGNAL(insertNewVar(const QString &, const QString &)),this,SLOT(insertNew(const QString &, const QString &)));
        connect(dock,SIGNAL(visibilityChanged(const bool &)),this,SLOT(insertClose(const bool &)));

        insertVar->DropVar(*varName);
    }
}

void OpenShowVarDock::insertNew(const QString &variabile, const QString &iprobot)
{
    QTreeWidgetItem *item;

    item = new QTreeWidgetItem(treeWidget);
    item->setText(CTreeVar::VARNAME, variabile.toUpper());
    item->setText(CTreeVar::ROBOTIP, iprobot);

    //Evita il problema del blocco durante il drag della riga
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    item->setToolTip(CTreeVar::VARNAME,tr("Robot IP %1").arg(iprobot));

    item->setTextAlignment(CTreeVar::TIME,(Qt::AlignRight | Qt::AlignVCenter));

    database->addVar(variabile.toUpper().toAscii(),QHostAddress(iprobot));
}

void OpenShowVarDock::deleteVar()
{
    QTreeWidgetItem *item;
    if(treeWidget->currentItem()!=NULL){
        if(treeWidget->currentItem()->parent()!=NULL)
            item=treeWidget->currentItem()->parent();
        else
            item=treeWidget->currentItem();

        database->deleteVar(item->text(CTreeVar::VARNAME).toAscii(),(QHostAddress)item->text(CTreeVar::ROBOTIP));
        statusBar()->showMessage(tr("Deleted '%1'").arg(item->text(CTreeVar::VARNAME)), 2000);
        delete item;
        item=NULL;
    }
}

void OpenShowVarDock::insertClose(const bool &visible)
{
    if(!visible){
        delete insertVar;
        insertVar=NULL;
    }
}

void OpenShowVarDock::lettura()
{
    QString qsVariabile, tempo;
    QByteArray qbVariabile, value;
    int readtime;

    for(int row=0;row<treeWidget->topLevelItemCount();row++)
    {
        //qDebug() << treeWidget->topLevelItem(row)->text(0);

        qbVariabile.clear();

        qsVariabile=treeWidget->topLevelItem(row)->text(CTreeVar::VARNAME);
        qbVariabile.append(qsVariabile);

        if(database->readVar(qsVariabile.toAscii(),(QHostAddress)treeWidget->topLevelItem(row)->text(CTreeVar::ROBOTIP), &value, &readtime)){

            //qDebug() << "Trovato " << value << " Tempo di lettura: " << readtime << " [ms]";

            tempo.setNum(readtime);
            tempo.append(" " + tr("[ms]"));

            QTreeWidgetItem *item = treeWidget->topLevelItem(row);

            item->setText(CTreeVar::VARVALUE,value);

            QBrush brush;

            if(readtime>=0){
                item->setText(CTreeVar::TIME,tempo);
                brush.setColor(Qt::black);
            }
            else{
                item->setText(CTreeVar::TIME,tr("TIMEOUT"));
                brush.setColor(Qt::red);
            }

            item->setForeground(CTreeVar::VARVALUE,brush);
            this->splitvaluetoview(item, item->text(CTreeVar::VARNAME), item->text(CTreeVar::VARVALUE));
        }//if
    }//for

    //QTreeWidgetItem *item;
    //this->splitvaluetoview(item, QString("PIPPO"), QString("{PRO_IP: NAME[] \"/R1/RAGGIUNGIBIL.SRC\", SNR 116, NAME_C[] \"/R1/RAGGIUNGIBIL.SRC\", SNR_C 116, I_EXECUTED FALSE, P_ARRIVED 0, SI01 {CALL_STACK: NAME[] \"/R1/RAGGIUNGIBIL.SRC\", SNR 48, INT_FLAG FALSE}}"));
    //cLog.writeList(treeWidget);
}

void OpenShowVarDock::splitvaluetoview(QTreeWidgetItem *item, QString varname, QString varvalue)
{
    varvalue=varvalue.trimmed();

    KukaVar *kukavarloc  = new KukaVar(&varname.toAscii(),&varvalue.toAscii());

    int datatype;

    //qDebug() << "Numero elementi: " << kukavarloc->getElementsNumber();
    //qDebug() << "Tipo variabile: " << kukavarloc->getVarType();

    switch(kukavarloc->getVarType()){
    case KukaVar::STRUCTURE:
        {
            QSet<QString> darobot;
            QHash<QString, int> dalista;
            for(int i=0;i<kukavarloc->getElementsNumber();i++)
                darobot.insert(kukavarloc->getStructureMember(i));

            for(int i=0;i<item->childCount();i++)
                dalista.insert(item->child(i)->text(CTreeVar::VARNAME),i);

            QHashIterator<QString, int> i(dalista);
            while (i.hasNext()){
                i.next();
                if(!darobot.contains(i.key())){
                    //remove this child
                    QTreeWidgetItem *child = item->child(i.value());
                    item->removeChild(child);
                    delete child;
                    child=NULL;
                }
            }

            for(int i=0;i<kukavarloc->getElementsNumber();i++){
                //qDebug() << "getElementNumber() " << kukavarloc->getStructureMember(i) << " " << kukavarloc->getElementsNumber();
                if(dalista.contains(kukavarloc->getStructureMember(i))){
                    //update vale

                    //qDebug() << "Valore i=" << i;

                    QTreeWidgetItem *child = item->child(i);
                    QByteArray elementvalue=kukavarloc->getStructureValue(i,datatype);
                    //tipo di dato struttura intero
                    switch(datatype)
                    {
                    case KukaVar::INT:
                        {
                            if(((QComboBox*)treeWidget->itemWidget(child,CTreeVar::OPTIONS))->currentText().toAscii()==tr("Binary code")){
                                QString binary;
                                toBinary(elementvalue.toInt(),&binary);
                                child->setText(CTreeVar::VARVALUE,binary);
                            }
                            else if(((QComboBox*)treeWidget->itemWidget(child,CTreeVar::OPTIONS))->currentText().toAscii()==tr("Hex code")){
                                QString hex;
                                toHex(elementvalue.toInt(),&hex);
                                child->setText(CTreeVar::VARVALUE,hex);
                            }
                            else
                                child->setText(CTreeVar::VARVALUE,elementvalue);
                            break;
                        }
                    case KukaVar::STRUCTURE:
                        {
                            //qDebug() << "OK, " << kukavarloc->getStructureMember(i) << " è una nuova struttura. Che faccio?";
                            this->splitvaluetoview(child,kukavarloc->getStructureMember(i),kukavarloc->getStructureValue(i,datatype));
                            break;
                        }
                        //altro tipo di dato
                    default:
                        {
                            child->setText(CTreeVar::VARVALUE,elementvalue);
                            //qDebug() << "Nome variabile: " << item->child(i)->text(1) << " tipo dato: " << datatype;
                            break;
                        }
                    }
                }
                else{
                    //add value
                    QTreeWidgetItem *child = new QTreeWidgetItem();
                    child->setText(CTreeVar::VARNAME,kukavarloc->getStructureMember(i));
                    child->setText(CTreeVar::VARVALUE,kukavarloc->getStructureValue(i,datatype));
                    item->insertChild(i,child);
                    if(datatype==KukaVar::INT){
                        addCombo(child);
                    }
                }
            }
            break;
        }
    case KukaVar::INT:
        {
            if(QComboBox* comboBox = dynamic_cast<QComboBox*>(treeWidget->itemWidget(item,CTreeVar::OPTIONS))){
                if(comboBox->currentText().toAscii()==tr("Binary code")){
                    QString binary;
                    toBinary(varvalue.toInt(),&binary);
                    item->setText(CTreeVar::VARVALUE,binary);
                }
                else if(comboBox->currentText().toAscii()==tr("Hex code")){
                    QString hex;
                    toHex(varvalue.toInt(),&hex);
                    item->setText(CTreeVar::VARVALUE,hex);
                }
                else{
                    //qDebug() << "Valore variabile int " << varvalue;
                    item->setText(CTreeVar::VARVALUE,varvalue);
                }

            }
            else
                addCombo(item);
            break;
        }
    default:
        {

            break;
        }
    }//case

    treeWidget->resizeColumnToContents(CTreeVar::VARNAME);
    delete kukavarloc;
    kukavarloc=NULL;
}

void OpenShowVarDock::editVar(QTreeWidgetItem * item)
{
    QByteArray varname=item->text(CTreeVar::VARNAME).toAscii();
    QByteArray varvalue=item->text(CTreeVar::VARVALUE).toAscii();
    QHostAddress varip=(QHostAddress)item->text(CTreeVar::ROBOTIP);

    RobotVarEdit *roboteditvar=new RobotVarEdit(&varvalue, &varname, varip, this);
    connect(roboteditvar,SIGNAL(writevalue(const QByteArray &, const QByteArray &, const QHostAddress &)),this,SLOT(on_writeVariable(const QByteArray &, const QByteArray &, const QHostAddress &)));

    QDockWidget *dock = new QDockWidget(tr("New robot variable"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(roboteditvar);
    connect(dock,SIGNAL(visibilityChanged(const bool &)),this,SLOT(on_editVarClose(const bool &)));
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

void OpenShowVarDock::addCombo(QTreeWidgetItem *child){
    QComboBox *combo = new QComboBox(this);
    combo->addItem(tr("Int code"));
    combo->addItem(tr("Binary code"));
    combo->addItem(tr("Hex code"));
    treeWidget->setItemWidget(child,2,combo);
}

void OpenShowVarDock::toBinary(int value, QString *binary){
    int mask;
    for(int i=0;i<16;i++){
        mask = 1 << i;
        if(i==4 || i==8 || i==12)
            binary->prepend(" ");
        if(value & mask)
            binary->prepend("1");
        else
            binary->prepend("0");
    }
    //binary->append(QString("%1").arg(value, 0, 2));
}

/*!	\brief Restituisce il valore esadecimale di una variabile
 *
 *	Restituisce il corrispondente valore esadecibale della variabile
 *	ricevuta come parametro
 *
 *	\param value Variabile contenente il valore da trasformare
 *	\param binary Stringa contenente il valore esadecimale
 */

void OpenShowVarDock::toHex(int value, QString *hex){
    hex->append(QString("0x%1").arg(value, 0, 16));
    hex->toUpper();
}

void OpenShowVarDock::updateGraph()
{
    CVarsGrid* g;
    foreach( g , m_gridList ){
        g->update();
    }
}

void OpenShowVarDock::on_editVar()
{
    QTreeWidgetItem *item;
    if(treeWidget->currentItem()!=NULL){
        if(treeWidget->currentItem()->parent()!=NULL)
            item=treeWidget->currentItem()->parent();
        else
            item=treeWidget->currentItem();

        editVar(item);
        statusBar()->showMessage(tr("Edit '%1'").arg(item->text(CTreeVar::VARNAME)), 2000);
    }
    item=NULL;
}

void OpenShowVarDock::on_writeVariable(const QByteArray &varname, const QByteArray &value, const QHostAddress &varip){
    int writetime;
    database->writeVar(varname, varip, value, &writetime);
}

void OpenShowVarDock::on_editVarClose(const bool &visible)
{
    if(!visible){
        qDebug() << "Edit chiuso";
    }
}

void OpenShowVarDock::on_saveVar()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save file as"),"./","Var list (*.xml)");
    if (!fileName.isEmpty())
        listVar.writeList(treeWidget,fileName);
}

void OpenShowVarDock::on_openVar()
{
    QString files = QFileDialog::getOpenFileName(this,tr("Select files to open"),"./","Var list (*.xml)");
    connect(&listVar,SIGNAL(insertNewVar(const QString &, const QString &)),this,SLOT(insertNew(const QString &, const QString &)));
    listVar.readList(files);
}

void OpenShowVarDock::on_clearList()
{
    for(int row=0;row<treeWidget->topLevelItemCount();row++)
    {
        database->deleteVar(treeWidget->topLevelItem(row)->text(CTreeVar::VARNAME).toAscii(),(QHostAddress)treeWidget->topLevelItem(row)->text(CTreeVar::ROBOTIP));
    }
    treeWidget->clear();
}

void OpenShowVarDock::on_refVarAct(const QString &text)
{
    qtimeLettura.start(text.toInt());
}

void OpenShowVarDock::on_refVarDatabaseAct(int i)
{
    database->setAllReadTime(&i);
}
