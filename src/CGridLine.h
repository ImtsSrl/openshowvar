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

#ifndef CGRIDLINE_H
#define CGRIDLINE_H

#include <QList>
#include <QPainter>
#include <QWidget>
#include <QLabel>

#include "CGridValue.h"

#include <QtXml>

class CGridField;

class CGridLine  : public QObject {

	Q_OBJECT

	friend class CGridField;

	private:

		int			m_msecMax;

		QColor			m_color;
		QList<CGridValue*>	*m_values;

		double		m_maxValue;
		double		m_minValue;

		QLabel*		m_tooltip;

		float		m_drawWidth;

	public:

		CGridLine();
		~CGridLine();

		void 	drawInWidget(QWidget* ,QPainter* ,QRect* , double maxRange , double minRange , QTime* minTime );

		void	getMaxMinValues(double* max,double* min);
		void	getMinTimeValue( QTime* );

		void 	addValue(double );
		void 	clearAll();

		void	setMaxTime(int );

		void 	setColor(QColor col);
		QColor	color() const;

		float	width();
		void	setWidth( float );

		void	setTooltip(const QString& );

		void	saveAttributeXml( QDomElement* );
		void	loadAttributeXml( QDomElement* );

	signals:

		void 	stateChanged();
};

#endif
