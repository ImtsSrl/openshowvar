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
    treeWidget = new CTreeVar();
    setCentralWidget(treeWidget);

    createActions();
    //createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    //newLetter();
    setUnifiedTitleAndToolBarOnMac(true);

    database = new VariableDB();

    qtimeLettura.start(REFRESHTIME);
    connect(&qtimeLettura, SIGNAL(timeout()), this, SLOT(lettura()));
}

void OpenShowVarDock::newVar()
{
    QString varName="";
    insertVar(&varName);
}

//! [3]
void OpenShowVarDock::print()
{
#ifndef QT_NO_PRINTDIALOG
    QTextDocument *document = textEdit->document();
    QPrinter printer;

    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() != QDialog::Accepted)
        return;

    document->print(&printer);

    statusBar()->showMessage(tr("Ready"), 2000);
#endif
}
//! [3]

void OpenShowVarDock::deleteVar()
{
    QTreeWidgetItem *item;
    if(treeWidget->currentItem()!=NULL){
        if(treeWidget->currentItem()->parent()!=NULL)
            item=treeWidget->currentItem()->parent();
        else
            item=treeWidget->currentItem();

        database->deleteVar(item->text(CTreeVar::VARNAME).toAscii(),(QHostAddress)item->text(CTreeVar::ROBOTIP));
    }
    statusBar()->showMessage(tr("Deleted '%1'").arg(item->text(CTreeVar::VARNAME)), 2000);
    delete item;
    item=NULL;
}

//! [5]
void OpenShowVarDock::undo()
{
    QTextDocument *document = textEdit->document();
    document->undo();
}
//! [5]

//! [6]
void OpenShowVarDock::insertCustomer(const QString &customer)
{
    if (customer.isEmpty())
        return;
    QStringList customerList = customer.split(", ");
    QTextDocument *document = textEdit->document();
    QTextCursor cursor = document->find("NAME");
    if (!cursor.isNull()) {
        cursor.beginEditBlock();
        cursor.insertText(customerList.at(0));
        QTextCursor oldcursor = cursor;
        cursor = document->find("ADDRESS");
        if (!cursor.isNull()) {
            for (int i = 1; i < customerList.size(); ++i) {
                cursor.insertBlock();
                cursor.insertText(customerList.at(i));
            }
            cursor.endEditBlock();
        }
        else
            oldcursor.endEditBlock();
    }
}
//! [6]

void OpenShowVarDock::about()
{
   QMessageBox::about(this, tr("About Dock Widgets"),
            tr("The <b>Dock Widgets</b> example demonstrates how to "
               "use Qt's dock widgets. You can enter your own text, "
               "click a customer to add a customer name and "
               "address, and click standard paragraphs to add them."));
}

void OpenShowVarDock::createActions()
{
    newLetterAct = new QAction(QIcon(":/images/add.png"), tr("&Add Robot Var"),this);
    newLetterAct->setShortcuts(QKeySequence::New);
    newLetterAct->setStatusTip(tr("Insert a new robot var"));
    connect(newLetterAct, SIGNAL(triggered()), this, SLOT(newVar()));

    deleteVarAct = new QAction(QIcon(":delete"), tr("&Save..."), this);
    deleteVarAct->setShortcuts(QKeySequence::Delete);
    deleteVarAct->setStatusTip(tr("Save the current form letter"));
    connect(deleteVarAct, SIGNAL(triggered()), this, SLOT(deleteVar()));

//    printAct = new QAction(QIcon(":/images/print.png"), tr("&Print..."), this);
//    printAct->setShortcuts(QKeySequence::Print);
//    printAct->setStatusTip(tr("Print the current form letter"));
//    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));
//
//    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
//    undoAct->setShortcuts(QKeySequence::Undo);
//    undoAct->setStatusTip(tr("Undo the last editing action"));
//    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));
//
//    quitAct = new QAction(tr("&Quit"), this);
//    quitAct->setShortcuts(QKeySequence::Quit);
//    quitAct->setStatusTip(tr("Quit the application"));
//    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
//
//    aboutAct = new QAction(tr("&About"), this);
//    aboutAct->setStatusTip(tr("Show the application's About box"));
//    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
//
//    aboutQtAct = new QAction(tr("About &Qt"), this);
//    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
//    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void OpenShowVarDock::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newLetterAct);
    fileMenu->addAction(deleteVarAct);

    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void OpenShowVarDock::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newLetterAct);
    fileToolBar->addAction(deleteVarAct);

    //fileToolBar->addAction(printAct);

    //editToolBar = addToolBar(tr("Edit"));
    //editToolBar->addAction(undoAct);
}

//! [8]
void OpenShowVarDock::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
//! [8]

//! [9]
void OpenShowVarDock::createDockWindows()
{
//    QDockWidget *dock = new QDockWidget(tr("Variabili robot"), this);
//    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
//
//    dock->setWidget(treeWidget);
//    addDockWidget(Qt::RightDockWidgetArea, dock);

//    dock = new QDockWidget(tr("Paragraphs"), this);
//    paragraphsList = new QListWidget(dock);
//    paragraphsList->addItems(QStringList()
//            << "Thank you for your payment which we have received today."
//            << "Your order has been dispatched and should be with you "
//               "within 28 days."
//            << "We have dispatched those items that were in stock. The "
//               "rest of your order will be dispatched once all the "
//               "remaining items have arrived at our warehouse. No "
//               "additional shipping charges will be made."
//            << "You made a small overpayment (less than $5) which we "
//               "will keep on account for you, or return at your request."
//            << "You made a small underpayment (less than $1), but we have "
//               "sent your order anyway. We'll add this underpayment to "
//               "your next bill."
//            << "Unfortunately you did not send enough money. Please remit "
//               "an additional $. Your order will be dispatched as soon as "
//               "the complete amount has been received."
//            << "You made an overpayment (more than $5). Do you wish to "
//               "buy more items, or should we return the excess to you?");
//    dock->setWidget(paragraphsList);
//    addDockWidget(Qt::RightDockWidgetArea, dock);
    //viewMenu->addAction(dock->toggleViewAction());

    //connect(customerList, SIGNAL(currentTextChanged(const QString &)),
    //        this, SLOT(insertCustomer(const QString &)));
    //connect(paragraphsList, SIGNAL(currentTextChanged(const QString &)),
    //        this, SLOT(addParagraph(const QString &)));
}
//! [9]

void OpenShowVarDock::insertVar(const QString *varName)
{
    QDockWidget *dock = new QDockWidget(tr("New robot variable"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    InsertVar* insertVar = new InsertVar();

    dock->setWidget(insertVar);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(insertVar,SIGNAL(insertNewVar(const QString &, const QString &)),this,SLOT(insertNew(const QString &, const QString &)));
    connect(insertVar,SIGNAL(insertClose()),this,SLOT(insertClose()));

    insertVar->DropVar(*varName);

    insertVar->setModal(true);
    insertVar->show();
    insertVar->activateWindow();
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

void OpenShowVarDock::insertClose()
{

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
