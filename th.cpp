#include "th.h"
#include <QTcpSocket>

th::th(QObject *parent, int socketDesc) :
    QThread(parent)
{
    m_SocketDesc = socketDesc;
    m_connected = true;
}

void th::run()
{
    QTcpSocket socket;
    QByteArray trame;
    QList<QString> liste;
    QString user = "", message = "";

    socket.setSocketDescriptor(m_SocketDesc);

    while(m_connected)
    {
        socket.waitForBytesWritten();
        trame = socket.readAll();
        int pos;

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
            liste.append(user);

            emit(siUpdateList(liste));

            break;
        case MESSAGE:
            for(pos = 1; pos < trame.length(); pos++)
            {
                message += trame[pos]; //Lecture du message
            }

            emit(siIncommingMessage(message));

            break;
        case ERROR:
            break;
        }
    }
}
