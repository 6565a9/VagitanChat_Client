#include "register.h"

Register::Register(QVagitanChatProtocol &protocol) : Login(protocol)
{

}

void Register::logining() noexcept {
    if(login->text().size()  && password->text().size()  ){
          protocol.reg(login->text(),password->text());
    }
    this->hide();
}
