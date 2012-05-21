#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "codes.h"
#include <QTcpSocket>
#include <QAbstractButton>
#include <QMessageBox>
#include <QList>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT
    
public:
    explicit login(QWidget *parent = 0);
    ~login();
    
private slots:
    void on_btnboxServeur_accepted();
    void on_btnAnnuler_clicked();
    void on_btnNewUser_clicked();
    void slAccepted();

    void on_btnOK_clicked();

private:
    Ui::login *ui;

signals:
    void siTryConn(QString, QString, QString);
    void siTryNewUserConn(QString, QString, QString);
};

#endif // LOGIN_H
