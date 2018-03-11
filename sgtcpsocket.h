#ifndef SGTCPSOCKET_H
#define SGTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class SGTCPSocket : public QObject
{
    Q_OBJECT
public:
    explicit SGTCPSocket(QObject *parent = nullptr);
    SGTCPSocket(const SGTCPSocket &sock);
    ~SGTCPSocket();
    void connect_server();
    void write(QString message);
    void write(QByteArray message);
    bool read_socket_helper(QString& out_message);

signals:
    void disconnected();
    void connected();
    void new_message(QString& object_name, QByteArray& full_message);

public slots:
    void reconnect_socket(QAbstractSocket::SocketState);
    void read_socket_send_signal();  // These will be moved to custom TCPSocket class which is instantiated and so will send signals to this class
    void error(QAbstractSocket::SocketError);

private:
    QByteArray single_message();
    QString get_object_name(QByteArray& message);

    QTcpSocket* my_tcp_socket;
    bool success_flag; // ***Always set this to 0 before checking, ONLY to be set to 1 by the read_socket_send_signal() function.
    bool fail_flag;    // ***Always set this to 0 before checking, ONLY to be set to 1 by the read_socket_send_signal() function.
    QString success_message; // ***Always set this to nullptr before checking, ONLY to be set by the read_socket_send_signal() function.
    bool reconnecting;
};

#endif // SGTCPSOCKET_H
