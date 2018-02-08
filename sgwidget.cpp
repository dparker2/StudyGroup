#include "sgwidget.h"
#include "server.h"

SGWidget::SGWidget(QString name, QWidget *parent, int work_interval_msec) : QWidget(parent)
{
    _work_timer.setInterval(work_interval_msec);
    _work_timer.start();
    connect(&_work_timer, SIGNAL(timeout()), this, SLOT(do_work()));
    server::add(name, this);
}

SGWidget::~SGWidget()
{
    server::remove(this);
}

void SGWidget::enqueue(QByteArray work_message)
{
    _work_queue.push_back(work_message);
}

QList<QByteArray> SGWidget::split(QByteArray &message, int max_split)
{
    QList<QByteArray> word_list;
    QByteArray word;
    for (QByteArray::const_iterator i = message.cbegin(); i != message.cend(); ++i)
    {
        if ((max_split > 0) && (word_list.length() == (max_split - 1)) || (*i != ' ')) {
            word.push_back(*i);
        }
        else {
            word_list.push_back(word);
            word = "";
        }
    }
    word_list.push_back(word);
    return word_list;
}
