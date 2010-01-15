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
		CGridLine* ln;
		foreach( ln , m_lines ){
			ln->clearAll();
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

	m_mutexUpdate.lock();

	for( int i = 0; i< m_lines.size(); i++ ){
		if( m_linesVar.at( i) == name ){

			m_field.removeLine( m_lines.at(i));

			delete (CGridLine*)m_lines.takeAt(i);

			m_linesVar.removeAt(i);

			m_linesIP.removeAt(i);

			delete (CVarsGridLineLegend*)m_legendsLayout->takeAt( m_legendsLayout->indexOf( m_legends.at(i) ) );

		}
	}

	m_mutexUpdate.unlock();
}

void CVarsGrid::addVar(const QString& str , const QString& ip){

	if( !m_field.isVisible() ){
		m_dragHere.hide();
		m_field.show();

		setGeometry( QRect( pos().x() , pos().y() , 400, 180 ));
	}

	CGridLine* ml = new CGridLine();

	m_field.addLine( ml );
	m_lines.append( ml );

	m_linesVar.append( str );
	m_linesIP.append( ip );

	CVarsGridLineLegend* leg = new CVarsGridLineLegend( ml , str );
	m_legends.append( leg );

	m_legendsLayout->removeItem( m_legendsLayout->itemAt( m_legendsLayout->count() - 1 ) );
	m_legendsLayout->addWidget( leg );
	m_legendsLayout->addStretch();
}

void CVarsGrid::update(){

	m_mutexUpdate.lock();

	int i=0;
	int temp;
	QByteArray value;

	for(i=0; i < m_lines.size() ; i++ ){

		m_dbVar->readVar( m_linesVar.at(i).toAscii() ,  QHostAddress( m_linesIP.at( i ) ) , &value , &temp );

		m_lines.at(i)->addValue( QString(value).toDouble() );
	}

	repaint();

	m_mutexUpdate.unlock();
}

CVarsGrid::~CVarsGrid(){
	int i=0;

	for(i=0; i < m_lines.size() ; i++ ){

		delete m_lines.at(i);
		delete m_legends.at(i);
	}
}
