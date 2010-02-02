
#include "CInlineEdit.h"

void CInlineEdit::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
//    int secs = index.model()->data(index, Qt::DisplayRole).toInt();
//    QString text = QString("CIAO");
//    QStyleOptionViewItem myOption = option;
//    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
//    drawDisplay(painter, myOption, myOption.rect, text);
//    drawFocus(painter, myOption, myOption.rect);
    qDebug() << "Paint: " << index.column();
    QItemDelegate::paint(painter, option, index);
}

QWidget *CInlineEdit::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    qDebug() << "Index: " << index.column();
    if(index.column()==1){
        QSpinBox *spinboxEdit = new QSpinBox(parent);
        //timeEdit->setDisplayFormat("mm:ss");
        //connect(timeEdit, SIGNAL(editingFinished()),this, SLOT(commitAndCloseEditor()));
        return spinboxEdit;
    }
    else
        return QItemDelegate::createEditor(parent, option, index);
}

void CInlineEdit::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    if(index.column()==1){
        QSpinBox *spinboxEdit = qobject_cast<QSpinBox *>(editor);
        spinboxEdit->setValue(30);
    }
    else
        QItemDelegate::setEditorData(editor, index);
}

void CInlineEdit::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
    QSpinBox *timeEdit = qobject_cast<QSpinBox *>(editor);
    int secs = timeEdit->value();
    model->setData(index, secs);
}

//void CInlineEdit::commitAndCloseEditor()
//{
//
//}
