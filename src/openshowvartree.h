#ifndef OPENSHOWVARTREE_H
#define OPENSHOWVARTREE_H

#include <QtGui>
#include <QHashIterator>

#include "insertvar.h"
#include "robotvaredit.h"
#include "showvarconst.h"
#include "variabledb.h"
#include "cvarsgrid.h"
#include "listvarxml.h"
#include "CLog.h"
#include "CTreeVar.h"

class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;
class QIcon;

class OpenShowVarTree : public QWidget
{
	Q_OBJECT

public:
	OpenShowVarTree(QWidget *parent = 0);
	~OpenShowVarTree();

private:
	KukaVar *kukavar;
	RobotVarEdit *roboteditvar;
	QTimer qtimeLettura;
	QTimer timeUpdateGraph;
	//RobotServer *listaRobot;

	VariableDB* database;

	QTreeWidget *treeWidget;
	QPushButton *addButton;
	QPushButton *delButton;
	QPushButton *startButton;
	QPushButton *stopButton;
	QPushButton *findRobot;
	QPushButton *graphButton;
	QCheckBox *sotCheckBox;

	void addCombo(QTreeWidgetItem *child);
	void toBinary(int value, QString *binary);
	void toHex(int value, QString *binary);

		void splitvaluetoview(QTreeWidgetItem *item, QString varname, QString varvalue);

	static int VarValue;

	//grafico
	QList<CVarsGrid*>	m_gridList;
	QByteArray graphVarName;
	QHostAddress graphVarIP;

	//xml
	ListVarXml listVar;
	CLog cLog;

private slots:
	void DoubleClicked(QTreeWidgetItem * item, int column);
	void on_addButton_clicked();
	void on_delButton_clicked();
	void on_editButton_clicked();
	void on_startButton_clicked();
	void on_stopButton_clicked();
	void on_graphButton_clicked();
	void insertNew(const QString &variabile, const QString &robotip);
	void insertClose();
	void lettura();
	void updateGraph();
	void writeVariable(const QByteArray &varname, const QByteArray &value, const QHostAddress &varip);
	void closeEvent(QCloseEvent * event);
	void insertVar(const QString *varName);

	void on_stateChanged(int state);
};

#endif
