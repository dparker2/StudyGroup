#ifndef SGOBJECT_H
#define SGOBJECT_H

#include <QWidget>
#include <QQueue>
#include <QTimer>
#include <QDebug>

/***
 * Most widgets used in SG should inherit this class.
 *
 * Every class that inherits SGWidget must define the private slot
 * do_work(). This function automatically is called several times a second
 * and is meant to be a place where the class deals with the messages
 * in its _work_queue put there by the server class.
 */

class SGWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SGWidget(QString name, QWidget *parent = nullptr, int work_interval_msec = 500);
    ~SGWidget();
    void enqueue(QByteArray work_message);

protected:
    QQueue<QByteArray> _work_queue;

signals:

public slots:

private slots:
    virtual void do_work() = 0;

private:
    QTimer _work_timer;
};

#endif // SGOBJECT_H
