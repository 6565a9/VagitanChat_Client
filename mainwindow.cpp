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
    ui->scrollTalks->setPageStep(101);
    ui->scrollTalks->setMaximum(1);

    ui->ChatBox->setEnabled(false);
    ui->msg->setEnabled(false);
    ui->send->setEnabled(false);

    connect(ui->actionJoinServer,SIGNAL(triggered(bool)),this,SLOT(LoginOnServer()));
    connect(ui->actionDisconnect,SIGNAL(triggered(bool)), this, SLOT(DissconectFromServer()));
    connect(ui->actionRegister, SIGNAL(triggered(bool)), this, SLOT(RegisterOnServer()));
    connect(ui->actionStart_PM,SIGNAL(triggered(bool)), this, SLOT(StartPM()));
    connect(ui->actionJoin, SIGNAL(toggled(bool)),this, SLOT(JoinRoom()));
    connect(ui->Talks, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(TalkChange(QListWidgetItem*)));
    connect(ui->send, SIGNAL(clicked(bool)), this, SLOT(sendMsg()));
    connect(ui->msg, SIGNAL(returnPressed()), this, SLOT(sendMsg()));
    //  void QuitRoom(const QString name);
    reader =  new reader_msg(MainProtocol,ui->Talks,ui->writted); // it not in : because it is wrong.
}

void MainWindow::sendMsg(void){
    try{
        MainProtocol.privmsg(chatting, ui->msg->text());
        ui->writted->insertPlainText("You: "+ui->msg->text()+"\n");
    }catch(std::runtime_error & e){
            QListWidgetItem* item = ui->Talks->item(ui->Talks->currentIndex().row());
            ui->Talks->removeItemWidget(item);
            delete item;
            ErrorMsg("ERROR", e.what());
            ui->writted->setPlainText("");
    }
    ui->msg->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TalkChange(QListWidgetItem * item){

    if(MainProtocol.isLogined())
        init_read_thread();

    qDebug() << item->text() <<"start talking";
    ui->ChatBox->setEnabled(true);
    ui->msg->setEnabled(true);
    ui->send->setEnabled(true);
    chatting=item->text();
    ui->writted->setPlainText("");
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
    }else{
        ConnectToServer();
    }
    return false;
}

void MainWindow::DissconectFromServer(void){
    MainProtocol.dissconnect();
    qDebug() << "Dissconnected from server";

}

void MainWindow::StartPM(void){
    bool ok;
    QString user =
            QInputDialog::getText(this,tr("Start PM"),tr("Username:"),QLineEdit::Normal,"user",&ok);
    for( int i = 0; i < ui->Talks->count();i++){
     QListWidgetItem* item = ui->Talks->item(i);
     if(item->text() == user)
     {
         ok = false;
         break;
     }
    }

    if(ok){
        try{
            MainProtocol.privmsg(user, "Start Talking");
            ui->Talks->addItem(user);
        }catch(std::runtime_error & e){
            ErrorMsg("ERROR", e.what());
        }

     }
}

void MainWindow::JoinRoom(void){

}

void MainWindow::QuitRoom(const QString name){ // from emit
        qDebug() << name;
}
