#include "cvarsgridlinelegend.h"

CVarsGridLineLegend::CVarsGridLineLegend( CGridLine* l , const QString& name ){
	m_name = name;
	m_line = l;

	setSizePolicy( QSizePolicy::Maximum , QSizePolicy::Maximum );
}

void CVarsGridLineLegend::paintEvent( QPaintEvent* eve ){
	QPainter paint( this );

	paint.fillRect( QRect(0,2,10,10) , m_line->color() );

	paint.drawText( QRect(13,0,100,100) , m_name );
}

QSize CVarsGridLineLegend::sizeHint() const {
	return QSize( 50 , 50 );
}
