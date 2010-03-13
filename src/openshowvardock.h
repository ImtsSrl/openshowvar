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
#include "robot_view/ckukarobot.h"
#include "treemodel.h"
#include "formatdelegate.h"
#include "vareditdelegate.h"

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

    void showHideRobotView();

    void about();

    void on_insertVar(const QString &varName);
    void on_refVarAct(const QString &text);
    void on_refVarDatabaseAct(int i);
    void on_readVarAct(bool checked);
    void on_onTopAct(bool checked);
    void on_log(bool checked);
    void on_writeVariable(const QByteArray &varname, const QByteArray &value, const QHostAddress &varip);
    void on_saveVar();
    void on_openVar();
    void on_clearList();
    void on_itemDoubleClicked(const QModelIndex &index);
    void closeEvent(QCloseEvent * event);
    void insertNew(const QString &variabile, const QString &robotip);
    void insertClose(const bool &visible);
    void on_editVarClose(const bool &visible);
    void lettura();
    void updateGraph();
    void on_dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    void splitvaluetoview(QModelIndex index, QString varname, QString varvalue);
    void editVar(QString varname, QString varvalue, QHostAddress varip);
    void addCombo(QTreeWidgetItem *child);
    QString toBinary(int value);
    QString toHex(int value);

    QDockWidget *dockInsertVar;

    VariableDB*     database;
    InsertVar*      insertVar;
    CKUKARobot*     m_robotView;

    CTreeVar* tree;
    TreeModel *model;

    QTimer qtimeLettura;
    QTimer timeUpdateGraph;

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

    QToolBar*   m_robotViewToolbar;
    QAction*    m_showRobotView;

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
