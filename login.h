#ifndef LOGIN_H
#define LOGIN_H

#include<QWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLineEdit>
#include<QPushButton>
#include<QDebug>
#include "QVagitanChatProtocol.h"


class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login( QVagitanChatProtocol &protocol );
protected:
    QVagitanChatProtocol &protocol;
    QLineEdit * login, * password;
    QPushButton * OK;
    QVBoxLayout * layout;
signals:
private slots:
    virtual void logining() noexcept ;
public slots:

};

#endif // LOGIN_H
