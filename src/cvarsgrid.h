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

public:

	CVarsGrid( VariableDB* );
	~CVarsGrid();

	void addVar(const QString& , const QString&  );
	void update();


	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
};

#endif // CVARSGRID_H
