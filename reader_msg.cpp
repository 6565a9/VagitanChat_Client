#include "reader_msg.h"

void reader_msg::reading_thread(){
    while(1){
        try{
            qDebug() << "Thread do";
            QString msg = protocol.read();
            QStringList listMsg( msg.split(" ") );
            QString text;
            qDebug() << listMsg.size() << msg ;
            if(listMsg.size() < 2 ) continue;
            if(listMsg[0] != ":PRIVMSG" ){
                QMessageBox messageBox;
                messageBox.critical(0,listMsg[0], listMsg[1]);
            }else{
                for(auto it = std::begin(listMsg)+2;it!=std::end(listMsg);it++)
                    text.append(*it+" ");


                if( listMsg[1] != list->currentItem()->text() ) continue;//TODO: add to log
                qDebug() << "inserting text";
                editor->insertPlainText(listMsg[1]+": "+text+"\n");

            }

        }catch(std::runtime_error & e){
            QMessageBox messageBox;
            messageBox.critical(0,"ERROR", e.what());
            break;
        }
    }
}
