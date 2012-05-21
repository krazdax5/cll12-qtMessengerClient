#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    ui->gbLogin->setEnabled(false);
    ui->gbInfoServeur->setEnabled(true);
}

login::~login()
{
    delete ui;
}

void login::on_btnboxServeur_accepted() //Bouton OK cliqué
{
    ui->gbLogin->setEnabled(true);
}

void login::slAccepted()
{
    this->accept();
}

void login::on_btnAnnuler_clicked()
{
    this->reject();
}

void login::on_btnNewUser_clicked()
{
    emit(siTryNewUserConn(ui->txtUtilisateur->text(), ui->txtMDP->text(), ui->txtIPServeur->text()));
}

void login::on_btnOK_clicked()
{
    emit(siTryConn(ui->txtUtilisateur->text(), ui->txtMDP->text(), ui->txtIPServeur->text()));
}
