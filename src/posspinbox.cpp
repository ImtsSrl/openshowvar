#include "posspinbox.h"

PosSpinBox::PosSpinBox(QWidget *parent) : QAbstractSpinBox(parent) {

    StepEnabled(QAbstractSpinBox::StepDownEnabled|QAbstractSpinBox::StepUpEnabled);
    //lineEdit()->setInputMask("NNNNN: \\X #000.000 \\, Y #0000, Z #0000, \\A #000, \\B #000, \\C #000;0");

}

PosSpinBox::~PosSpinBox(){

}

void PosSpinBox::setValue(QString value){
    lineEdit()->setText(value);
    kukavar = new KukaVar("TEST",value.toAscii());
}

void PosSpinBox::stepBy(int steps)
{
    int index=kukavar->currentSectionIndex(lineEdit()->cursorPosition());
    qDebug() << "Passo " << steps;
    qDebug() << "Indice: " << index << " valore: ";
    lineEdit()->setSelection(kukavar->getSectionStart(index),kukavar->getSectionLength(index));
}

PosSpinBox::StepEnabled PosSpinBox::stepEnabled() const
{
    StepEnabled ret;
    ret |= QAbstractSpinBox::StepDownEnabled;
    ret |= QAbstractSpinBox::StepUpEnabled;
    return ret;
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

void PosSpinBox::mousePressEvent(QMouseEvent *event)
{
    QAbstractSpinBox::mousePressEvent(event);
}
