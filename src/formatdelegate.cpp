
#include <QPainter>
#include <QSlider>
#include <QModelIndex>
#include <QComboBox>

#include "formatdelegate.h"

FormatDelegate::FormatDelegate( QObject *parent ) : QAbstractItemDelegate( parent ) { }

void FormatDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{

}

QSize FormatDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  return QSize( 45, 15 ); 
}

QWidget *FormatDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    QComboBox *combo = new QComboBox(parent);
    combo->addItem(tr("Int code"));
    combo->addItem(tr("Binary code"));
    combo->addItem(tr("Hex code"));

    combo->installEventFilter( const_cast<FormatDelegate*>(this) );

    return combo;
}

void FormatDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
  editor->setGeometry( option.rect );
}

void FormatDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
  int value = index.model()->data( index, Qt::DisplayRole ).toInt();
  static_cast<QSlider*>( editor )->setValue( value );
}

void FormatDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
  model->setData( index, static_cast<QSlider*>( editor )->value() );
}

