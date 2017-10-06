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
    void connectServer();
    bool login(QString&, QString&);
    bool createAccount(QString&, QString&, QString&);

signals:
    void disconnected();
    void new_chat(QString&);
    void user_joined(QString&);
    void user_left(QString&);

public slots:
    void reconnect_socket(QAbstractSocket::SocketState);
    void read_socket_send_signal();
    void error(QAbstractSocket::SocketError);

private:
    QTcpSocket* my_socket;
};

#endif // SERVER_H
