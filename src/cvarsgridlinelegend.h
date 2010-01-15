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
	~CVarsGridLineLegend();

	void paintEvent( QPaintEvent* );
	QSize sizeHint() const;

	void mouseReleaseEvent( QMouseEvent* eve );

public slots:

	void menuTrig( QAction* );
};

#endif // CVARSGRIDLINELEGEND_H
