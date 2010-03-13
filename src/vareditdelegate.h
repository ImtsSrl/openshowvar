#ifndef VAREDITDELEGATE_H
#define VAREDITDELEGATE_H

#include <QStyledItemDelegate>
#include <QDebug>

#include "CShowModelIndex.h"

class VarEditDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
  VarEditDelegate( QObject *parent = 0 );

  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
  QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;

  QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
  void setEditorData( QWidget *editor, const QModelIndex &index ) const;
  void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
  void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const;  

private:
  //QVariant valueatedit;

private slots:
  void commitAndCloseEditor(const QString &text);

};

#endif
