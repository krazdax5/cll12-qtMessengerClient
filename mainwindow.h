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

    QTcpSocket *m_socket;
    QList<QString> m_liste;
    int m_retour;

private slots:
    void slMainWindow(int, QList<QString>, char);
    void slIncommingMessage(QString);
    void slUpdateList(QList<QString>);
    void on_btnEnvoyer_clicked();
    void on_btnKick_clicked();
    void on_btnCreer_clicked();
    void on_btnSupprimer_clicked();
};

#endif // MAINWINDOW_H
