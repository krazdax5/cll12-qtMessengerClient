#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    //On entre les infos après être connecté au serveur
    ui->gbLogin->setEnabled(false);
    ui->gbInfoServeur->setEnabled(true);

    m_socket = new QTcpSocket(this); //initialisation du socket

    m_message = new QMessageBox(this);
}

login::~login()
{
    delete ui;
}

void login::on_btnboxServeur_accepted() //Bouton OK cliqué
{
    m_socket->connectToHost(ui->txtIPServeur->text(), 31331);

    if(!m_socket->waitForConnected(5000))
    {
        m_message->setText("Impossible de se connecter au serveur");
        m_message->exec();
        return;
    }

    ui->gbInfoServeur->setEnabled(false);
    ui->gbLogin->setEnabled(true);
}

void login::on_btnboxUtilisateur_accepted() //Bouton OK cliqué
{
    QByteArray trame;
    QString message = "", user = "";
    QList<QString> liste;
    int pos;
    //Construction de la trame
    trame.append(LOGIN + ui->txtUtilisateur->text() + SEPARATOR + ui->txtMDP->text());

    if(!m_socket->waitForReadyRead(5000))
    {
        m_message->setText("Impossible de joindre le serveur. Veuillez réessayer");
        m_message->exec();
        return;
    }

    m_socket->write(trame);

    if (!m_socket->waitForBytesWritten(5000))
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
            return;
        case ALIVE:
            pos = 2;
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
            liste.append(user);//Ajout du dernier utilisateur

            //Envoie des informations importantes a MainWindow
            emit(siMainWindow(m_socket->socketDescriptor(), liste, trame[1]));

            this->accept();
            break;
        }
    }
}

void login::on_btnAnnuler_clicked()
{
    this->reject();
}
