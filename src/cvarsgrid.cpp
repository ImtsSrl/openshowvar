#include "cvarsgrid.h"

CVarsGrid::CVarsGrid( VariableDB* db ){
	m_dbVar = db;

	QHBoxLayout* lay = new QHBoxLayout();
	lay->addWidget( &m_field );

	m_field.setMarkPrecision( 10 );
	m_field.setMaxTime( 60000 );

	m_legendsLayout = new QVBoxLayout();

	lay->addLayout( m_legendsLayout );

	setLayout( lay );
	show();

	setAcceptDrops( true );
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

void CVarsGrid::addVar(const QString& str , const QString& ip){

	CGridLine* ml = new CGridLine();

	m_field.addLine( ml );
	m_lines.append( ml );

	m_linesVar.append( str );
	m_linesIP.append( ip );

	CVarsGridLineLegend* leg = new CVarsGridLineLegend( ml , str );
	m_legends.append( leg );

	m_legendsLayout->addWidget( leg );
}

void CVarsGrid::update(){

	int i=0;
	int temp;
	QByteArray value;

	for(i=0; i < m_lines.size() ; i++ ){

		m_dbVar->readVar( m_linesVar.at(i).toAscii() ,  QHostAddress( m_linesIP.at( i ) ) , &value , &temp );

		m_lines.at(i)->addValue( QString(value).toDouble() );
	}

	repaint();
}

CVarsGrid::~CVarsGrid(){
	int i=0;

	for(i=0; i < m_lines.size() ; i++ ){

		delete m_lines.at(i);
		delete m_legends.at(i);
	}
}
