#ifndef TH_H
#define TH_H

#include <QThread>
#include <QList>
#include <QMessageBox>

class th : public QThread
{
    Q_OBJECT
public:
    explicit th(QObject *parent = 0, int socketDesc = -1);

protected:
    void run();
    
signals:
    void siIncommingMessage(QString);
    void siUpdateList(QList<QString>);
    
public slots:

private:
    int m_SocketDesc;
    bool m_connected;

    QMessageBox *msgBox;
    
};

#endif // TH_H
