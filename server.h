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
    void connect_server();
    // Account Functions
    bool login(QString& username, QString& password);
    bool create_account(QString& username, QString& password, QString& email);
    // Group Functions
    bool create_group(QString& group_name, QString& group_id);

    // Access
    QString get_username();
    // Set
    void set_username(QString& username);

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
    QString username;
};

#endif // SERVER_H
