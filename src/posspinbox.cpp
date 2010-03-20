#include "posspinbox.h"

PosSpinBox::PosSpinBox(QWidget *parent) : QAbstractSpinBox(parent) {

    //lineEdit()->setInputMask("NNNNN: \\X #000.000 \\, Y #0000, Z #0000, \\A #000, \\B #000, \\C #000;0");

}

PosSpinBox::~PosSpinBox(){

}

void PosSpinBox::setValue(QString value){
    lineEdit()->setText(value);
}

void PosSpinBox::stepBy(int steps)
{
    qDebug() << "Passo";
}

void PosSpinBox::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Select:{
            qDebug() << "Selezione";
        }
        break;
    default:
        qDebug() << "Altro";
        break;
    }
}
