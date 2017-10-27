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
    bool join_group(QString& group_id);
    bool leave_group(QString& group_id);

signals:
    void disconnected();
    void new_chat(QString,QString,QString);
    void users_changed();
    void user_joined(QString);
    void user_left(QString);

public slots:
    void reconnect_socket(QAbstractSocket::SocketState);
    void read_socket_send_signal();
    void send_chat(QString& groupID, QString& message);
    void error(QAbstractSocket::SocketError);

private:
    QTcpSocket* my_socket;
    QString username;
    bool success_flag; // ***Always set this to 0 before checking, ONLY to be set to 1 by the read_socket_send_signal() function.
    bool fail_flag;    // ***Always set this to 0 before checking, ONLY to be set to 1 by the read_socket_send_signal() function.
    QString success_message; // ***Always set this to nullptr before checking, ONLY to be set by the read_socket_send_signal() function.

    QByteArray format_socket_request(const QString& request_code, QString request_arg);
    bool read_socket_helper(QString& out_message);
};

#endif // SERVER_H
