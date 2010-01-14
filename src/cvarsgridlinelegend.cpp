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
	QMenu* menu = new QMenu();

	connect( menu , SIGNAL(triggered(QAction*)) , this , SLOT(menuTrig(QAction*)));

	menu->addAction( QIcon( "graph_delete.png" ) , "Delete" );
	menu->addAction( QIcon( "graph_color.png" ) , "Change color" );

	menu->popup( eve->globalPos() );
}

void CVarsGridLineLegend::menuTrig( QAction* act){
	if( act->text() == "Delete" ){
		((CVarsGrid*)parentWidget())->removeVar( m_name );
	}

	if( act->text() == "Change color" ){
		QColor col( QColorDialog::getColor( m_line->color() ) );
		m_line->setColor( col );

		repaint();
	}
}

QSize CVarsGridLineLegend::sizeHint() const {
	return QSize( 50 , 50 );
}
