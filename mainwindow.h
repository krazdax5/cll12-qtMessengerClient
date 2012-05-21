#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTimer>
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
    bool m_nonConnect;
    QTcpSocket *m_socket;
    QMessageBox *m_message;
    QTimer *timer;

    bool Conn();
    void TryConn(QString, QString, QString, char);
    void sleep();

private slots:
    void on_btnEnvoyer_clicked();
    void on_btnKick_clicked();
    void on_btnCreer_clicked();
    void on_btnSupprimer_clicked();
    void messageRecu();
    void slTryConn(QString, QString, QString);
    void slTryNewUserConn(QString, QString, QString);
    void slTimer();

signals:
    void siAccepted();
};

#endif // MAINWINDOW_H
