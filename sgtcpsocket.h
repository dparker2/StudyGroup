#ifndef SGTCPSOCKET_H
#define SGTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class SGTCPSocket : public QObject
{
    Q_OBJECT
public:
    explicit SGTCPSocket(QObject *parent = nullptr);

signals:

public slots:

private:
    QTcpSocket* _socket;
};

#endif // SGTCPSOCKET_H
