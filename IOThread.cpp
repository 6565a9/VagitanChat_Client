#include "IOThread.h"

IOThread::~IOThread(){
   // for(auto & logfile : logsfile){}
}
void IOThread::getRoomList(const QString nameRoom){
    mut.lock();
    room_users->setVisible(true);
    QStringList users_room = protocol.getList(nameRoom);
    room_users->clear();
    if(users_room.size() > 2)
        for(auto it = users_room.begin()+2;it!=users_room.end();it++)
            room_users->addItem(*it);
    mut.unlock();
}

void IOThread::SendMsg(QString msg, QString to){
    mut.lock();
    try{
        protocol.privmsg(to, msg);
        QString endMsg = "You: "+msg+"\n";
        std::string chatting_str = to.toStdString();
        editor->insertPlainText(endMsg);
        logs[chatting_str] << endMsg;
    }catch(std::runtime_error & e){
            QListWidgetItem* item = list->item(list->currentIndex().row());
            list->removeItemWidget(item);
            delete item;
            Util::ErrorMsg("ERROR", e.what());
            editor->setPlainText("");
            editor->setDisabled(true);
    }
    mut.unlock();
    editor->verticalScrollBar()->setValue(editor->verticalScrollBar()->maximum());

}

void IOThread::reading_thread(){

    while(m_work){
        try{
            qDebug() << "Thread do";
            mut.unlock();
            QString msg = protocol.read();
            mut.lock();
            QStringList listMsg( msg.split(" ") );
            QString text;
            qDebug() << listMsg.size() << msg ;
            if(msg.size() < 1 || msg == "Timeout") continue;
            else if(listMsg.size() < 2 || listMsg[0] != ":PRIVMSG" ){
                if(listMsg[0]==":ERROR"){
                    editor->insertPlainText(msg+"\n");
                    continue;
                }
                bool isRoomSelected = listMsg[1] != list->currentItem()->text().mid(1);
                bool isRoomSelected_withoutdog = listMsg[2] != list->currentItem()->text().mid(1);
                if(listMsg[0]==":ROOM"){
                    if( isRoomSelected )continue;
                    QStringList users_room = msg.split(" ");
                    if(users_room.size() > 2)
                        for(auto it = users_room.begin()+2;it!=users_room.end();it++)
                            room_users->addItem(*it);
                }
                else if(listMsg[0]==":ADDED"){
                    if( isRoomSelected_withoutdog )continue;
                    room_users->addItem(listMsg[1]);
                }
                else if(listMsg[0]==":REMOVED"){
                    if( isRoomSelected_withoutdog )continue;
                    QListWidgetItem * item = Util::findItemInList(room_users,listMsg[1]);
                    if(item != 0 )
                        room_users->removeItemWidget(item);
                }
                continue;
            }

               for(auto it = std::begin(listMsg)+2;it!=std::end(listMsg);it++)
                    text.append(*it+" ");

                QString endMsg = listMsg[1]+": "+text+"\n";
                std::string sender = std::move(listMsg[1].toStdString());
                if( logs.find(sender) == std::end(logs) ){
                    logs[sender]=QStringList{};
                }
                logs[sender] << (endMsg) ;

                if( list->count() > 0 && sender == list->currentItem()->text().toStdString() ) {
                    qDebug() << "inserting text";
                    editor->insertPlainText(endMsg);
                }else{
                    qDebug() << "Add item";
                    QListWidgetItem * item = Util::findItemInList(list, listMsg[1] );
                    if(item == 0)
                        list->addItem( listMsg[1] );
                }


        }catch(std::runtime_error & e){
            QMessageBox messageBox;
            messageBox.critical(0,"ERROR", e.what());
            editor->setDisabled(true);
            break;
        }
    }
}
