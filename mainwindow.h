#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "QVagitanChatProtocol.h"
#include <QMainWindow>
#include<QInputDialog>
#include<QMessageBox>
#include<QDebug>
#include<QListWidget>
#include<QThread>

#include"login.h"
#include"register.h"
#include"reader_msg.h"

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
    void init_read_thread(void){
        if(!thread_initted){
            thread_initted=true;
            reader->moveToThread(&readmsg);

            connect(&readmsg, SIGNAL(finished()), this, SLOT(DissconectFromServer()));
            connect(&readmsg, SIGNAL(started()), reader, SLOT(reading_thread()));
            readmsg.start();

            qDebug() << "Thread inited";
        }
    }
private:
    QThread readmsg;
    reader_msg * reader;


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
signals:


};

#endif // MAINWINDOW_H
