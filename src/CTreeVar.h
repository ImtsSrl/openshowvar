#ifndef CTREEVAR_H
#define CTREEVAR_H

#include "showvarconst.h"
#include "kukavar.h"

#include <QtGui>
#include <QTreeWidget>

class CTreeVar : public QTreeWidget
{
	Q_OBJECT

	public:
		CTreeVar(QWidget *parent = 0);
		~CTreeVar();
		
	protected:
		void dropEvent(QDropEvent *event);
		void dragEnterEvent(QDragEnterEvent *event);
		void dragMoveEvent(QDragMoveEvent *event);
		
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		
	private:
		void startDrag();
		
		QPoint startPos;
		
	signals:
		void dropVar(const QString *varName);
};

#endif
