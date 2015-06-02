#include <QtWidgets>

#include "acceptwidget.h"
#include "dragwidget.h"

//! [0]
AcceptWidget::AcceptWidget(QWidget *parent)
    : QFrame(parent)
{
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);

}
//! [0]
//!
//!

void AcceptWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("1") || event->mimeData()->hasFormat("2") || event->mimeData()->hasFormat("1") || event->mimeData()->hasFormat("3") ||event->mimeData()->hasFormat("4") || event->mimeData()->hasFormat("5") || event->mimeData()->hasFormat("6") || event->mimeData()->hasFormat("7") || event->mimeData()->hasFormat("8")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void AcceptWidget::dragMoveEvent(QDragMoveEvent *event)
{

}

void AcceptWidget::dropEvent(QDropEvent *event)
{
    if ((event->mimeData()->hasFormat("1") || event->mimeData()->hasFormat("2") || event->mimeData()->hasFormat("1") || event->mimeData()->hasFormat("3") ||event->mimeData()->hasFormat("4") || event->mimeData()->hasFormat("5") || event->mimeData()->hasFormat("6") || event->mimeData()->hasFormat("7") || event->mimeData()->hasFormat("8"))

            && event->source() != this) {
        qDebug() << _numberOfDrops;
        //ispravi, nije dobro
        QByteArray itemData = event->mimeData()->data("1");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        dataStream >> pixmap >> offset;

        QLabel *newIcon1 = new QLabel(this);
        QLabel* newIcon = new QLabel(this);
        QLabel *newIcon2 = new QLabel(this);

     if(_numberOfDrops == 0)
     {
        newIcon1->setFrameStyle(3);
        newIcon2->setFrameStyle(3);
        newIcon1->setFixedSize(100,30);
        newIcon2->setFixedSize(100,30);

        newIcon1->move(rect().center());
        newIcon2->move(rect().center() + QPoint(110,0));
        newIcon->move(rect().center() + QPoint(103,15));

        newIcon1->show();


        if((event->mimeData()->hasFormat("2")))
        {

                newIcon->setText(".");
                newIcon->show();
                newIcon2->show();
        }

        if((event->mimeData()->hasFormat("3")))
        {

                newIcon->setText("|");
                newIcon->show();
                newIcon2->show();
        }

        if((event->mimeData()->hasFormat("4")))
        {

                newIcon->setText("?");
                newIcon->show();

        }

        if((event->mimeData()->hasFormat("5")))
        {

                newIcon->setText("+");
                newIcon->show();

        }

        if((event->mimeData()->hasFormat("6")))
        {

                newIcon->setText("*");
                newIcon->show();

        }

        if((event->mimeData()->hasFormat("7")))
        {

                newIcon->setText("{a,b}");
                newIcon->show();

        }

        if((event->mimeData()->hasFormat("8")))
        {

                newIcon->setText("[]");
                newIcon->show();

        }

    }

     else
     {

     }



        newIcon1->setAttribute(Qt::WA_DeleteOnClose);
        newIcon2->setAttribute(Qt::WA_DeleteOnClose);


        //Treba dodati posebno ponasanje za svaku sliku

        //takodje ako se nalazimo unutar neke labele, treba u njoj postavljati

        if (event->source() == this) {
            //event->setDropAction(Qt::MoveAction);
            //event->accept();
            event->ignore();
        } else {
            event->acceptProposedAction();
       }
    } else {
        event->ignore();
    }
    _numberOfDrops++;
}

//! [1]
void AcceptWidget::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child)
        return;

    QPixmap pixmap = *child->pixmap();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos());
//! [1]

//! [2]
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-acceptdata", itemData);
//! [2]

//! [3]
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - child->pos());
//! [3]

    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        child->close();
    } else {
        child->show();
        child->setPixmap(pixmap);
    }
}
