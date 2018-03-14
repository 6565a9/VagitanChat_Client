#ifndef REGISTER_H
#define REGISTER_H
#include"login.h"
#include"QVagitanChatProtocol.h"

class Register : public Login
{
    Q_OBJECT;
public:
    Register(QVagitanChatProtocol & protocol);
private slots:
    void logining() noexcept override;
};

#endif // REGISTER_H
