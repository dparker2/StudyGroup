#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QTimer>

class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = nullptr);
    bool connectServer();
    bool login(QString&, QString&);
    bool createAccount(QString&, QString&, QString&);

signals:
    void disconnected();

public slots:
    void reconnect_socket(QAbstractSocket::SocketState);
    void error(QAbstractSocket::SocketError);

private:
    QTcpSocket* my_socket;
};

#endif // SERVER_H
