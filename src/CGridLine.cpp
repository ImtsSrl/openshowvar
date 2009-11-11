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

CGridLine::CGridLine(){
	m_values = new QList<CGridValue*>;
	
	m_color.setRgb( rand()%255, rand()%255, rand()%255 );
	m_maxValue = -999999;
	m_minValue = 999999;
}

void	CGridLine::getMaxMinValues(double* max,double* min){
	if( max == NULL || min == NULL )
		return;

	*max = m_maxValue;
	*min = m_minValue;
}

void CGridLine::drawInWidget(QWidget* qw,QPainter* paint,QRect* drawR){
	int	items = m_values->size();
	double 	px,py;
	double	totalTime = 0;
	double 	jump = 0.0;
	double 	tempy = 0.0;
	
	paint->setPen( m_color );
	
	if( items < 2 || drawR == 0)
		return;
	
	totalTime = ((*m_values)[m_values->size()-1])->m_time.msecsTo(((*m_values)[0])->m_time);
	
	
	
	px = 0.0;
	py = drawR->height() - (((m_values->at(0)->m_value) / m_maxValue) * (double)drawR->height());
	
	for(int i=1;i<items;i++){
		tempy = (drawR->height() - ((m_values->at(i)->m_value) / m_maxValue) *  (double)drawR->height());
		
		jump = ((m_values->at(i)->m_time.msecsTo(m_values->at(i-1)->m_time)) / totalTime) * drawR->width();
		
		paint->drawLine(
			px + drawR->x(),
			py + drawR->y(),

			px+jump + drawR->x(),
			tempy + drawR->y());
		
			
		py = tempy;
		px += jump;
	}
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
	m_maxValue = -999999;
	m_minValue = 999999;
	
	emit stateChanged();
}

CGridLine::~CGridLine(){
	delete m_values;
}
