#ifndef QVAGITANCHAT_H
#define QVAGITANCHAT_H

#include <QObject>
#include<QTcpSocket>
#include<QDebug>
#include<QMessageBox>
#include<QStringList>
//#include<QTextCodec>
enum class type_privmsg{
    user, chat
};

class QVagitanChatProtocol : public QObject
{
    Q_OBJECT
private:
/*
    inline bool checkStrs(const QString msg){
        return false;
    }
*/
    template <typename ... Allowed >
    inline bool checkStrs(const QString msg, const QString need, const Allowed ... all){
           // return msg == need ? true : checkStrs(msg, all...);
            const QString args[] = {need,all...};
            for(auto arg : args)
                if( arg == msg ) return true;
            throw( std::runtime_error(msg.toStdString()) );
    }

    inline bool checkAnswer(const QString msg, const QString answNeed){
        if(! write(msg) ) throw(cant_connect());
        QString answ = read();
        //qDebug() << answ << " need " << answNeed;
       // return  answ == answNeed ?  true : throw(std::runtime_error(answ.toStdString()));
        if(answ == answNeed) return true;
        QMessageBox messageBox;
        messageBox.critical(0,"Error", answ);
        dissconnect();
        return false;
    }

public:

    explicit QVagitanChatProtocol(QObject *parent = 0);
    bool connect(const QString addr, unsigned int port) noexcept;
    void login(const QString username, const QString password);
    void reg(const QString username, const QString password);
    void dissconnect(void) noexcept;
    QString join(const QString channame);
    void privmsg(const QString to, const QString msg,  const type_privmsg type=type_privmsg::user);

    QString read(unsigned int ms=30000) noexcept;
    bool isLogined(void){
        return logined;
    }
    QStringList getList(const QString room_name);
    void joinToRoom(const QString);

private:
    bool write(QString text) noexcept;
    inline void logining(void){
        qDebug() << "Logining";
        logined=true;
        this->username=username;
        emit wasLogined(true);
    }

private:
    QTcpSocket * m_socket;
    bool connected=false, ready=false, logined;
    QString username;
private:
    std::runtime_error cant_connect(){
        return std::runtime_error("Can't write to server");
    }

signals:
    void SuccefullyConnected(bool);
    void lost(bool);
    void wasLogined(bool);
public slots:
    void wasConnected(void) noexcept;
    void readyRead(void) noexcept;
    void wasDissconnected(void) noexcept;
};

#endif // QVAGITANCHAT_H
