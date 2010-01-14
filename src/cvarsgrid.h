#ifndef CVARSGRID_H
#define CVARSGRID_H

#include "CGridField.h"
#include "variabledb.h"

#include "cvarsgridlinelegend.h"

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

public:

	CVarsGrid( VariableDB* );
	~CVarsGrid();

	void addVar(const QString& , const QString&  );
	void removeVar( const QString& );

	void update();

	void mouseReleaseEvent( QMouseEvent* eve );

	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

public slots:

	void menuTrig( QAction* );
};

#endif // CVARSGRID_H
