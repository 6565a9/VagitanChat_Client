#include<QMessageBox>

#ifndef UTIL_H
#define UTIL_H
#include<QListWidget>

namespace Util{


    QListWidgetItem* findItemInList(QListWidget* list, QString text);
    inline void ErrorMsg(  const QString title, const QString msg){
        QMessageBox messageBox;
        messageBox.critical(0,title, msg);
    }
}
#endif // UTIL_H
