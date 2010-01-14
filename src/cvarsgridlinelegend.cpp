#include "cvarsgridlinelegend.h"
#include "cvarsgrid.h"

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

void CVarsGridLineLegend::mouseReleaseEvent( QMouseEvent* eve ){

	if( eve->button() != Qt::RightButton )
		return;

	QMenu* menu = new QMenu();

	connect( menu , SIGNAL(triggered(QAction*)) , this , SLOT(menuTrig(QAction*)));

	// Ogni voce deve corrispondere alla funzione menuTrig
	menu->addAction( QIcon( ":/images/graph_delete.png" ) , "Delete" );
	menu->addAction( QIcon( ":/images/graph_color.png" ) , "Change color" );
	menu->addAction( QIcon( ":/images/graph_plus.png" ) , "Increase width" );
	menu->addAction( QIcon( ":/images/graph_minus.png" ) , "Decrease width" );

	menu->popup( eve->globalPos() );
}

void CVarsGridLineLegend::menuTrig( QAction* act){
	if( act->text() == "Delete" ){
		((CVarsGrid*)parentWidget())->removeVar( m_name );
	}

	if( act->text() == "Increase width" ){
		m_line->setWidth( m_line->width() + 1 );
	}
	if( act->text() == "Decrease width" ){
		m_line->setWidth( m_line->width() - 1 );

	}

	if( act->text() == "Change color" ){
		QColor col( QColorDialog::getColor( m_line->color() ) );
		m_line->setColor( col );
	}

	repaint();
}

QSize CVarsGridLineLegend::sizeHint() const {
	return QSize( 50 , 50 );
}
