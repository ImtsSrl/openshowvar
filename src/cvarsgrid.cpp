#include "cvarsgrid.h"

CVarsGrid::CVarsGrid( VariableDB* db ){
	m_dbVar = db;

	m_dragHere.setText( "Drag Variable Here!");

	QHBoxLayout* lay = new QHBoxLayout();
	lay->addWidget( &m_field );
	lay->addWidget( &m_dragHere );

	m_field.setMarkPrecision( 10 );
	m_field.setMaxTime( 60000 );

	//m_field.setFocusProxy( this );

	m_legendsLayout = new QVBoxLayout();

	lay->addLayout( m_legendsLayout );

	setLayout( lay );

	m_field.hide();

	show();

	setAcceptDrops( true );
}

void CVarsGrid::mouseReleaseEvent( QMouseEvent* eve ){

	if( eve->button() != Qt::RightButton )
		return;

	QMenu *menu = new QMenu();

	connect( menu , SIGNAL(triggered(QAction*)), this , SLOT(menuTrig(QAction*)));

	menu->addAction( QIcon( ":/images/graph_delete.png" ) , "Clear" );
	menu->addAction( QIcon( ":/images/graph_plus.png" ) , "Increase details" );
	menu->addAction( QIcon( ":/images/graph_minus.png" ) , "Decrease details" );

	menu->popup( eve->globalPos() );
}

void CVarsGrid::menuTrig( QAction* act ){
	if( act->text() == "Clear" ){
		CVarsGrid_Line* ln;
		foreach( ln , m_lines ){
			ln->line->clearAll();
		}
	}

	if( act->text() == "Increase details" ){
		m_field.setMarkPrecision( m_field.markPrecision() +  2 );
		repaint();
	}

	if( act->text() == "Decrease details" ){
		m_field.setMarkPrecision( m_field.markPrecision() -  2 );
		repaint();
	}
}

void CVarsGrid::dropEvent(QDropEvent *event) {
	 event->acceptProposedAction();

	 QString varname( event->mimeData()->data( "openshowvar/graphdata" ) );
	 QString ip( event->mimeData()->data("openshowvar/graphdataip") );

	 addVar( varname , ip );
 }

void CVarsGrid::dragEnterEvent(QDragEnterEvent *event) {
	 if (event->mimeData()->hasFormat("openshowvar/graphdata") && event->mimeData()->hasFormat("openshowvar/graphdataip"))
		 event->acceptProposedAction();
 }

void CVarsGrid::removeVar( const QString& name ){

	CVarsGrid_Line* ln = 0;
	foreach( ln , m_lines ){
		if( ln->m_var == name ){

			m_field.removeLine( ln->line );
			m_lines.removeOne( ln );
			delete ln->line;

			m_legendsLayout->removeWidget( ln->m_legend );
			delete ln->m_legend;
			break;
		}
	}

}

void CVarsGrid::addVar(const QString& str , const QString& ip){

	if( !m_field.isVisible() ){
		m_dragHere.hide();
		m_field.show();

		setGeometry( QRect( pos().x() , pos().y() , 400, 180 ));
	}

	CVarsGrid_Line* ln = new CVarsGrid_Line;

	ln->line = new CGridLine();
	ln->m_var = str;
	ln->m_ip = ip;
	ln->m_legend = new CVarsGridLineLegend( ln->line , ln->m_var );

	m_lines.append( ln );
	m_field.addLine( ln->line );

	// toglio l'ultimo item che dovrebbe essere uno Stretch
	m_legendsLayout->removeItem( m_legendsLayout->itemAt( m_legendsLayout->count() - 1 ) );
	m_legendsLayout->addWidget( ln->m_legend );
	m_legendsLayout->addStretch();
}

void CVarsGrid::update(){

	CVarsGrid_Line* ln;
	int temp;
	QByteArray value;

	foreach( ln , m_lines ){

		m_dbVar->readVar( ln->m_var.toAscii() ,  QHostAddress( ln->m_ip ) , &value , &temp );

		ln->line->addValue( QString(value).toDouble() );
	}

	repaint();
}

CVarsGrid::~CVarsGrid(){

	CVarsGrid_Line* ln;
	foreach( ln , m_lines ){
		delete ln->line;
		delete ln->m_legend;
	}
}
