/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>

#include "treeitem.h"
#include "treemodel.h"

TreeModel::TreeModel(const QStringList &headers, const QString &data,
                     QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new TreeItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ShowModelIndex indice = index;

    TreeItem *item = getItem(index);

    if (role == Qt::BackgroundRole) {

    } else if (role == Qt::ForegroundRole ) {

        if (item->data(TreeModel::TIME)==tr("TIMEOUT")){
            return QColor(Qt::red);
        }
        else{
            return QColor(Qt::black);
        }
    }

    if(role==Qt::FontRole){
        if(index.column()==TreeModel::VARNAME && index.parent() == QModelIndex()){
            QFont f;
            f.setPixelSize(15);
            f.setBold(true);
            return f;
        }
    }

    if (role == Qt::DecorationRole) {
        if (index.column() == 0 && indice.isRobot())
            return QIcon(":openshowvar");
    }


//    if (role==Qt::DisplayRole && item->data(TreeModel::OPTIONS)==tr("Binary code")){
//        int value=item->data(TreeModel::VARVALUE).toInt();
//        item->setData(TreeModel::VARVALUE,toBinary(value));
//    }

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if(index.column()==2 && (this->isInt(index) || data(index,Qt::DisplayRole) == tr("Binary code") || data(index,Qt::DisplayRole) == tr("Hex code")))
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
//! [7]

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}
//! [8]

bool TreeModel::setData(const QModelIndex &index, const QVariant &value,
                        int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            TreeItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        number++;
    }
}

QWidget* TreeModel::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index){

}

Qt::DropActions TreeModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList TreeModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}


QMimeData *TreeModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray varname,varvalue,robotip;

    robotip = indexes.at(0).parent().data(Qt::DisplayRole).toByteArray();

    foreach (QModelIndex index, indexes) {
        qDebug() <<  data(index, Qt::DisplayRole);
        switch(index.column()){  
        case TreeModel::VARNAME:
            {
                varname = data(index, Qt::DisplayRole).toByteArray();
            break;
        }
        case TreeModel::VARVALUE:
            {
                varvalue = data(index, Qt::DisplayRole).toByteArray();
            break;
        }
        default:
            break;
        }
    }

    //Se la variabile e' una struttura elimino il tipo di dato e passo solo il valore
    KukaVar *kukavar = new KukaVar(varname,varvalue);

    switch(kukavar->getVarType()){
    case KukaVar::STRUCTURE:
        {
            varvalue=kukavar->getStructureValue();
            break;
        }
    default:
        {
            varvalue=kukavar->getValue();
            break;
        }
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(varvalue);

    //qDebug() << "Nome variabile: " << varname << " ip robot: " << robotip;

    // dati per il grafico
    mimeData->setData( "openshowvar/graphdata" , varname );
    mimeData->setData( "openshowvar/graphdataip" , robotip );

    QPixmap pixmap(":images/AWESOM-O.png");
    //drag->setPixmap(pixmap);

    delete kukavar;
    return mimeData;
}

bool TreeModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent ){
    qDebug() << "DROPMIMEDATA";
}

bool TreeModel::isInt(const QModelIndex &index) const{
    bool isint=false;

    QByteArray varvalue = data(this->index(index.row(),TreeModel::VARVALUE,index.parent()),Qt::DisplayRole).toByteArray();

    KukaVar *kukavar = new KukaVar(QByteArray("TEST"),varvalue);
    switch(kukavar->getVarType()){
    case KukaVar::INT:
        {
            isint=true;
            break;
        }
    case KukaVar::ERRTYPE:
        {
            //qDebug() << "Tipo in errore";
        }
    default:
        {
            break;
        }
    }

    delete kukavar;
    if(isint)
        return true;
    else
        return false;
}

QString TreeModel::toBinary(int value) const
{
    int mask;
    QString binary;
    for(int i=0;i<32;i++){
        mask = 1 << i;
        if(i==4 || i==8 || i==12 || i==16 || i==20 || i==24 || i==28)
            binary.prepend(" ");
        if(value & mask)
            binary.prepend("1");
        else
            binary.prepend("0");
    }
    //binary->append(QString("%1").arg(value, 0, 2));
    return binary;
}




ShowModelIndex::ShowModelIndex() : QModelIndex() {}

ShowModelIndex::ShowModelIndex(const QModelIndex & other) : QModelIndex(other) {}

bool ShowModelIndex::isRobot() {
    QModelIndex root = QModelIndex();
    if(this->parent()==root)
        return true;

    return false;
}

bool ShowModelIndex::isVar() {
    QModelIndex root = QModelIndex();
    if(this->parent()==root)
        return false;

    if(this->parent().parent()!=root)
        return false;

    return true;
}

QString ShowModelIndex::robotIP(){
    if(this->isRobot())
        return this->data(Qt::DisplayRole).toString();

    return "";
}

QString ShowModelIndex::varNAME(){
    if(this->isVar())
        return this->data(Qt::DisplayRole).toString();

    return "";
}
