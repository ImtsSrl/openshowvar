#ifndef CVARSGRIDLINELEGEND_H
#define CVARSGRIDLINELEGEND_H

#include <QtGui>
#include "CGridLine.h"

class CVarsGridLineLegend : public QWidget {

	Q_OBJECT

private:

	QString			m_name;
	CGridLine*		m_line;

public:
	CVarsGridLineLegend( CGridLine* , const QString& );

	void paintEvent( QPaintEvent* );
	QSize sizeHint() const;
};

#endif // CVARSGRIDLINELEGEND_H
