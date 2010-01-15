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

/*!	\file
 *	\class CGridLine
 *	\author Claudio Cannata'
 *	\version 1.0
 *	\date 2008
 *	\brief Gestione grafico
 *
 */

#include "CGridLine.h"
#include "float.h"

CGridLine::CGridLine(){
	m_values = new QList<CGridValue*>;

	m_color.setRgb( rand()%255, rand()%255, rand()%255 );
	m_maxValue = DBL_MIN;
	m_minValue = DBL_MAX;

	m_drawWidth = 1;
}

QColor CGridLine::color() const {
	return m_color;
}

void	CGridLine::getMinTimeValue( QTime* t ){
	if( m_values->size() < 1 ) return;

	*t = m_values->at(0)->m_time;
}

void	CGridLine::getMaxMinValues(double* max,double* min){
	if( max == NULL || min == NULL )
		return;

	*max = m_maxValue;
	*min = m_minValue;
}

float CGridLine::width(){
	return m_drawWidth;
}

void CGridLine::setWidth( float f ){
	if( f > 0 ) m_drawWidth = f;
}

void CGridLine::drawInWidget(QWidget* qw,QPainter* paint,QRect* drawR,double maxRange , double minRange , QTime* minTime ){
	int	items = m_values->size();
	double 	px,py;
	double	tx,ty;
	double	totalTime = 0;

	QPen pen( m_color );
	pen.setWidthF( m_drawWidth );
	pen.setBrush( QBrush( m_color ));
	paint->setPen( pen );

	if( items < 2 || drawR == 0)
		return;

	totalTime = ((*m_values)[m_values->size()-1])->m_time.msecsTo(*minTime);

	px = 0.0;
	py = (1 - (( m_values->at(0)->m_value - minRange ) / ( maxRange - minRange ))) * drawR->height();

	for(int i=1;i<items;i++){
		tx = ((*m_values)[i])->m_time.msecsTo(*minTime) / totalTime;
		tx *= drawR->width();

		ty = (1 - (( m_values->at(i)->m_value - minRange ) / ( maxRange - minRange ))) * drawR->height();

		paint->drawEllipse(QPoint((int)tx + drawR->x(),(int)ty + drawR->y()) , 2,2);
		paint->drawLine(
			(int)px + drawR->x(),
			(int)py + drawR->y(),

			(int)tx + drawR->x(),
			(int)ty + drawR->y());

		px = tx;
		py = ty;
	}

}

void CGridLine::setTooltip(const QString& str ){
	m_tooltip = new QLabel( str );
}

void CGridLine::setColor(QColor col){
	m_color = col;

	emit stateChanged();
}

void CGridLine::setMaxTime(int ms){
	m_msecMax = ms;
}

void CGridLine::addValue(double val){
	if( val > m_maxValue )
		m_maxValue = val;

	if( val < m_minValue )
		m_minValue = val;

	CGridValue *vval = new CGridValue(val);

	if( m_values->count() > 0 ){
		int timeTo = m_values->at(0)->m_time.msecsTo(vval->m_time);

		if( timeTo > m_msecMax )
			m_values->removeAt(0);
	}

	m_values->append( vval );

	emit stateChanged();
}

void CGridLine::clearAll(){
	m_values->clear();
	m_maxValue = DBL_MIN;
	m_minValue = DBL_MAX;

	emit stateChanged();
}

CGridLine::~CGridLine(){
	delete m_values;
}
