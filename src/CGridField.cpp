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

#include "CGridField.h"
#include <QPainter>
#include "cvarsgrid.h"

void CGridField::setMaxTime(int msec){
	m_MaxTimeMSec = msec;

	//Aggiorniamo tutte le linee del gra-fico
	int nLines = m_lineValue.count();
	for(int i=0;i<nLines;i++)
		m_lineValue[i]->setMaxTime(m_MaxTimeMSec);
}

int CGridField::getMaxTime(){
	return m_MaxTimeMSec;
}

CGridField::CGridField(){

	m_MarkPrec = 4;
	m_MaxTimeMSec = 3000;
	m_MarkOpacity = 0.3;

	setSizePolicy( QSizePolicy::Minimum , QSizePolicy::Minimum );

}

int CGridField::markPrecision(){
	return m_MarkPrec;
}

void CGridField::setMarkPrecision(int mp){
	if( mp > 0 )
		m_MarkPrec = mp;
}

void CGridField::setMarkOpacity(float v){
	if( v > 0 && v <= 1)
		m_MarkOpacity = v;
}

void CGridField::paintEvent(QPaintEvent* qpEve){
	QPainter paint(this);
	double	max = -999999,min = 999999;

	paint.setRenderHints(QPainter::Antialiasing);

	QRect	r(10,10,width()-55,height()-20);
	QTime	minTime; minTime = QTime::currentTime();

	CGridLine* ll;
	double tmin = min,tmx = max;
	QTime	ttime;

	// Calcolo il range dei valori per tutte le linee
	foreach( ll , m_lineValue ){
		ll->getMaxMinValues(&tmx,&tmin);

		max = qMax( tmx , max );
		min = qMin( tmin , min );

		ll->getMinTimeValue( &ttime );
		if( ttime.isNull() )
			minTime = ttime;
		else {
			minTime = qMin( ttime , minTime );
		}
	}

	// Ci aggiungo una tolleranza per evitare che i valori massimi e
	// minimi coincidano graficamente
	double plus = (max - min) * 0.15;
	min -= plus;
	max += plus;

	if( m_lineValue.size() > 0 ){
		//m_lineValue[0]->getMaxMinValues(&max,&min);

		paint.drawRect(r);
		paint.drawText(	12,	10,
					QString::number(max));

		paint.drawText(	12,	height() - 13,
					QString::number(min));

		int co = m_MarkPrec - 2;
		double jump = 0;
		float posy = r.height()+r.top();
		double val = 0;

		if( co > 0 ){
			jump = (max-min) / (float)(co+1);

			if( jump > 0 ){

				val = min;

				QPen pen_black;
				QPen pen_line;

				pen_line.setStyle(Qt::DashLine);
				pen_line.setColor(QColor(0,0,0,(int)( m_MarkOpacity * 255 )));

				for(int i=0;i<co;i++){
					val += jump;
					posy -= r.height() / (float)(co+1);

					paint.setPen(pen_black);
					paint.drawText(	12,	(int)posy,
							QString::number(val));

					paint.setPen(pen_line);
					paint.drawLine(r.left(),(int)posy,r.right(),(int)posy);
				}
			}
		}
	}

	int nLines = m_lineValue.count();

	for(int i=0;i<nLines;i++)
		m_lineValue[i]->drawInWidget(this,&paint,&r,max,min,&minTime);
}

void CGridField::updateState(){
	update();
}

void CGridField::removeLine(CGridLine * ln ){
	m_lineValue.removeOne(ln);
}

void CGridField::addLine(CGridLine* line){
	m_lineValue.append(line);

	line->setMaxTime(m_MaxTimeMSec);

	connect( line, SIGNAL( stateChanged() ), this, SLOT( updateState() ));

	update();
}
