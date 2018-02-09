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
