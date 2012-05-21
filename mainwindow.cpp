#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_message = new QMessageBox(this);
    m_socket = new QTcpSocket(this);
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(slTimer()));

    bool connected;

    connected = Conn();

    if(!connected)
        QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection); //quitter le programme
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

//création du dialog de connection attend qu'il soit connecter
bool MainWindow::Conn()
{
    login *log = new login(this);

    connect(log, SIGNAL(siTryConn(QString,QString,QString)), this, SLOT(slTryConn(QString,QString,QString)));
    connect(this, SIGNAL(siAccepted()), log, SLOT(slAccepted()));
    connect(log, SIGNAL(siTryNewUserConn(QString,QString,QString)), this, SLOT(slTryNewUserConn(QString,QString,QString)));

    int retour;
    m_nonConnect = true;

    while(m_nonConnect)
    {
        retour = log->exec();
        if(retour == QDialog::Rejected)
            return false;
    }
    return true;
}

//Enoyer un message
void MainWindow::on_btnEnvoyer_clicked()
{
    QByteArray trame;

    trame.append(MESSAGE + ui->txtEnvoyer->toPlainText());

    m_socket->write(trame);

    ui->txtEnvoyer->clear();
}
//Exclure un usager
void MainWindow::on_btnKick_clicked()
{
    QByteArray trame;
    trame.append(KICK + ui->txtUser->text());

    m_socket->write(trame);
}
//creer un usager
void MainWindow::on_btnCreer_clicked()
{
    QByteArray trame;
    trame.append(CREATE + ui->txtUser->text() + SEPARATOR + ui->txtUserMDP->text());

    m_socket->write(trame);
}
//supprimer un usager
void MainWindow::on_btnSupprimer_clicked()
{
    QByteArray trame;
    trame.append(DELETE + ui->txtUser->text());

    m_socket->write(trame);
}

void MainWindow::slTryConn(QString user, QString pass, QString ip)
{
    TryConn(user, pass, ip, LOGIN); //Connecter un utilisateur
}
void MainWindow::slTryNewUserConn(QString user, QString pass, QString ip)
{
    TryConn(user, pass, ip, CREATE); //Créer un utilisateur et le connecter
}

//Connection au serveur
void MainWindow::TryConn(QString user, QString pass, QString ip, char code)
{
    QByteArray trame;
    QString message = "", util = "";
    QList<QString> liste;
    int pos;

    trame.append(code + user + SEPARATOR + pass);

    m_socket->connectToHost(ip, 31331);

    if(!m_socket->waitForConnected(5000))
    {
        m_message->setText("Impossible de se connecter au serveur");
        m_message->exec();
        return;
    }

    m_socket->write(trame);

    if (m_socket->waitForReadyRead(5000))
    {
        trame = m_socket->readAll();

        switch(trame[0])
        {
        case ERROR:
            for (pos = 1; pos < trame.length(); pos++)
            {
                message += trame[pos];//Lecture du message d'erreur
            }

            m_message->setText(message);//Affichage du message et retour a l'authentification
            m_message->exec();
            break;
        case ALIVE:
            pos = 2;
            while(pos < trame.length())
            {
                if((char)trame[pos] == (char)SEPARATOR)//Quand on rencontre le séparateur
                {
                    liste.append(util);//Ajout d'un utilisateur a la liste
                    util = "";
                }
                else
                {
                    util += trame[pos];//Lecture de l'utilisateur
                }
                pos++;
            }
            liste.append(util);//Ajout du dernier utilisateur reçu

            m_liste = liste;
            timer->start(5000); //Actualisation de la liste de connection (5 secondes)

            //Activer les options administrateurs selon le status
            if((char)trame[1] == (char)USER)
                ui->gbOptionsAdmin->setEnabled(false);
            else
                if((char)trame[1] == (char)ADMIN)
                    ui->gbOptionsAdmin->setEnabled(true);

            connect(m_socket, SIGNAL(readyRead()), this, SLOT(messageRecu()));

            m_nonConnect = false;

            emit(siAccepted());

            break;
        }
    }
}

//Traitement des trames reçues
void MainWindow::messageRecu()
{
    QByteArray trame;
    QList<QString> liste;
    QString message = "", user = "";
    int pos;

    trame = m_socket->readAll();

    switch(trame[0])
    {
    case ALIVE:
        pos = 1;
        while(pos < trame.length())
        {
            if((char)trame[pos] == (char)SEPARATOR)//Quand on rencontre le séparateur
            {
                liste.append(user);//Ajout d'un utilisateur a la liste
                user = "";
            }
            else
            {
                user += trame[pos];//Lecture de l'utilisateur
            }
            pos++;
        }
        liste.append(user);//Ajout du dernier utilisateur reçu

        m_liste = liste;
        break;
    case MESSAGE:
        for(pos = 1; pos < trame.length(); pos++)
        {
            message += trame[pos];//Lecture du message
        }
        ui->lstMessages->setText(ui->lstMessages->toPlainText() + message + "\n");
        break;
    }
}

void MainWindow::slTimer()
{
    ui->lstUtils->clear();
    ui->lstUtils->addItems(m_liste);
}
