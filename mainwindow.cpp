#include "mainwindow.h"
#include "ui_mainwindow.h"

//TODO: thread for get messages
//    Design, rooms


MainWindow::MainWindow(QWidget *parent) :
     QMainWindow(parent),
     ui(new Ui::MainWindow),
     MainProtocol(parent),
     logWid( new Login(MainProtocol) ),
     regWid (new Register(MainProtocol))
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(ui->MailLayout);

    ui->ChatBox->setEnabled(false);
    ui->msg->setEnabled(false);
    ui->send->setEnabled(false);
    ui->RoomUsers->setVisible(false);
    connect(ui->actionJoinServer,SIGNAL(triggered(bool)),this,SLOT(LoginOnServer()));
    connect(ui->actionDisconnect,SIGNAL(triggered(bool)), this, SLOT(DissconectFromServer()));
    connect(ui->actionRegister, SIGNAL(triggered(bool)), this, SLOT(RegisterOnServer()));
    connect(ui->actionStart_PM,SIGNAL(triggered(bool)), this, SLOT(StartPM()));
    connect(ui->actionJoin, SIGNAL(triggered(bool)),this, SLOT(JoinRoom()));
    connect(ui->Talks, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(TalkChange(QListWidgetItem*)));
    connect(ui->send, SIGNAL(clicked(bool)), this, SLOT(sendMsg()));
    connect(ui->msg, SIGNAL(returnPressed()), this, SLOT(sendMsg()));
    connect(&MainProtocol,SIGNAL(wasLogined(bool)), this, SLOT(InitThread()) );

    //  void QuitRoom(const QString name);
    io =  new IOThread(MainProtocol,ui->Talks,ui->writted, logs, ui->RoomUsers);


}

void MainWindow::InitThread(void){
    qDebug() << "InitThread";
    if(MainProtocol.isLogined())
        init_IO_thread();
}

void MainWindow::sendMsg(void){
 io->SendMsg(ui->msg->text(), chatting);
 ui->msg->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TalkChange(QListWidgetItem * item){
    qDebug() << item->text() <<"start talking";
    ui->ChatBox->setEnabled(true);
    ui->msg->setEnabled(true);
    ui->send->setEnabled(true);
    ui->writted->setEnabled(true);
    chatting=item->text();
    io->setChatting(item->text());
    qDebug() << "Chatting " << chatting;
    std::string chattings_str = chatting.toStdString();
    std::ostringstream msgs;

    if( logs.find(chattings_str) == std::end(logs) ){
        logs[chattings_str] = QStringList{};
        if(chattings_str.c_str()[0]=='@'){
            MainProtocol.joinToRoom(chatting.mid(1));
            io->getRoomList(chatting.mid(1));
        }
    }



    for(auto it = logs[chattings_str].begin();
        it!=logs[chattings_str].end();
        it++){
        msgs << it->toStdString() ;
    }

    ui->writted->setPlainText(msgs.str().c_str());
    ui->writted->moveCursor(QTextCursor::End);
}

void MainWindow::RegisterOnServer(void){
    if( ConnectToServer() )
        regWid->show();
}

void MainWindow::LoginOnServer(void){
    if( ConnectToServer() )
     logWid->show();
}

bool MainWindow::ConnectToServer(void){
    bool ok;
    QString addr = QInputDialog::getText(this,tr("Write adress of server"),tr("Adress:"),QLineEdit::Normal,"localhost",&ok);
    unsigned int port = QInputDialog::getInt(this,"Write port of adress", "port",4545,0,(1<<16),1,&ok);

    if(!addr.isEmpty() && port && ok){
            if( !MainProtocol.connect(addr, port) ){
                ErrorMsg("Can't connect to server","Can't connect to server with addr: "+addr);

                return false;
            }else{
                qDebug() << "Connected to server";
                return true;
            }
    }else if(ok){
        ConnectToServer();
    }
    return false;
}

void MainWindow::DissconectFromServer(void){
    stop_IO_thread();
    MainProtocol.dissconnect();
    qDebug() << "Dissconnected from server";

}

void MainWindow::StartPM(void){
    bool ok;
    QString user =
            QInputDialog::getText(this,tr("Start PM"),tr("Username:"),QLineEdit::Normal,"user",&ok);
    QListWidgetItem * item = Util::findItemInList(ui->Talks, user );
    if( item != 0 ){
        TalkChange(item);
        ok = false;
    }

    if(ok){
        try{
            MainProtocol.privmsg(user, "Start Talking");
            ui->Talks->addItem(user);
            TalkChange( ui->Talks->item(ui->Talks->count()-1) );
        }catch(std::runtime_error & e){
            ErrorMsg("ERROR", e.what());
        }

     }
}

void MainWindow::JoinRoom(void){
    qDebug() << "JoinRoom";
    bool ok;
    QString RoomName =
            QInputDialog::getText(this,tr("Join to Room"),tr("Room name:"),QLineEdit::Normal,"main",&ok);
    if(ok){
            io->setChatting("@"+RoomName);
            chatting="@"+RoomName;
            MainProtocol.joinToRoom(chatting.mid(1));
            io->getRoomList(chatting.mid(1));
    }
}

void MainWindow::QuitRoom(const QString name){ // from emit
        qDebug() << name;
}
