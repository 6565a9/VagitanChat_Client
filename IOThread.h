#ifndef IOThread_H
#define IOThread_H
#include "QVagitanChatProtocol.h"

#include <QObject>
#include<QListWidget>
#include<QPlainTextEdit>
#include<QMessageBox>
#include<fstream>
#include<map>
#include<QMutex>
#include<QStringList>
#include<QScrollBar>
#include"util.h"


using logmap = std::map<std::string, QStringList>;
class IOThread : public QObject
{
    Q_OBJECT
public:
    explicit IOThread(QVagitanChatProtocol & protocol, QListWidget * list, QPlainTextEdit * editor, logmap & logs, QListWidget * room_users) noexcept:
        QObject(0),
        protocol(protocol),
        list(list),
        editor(editor),
        logs(logs),room_users(room_users){}
    ~IOThread();
    void SendMsg(QString msg, QString to);

   inline void stop(){
        m_work=false;
    }

   inline void start(){
        m_work=true;
    }

   void getRoomList( const QString );


private:
    QVagitanChatProtocol & protocol;
    QListWidget * list;
    QPlainTextEdit * editor;
    logmap & logs;
    bool m_work=false;
    QMutex mut;
    QListWidget * room_users;
private:
signals:

public slots:
    void reading_thread();
};

#endif // IOThread_H
