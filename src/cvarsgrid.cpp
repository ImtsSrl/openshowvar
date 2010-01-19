#include "cvarsgrid.h"

CVarsGrid::CVarsGrid( VariableDB* db ){
	m_dbVar = db;

	m_dragHere.setText( "Drag Variable Here!");

	QVBoxLayout* vlay = new QVBoxLayout();

	m_maintoolMarkDot = m_mainToolbar.addAction( "Dot" );
	//m_maintoolBall = m_mainToolbar.addAction( "Ball" );
	m_maintoolClear = m_mainToolbar.addAction( "Clr" );
	vlay->addWidget( &m_mainToolbar );
	connect( &m_mainToolbar,SIGNAL(actionTriggered(QAction*)),this,SLOT(menuTrig(QAction*)));

	QHBoxLayout* lay = new QHBoxLayout();

	lay->addWidget( &m_field );
	lay->addWidget( &m_dragHere );

	m_field.setMarkPrecision( 10 );
	m_field.setMaxTime( 60000 );

	//m_field.setFocusProxy( this );

	m_legendsLayout = new QVBoxLayout();

	lay->addLayout( m_legendsLayout );

	vlay->addLayout( lay );
	setLayout( vlay );

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

const QList<CVarsGrid*>& CVarsGrid::loadAllFromXml( const QString& filename , VariableDB* vardb ) {
	QDomDocument main;
	QList<CVarsGrid*> ret;

	QFile file(filename);
	file.open(QIODevice::ReadOnly);

	main.setContent(&file);
	file.close();

	QDomNode node = main.documentElement().firstChild();
	QDomNode child;

	CVarsGrid* grid;

	while( !node.isNull() ){

	grid = new CVarsGrid( vardb );
	ret.append( grid );

	QDomElement ele = node.toElement();
	grid->loadFromXml( &ele );

	node = node.nextSibling();
	}

	qDebug() << "loading done";
	return ret;
}

void CVarsGrid::loadFromXml( QDomElement* dom ){
	QRect rct;

	/*rct.setX( dom->attribute( "X" , "10" ).toInt() );
	rct.setY( dom->attribute( "Y" , "10" ).toInt() );
	rct.setWidth( dom->attribute( "WIDTH" , "100" ).toInt() );
	rct.setHeight( dom->attribute( "HEIGHT" , "200" ).toInt() );*/

	QDomElement ln = dom->firstChildElement( "LINE" );

	while( !ln.isNull() ){
		QString name ( ln.attribute( "NAME" , "" ) );
		QString ip ( ln.attribute( "IP" , "" ) );

		if( name.length() > 0 && ip.length() > 0 )
			addVar(name,ip);

		m_lines.last()->loadAttributeXml( &ln );

		ln = ln.nextSiblingElement( "LINE" );
	}

	//setGeometry( rct );
}

void CVarsGrid::saveAllToXml(const QList<CVarsGrid*>& list , const QString& filename ){
	QDomDocument main;
	CVarsGrid* grid;

	foreach( grid , list ){

	QDomElement grids = main.createElement("VARSGRIDS");

	grid->saveToXml( &grids );

	main.appendChild( grids );

	}

	QFile file( filename );
	QTextStream out(&file);
	file.open(QIODevice::WriteOnly);
	const int Indent=4;
	main.save(out, Indent);
	file.close();
}

const QDomElement& CVarsGrid::saveToXml( QDomElement* main ){


	QDomElement vg = main->ownerDocument().createElement( "VARSGRID" );

	/*vg.setAttribute( "X" , geometry().x() );
	vg.setAttribute( "Y" , geometry().y() );
	vg.setAttribute( "WIDTH" , geometry().width() );
	vg.setAttribute( "HEIGHT" , geometry().height() );*/

	for(int i = 0; i < m_lines.size(); i++ ){
	QDomElement ln = main->ownerDocument().createElement( "LINE" );

	ln.setAttribute( "NAME" , m_linesVar[i] );
	ln.setAttribute( "IP" , m_linesIP[i] );

	m_lines[i]->saveAttributeXml( &ln );

	vg.appendChild( ln );
	}

	main->appendChild( vg );
}

void CVarsGrid::menuTrig( QAction* act ){

	if( act == m_maintoolMarkDot ){
		if( m_field.markOpacity() != 0 )
			m_field.setMarkOpacity( 0 );
		else
			m_field.setMarkOpacity( 0.3 );
	}

	if( act->text() == "Clear" || act == m_maintoolClear ){
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

const QSize CVarsGrid::sizeHint(){
	return QSize(200 , 60 );
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
