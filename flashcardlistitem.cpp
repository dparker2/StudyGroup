#include "flashcardlistitem.h"
#include "ui_flashcardlistitem.h"

FlashcardListItem::FlashcardListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlashcardListItem)
{
    ui->setupUi(this);
}

FlashcardListItem::~FlashcardListItem()
{
    delete ui;
}

void FlashcardListItem::set_index(QString &index)
{
    ui->index->setText(index);
}

void FlashcardListItem::set_front(QString &text)
{
    put_ellipsis(ui->front, text);
}

void FlashcardListItem::set_back(QString &text)
{
    put_ellipsis(ui->back, text);
}

void FlashcardListItem::mouseReleaseEvent(QMouseEvent *)
{
    emit card_clicked(ui->index->text().toInt() - 1);
}

void FlashcardListItem::put_ellipsis(QLabel *label, QString &text)
{
    QFontMetrics metrics(label->font());
    QString elidedText = metrics.elidedText(text, Qt::ElideRight, label->width());
    label->setText(elidedText);
}
