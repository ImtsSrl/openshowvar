

#ifndef INLINEEDIT_H
#define INLINEEDIT_H

#include <QItemDelegate>
#include <QSpinBox>
#include <QString>
#include <QDebug>

class CInlineEdit : public QItemDelegate
{
    Q_OBJECT

public:
    CInlineEdit(QWidget *parent = 0) : QItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

//private slots:
//    void commitAndCloseEditor();
};

#endif
