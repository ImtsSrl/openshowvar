#include <QPainter>
#include <QModelIndex>
#include <QSpinBox>
#include <QApplication>

#include "vareditdelegate.h"

VarEditDelegate::VarEditDelegate( QObject *parent ) : QStyledItemDelegate( parent ) { }

void VarEditDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

QSize VarEditDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    return QSize( 45, 20 );
}

QWidget *VarEditDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    ShowModelIndex indice=index;

    switch(indice.varTYPE()){
    case KukaVar::INT:{
//            if(indice.varNAME()=="$OV_PRO"){
//                QSlider *slider = new QSlider(parent);
//
//                slider->setRange(0,100);
//                slider->setOrientation(Qt::Horizontal);
//
//                return slider;
//            }
//            else{
                QSpinBox *spinbox = new QSpinBox(parent);

                spinbox->setRange(KukaVar::MININTVALUE,KukaVar::MAXINTVALUE);
                spinbox->setAccelerated(true);
                spinbox->setValue(index.data(Qt::DisplayRole).toInt());

                //    connect(spinbox,SIGNAL(valueChanged(QString)),this,SLOT(commitAndCloseEditor(const QString &)));
                //    combo->installEventFilter( const_cast<FormatDelegate*>(this) );
                return spinbox;
//            }
        }
    case KukaVar::REAL:{
            QDoubleSpinBox *spinbox = new QDoubleSpinBox(parent);

            spinbox->setRange(KukaVar::MINREALVALUE,KukaVar::MAXREALVALUE);
            spinbox->setDecimals(5);
            spinbox->setSingleStep(0.1);
            spinbox->setAccelerated(true);
            spinbox->setValue(index.data(Qt::DisplayRole).toDouble());

            return spinbox;
        }
    case KukaVar::STRUCTURE:{
            PosSpinBox *structurespinbox = new PosSpinBox(parent);

            structurespinbox->setValue(index.data(Qt::DisplayRole).toString());

            return structurespinbox;
        }
    default:
        return NULL;
    }

//    if(indice.isInt(1)){
//        QSpinBox *spinbox = new QSpinBox(parent);
//
//        spinbox->setRange(KukaVar::MININTVALUE,KukaVar::MAXINTVALUE);
//        spinbox->setValue(index.data(Qt::DisplayRole).toInt());
//    //    connect(spinbox,SIGNAL(valueChanged(QString)),this,SLOT(commitAndCloseEditor(const QString &)));
//    //    combo->installEventFilter( const_cast<FormatDelegate*>(this) );
//        return spinbox;
//    }
}

void VarEditDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    editor->setGeometry( option.rect );
}

void VarEditDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    //static_cast<QSpinBox*>(editor)->setValue(valueatedit.toInt());
}

void VarEditDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    ShowModelIndex indice=index;

    QByteArray varname = indice.varNAME().toAscii();
    QByteArray varvalue;
    QHostAddress varip = QHostAddress(indice.robotIP());

    switch(indice.varTYPE()){
    case KukaVar::INT:{
            //model->setData(index, static_cast<QSpinBox*>(editor)->value());
            varvalue.setNum(static_cast<QSpinBox*>(editor)->value());
            break;
        }
    case KukaVar::REAL:{
            static_cast<QDoubleSpinBox*>(editor)->setDecimals(5);
            //varvalue.setNum(static_cast<QDoubleSpinBox*>(editor)->value(),'e',5);
            varvalue.setNum(static_cast<QDoubleSpinBox*>(editor)->value());
            break;
        }
    case KukaVar::STRUCTURE:{
            varvalue = static_cast<PosSpinBox*>(editor)->value();
            break;
        }
    default:
        varvalue="";
    }
    qDebug() << "Valore precedente: " << indice.data(Qt::DisplayRole).toByteArray().trimmed();
    if(indice.data(Qt::DisplayRole).toByteArray().trimmed()!=varvalue){
        qDebug() << "Scrittura variabile " << varname << " valore " << varvalue << " ip " << varip;
        emit writevalue(varname,varvalue,varip);
    }
}

void VarEditDelegate::commitAndCloseEditor(const QString &text){
    QSpinBox* spinbox = qobject_cast<QSpinBox *>(sender());
    emit commitData(spinbox);
    emit closeEditor(spinbox);
}
