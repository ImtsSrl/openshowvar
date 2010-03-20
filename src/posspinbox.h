#ifndef POSSPINBOX_H
#define POSSPINBOX_H

#include <QAbstractSpinBox>
#include <QLineEdit>
#include <QDebug>
#include <QKeyEvent>

class PosSpinBox : public QAbstractSpinBox{
    Q_OBJECT

public:

    enum Section {
        NoSection = 0x0000,
        XPosSection = 0x0001,
    };

    PosSpinBox(QWidget *parent = 0);
    ~PosSpinBox();

    void setValue(QString value);
    void stepBy(int steps);

protected:
    virtual void keyPressEvent(QKeyEvent *event);

};

#endif // POSSPINBOX_H
