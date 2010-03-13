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

    if(indice.isInt(1)){
        QSpinBox *spinbox = new QSpinBox(parent);

        spinbox->setValue(index.data(Qt::DisplayRole).toInt());

    //    connect(spinbox,SIGNAL(valueChanged(QString)),this,SLOT(commitAndCloseEditor(const QString &)));
    //    combo->installEventFilter( const_cast<FormatDelegate*>(this) );
        return spinbox;
    }
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

    //model->setData(index, static_cast<QSpinBox*>(editor)->value());
    QByteArray varname = indice.varNAME().toAscii();
    QByteArray varvalue;
    varvalue.setNum(static_cast<QSpinBox*>(editor)->value());
    QHostAddress varip = QHostAddress(indice.robotIP());
    qDebug() << "Scrittura variabile " << varname << " valore " << varvalue << " ip " << varip;
    emit writevalue(varname,varvalue,varip);
}

void VarEditDelegate::commitAndCloseEditor(const QString &text){
    QSpinBox* spinbox = qobject_cast<QSpinBox *>(sender());
    emit commitData(spinbox);
    emit closeEditor(spinbox);
}
