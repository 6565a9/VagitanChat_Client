#include "QVagitanChatProtocol.h"

QVagitanChatProtocol::QVagitanChatProtocol(QObject *parent) : QObject(parent){}

bool QVagitanChatProtocol::connect(const QString addr, unsigned int port) noexcept{
    if(connected || ready) return false;

    m_socket = new QTcpSocket(this);
    m_socket->connectToHost(addr, port);
    QObject::connect(m_socket,SIGNAL(connected()), this, SLOT(wasConnected()));
    QObject::connect(m_socket,SIGNAL(readyRead()), this, SLOT(readyRead()));
    QObject::connect(m_socket,SIGNAL(disconnected()), this, SLOT(wasDissconnected()));
    if( !m_socket->waitForConnected() )
            return false;
    return true;
}

void QVagitanChatProtocol::joinToRoom(const QString room_name){
    write(":JOIN "+room_name);
}

QStringList QVagitanChatProtocol::getList(const QString room_name){
    write(":LIST "+room_name);
    return read().split(" ");
}

QString QVagitanChatProtocol::read(unsigned int ms) noexcept{
    if(!ready) QString("Not ready read");
    //return QString( m_socket->readAll() );
    if( ! m_socket->waitForReadyRead(ms) ) return QString("Timeout");
    QByteArray tmp = m_socket->read(1024);
    int newline=tmp.indexOf('\n');
    if(newline != -1)
           tmp = tmp.remove(newline,1);
    return QString::fromUtf8(  tmp );
}

bool QVagitanChatProtocol::write(QString text) noexcept{
     if( !connected ) return false;
     qDebug() << "Write data " << text;
     m_socket->write( (text+"\n").toUtf8().data() );
     return true;

}

QString QVagitanChatProtocol::join(const QString channame){
    if(! write(":JOIN "+channame) ) throw(cant_connect());
    QString answ = read();
    if(answ == ":ROOM "+channame+" CREATED") return QString("Room "+channame+" was created");
    else if(answ == ":ADDED "+username+" "+channame) return QString(channame+" joined");
    else return answ;
}

void QVagitanChatProtocol::privmsg(const QString to, const QString msg, type_privmsg type){
    QString tmp;

    switch( type ){
        case type_privmsg::user:
            tmp = ":PRIVMSG "+to+" "+msg;
            break;
        case type_privmsg::chat:
            tmp = ":PRIVMSG @"+to+" "+msg;
            break;
    }
    write(tmp);
    if(read(1024) == ":ERROR NOT FOUND" )
        throw(std::runtime_error("Not found user"));
}

void QVagitanChatProtocol::reg(const QString username, const QString password){
    if( checkAnswer(":REGISTER "+username+" "+password, ":REGISTERED" ) )
        logining();
}

void QVagitanChatProtocol::login(const QString username, const QString password){
    if( checkAnswer(":USER "+username+" "+password, ":WELCOME "+username ) )
     logining();
}

void QVagitanChatProtocol::dissconnect() noexcept{
        qDebug() << "Disconnecting";
        m_socket->write(":QUIT Leaving");
        m_socket->disconnect();
        connected=false;
        ready=false;
        logined=false;

}

void QVagitanChatProtocol::wasConnected(void) noexcept{
    connected=true;
}

void QVagitanChatProtocol::readyRead(void) noexcept{
    ready=true;
    if(connected && ready) emit SuccefullyConnected(true);
}

void QVagitanChatProtocol::wasDissconnected(void) noexcept{
    connected = ready = false;

    emit lost(true);
}
