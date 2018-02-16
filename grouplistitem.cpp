#include "grouplistitem.h"
#include "ui_grouplistitem.h"

GroupListItem::GroupListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupListItem)
{
    ui->setupUi(this);
}

GroupListItem::~GroupListItem()
{
    delete ui;
}
