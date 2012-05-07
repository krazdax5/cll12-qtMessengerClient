#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
#include "codes.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

    QList<QString> m_liste;

private slots:
    void slMainWindow(int, QList<QString>, char);
    void slIncommingMessage(QString);
    void slUpdateList(QList<QString>);
};

#endif // MAINWINDOW_H
