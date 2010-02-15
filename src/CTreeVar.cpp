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

CTreeVar::CTreeVar(QWidget *parent) : QTreeView(parent) { }

CTreeVar::~CTreeVar() { }

void CTreeVar::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();
    if(event->source()!=this){
        emit dropVar(event->mimeData()->text());
    }
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

void CTreeVar::startDrag()
{

}
