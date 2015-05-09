#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "uvodform.h"
#include "editorform.h"
#include "automatform.h"
#include "diagramform.h"

#include <QPushButton>
#include <QDockWidget>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/images/icon/prog.jpg"));
    this->setWindowTitle("T-Rex");
    this->setMinimumSize(QSize(450, 350));

    createStatusBar();
    setUpMenuActions();
    createMainView();
    createButtonGroup();

}

void MainWindow::updateStatusBar(QString& s){
    lStatus->setText(s);
}

 void MainWindow::createStatusBar(){

     lStatus = new QLabel("");
     lStatus->setMinimumSize(lStatus->sizeHint());
     ui->statusBar->addWidget(lStatus, 1);

 }

void MainWindow::setUpMenuActions(){

    connect(ui->actionNapusti_program, SIGNAL(triggered()), this, SLOT(napustiProgram()));
    connect(ui->actionAbout_T_Rex, SIGNAL(triggered()), this, SLOT(about_app()));
    connect(ui->actionCeline, SIGNAL(triggered(bool)), this, SLOT(prikazCeline(bool)));

}

void MainWindow::prikazCeline(bool chk){
    if(chk)
        content->show();
    else
        content->hide();
}

void MainWindow::about_app(){
    QMessageBox::about(this, tr("About T-Rex"),
    tr("<h2>T-Rex 0.1</h2>"
    "<p>Copyright &copy; 2015 Software Inc."
    "<p>T-Rex is a small application that "
    "help us understand and learn regular expressions."
    "<p>Implement in Qt."));
}

void MainWindow::napustiProgram(){

    QMessageBox exitBox;
    exitBox.setWindowTitle("T-Rex");
    exitBox.setText(tr("Da li sigurno zelite da napustite program?"));
    QPixmap pic(QPixmap(":/msn/images/exitdino.jpg"));
    exitBox.setIconPixmap(pic.scaledToHeight(150, Qt::SmoothTransformation));
    exitBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
    exitBox.setDefaultButton(QMessageBox::Yes);
    if(exitBox.exec() == QMessageBox::Yes)
        QApplication::exit();
}

void MainWindow::createMainView(){

    mainw = new QStackedWidget();
    mainw->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainw->addWidget(UvodForm::getUvodForm());
    mainw->addWidget(EditorForm::getEditorForm());
    mainw->addWidget(AutomatForm::getAutomatForm());
    mainw->addWidget(DiagramForm::getDiagramForm());

    this->setCentralWidget(mainw);
}

QPushButton* MainWindow::createButton(const char *name, const char *info){

    QPushButton *tmp = new QPushButton();

    tmp->setCheckable(true);
    tmp->setFocusPolicy(Qt::NoFocus); //sta ce mi?!
    tmp->setToolTip(info);

    std::string ikonica(":/images/icon/"+std::string(name)+".png");
    tmp->setIcon(QIcon(&ikonica[0]));
    tmp->setIconSize(QSize(50,50));

    return tmp;

}

void MainWindow::createButtonGroup(){

    bttnGroup = new QButtonGroup();
    bttnBar = new QToolBar(tr("Celine"));

    bttnBar->setOrientation(Qt::Vertical);

    bUvod = createButton("uvod", "Uvod u svet regularnih izraza");
    bEditor = createButton("editor", "Pronadji regularne izraze u tekstu");
    bAutomat = createButton("automat", "Konstruisi automat za regularni izraz");
    bDiagram = createButton("diagram", "Sastavi regularni izraz");

    bttnGroup->addButton(bUvod);
    bttnGroup->addButton(bEditor);
    bttnGroup->addButton(bAutomat);
    bttnGroup->addButton(bDiagram);

    bttnBar->addWidget(bUvod);
    bttnBar->addWidget(bEditor);
    bttnBar->addWidget(bAutomat);
    bttnBar->addWidget(bDiagram);

    //centritati button-e na sredinu kolone?!

    content = new QDockWidget(tr("Celine"));
    content->setWidget(bttnBar);
    content->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    content->setMinimumWidth(90);
    addDockWidget(Qt::LeftDockWidgetArea, content);

    //postavljanje dogadjaja
    connect(&switchForm, SIGNAL(mapped(int)), this, SLOT(go_to_form(int)));

    connect(bUvod, SIGNAL(clicked()), &switchForm, SLOT(map()));
    switchForm.setMapping(bUvod, 0);

    connect(bEditor, SIGNAL(clicked()), &switchForm, SLOT(map()));
    switchForm.setMapping(bEditor, 1);

    connect(bAutomat, SIGNAL(clicked()), &switchForm, SLOT(map()));
    switchForm.setMapping(bAutomat, 2);

    connect(bDiagram, SIGNAL(clicked()), &switchForm, SLOT(map()));
    switchForm.setMapping(bDiagram, 3);

    /*
     * napraviti neku novu formu, da inicijalno nju prikazije
     * ne mora da ima mehanizam povratka na nju
     * ui->stackedWidget->setCurrentIndex();
     */

}

void MainWindow::go_to_form(int i){
    mainw->setCurrentIndex(i);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete bUvod; delete bEditor; delete bAutomat; delete bDiagram; delete lStatus;
    delete bttnGroup; delete bttnBar; delete content;
    delete mainw;
}
