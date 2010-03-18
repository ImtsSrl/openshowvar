#ifndef CSHOWMODELINDEX_H
#define CSHOWMODELINDEX_H

#include <QModelIndex>

#include "kukavar.h"

class ShowModelIndex : public QModelIndex
{

public:
    ShowModelIndex();
    ShowModelIndex(const QModelIndex & other);
    bool isRobot();
    bool isVar();
    bool isInt(int column) const;
    int varTYPE();
    QString robotIP();
    QString varNAME();
};

#endif // CSHOWMODELINDEX_H
