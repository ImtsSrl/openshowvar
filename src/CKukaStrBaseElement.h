#ifndef CKUKASTRBASEELEMENT_H
#define CKUKASTRBASEELEMENT_H

#include <QByteArray>
#include <QDebug>

class KukaStrBaseElement
{

public:
    KukaStrBaseElement();
    KukaStrBaseElement(QByteArray elementName, QByteArray elementValue);
    ~KukaStrBaseElement();

    void setElement(QByteArray elementName, int start, int length);
    void setValue(QByteArray elementValue, int start, int length);

    int getValueStart();
    int getValueLength();

private:
    QByteArray elementName, elementValue;
    int elementStart, elementLength, valueStart, valueLength;

};

#endif // CKUKASTRBASEELEMENT_H
