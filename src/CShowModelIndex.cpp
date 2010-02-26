#include "CShowModelIndex.h"

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

bool ShowModelIndex::isInt(int column) const
{
    bool isint=false;

    QByteArray varvalue = model()->data(model()->index(row(),column,parent()),Qt::DisplayRole).toByteArray();

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

