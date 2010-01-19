#ifndef CVARSGRID_H
#define CVARSGRID_H

#include "CGridField.h"
#include "variabledb.h"

#include "cvarsgridlinelegend.h"

#include <QtXml>

class CVarsGrid : public QWidget {

	Q_OBJECT

private:

	CGridField						m_field;

	QList<CGridLine*>				m_lines;
	QStringList						m_linesVar;
	QStringList						m_linesIP;
	QList<CVarsGridLineLegend*>		m_legends;

	VariableDB*			m_dbVar;

	QVBoxLayout*		m_legendsLayout;

	QLabel				m_dragHere;

	QMutex				m_mutexUpdate;

	QToolBar			m_mainToolbar;
	QAction*			m_maintoolMarkDot;
	QAction*			m_maintoolBall;
	QAction*			m_maintoolClear;

public:

	CVarsGrid( VariableDB* );
	~CVarsGrid();

	void addVar(const QString& , const QString&  );
	void removeVar( const QString& );

	void update();

	void mouseReleaseEvent( QMouseEvent* eve );

	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

	void loadFromXml( QDomElement* );
	const QDomElement& saveToXml( QDomElement* );

	static const QList<CVarsGrid*>& loadAllFromXml( const QString& file , VariableDB* vardb );
	static void saveAllToXml(const QList<CVarsGrid*>& list , const QString& file );

	const QSize sizeHint();

public slots:

	void menuTrig( QAction* );
};

#endif // CVARSGRID_H
