#ifndef POSSPINBOX_H
#define POSSPINBOX_H

#include <QAbstractSpinBox>
#include <QLineEdit>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

#include "kukavar.h"

class PosSpinBox : public QAbstractSpinBox{
    Q_OBJECT

public:

    enum Section {
        NoSection = 0x0000,
        TypePosSection = 0x0001,
        XPosSection = 0x0002,
        YPosSection = 0x0004,
        ZPosSection = 0x0008,
        APosSection = 0x0010,
        BPosSection = 0x0020,
        CPosSection = 0x0040,
    };

    PosSpinBox(QWidget *parent = 0);
    ~PosSpinBox();

    void setValue(QString value);
    QByteArray value();
    void stepBy(int steps);

protected:
    virtual StepEnabled stepEnabled() const;
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

private:
    KukaVar *kukavar;

};

#endif // POSSPINBOX_H
