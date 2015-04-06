#ifndef WTERM_H
#define WTERM_H

#include <QPlainTextEdit>
#include <QDesignerCustomWidgetInterface>

class wTerm : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit wTerm(QWidget *parent = 0);
    void putData(const QByteArray &data);
    void setLocaleEchoEnabled(bool set);

signals:
    void getData(const QByteArray &data);

public slots:

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);

private:
    bool localEchoEnabled;
};

#endif // WTERM_H
