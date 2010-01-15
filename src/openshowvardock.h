#ifndef OPENSHOWVARDOCK_H
#define OPENSHOWVARDOCK_H

#include <QMainWindow>

#include "CTreeVar.h"
#include "insertvar.h"
#include "variabledb.h"
#include "cvarsgrid.h"

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

private slots:
    void newVar();
    void deleteVar();
    void addGraph();

    void undo();
    void about();
    void insertCustomer(const QString &customer);

    void on_insertVar(const QString *varName);
    void insertNew(const QString &variabile, const QString &robotip);
    void insertClose(const bool &visible);
    void lettura();
    void updateGraph();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();

    void splitvaluetoview(QTreeWidgetItem *item, QString varname, QString varvalue);
    void addCombo(QTreeWidgetItem *child);
    void toBinary(int value, QString *binary);
    void toHex(int value, QString *binary);

    VariableDB* database;
    InsertVar* insertVar;

    QTimer qtimeLettura;
    QTimer timeUpdateGraph;
    QTreeWidget *treeWidget;

    QTextEdit *textEdit;
    QListWidget *customerList;
    QListWidget *paragraphsList;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newLetterAct;

    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;

    QAction *deleteVarAct;
    QAction *addGraphAct;

    //grafico
    QList<CVarsGrid*>	m_gridList;
    QByteArray graphVarName;
    QHostAddress graphVarIP;
};

#endif
