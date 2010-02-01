#ifndef OPENSHOWVARDOCK_H
#define OPENSHOWVARDOCK_H

#include <QMainWindow>

#include "CTreeVar.h"
#include "insertvar.h"
#include "variabledb.h"
#include "cvarsgrid.h"
#include "robotvaredit.h"
#include "listvarxml.h"
#include "CLog.h"

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
QT_END_NAMESPACE

class OpenShowVarDock : public QMainWindow
{
    Q_OBJECT

public:
    OpenShowVarDock();
    ~OpenShowVarDock();

private slots:
    void newVar();
    void deleteVar();
    void addGraph();
    void on_editVar();

    void about();

    void on_insertVar(const QString *varName);
    void on_refVarAct(const QString &text);
    void on_refVarDatabaseAct(int i);
    void on_valueChanged(int i);
    void on_readVarAct(bool checked);
    void on_onTopAct(bool checked);
    void on_log(bool checked);
    void on_writeVariable(const QByteArray &varname, const QByteArray &value, const QHostAddress &varip);
    void on_saveVar();
    void on_openVar();
    void on_clearList();
    void on_itemClicked(QTreeWidgetItem *item, int column);
    void on_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void closeEvent(QCloseEvent * event);
    void insertNew(const QString &variabile, const QString &robotip);
    void insertClose(const bool &visible);
    void on_editVarClose(const bool &visible);
    void lettura();
    void updateGraph();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    void splitvaluetoview(QTreeWidgetItem *item, QString varname, QString varvalue);
    void editVar(QTreeWidgetItem * item);
    void addCombo(QTreeWidgetItem *child);
    void toBinary(int value, QString *binary);
    void toHex(int value, QString *binary);

    QDockWidget *dockInsertVar;

    VariableDB* database;
    InsertVar* insertVar;

    QTimer qtimeLettura;
    QTimer timeUpdateGraph;
    QTreeWidget *treeWidget;

    QTextEdit *textEdit;
    QListWidget *customerList;
    QListWidget *paragraphsList;

    QMenu *robotMenu;

    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QToolBar *robotToolBar;

    QToolBar *editToolBar;

    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;

    QAction *newVarAct;
    QAction* readVarAct;
    QAction* onTopAct;
    QComboBox *refVarAct;
    QSpinBox* refVarDatabaseAct;
    QAction *deleteVarAct;
    QAction *addGraphAct;
    QAction *editVarAct;
    QAction *saveVarAct;
    QAction *openVarAct;
    QAction *clearListAct;
    QAction* logAct;

    ListVarXml listVar;

    //grafico
    QList<CVarsGrid*>	m_gridList;
    QByteArray graphVarName;
    QHostAddress graphVarIP;

    //log
    CLog* cLog;
    bool saveLog;
};

#endif
