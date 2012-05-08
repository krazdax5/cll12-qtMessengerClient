#include "th.h"
#include <QTcpSocket>
#include "codes.h"

th::th(QObject *parent, int socketDesc) :
    QThread(parent)
{
    m_SocketDesc = socketDesc;
    m_connected = true;

    msgBox = new QMessageBox();
}

void th::run()
{
    QTcpSocket socket;
    QByteArray trame;
    QList<QString> liste;
    QString user = "", message;

    socket.setSocketDescriptor(m_SocketDesc);

    while(m_connected)
    {
        message = "";
        socket.waitForBytesWritten();
        trame = socket.readAll();
        int pos;

        switch(trame[0])
        {
        case ALIVE: //Trame Alive
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
            liste.append(user);

            emit(siUpdateList(liste));

            break;
        case MESSAGE: //Trame Message
            for(pos = 1; pos < trame.length(); pos++)
            {
                message += trame[pos]; //Lecture du message
            }

            emit(siIncommingMessage(message));

            break;
        case ERROR: //Trame Erreur
            for (pos = 1; pos < trame.length(); pos++)
            {
                message += trame[pos]; //Lecture du message d'erreur
            }
            msgBox->setText(message);
            msgBox->show(); //Affichage de l'erreur
            break;
        }
    }
}
