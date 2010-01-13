#ifndef CVARSGRID_H
#define CVARSGRID_H

#include "CGridField.h"
#include "variabledb.h"

class CVarsGrid : public QWidget {

	Q_OBJECT

private:

	CGridField			m_field;
	QList<CGridLine*>	m_lines;
	QStringList			m_linesVar;
	QStringList			m_linesIP;

	VariableDB*			m_dbVar;

public:

	CVarsGrid( VariableDB* );

	void addVar(const QString& , const QString&  );
	void update();


	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
};

#endif // CVARSGRID_H
