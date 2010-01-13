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
 *	\class OpenShowVarTree
 *	\author Massimiliano Fago
 *	\version 1.0
 *	\date 2007
 *	\brief Finestra principale
 *
 */

#include "openshowvartree.h"

int OpenShowVarTree::VarValue=1;

//!\todo Tasto edit nome variabile nella finestra principale
//!\todo Distinguere il robot di provenienza della variabile

OpenShowVarTree::OpenShowVarTree(QWidget *parent)
    : QWidget(parent)
{
	
    treeWidget = new CTreeVar(); // QTreeWidget;
    treeWidget->setColumnCount(5);
    treeWidget->setHeaderLabels(QStringList() << tr("Variable name") << tr("Variable value") << tr("") << tr("Read time") << tr("IP Robot"));
    //treeWidget->header()->setResizeMode(0, QHeaderView::Stretch);
    //treeWidget->header()->setResizeMode(1, QHeaderView::Stretch);
    treeWidget->setColumnWidth(VARNAME,110);
    treeWidget->setColumnWidth(VARVALUE,350);
    treeWidget->setColumnHidden(4,true);

    /*
    abilitazione Drop
    */
    treeWidget->setAcceptDrops(true);
    treeWidget->setDragEnabled(true);
    //treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    addButton = new QPushButton(tr("Add"));
    delButton = new QPushButton(tr("Delete"));
    startButton = new QPushButton(tr("Start"));
    stopButton = new QPushButton(tr("Stop"));
    graphButton = new QPushButton(tr("Graph"));
    
    sotCheckBox = new QCheckBox(tr("Always on top"), this);

    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    
    //listaRobot = new RobotServer();

    connect(treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),this,SLOT(DoubleClicked(QTreeWidgetItem *, int)));
    connect(sotCheckBox, SIGNAL(stateChanged (int)), this, SLOT(on_stateChanged (int)));
    connect(addButton, SIGNAL(clicked()), this, SLOT(on_addButton_clicked()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(on_delButton_clicked()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(on_startButton_clicked()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(on_stopButton_clicked()));
    connect(graphButton, SIGNAL(clicked()), this, SLOT(on_graphButton_clicked()));
    
    connect(&listVar,SIGNAL(insertNewVar(const QString &, const QString &)),this,SLOT(insertNew(const QString &, const QString &)));

    treeWidget->setAlternatingRowColors(true);
    connect(treeWidget,SIGNAL(dropVar(const QString *)),this,SLOT(insertVar(const QString *)));

    QGridLayout *gridLayout = new QGridLayout();
    QHBoxLayout *hBox = new QHBoxLayout();
    hBox->addStretch(10);
    gridLayout->addLayout(hBox,0,0);
    gridLayout->addWidget(graphButton,0,1);
    gridLayout->addWidget(sotCheckBox,0,0);
    gridLayout->addWidget(startButton,0,2);
    gridLayout->addWidget(stopButton,1,2);
    gridLayout->addWidget(addButton,0,3);
    gridLayout->addWidget(delButton,1,3);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(treeWidget);
    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    connect(&qtimeLettura, SIGNAL(timeout()), this, SLOT(lettura()));
    connect(&timeUpdateGraph, SIGNAL(timeout()), this, SLOT(updateGraph()));

    setWindowIcon(QIcon("openshowvar.png"));
    
    resize(700,500);
    
    database = new VariableDB();
    
    //grafico
    m_Window = new QWidget();
    m_grid = new CGridField();
		
    QGridLayout* lay = new QGridLayout();
    lay->addWidget(m_grid,0,1, 2,2);
    m_Window->setLayout(lay);

    listVar.readList();
}

OpenShowVarTree::~OpenShowVarTree()
{
	
}

void OpenShowVarTree::closeEvent ( QCloseEvent * event )
{	
	qtimeLettura.stop();
	listVar.writeList(treeWidget);
	
	//delete listaRobot;
	delete database;
	delete m_grid;
	delete m_Window;
}

/*!	\brief Apre una finestra di edit
 *	
 *	Dopo aver effettuato il doppioclick, apre la finestra di edit della
 *	variabile.
 *	
 */

void OpenShowVarTree::DoubleClicked(QTreeWidgetItem * item, int column)
{
	if(!item->text(VARVALUE).isEmpty()){
		QByteArray varname=item->text(VARNAME).toAscii();
		QByteArray varvalue=item->text(VARVALUE).toAscii();
		QHostAddress varip=(QHostAddress)item->text(ROBOTIP);
		
		QTreeWidgetItem *structurename=item->parent();
		if(structurename!=NULL)
			roboteditvar = new RobotVarEdit(&structurename->text(VARVALUE).toAscii(), &structurename->text(VARNAME).toAscii(), varip, this);
		else
			roboteditvar = new RobotVarEdit(&varvalue, &varname, varip, this);
		
		connect(roboteditvar,SIGNAL(writevalue(const QByteArray &, const QByteArray &, const QHostAddress &)),this,SLOT(writeVariable(const QByteArray &, const QByteArray &, const QHostAddress &)));
		
		QRect rect=frameGeometry();
		QPoint posmain=pos();
		
		QPoint pos(rect.width()+posmain.x(),posmain.y());
	
		//Il metodo show deve essere chiamato dopo il move per avere un corretto posizionamento
		//della finestra.
		
		Qt::WindowFlags flags = roboteditvar->windowFlags();
		
		//roboteditvar->move(pos);
		roboteditvar->setWindowFlags(flags & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowSystemMenuHint);
		roboteditvar->show();
	}
}

void OpenShowVarTree::insertVar(const QString *varName)
{
	InsertVar* insertVar = new InsertVar();
	connect(insertVar,SIGNAL(insertNewVar(const QString &, const QString &)),this,SLOT(insertNew(const QString &, const QString &)));
	connect(insertVar,SIGNAL(insertClose()),this,SLOT(insertClose()));

	insertVar->DropVar(*varName);
	
	insertVar->setModal(true);
	insertVar->show();
	insertVar->activateWindow();
	
	addButton->setEnabled(false);
}

void OpenShowVarTree::on_addButton_clicked()
{
	QString varName="";
	insertVar(&varName);
}

/*!	\brief Cancellazione variabile
 *	
 *	Rimuove la variabile selezionata dalla lista delle variavili.
 *	Selezionando un membro della struttura e premendo elimina, verra' rimossa
 *	l'intera struttura.
 *	
 */

void OpenShowVarTree::on_delButton_clicked()
{
	QTreeWidgetItem *item;
	if(treeWidget->currentItem()!=NULL){
		if(treeWidget->currentItem()->parent()!=NULL)
			item=treeWidget->currentItem()->parent();
		else
			item=treeWidget->currentItem();
		
		database->deleteVar(item->text(VARNAME).toAscii(),(QHostAddress)item->text(ROBOTIP));
	}
	delete item;
	item=NULL;
}

/*!	\brief Edit nome variabile
 *	
 *	Modifica il nome della variabile selezionata nella lista delle variavili.
 *	
 */

void OpenShowVarTree::on_editButton_clicked()
{
	QTreeWidgetItem *item;
	if(treeWidget->currentItem()!=NULL){
		if(treeWidget->currentItem()->parent()!=NULL)
			item=treeWidget->currentItem()->parent();
		else
			item=treeWidget->currentItem();
		
		InsertVar* editVar = new InsertVar();
		//connect(insertVar,SIGNAL(insertNewVar(const QString &, const QString &)),this,SLOT(insertNew(const QString &, const QString &)));
		//connect(insertVar,SIGNAL(insertClose()),this,SLOT(insertClose()));

		editVar->setModal(true);
		editVar->show();
		editVar->activateWindow();
	
		//editButton->setEnabled(false);
	}
}

void OpenShowVarTree::on_startButton_clicked()
{	
	//TODO rendere questo tempo variabile
	qtimeLettura.start(REFRESHTIME);
	startButton->setEnabled(false);
	stopButton->setEnabled(true);
}

void OpenShowVarTree::on_stopButton_clicked()
{
	qtimeLettura.stop();
	startButton->setEnabled(true);
	stopButton->setEnabled(false);
}

/*!	\brief Pressione del tasto grafico
 *	
 *	Crea una nuova finestra che visualizza la variabile selezionata
 *
 */

void OpenShowVarTree::on_graphButton_clicked()
{
	if(treeWidget->currentItem()!=NULL){
		m_lnOne = new CGridLine();
		m_grid->addLine(m_lnOne);
		m_grid->setMaxTime(30000);
		
		graphVarName = treeWidget->currentItem()->text(VARNAME).toAscii();
		graphVarIP = treeWidget->currentItem()->text(ROBOTIP);
		
		timeUpdateGraph.start(1000);
		
		m_Window->setWindowTitle(tr("Graph of: %1").arg(treeWidget->currentItem()->text(VARNAME)));
		
		m_Window->show();
                m_Window->resize(500,400);
	}
}

void OpenShowVarTree::insertNew(const QString &variabile, const QString &iprobot)
{
	QTreeWidgetItem *item;

	item = new QTreeWidgetItem(treeWidget);
	item->setText(VARNAME, variabile.toUpper());
	item->setText(ROBOTIP, iprobot);
	
	item->setToolTip(VARNAME,tr("Robot IP %1").arg(iprobot));
	
	item->setTextAlignment(TIME,(Qt::AlignRight | Qt::AlignVCenter));
	
        database->addVar(variabile.toUpper().toAscii(),QHostAddress(iprobot));
	
	addButton->setEnabled(true);
}

void OpenShowVarTree::insertClose()
{	
	addButton->setEnabled(true);
}

void OpenShowVarTree::lettura()
{
	QString qsVariabile, tempo;
	QByteArray qbVariabile, value;
	int readtime;
	
	for(int row=0;row<treeWidget->topLevelItemCount();row++)
	{	
		//qDebug() << treeWidget->topLevelItem(row)->text(0);
		
		qbVariabile.clear();
		
		qsVariabile=treeWidget->topLevelItem(row)->text(VARNAME);
		qbVariabile.append(qsVariabile);

		if(database->readVar(qsVariabile.toAscii(),(QHostAddress)treeWidget->topLevelItem(row)->text(ROBOTIP), &value, &readtime)){

                        //qDebug() << "Trovato " << value << " Tempo di lettura: " << readtime << " [ms]";
			
			tempo.setNum(readtime);
			tempo.append(" " + tr("[ms]"));
			
			QTreeWidgetItem *item = treeWidget->topLevelItem(row);
			
			item->setText(VARVALUE,value);
			
			QBrush brush;
			
			if(readtime>=0){
				item->setText(TIME,tempo);
				brush.setColor(Qt::black);
			}
			else{
				item->setText(TIME,tr("TIMEOUT"));
				brush.setColor(Qt::red);
			}
			
			item->setForeground(VARVALUE,brush);


                        this->splitvaluetoview(item, item->text(VARNAME), item->text(VARVALUE));
		}//if
	}//for
	
        //QTreeWidgetItem *item;
        //this->splitvaluetoview(item, QString("PIPPO"), QString("{PRO_IP: NAME[] \"/R1/RAGGIUNGIBIL.SRC\", SNR 116, NAME_C[] \"/R1/RAGGIUNGIBIL.SRC\", SNR_C 116, I_EXECUTED FALSE, P_ARRIVED 0, SI01 {CALL_STACK: NAME[] \"/R1/RAGGIUNGIBIL.SRC\", SNR 48, INT_FLAG FALSE}}"));
        cLog.writeList(treeWidget);
}

///\todo Impostare correttamente l'ip della variabile da correggere

void OpenShowVarTree::writeVariable(const QByteArray &varname, const QByteArray &value, const QHostAddress &varip){
	int writetime;
        database->writeVar(varname, varip, value, &writetime);
}

void OpenShowVarTree::addCombo(QTreeWidgetItem *child){
	QComboBox *combo = new QComboBox(this);
	combo->addItem(tr("Int code"));
	combo->addItem(tr("Binary code"));
	combo->addItem(tr("Hex code"));
	treeWidget->setItemWidget(child,2,combo);
}

void OpenShowVarTree::toBinary(int value, QString *binary){
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

void OpenShowVarTree::toHex(int value, QString *hex){
	hex->append(QString("0x%1").arg(value, 0, 16));
	hex->toUpper();
}

/*!	\brief Aggiornamento grafico
 *	
 *	Viene richiamata allo scadere del timeout di aggiornamento del
 *	grafico aggiungendo un valore al grafico
 *
 */

void OpenShowVarTree::updateGraph()
{
	int readtime;
	QByteArray value;
	database->readVar(graphVarName,graphVarIP, &value, &readtime);
	m_lnOne->addValue(value.toDouble());
}

void OpenShowVarTree::on_stateChanged(int state)
{
	Qt::WindowFlags flags = windowFlags();
	
	if(state==2){
		//setWindowFlags((flags | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint) & 0xffff);
		setWindowFlags(flags | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);
		this->setVisible(true);
		qDebug() << QString::number(flags | Qt::WindowStaysOnTopHint,2);
		//qDebug() << QString::number(flags,2);
	}else{
		this->setWindowFlags((flags & (0xffffffff^Qt::WindowStaysOnTopHint)));//  &! Qt::WindowStaysOnTopHint &! Qt::CustomizeWindowHint);
		qDebug() << QString::number(Qt::WindowStaysOnTopHint,2);
		qDebug() << QString::number(0xffffffff^Qt::WindowStaysOnTopHint,2);
		this->setVisible(true);
	}
}

void OpenShowVarTree::splitvaluetoview(QTreeWidgetItem *item, QString varname, QString varvalue)
{

    QByteArray value;

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
                            dalista.insert(item->child(i)->text(VARNAME),i);

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
                                            if(((QComboBox*)treeWidget->itemWidget(child,OPTIONS))->currentText().toAscii()==tr("Binary code")){
                                                    QString binary;
                                                    toBinary(elementvalue.toInt(),&binary);
                                                    child->setText(VARVALUE,binary);
                                            }
                                            else if(((QComboBox*)treeWidget->itemWidget(child,OPTIONS))->currentText().toAscii()==tr("Hex code")){
                                                    QString hex;
                                                    toHex(elementvalue.toInt(),&hex);
                                                    child->setText(VARVALUE,hex);
                                            }
                                            else
                                                    child->setText(VARVALUE,elementvalue);
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
                                            child->setText(VARVALUE,elementvalue);
                                            //qDebug() << "Nome variabile: " << item->child(i)->text(1) << " tipo dato: " << datatype;
                                            break;
                                        }
                                    }
                            }
                            else{
                            //add value
                                    QTreeWidgetItem *child = new QTreeWidgetItem();
                                    child->setText(VARNAME,kukavarloc->getStructureMember(i));
                                    child->setText(VARVALUE,kukavarloc->getStructureValue(i,datatype));
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
                    if(QComboBox* comboBox = dynamic_cast<QComboBox*>(treeWidget->itemWidget(item,OPTIONS))){
                            if(comboBox->currentText().toAscii()==tr("Binary code")){
                                    QString binary;
                                    toBinary(varvalue.toInt(),&binary);
                                    item->setText(VARVALUE,binary);
                            }
                            else if(comboBox->currentText().toAscii()==tr("Hex code")){
                                    QString hex;
                                    toHex(varvalue.toInt(),&hex);
                                    item->setText(VARVALUE,hex);
                            }
                            else{
                                //qDebug() << "Valore variabile int " << varvalue;
                                    item->setText(VARVALUE,varvalue);
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

    treeWidget->resizeColumnToContents(VARNAME);
    delete kukavarloc;
    kukavarloc=NULL;
}
