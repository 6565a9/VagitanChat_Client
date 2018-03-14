#ifndef READER_MSG_H
#define READER_MSG_H
#include "QVagitanChatProtocol.h"

#include <QObject>
#include<QListWidget>
#include<QPlainTextEdit>
#include<QMessageBox>

class reader_msg : public QObject
{
    Q_OBJECT
public:
    explicit reader_msg(QVagitanChatProtocol & protocol, QListWidget * list, QPlainTextEdit * editor) noexcept:
        QObject(0),
        protocol(protocol),
        list(list),
        editor(editor)  {}

private:
    QVagitanChatProtocol & protocol;
    QListWidget * list;
    QPlainTextEdit * editor;
signals:

public slots:
    void reading_thread();
};

#endif // READER_MSG_H
