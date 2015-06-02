#include "diagramframe.h"
#include "ui_diagramframe.h"

#include "guibuilder.h"
#include "dragwidget.h"
#include "diagram.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

DiagramFrame::DiagramFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DiagramFrame)
{
    ui->setupUi(this);
    w->hide();
    makePanel();
    makeShow();
}

DiagramFrame::~DiagramFrame()
{
    delete ui;
}

void DiagramFrame::makeShow(){

    auto lay = ui->bottomLayout;

    regex = new QLineEdit();
    regex->setToolTip("Rezultat konstrukcije");
    regex->setEnabled(false);

    makeReg = GuiBuilder::createIconButton(":/diagram/images/makeReg.ico", "Generisi regularni izraz", 35, false);
    cleanDiagram = GuiBuilder::createIconButton(":/diagram/images/clearDiagram.ico", "Ocisti radnu povrsinu", 35, false);

    lay->addWidget(cleanDiagram);
    lay->addWidget(makeReg);
    lay->addWidget(regex);

}

void DiagramFrame::makePanel(){

    auto lay1 = ui->centralLayout;
    QFrame* d = new QFrame(this);
    d->setContextMenuPolicy(Qt::CustomContextMenu);
    lay1->addWidget(d,1);
    lay->addLayout(lay1);
    connect(d,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(ProvideContextMenu(const QPoint &)));


}
void DiagramFrame::ProvideContextMenu(const QPoint &pos)
{

//item = ui->tableWidget->itemAt(pos);
// QPoint globalPos = ui->tableWidget->mapToGlobal(pos);
QAction *i = new QAction(".",this);
connect(i,SIGNAL(triggered()),this,SLOT(newRow()));
QAction *ili = new QAction("|",this);
connect(ili ,SIGNAL(triggered()),this,SLOT(deleteRow()));
QAction *upitnik = new QAction("?",this);
connect(upitnik ,SIGNAL(triggered()),this,SLOT(Update()));
QAction *plus = new QAction("+",this);
connect(plus ,SIGNAL(triggered()),this,SLOT(Plus()));
QAction *zvezda = new QAction("*",this);
connect(zvezda ,SIGNAL(triggered()),this,SLOT(Zvezda()));
QMenu *pContextMenu = new QMenu( this);
pContextMenu->addAction(i);
pContextMenu->addAction(ili);
pContextMenu->addAction(upitnik);
pContextMenu->addAction(plus);
pContextMenu->addAction(zvezda);
pContextMenu->exec( mapToGlobal(pos) );
delete pContextMenu;
pContextMenu = NULL;
}

void DiagramFrame::newRow()
{
QTextEdit* e1 = new QTextEdit(this);
QTextEdit* e2 = new QTextEdit(this);
//e1->set
e1->show();
e2->show();


//w->show();
//dodavati u drvo sve stavke
//prvo koren jer ga znas
//apoddrveta kako se unose dodajes npr
}

void DiagramFrame::deleteRow()
{
    qDebug() << "2";
//int row = ui->tableWidget->row(item);
//ui->tableWidget->removeRow(row);
}

void DiagramFrame::Plus()
{
    qDebug() << "1";
//int row = ui->tableWidget->rowCount();
//ui->tableWidget->insertRow(row);
}

void DiagramFrame::Zvezda()
{
    qDebug() << "2";
//int row = ui->tableWidget->row(item);
//ui->tableWidget->removeRow(row);
}
void DiagramFrame::Update()
{
    qDebug() << "1";
//int row = ui->tableWidget->rowCount();
//ui->tableWidget->insertRow(row);
}

