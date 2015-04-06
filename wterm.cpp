#include "wterm.h"

#include <QScrollBar>


wTerm::wTerm(QWidget *parent) :
    QPlainTextEdit(parent), localEchoEnabled(false)
{
    document()->setMaximumBlockCount(100);

    document()->setMaximumBlockCount(100);

    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);

    setFont(QFont("Monospace",12));
}

void wTerm::putData(const QByteArray &data){

    insertPlainText(QString(data));
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void wTerm::setLocaleEchoEnabled(bool set){
    localEchoEnabled = set;
}

void wTerm::keyPressEvent(QKeyEvent *e){
    switch(e->key()){
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        if(localEchoEnabled)
            QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
    }

}

void wTerm::mousePressEvent(QMouseEvent *e){

}

void wTerm::mouseDoubleClickEvent(QMouseEvent *e){

}

void wTerm::contextMenuEvent(QContextMenuEvent *e){

}
