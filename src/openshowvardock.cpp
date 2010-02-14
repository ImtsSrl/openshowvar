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


        tree=new QTreeView(this);
        QStringList headers;
        headers << tr("Variable name") << tr("Variable value") << "" << tr("Read time");
        model = new TreeModel(headers,"");
        tree->setModel(model);

        tree->setItemDelegateForColumn(TreeModel::OPTIONS, new FormatDelegate);

        treeWidget = new CTreeVar(this);
        //setCentralWidget(treeWidget);

        setCentralWidget(tree);
        tree->setColumnWidth(CTreeVar::VARNAME,110);
        tree->setColumnWidth(CTreeVar::VARVALUE,350);
        //drag&drop
        tree->setDragEnabled(true);

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
	//m_gridList.append(CVarsGrid::loadAllFromXml("graph.xml",database));

	connect(&listVar,SIGNAL(insertNewVar(const QString &, const QString &)),this,SLOT(insertNew(const QString &, const QString &)));

        dockInsertVar = new QDockWidget(tr("New robot variable"), this);
        dockInsertVar->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        insertVar = new InsertVar();
        dockInsertVar->setWidget(insertVar);
        addDockWidget(Qt::BottomDockWidgetArea, dockInsertVar);
        connect(insertVar,SIGNAL(insertNewVar(const QString &, const QString &)),this,SLOT(insertNew(const QString &, const QString &)));
        connect(dockInsertVar,SIGNAL(visibilityChanged(const bool &)),this,SLOT(insertClose(const bool &)));
        //insertVar->DropVar(*varName);
        dockInsertVar->setVisible(false);

        connect(treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(on_itemClicked(QTreeWidgetItem*,int)));
        connect(treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(on_itemDoubleClicked(QTreeWidgetItem*,int)));

        listVar.readList("varlist.xml");

        cLog = new CLog("log.xml");
        saveLog=false;

	setWindowIcon(QIcon(":openshowvar"));
	resize(700,500);
}

OpenShowVarDock::~OpenShowVarDock()
{
	//CVarsGrid::saveAllToXml(m_gridList,"graph.xml");
}

void OpenShowVarDock::newVar()
{
	QString varName="";
	on_insertVar(&varName);
}

void OpenShowVarDock::addGraph()
{
	QDockWidget *dock = new QDockWidget(tr("New Graph"), this);
	dock->setAllowedAreas(Qt::TopDockWidgetArea);

	CVarsGrid* g = new CVarsGrid( database );
	m_gridList.append( g );

	dock->setWidget(g);
	addDockWidget(Qt::TopDockWidgetArea, dock);

	statusBar()->showMessage(tr("Add new graph window"), 2000);
}

void OpenShowVarDock::about()
{
   QMessageBox::about(this, tr("About OpenShowVar"),
                        tr("<b>OpenShowVar</b> e' un editor di variabili "
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

    readVarAct=new QAction(QIcon(":run"),tr("&Run"), this);
    readVarAct->setCheckable(true);
    readVarAct->setChecked(true);
    connect(readVarAct,SIGNAL(toggled(bool)),this,SLOT(on_readVarAct(bool)));

    onTopAct=new QAction(QIcon(":onTop"),tr("&Always on top"), this);
    onTopAct->setCheckable(true);
    connect(onTopAct,SIGNAL(toggled(bool)),this,SLOT(on_onTopAct(bool)));

    addGraphAct = new QAction(QIcon(":addGraph"), tr("&Show graph"), this);
    addGraphAct->setStatusTip(tr("Show var graph"));
    connect(addGraphAct, SIGNAL(triggered()), this, SLOT(addGraph()));

    refVarAct = new QComboBox(this);
    QStringList scaleTime;
    scaleTime << "100" << "500" << "1000" << "2000";
    refVarAct->addItems(scaleTime);
    refVarAct->setCurrentIndex(refVarAct->findText("1000"));
    refVarAct->setToolTip(tr("Set video refresh time"));
    connect(refVarAct,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(on_refVarAct(const QString &)));

    refVarDatabaseAct = new QSpinBox(this);
    refVarDatabaseAct->setRange(10,2000);
    refVarDatabaseAct->setSingleStep(50);
    refVarDatabaseAct->setSuffix(" [ms]");
    refVarDatabaseAct->setToolTip(tr("Set database refresh time"));
    connect(refVarDatabaseAct,SIGNAL(valueChanged(int)),this,SLOT(on_refVarDatabaseAct(int)));

    editVarAct = new QAction(QIcon(":editvar"), tr("&Edit Var..."), this);
    editVarAct->setStatusTip(tr("Edit variable value"));
#warning rimuovere il commento dopo aver messo a posto l'evento di selezione variabile'
    //editVarAct->setEnabled(false);
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

    logAct = new QAction(QIcon(":log"), tr("&Start/Stop log"), this);
    logAct->setStatusTip(tr("Log"));
    logAct->setCheckable(true);
    connect(logAct, SIGNAL(toggled(bool)), this, SLOT(on_log(bool)));

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
    QFont ToolBarFont;
    ToolBarFont.setBold(true);

    robotToolBar = addToolBar(tr("Robot"));
    robotToolBar->addAction(newVarAct);
    robotToolBar->addAction(deleteVarAct);
    robotToolBar->addAction(readVarAct);
    robotToolBar->addAction(onTopAct);
    robotToolBar->addAction(addGraphAct);
    robotToolBar->addSeparator();
    QLabel *refTime=new QLabel(tr("Ref. TIME:"));
    refTime->setFont(ToolBarFont);
    robotToolBar->addWidget(refTime);
    robotToolBar->addWidget(refVarAct);
    QLabel *readTime=new QLabel(tr("Read TIME:"));
    readTime->setFont(ToolBarFont);
    robotToolBar->addWidget(readTime);
    robotToolBar->addWidget(refVarDatabaseAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(editVarAct);
    editToolBar->addAction(clearListAct);
    editToolBar->addAction(openVarAct);
    editToolBar->addAction(saveVarAct);
    editToolBar->addAction(logAct);
}

void OpenShowVarDock::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void OpenShowVarDock::on_insertVar(const QString *varName)
{
    dockInsertVar->setVisible(true);
}

void OpenShowVarDock::insertNew(const QString &variabile, const QString &iprobot)
{
    QModelIndex index,root,robotip,varname;
    QAbstractItemModel *model = tree->model();
    bool robotpresent=false;

    //Robot
    root = model->index(0,TreeModel::VARNAME,QModelIndex());
    for(int row=0;row<=model->rowCount(root);row++){
        qDebug() << "Indice: " << row << " ip: " << iprobot;
        index = model->index(row,TreeModel::VARNAME,QModelIndex());
        if(model->data(index,Qt::DisplayRole)==iprobot){
            qDebug() << "Trovato robot";
            root = model->index(row,TreeModel::VARNAME,QModelIndex());
            robotpresent=true;
            break;
        }
    }

    if(!robotpresent){
        root = model->index(model->rowCount()+1,TreeModel::VARNAME,QModelIndex());
        model->insertRow(0, root);
        robotip = model->index(0,TreeModel::VARNAME,root);
        model->setData(robotip, QVariant(iprobot), Qt::EditRole);
        qDebug() << "Aggiunto robot";
    }
    else{
        qDebug() << "Aggiunto solo elemento";
        robotip = model->index(0,TreeModel::VARNAME,QModelIndex());
        robotip=root;
    }

    //Nome variabile
    model->insertRow(0, robotip);
    varname = model->index(0,TreeModel::VARNAME,robotip);
    model->setData(varname,QVariant(variabile.toUpper()),Qt::EditRole);


    database->addVar(variabile.toUpper().toAscii(),QHostAddress(iprobot));
}

void OpenShowVarDock::deleteVar()
{
    QModelIndex index = tree->selectionModel()->currentIndex();
    if(!index.parent().isValid())
        return;

    if(index.parent().parent().isValid())
        return;

    QString varname = index.data(Qt::DisplayRole).toByteArray();
    QString varip = index.parent().data(Qt::DisplayRole).toByteArray();

    //qDebug() << "Nome variabile: " << varname << " indirizzo ip: " << varip;

    if(model->rowCount(index.parent())==1){
        model->removeRow(index.parent().row(),index.parent().parent());
        statusBar()->showMessage(tr("Deleted '%1'").arg(varname), 2000);
        return;
    }

    if(model->removeRow(index.row(),index.parent())){
        database->deleteVar(varname.toAscii(),(QHostAddress)varip);
        statusBar()->showMessage(tr("Deleted '%1'").arg(varname), 2000);
    }

}

void OpenShowVarDock::insertClose(const bool &visible)
{

}

void OpenShowVarDock::lettura()
{
    QString tempo;
    QByteArray value;
    int readtime;

    QModelIndex index,root;

    for(int row=0;row<model->rowCount(root);row++){
        QModelIndex robotip = model->index(row, TreeModel::VARNAME, QModelIndex());
        QString iprobot = robotip.data(Qt::DisplayRole).toString();
        //qDebug() << "IP robot: " << iprobot;

        //qDebug() << "Robot " << model->rowCount(root);
        
        for(int var=0;var<model->rowCount(robotip);var++){
            QModelIndex varname = model->index(var, TreeModel::VARNAME, robotip);
            QString variabile = varname.data(Qt::DisplayRole).toString();

            //qDebug() << "Variabili " << model->rowCount(robotip);

            database->readVar(variabile.toAscii(),(QHostAddress)iprobot, &value, &readtime);
            //qDebug() << "Trovato " << value << " Tempo di lettura: " << readtime << " [ms]" << " ip: " << iprobot;

            index = model->index(var,TreeModel::VARVALUE,robotip);
            model->setData(index, QVariant(value), Qt::EditRole);
            index = model->index(var,TreeModel::VARNAME,robotip);
            model->setData(index, QVariant(variabile), Qt::EditRole);
            index = model->index(var,TreeModel::TIME,robotip);
            model->setData(index, QVariant(readtime), Qt::EditRole);
            
            index = model->index(var,TreeModel::VARNAME,robotip);
            //qDebug() << "Nome variabile: " << model->data(index,Qt::DisplayRole);
            this->splitvaluetoview(index, variabile, value);
        }
    }

    tree->resizeColumnToContents(TreeModel::VARNAME);

//    if(saveLog)
//        cLog->writeList(treeWidget);

}

void OpenShowVarDock::splitvaluetoview(QModelIndex index, QString varname, QString varvalue)
{
    int datatype;
    varvalue=varvalue.trimmed();

    //qDebug() << "nome variabile: " << varname << " valore variabile: " << varvalue;

    KukaVar *kukavarloc  = new KukaVar(varname.toAscii(),varvalue.toAscii());

    switch(kukavarloc->getVarType()){
    case KukaVar::STRUCTURE:
        {
            for(int i=0;i<kukavarloc->getElementsNumber();i++){
                //qDebug() << "Elemento numero: " << i << " Nome variabile: " << model->data(index,Qt::DisplayRole);
                QModelIndex child = model->index(i,TreeModel::VARNAME,index);
                if(!child.isValid())
                    if (!model->insertRow(i, index))
                        return;             

                model->setData(child, QVariant(kukavarloc->getStructureMember(i)), Qt::EditRole);
                child = model->index(i,TreeModel::VARVALUE,index);
                model->setData(child, QVariant(kukavarloc->getStructureValue(i,datatype)), Qt::EditRole);

                }
            }
            break;
    case KukaVar::INT:
            {

                break;
            }
        }

    delete kukavarloc;
    kukavarloc=NULL;
}

void OpenShowVarDock::editVar(QString varname, QString varvalue, QHostAddress varip)
{
    RobotVarEdit *roboteditvar=new RobotVarEdit(varvalue.toAscii(), varname.toAscii(), varip, this);
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
    for(int i=0;i<32;i++){
        mask = 1 << i;
        if(i==4 || i==8 || i==12 || i==16 || i==20 || i==24 || i==28)
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

void OpenShowVarDock::on_editVar(){
    QModelIndex index = tree->selectionModel()->currentIndex();
    if(!index.parent().isValid())
        return;

    if(index.parent().parent().isValid())
        return;

    QModelIndex varnameindex = model->index(index.row(),0,index.parent());
    QModelIndex varvalueindex = model->index(index.row(),TreeModel::VARVALUE,index.parent());

    QString varname = varnameindex.data(Qt::DisplayRole).toByteArray();
    QString varip = index.parent().data(Qt::DisplayRole).toByteArray();
    QString varvalue = varvalueindex.data(Qt::DisplayRole).toByteArray();

    //qDebug() << "Nome variabile: " << varname << " ip: " << varip << " valore: " << varvalue;

    editVar(varname,varvalue,(QHostAddress)varip);
    statusBar()->showMessage(tr("Edit '%1'").arg(varname), 2000);
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
        listVar.writeList(model,fileName);
}

void OpenShowVarDock::on_openVar()
{
    QString files = QFileDialog::getOpenFileName(this,tr("Select files to open"),"./","Var list (*.xml)");
    listVar.readList(files);
}

void OpenShowVarDock::on_clearList()
{
    QModelIndex index = QModelIndex();

    for(int row=0;row<model->rowCount(index);row++){
        QModelIndex robotip = model->index(row,TreeModel::VARNAME,index);
        for(int var=0;var<model->rowCount(robotip);var++){
            QModelIndex varname = model->index(var,TreeModel::VARNAME,robotip);
            database->deleteVar(model->data(varname,Qt::DisplayRole).toByteArray(),(QHostAddress)model->data(robotip,Qt::DisplayRole).toString());
        }
        model->removeRow(row,index);
    }
}

void OpenShowVarDock::on_log(bool checked)
{
    saveLog=checked;
}

void OpenShowVarDock::on_refVarAct(const QString &text)
{
	qtimeLettura.start(text.toInt());
}

void OpenShowVarDock::on_refVarDatabaseAct(int i)
{
	database->setAllReadTime(&i);
}

void OpenShowVarDock::on_readVarAct(bool checked)
{
    if(checked)
    {
        readVarAct->setIcon(QIcon(":run"));
        qtimeLettura.start();
    }
    else
    {
        readVarAct->setIcon(QIcon(":resume"));
        qtimeLettura.stop();
    }
}

void OpenShowVarDock::on_onTopAct(bool checked)
{
    Qt::WindowFlags flags = windowFlags();

    if(checked)
    {
        setWindowFlags(flags | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
        show();
    }
    else
    {
        setWindowFlags((flags & (0xffffffff^Qt::WindowStaysOnTopHint)));//  &! Qt::WindowStaysOnTopHint &! Qt::CustomizeWindowHint);
        show();
    }
}

void OpenShowVarDock::closeEvent ( QCloseEvent * event )
{
    timeUpdateGraph.stop();
    qtimeLettura.stop();
    listVar.writeList(model,"varlist.xml");
    delete cLog;
    delete database;
}

void OpenShowVarDock::on_itemClicked(QTreeWidgetItem *item, int column)
{
    if(treeWidget->currentItem()!=NULL){
        if(item->text(CTreeVar::TIME)!=tr("TIMEOUT"))
            editVarAct->setEnabled(true);
    }
    else
        editVarAct->setEnabled(false);
}

void OpenShowVarDock::on_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(treeWidget->currentItem()!=NULL){
        if(item->text(CTreeVar::TIME)!=tr("TIMEOUT")){
#warning modificare questa funzione
            editVarAct->setEnabled(true);
            //editVar(item);
        }
    }
    else
        editVarAct->setEnabled(false);
}
