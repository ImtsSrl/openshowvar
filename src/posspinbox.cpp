#include "posspinbox.h"

PosSpinBox::PosSpinBox(QWidget *parent) : QAbstractSpinBox(parent) {

    StepEnabled(QAbstractSpinBox::StepDownEnabled|QAbstractSpinBox::StepUpEnabled);
    //lineEdit()->setInputMask("NNNNN: \\X #000.000 \\, Y #0000, Z #0000, \\A #000, \\B #000, \\C #000;0");

}

PosSpinBox::~PosSpinBox(){
    delete kukavar;
}

void PosSpinBox::setValue(QString value){
    lineEdit()->setText(value);
    lineEdit()->setCursorPosition(1);
    kukavar = new KukaVar("TEST",value.toAscii());
}

QByteArray PosSpinBox::value(){
    return lineEdit()->text().toAscii();
}

void PosSpinBox::stepBy(int steps)
{
    int index=kukavar->currentSectionIndex(lineEdit()->cursorPosition());
    qDebug() << "Passo: " << steps;
    qDebug() << "Indice: " << index << " valore: ";
    lineEdit()->setSelection(kukavar->getSectionStart(index),kukavar->getSectionLength(index));

    QString testo = lineEdit()->text().mid(0,kukavar->getSectionStart(index));
    QString newvalue;
    newvalue.setNum(lineEdit()->selectedText().toFloat()+steps);
    testo.append(newvalue);
    testo.append(lineEdit()->text().mid(kukavar->getSectionStart(index)+kukavar->getSectionLength(index),lineEdit()->text().length()));
    kukavar = new KukaVar("TEST",testo.toAscii());
    lineEdit()->setText(testo);
    lineEdit()->setSelection(kukavar->getSectionStart(index),kukavar->getSectionLength(index));
    qDebug() << "Nuovo valore: " << testo;
}

PosSpinBox::StepEnabled PosSpinBox::stepEnabled() const
{
    StepEnabled ret;

    //return StepNone;

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
