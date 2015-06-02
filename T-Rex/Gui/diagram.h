#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QFrame>
#include <QAction>
#include <QMenu>
#include <QtWidgets>

class Diagram : public QFrame
{
    Q_OBJECT
public:
    explicit Diagram(QObject *parent = 0);

signals:

public slots:
    void ProvideContextMenu(const QPoint &pos);
    void newRow();
    void deleteRow();
};

#endif // DIAGRAM_H
