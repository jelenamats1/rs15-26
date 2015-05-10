#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "uvodframe.h"
#include "editorframe.h"
#include "automatframe.h"
#include "diagramframe.h"

#include <QPushButton>
#include <QDockWidget>
#include <QMessageBox>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/images/icon/prog.jpg"));
    this->setWindowTitle("T-Rex");
    this->setMinimumSize(QSize(450, 350));

    createStatusBar();
    createMainView();
    createButtonGroup();

    //samo konekcije
    setUpMenuActions();

    /*
     * istorija moze da se cuva npr u XML formatu
     * ovde bi mogla da se ucitava
     * da se npr trazi od korisnika da cekira workspace
     * i tako to
     * mozda u engine imati posebnu klasu za istoriju
     */
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

    //postavljanje dogadjaja za celine
    connect(&switchForm, SIGNAL(mapped(int)), this, SLOT(go_to_form(int)));

    connect(bUvod, SIGNAL(clicked()), &switchForm, SLOT(map()));
    switchForm.setMapping(bUvod, 0);

    connect(bEditor, SIGNAL(clicked()), &switchForm, SLOT(map()));
    switchForm.setMapping(bEditor, 1);

    connect(bAutomat, SIGNAL(clicked()), &switchForm, SLOT(map()));
    switchForm.setMapping(bAutomat, 2);

    connect(bDiagram, SIGNAL(clicked()), &switchForm, SLOT(map()));
    switchForm.setMapping(bDiagram, 3);

    //postavljanje dogadjaja za menu bar
    connect(ui->actionNapusti_program, SIGNAL(triggered()), this, SLOT(napustiProgram()));
    connect(ui->actionAbout_T_Rex, SIGNAL(triggered()), this, SLOT(about_app()));
    connect(ui->actionCeline, SIGNAL(triggered(bool)), this, SLOT(prikazCeline(bool)));
    connect(content,SIGNAL(visibilityChanged(bool)), this, SLOT(azurirajMeniContent(bool)));

    connect(ui->actionUvod, SIGNAL(triggered()), &switchForm, SLOT(map()));
    switchForm.setMapping(ui->actionUvod, 0);

    connect(ui->actionEditor, SIGNAL(triggered()), &switchForm, SLOT(map()));
    switchForm.setMapping(ui->actionEditor, 1);

    connect(ui->actionAutomat, SIGNAL(triggered()), &switchForm, SLOT(map()));
    switchForm.setMapping(ui->actionAutomat, 2);

    connect(ui->actionDiagram, SIGNAL(triggered()), &switchForm, SLOT(map()));
    switchForm.setMapping(ui->actionDiagram, 3);

    connect(ui->actionBroj_prepoznatih, SIGNAL(triggered(bool)), _editor, SLOT(showNumMatched(bool)));

    //slotovi za net
    connect(&goToWeb, SIGNAL(mapped(QString)), this, SLOT(loadWebPage(QString)));

    connect(ui->actionT_Rex_GitHub, SIGNAL(triggered()), &goToWeb, SLOT(map()));
    goToWeb.setMapping(ui->actionT_Rex_GitHub, "GitHub");

    connect(ui->actionT_Rex_Fb, SIGNAL(triggered()), &goToWeb, SLOT(map()));
    goToWeb.setMapping(ui->actionT_Rex_Fb, "FB");
}

void MainWindow::loadWebPage(QString id){
    bool anw = false;

    if(id.compare("GitHub") == 0){
        anw = QDesktopServices::openUrl(QUrl("https://github.com/MATF-RS15/rs15-26", QUrl::TolerantMode));
    }
    else if(id.compare("FB") == 0){
        anw = QDesktopServices::openUrl(QUrl("https://www.facebook.com", QUrl::TolerantMode));
    }

    /*
     * Slanje mail sa:
     * Test - naslov
     * Just a test - poruka
     * user@foo.com = mail
     * mailto:user@foo.com?subject=Test&body=Just a test
     */

    if(!anw){
        QMessageBox mBox;
        mBox.setWindowTitle("T-Rex");
        mBox.setText(tr("Greska!"
                        "<p>Proverite internet konekciju ili ispravnost podrazumevanog web pretrazivaca..."));
        QPixmap pic(QPixmap(":/images/saddino.jpg"));
        mBox.setIconPixmap(pic.scaledToHeight(150, Qt::SmoothTransformation));
        mBox.setStandardButtons(QMessageBox::Ok);
        mBox.exec();
    }
}

void MainWindow::azurirajMeniContent(bool tmp){
    ui->actionCeline->setChecked(tmp);
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

    _uvod = new UvodFrame();
    _editor = new EditorFrame();
    _automat = new AutomatFrame();
    _diagram = new DiagramFrame();

    mainw->addWidget(_uvod);
    mainw->addWidget(_editor);
    mainw->addWidget(_automat);
    mainw->addWidget(_diagram);

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