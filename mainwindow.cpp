#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "th.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    login *log = new login(this);
    connect(log, SIGNAL(siMainWindow(int,QList<QString>,char)), this, SLOT(slMainWindow(int,QList<QString>,char)));
    log->exec();

    //A FAIRE: Gérer le this->reject()

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::slMainWindow(int socketDesc, QList<QString> liste, char ua)
{
    th *client = new th(this, socketDesc);

    //connection des messages avec la thread
    connect(client, SIGNAL(siIncommingMessage(QString)), this, SLOT(slIncommingMessage(QString)));
    connect(client, SIGNAL(siUpdateList(QList<QString>)), this, SLOT(slUpdateList(QList<QString>)));

    m_liste = liste;

    if(ua == (char)USER) //Mettre disponible les options d'administrateur selon le statut de l'usager
        ui->gbOptionsAdmin->setEnabled(false);
    else
        if(ua == (char)ADMIN)
            ui->gbOptionsAdmin->setEnabled(true);

    client->start();
}

void MainWindow::slIncommingMessage(QString message)
{

}

void MainWindow::slUpdateList(QList<QString>)
{

}
