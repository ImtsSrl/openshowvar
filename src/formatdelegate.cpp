#include <QPainter>
#include <QSlider>
#include <QModelIndex>
#include <QComboBox>

#include "formatdelegate.h"

FormatDelegate::FormatDelegate( QObject *parent ) : QStyledItemDelegate( parent ) { }

void FormatDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
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
    static_cast<QComboBox*>(editor)->setCurrentIndex(1);
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
