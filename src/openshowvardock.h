#ifndef OPENSHOWVARDOCK_H
#define OPENSHOWVARDOCK_H

#include <QMainWindow>

#include "CTreeVar.h"
#include "insertvar.h"
#include "variabledb.h"

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

    void print();
    void undo();
    void about();
    void insertCustomer(const QString &customer);

    void insertVar(const QString *varName);
    void insertNew(const QString &variabile, const QString &robotip);
    void insertClose();
    void lettura();

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

    QTimer qtimeLettura;
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

    QAction *printAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;

    QAction *deleteVarAct;
};

#endif
