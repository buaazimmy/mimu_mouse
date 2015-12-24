#include <QtGui>
class myclass: public QWidget
{
    Q_OBJECT
public:
    myclass();
private:
    QPushButton *start;
    QPushButton *quit;
    QLineEdit *Comnum;
    QLabel *Comname;
    QLabel *statelabel;
    QSystemTrayIcon *trayIcon;
    char *msg;
    void showMessage(char *msg);
    void createActions();
    void createTrayIcon();
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QMenu *trayIconMenu;
private slots:
    void showM();
    void MouseStart();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
};
