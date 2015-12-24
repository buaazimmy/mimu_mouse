#include <QtGui>
#include "stdafx.h"
//#include "MIMUGUI.h"
#include "MIMUGUI.moc"
myclass::myclass()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    setMinimumSize(380,100);
    start = new QPushButton("&Start",this);
    start->setGeometry(20,70,160,40);
    quit = new QPushButton("&Quit",this);
    quit->setGeometry(200,70,160,40);
    statelabel = new QLabel(this);
    statelabel -> setText("STATE:  OFF");
    statelabel -> setGeometry(250,20,100,40);
    Comnum = new QLineEdit("3",this);
    Comnum->setGeometry(130,20,80,40);
    Comname = new QLabel(this);
    Comname->setText("Serial Com Number:");
    Comname->setGeometry(20,20,120,40);
    QIcon icon = QIcon("./ps.ico");
    setWindowIcon(icon);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("Welcome to MIMU MOUSE!");
    createActions();
    createTrayIcon();
    trayIcon->show();
    setWindowTitle(tr("MIMU MOUSE"));
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(start,SIGNAL(clicked()),this,SLOT(MouseStart()));
    connect(quit, SIGNAL(clicked()),qApp,SLOT(quit()));
}

void myclass::showM()
{
    QString titlec=tr("�Ҽ��򿪲˵�");
    QString textc=QString::fromLocal8Bit("�Ҽ��򿪲˵����ɵ����ڴ�С");
    trayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information,5000);
}

void myclass::showMessage(char *msg)
{
    QString titlec=tr(msg);
    QString textc=QString::fromLocal8Bit("�Ҽ��򿪲˵����ɵ����ڴ�С");
    trayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information,5000);
}

void myclass::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        showMessage("MIMU MOUSE");
        break;
    case QSystemTrayIcon::DoubleClick:
        showMessage("˫����");
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage("���õ���������껹�ǹ�����갡");
        break;
    default:
        ;
    }
}

void myclass::createActions()
{
    minimizeAction = new QAction(tr("��С�� (&I)"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("��� (&X)"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("��ԭ (&R)"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("�˳� (&Q)"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void myclass::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
}
void myclass::MouseStart()
{
    QString num = this->Comnum->text();
    char *ptr = num.toAscii().data();
    HANDLE InitMouse = CreateThread(NULL,0,InitMIMU,ptr,0,NULL);
    this -> statelabel -> setText("STATE:  ON");
}


