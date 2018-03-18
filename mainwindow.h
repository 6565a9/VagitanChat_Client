#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "QVagitanChatProtocol.h"
#include <QMainWindow>
#include<QInputDialog>
#include<QDebug>
#include<QListWidget>
#include<QThread>

#include<sstream>

#include"login.h"
#include"register.h"
#include"IOThread.h"
#include"util.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0) ;
    ~MainWindow();
    inline void ErrorMsg(const QString title, const QString msg){
        QMessageBox messageBox;
        messageBox.critical(this,title, msg);
    }

private:
    Ui::MainWindow *ui;
    QVagitanChatProtocol MainProtocol;
    Login * logWid;
    Register * regWid;
    type_privmsg type_chatting;
    QString chatting;
private:
    bool thread_initted=false;
    inline void init_IO_thread(void){
        if(!thread_initted){
            thread_initted=true;
            io->moveToThread(&thread_io);
            io->start();
            connect(&thread_io, SIGNAL(finished()), this, SLOT(DissconectFromServer()));
            connect(&thread_io, SIGNAL(started()), io, SLOT(reading_thread()));
            thread_io.start();

            qDebug() << "Thread inited";
        }
    }
    inline void stop_IO_thread(void){
        if(thread_initted){
            qDebug() << "Stop thread for a while";
            thread_initted=false;
            io->stop();
            thread_io.terminate();
            while(thread_io.isRunning());
        }
    }

private:
    QThread thread_io;
    IOThread * io;
    logmap logs;

private slots:
    bool ConnectToServer(void);
    void DissconectFromServer(void);
    void RegisterOnServer(void);
    void LoginOnServer(void);
    void TalkChange(QListWidgetItem *);
    void StartPM(void);
    void JoinRoom(void);
    void QuitRoom(const QString name);
    void sendMsg(void);

    void InitThread(void);
signals:

};

#endif // MAINWINDOW_H
