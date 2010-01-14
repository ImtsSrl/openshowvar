/***************************************************************************
 *   Copyright (C) 2007 by Claudio                                         *
 *   cyberpro@cyberpro-laptop                                              *
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

#ifndef CGRIDFIELD_H
#define CGRIDFIELD_H

#include <QtGui>
#include "CGridLine.h"

class CGridField : public QWidget {

	Q_OBJECT

	public:

		int		m_MaxTimeMSec;

		void		setMaxTime(int msec);
		int		getMaxTime();

	private:

		QList<CGridLine*>	m_lineValue;
		int			m_MarkPrec;
		float			m_MarkOpacity;

	public:

		CGridField();

		void addLine(CGridLine* );
		void removeLine( CGridLine* );

		void setMarkOpacity(float );

		void setMarkPrecision(int );
		int markPrecision();


		void mouseReleaseEvent( QMouseEvent* eve );


	protected:

		void paintEvent(QPaintEvent* qpEve);

	public slots:

		void updateState();
};

#endif
