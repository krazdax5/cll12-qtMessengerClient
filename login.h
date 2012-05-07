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

    void on_btnboxUtilisateur_accepted();

    void on_btnAnnuler_clicked();

private:
    Ui::login *ui;
    QMessageBox *m_message;
    QTcpSocket *m_socket;

signals:
    void siMainWindow(int, QList<QString>, char);
};

#endif // LOGIN_H
