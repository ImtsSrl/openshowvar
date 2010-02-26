#include <QPainter>
#include <QSlider>
#include <QModelIndex>
#include <QComboBox>
#include <QApplication>

#include "formatdelegate.h"

FormatDelegate::FormatDelegate( QObject *parent ) : QStyledItemDelegate( parent ) { }

void FormatDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    //QStyledItemDelegate::paint(painter, option, index);

    ShowModelIndex indice=index;

    if(indice.isInt(1) || indice.data(Qt::DisplayRole)==tr("Hex code") || indice.data(Qt::DisplayRole) == tr("Binary code")){
        QStyleOptionComboBox comboBoxOption;
        //comboBoxOption.init((QWidget*)parent());
        comboBoxOption.state |= QStyle::State_Enabled;
        comboBoxOption.state &= !QStyle::State_HasFocus;
        comboBoxOption.direction = QApplication::layoutDirection();
        comboBoxOption.rect = option.rect;
        comboBoxOption.fontMetrics = QApplication::fontMetrics();
        if(indice.data(Qt::DisplayRole).toString()=="")
            comboBoxOption.currentText = tr("Int code");
        else
            comboBoxOption.currentText = indice.data(Qt::DisplayRole).toString();
        comboBoxOption.editable = false;
        //QApplication::style()->drawComplexControl(QStyle::CC_ComboBox,
        //                                          &comboBoxOption, painter, (QWidget*)parent());
        QApplication::style()->drawComplexControl(QStyle::CC_ComboBox,
                                                  &comboBoxOption, painter);
        QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel,
                                           &comboBoxOption, painter);
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize FormatDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  return QSize( 45, 20 );
}

QWidget *FormatDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    QComboBox *combo = new QComboBox(parent);
    combo->addItem(tr("Int code"));
    combo->addItem(tr("Binary code"));
    combo->addItem(tr("Hex code"));

    connect(combo,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(commitAndCloseEditor(const QString &)));

//    combo->installEventFilter( const_cast<FormatDelegate*>(this) );

    return combo;
}

void FormatDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  editor->setGeometry( option.rect );
}

void FormatDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    QString value = index.model()->data( index, Qt::DisplayRole ).toString();
    //QString value = static_cast<QComboBox*>(editor)->currentText();
    int comboindex = static_cast<QComboBox*>(editor)->findText(value,Qt::MatchExactly);
    qDebug() << "Valore: " << value << " indice: " << comboindex;
    if(comboindex==-1)
        comboindex=0;
    static_cast<QComboBox*>(editor)->setCurrentIndex(comboindex);
}

void FormatDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
  //model->setData( index, static_cast<QSlider*>( editor )->value() );
  model->setData(index, static_cast<QComboBox*>( editor )->currentText());
}

void FormatDelegate::commitAndCloseEditor(const QString &text){
    QComboBox* combo = qobject_cast<QComboBox *>(sender());
    emit commitData(combo);
    emit closeEditor(combo);
}
