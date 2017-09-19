#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>

class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = nullptr);
    bool verifyUserInfo(QString&, QString&);
    bool createAccount(QString&, QString&, QString&);

signals:

public slots:
    void socket_connected();
    void socket_readyRead();
    void error(QAbstractSocket::SocketError);

private:
    QTcpSocket* my_socket;
};

#endif // SERVER_H
