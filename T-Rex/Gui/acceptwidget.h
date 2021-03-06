#ifndef ACCEPTWIDGET_H
#define ACCEPTWIDGET_H

#include <QFrame>
#include "dragwidget.h"

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class AcceptWidget: public QFrame
{
public:
    AcceptWidget(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    int _numberOfDrops = 0;
};

#endif // ACCEPTWIDGET_H
