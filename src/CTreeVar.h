#ifndef CTREEVAR_H
#define CTREEVAR_H

#include "showvarconst.h"
#include "kukavar.h"

#include <QtGui>
#include <QTreeView>

class CTreeVar : public QTreeView
{
	Q_OBJECT

	public:
		CTreeVar(QWidget *parent = 0);
		~CTreeVar();
		
	protected:
		void dropEvent(QDropEvent *event);
		void dragEnterEvent(QDragEnterEvent *event);
		void dragMoveEvent(QDragMoveEvent *event);
		
	private:
		void startDrag();
		
		QPoint startPos;
		
	signals:
                void dropVar(const QString& varName);
};

#endif
