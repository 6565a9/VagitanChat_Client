#include "util.h"
namespace Util{

    QListWidgetItem* findItemInList(QListWidget* list, QString text){
     for( int i = 0; i < list->count();i++){
        QListWidgetItem* item = list->item(i);
        if(item->text() == text) return item;
     }
     return 0;
    }
}
