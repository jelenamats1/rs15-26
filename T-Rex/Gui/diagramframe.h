#ifndef DIAGRAMFRAME_H
#define DIAGRAMFRAME_H

#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsView>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace Ui {
class DiagramFrame;
}

class DiagramFrame : public QFrame
{
    Q_OBJECT

public:
    explicit DiagramFrame(QWidget *parent = 0);
    ~DiagramFrame();
public slots:
    void ProvideContextMenu(const QPoint &pos);
    void newRow();
    void deleteRow();
    void Update();
    void Plus();
    void Zvezda();

private:
    Ui::DiagramFrame *ui;
    QVBoxLayout* lay;
    QTreeWidget* w = new QTreeWidget(this);
    int level = 1;
    QLineEdit* regex;
    QPushButton *makeReg, *cleanDiagram;
    QGraphicsView *view;

    void makePanel();
    void makeShow();

};

#endif // DIAGRAMFRAME_H
