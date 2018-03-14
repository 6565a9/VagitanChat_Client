#include "login.h"

Login::Login(QVagitanChatProtocol & protocol) :
      QWidget(0),
      protocol(protocol),
      login(new QLineEdit("Login", this)),
      password(new QLineEdit("Password",this)),
      OK(new QPushButton("OK",this)),
      layout(new QVBoxLayout(this))
{
    this->setLayout(layout);
    layout->addWidget(login);
    layout->addWidget(password);
    layout->addWidget(OK);
    connect(OK,SIGNAL(clicked(bool)),this,SLOT(logining()));
    qDebug() << "Logining";
    this->hide();
}

void Login::logining() noexcept {
    if(login->text().size()  && password->text().size()  ){
          protocol.login(login->text(),password->text());
    }
    this->hide();
}
