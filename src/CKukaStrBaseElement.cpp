#include "CKukaStrBaseElement.h"

KukaStrBaseElement::KukaStrBaseElement(){

}

KukaStrBaseElement::KukaStrBaseElement(QByteArray elementName, QByteArray elementValue){
    this->elementName=elementName;
    this->elementValue=elementValue;
    //qDebug() << "Nome variabile: " << this->elementName << " Valore variabile: " << this->elementValue;
}

KukaStrBaseElement::~KukaStrBaseElement(){

}

void KukaStrBaseElement::setElement(QByteArray elementName, int start, int length){
    this->elementName=elementName;
    this->elementStart=start;
    this->elementLength=length;
    //qDebug() << "Nome elemento: " << this->elementName << " start: " << this->elementStart << " lunghezza: " << this->elementLength;
}

void KukaStrBaseElement::setValue(QByteArray elementValue, int start, int length){
    this->elementValue=elementValue;
    this->valueStart=start;
    this->valueLength=length;
    //qDebug() << "valore elemento: " << this->elementValue << " start: " << this->valueStart << " lunghezza: " << this->valueLength;
}

int KukaStrBaseElement::getValueStart(){
    return valueStart;
}

int KukaStrBaseElement::getValueLength(){
    return valueLength;
}
