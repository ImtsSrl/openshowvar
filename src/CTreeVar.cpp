/***************************************************************************
 *   Copyright (C) 2007 by Massimiliano Fago                               *
 *   massimiliano.fago@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/*!	\file
 *	\class CTreeVar
 *	\author Massimiliano Fago
 *	\version 1.1
 *	\date 2009
 *	\brief Reimplementazione TreeWidget per gestione Drop
 *
 */

#include "CTreeVar.h"

CTreeVar::CTreeVar(QWidget *parent)
	: QTreeWidget(parent)
{
	
}

CTreeVar::~CTreeVar()
{
	
}

void CTreeVar::dropEvent(QDropEvent *event)
{
	event->acceptProposedAction();
	
	emit dropVar(&event->mimeData()->text());
}

void CTreeVar::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasText()){
		event->acceptProposedAction();
	}
}

void CTreeVar::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasText()){
		event->acceptProposedAction();
	}
}


void CTreeVar::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		startPos=event->pos();
	QTreeWidget::mousePressEvent(event);
}

void CTreeVar::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons() & Qt::LeftButton){
		int distance=(event->pos() - startPos).manhattanLength();
		if(distance >= QApplication::startDragDistance())
			startDrag();
	}
	QTreeWidget::mouseMoveEvent(event);
}

void CTreeVar::startDrag()
{
    QByteArray varvalue;
    QTreeWidgetItem *item;
    if(this->currentItem()!=NULL){
        if(this->currentItem()->parent()!=NULL)
            item=this->currentItem()->parent();
        else
            item=this->currentItem();
    }

    //Se la variabile e' una struttura elimino il tipo di dato e passo solo il valore
    KukaVar *kukavar = new KukaVar(&item->text(VARNAME).toAscii(),&item->text(VARVALUE).toAscii());

    switch(kukavar->getVarType()){
    case STRUCTURE:
        {
            varvalue=kukavar->getStructureValue();
            break;
        }
    default:
        {
            varvalue=kukavar->getValue();
            break;
        }
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(varvalue);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    //QPixmap pixmap("C:/Documents and Settings/mfa/Documenti/GIT/openshowvar/AWESOM-O.png");
    QPixmap pixmap("AWESOM-O.png");
    QPixmap alphaChannel(pixmap.width(), pixmap.height());
    alphaChannel.fill(QColor(128,128,128));
    pixmap.setAlphaChannel(alphaChannel);
    drag->setPixmap(pixmap);

    Qt::DropAction dropAction = drag->exec();
}

