#include "diagram.h"

Diagram::Diagram(QObject *parent)
{


}

void Diagram::ProvideContextMenu(const QPoint &pos)
{

//item = ui->tableWidget->itemAt(pos);
// QPoint globalPos = ui->tableWidget->mapToGlobal(pos);
QAction *pAddAction = new QAction("Add",this);
connect(pAddAction,SIGNAL(triggered()),this,SLOT(newRow()));
QAction *pRemoveAction = new QAction("Remove",this);
connect(pRemoveAction ,SIGNAL(triggered()),this,SLOT(deleteRow()));
QAction *pUpdateAction = new QAction("Update",this);
//connect(pUpdateAction ,SIGNAL(triggered()),this,SLOT(Update()));
QMenu *pContextMenu = new QMenu( this);
pContextMenu->addAction(pAddAction);
pContextMenu->addAction(pRemoveAction );
pContextMenu->addAction(pUpdateAction );
pContextMenu->exec( mapToGlobal(pos) );
delete pContextMenu;
pContextMenu = NULL;
}

void Diagram::newRow()
{
    qDebug() << "1";
//int row = ui->tableWidget->rowCount();
//ui->tableWidget->insertRow(row);
}

void Diagram::deleteRow()
{
    qDebug() << "2";
//int row = ui->tableWidget->row(item);
//ui->tableWidget->removeRow(row);
}
