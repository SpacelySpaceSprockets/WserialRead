#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QtSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSerialPort *serial;

private:
    Ui::MainWindow *ui;

    QSerialPort::BaudRate       w_baudRate;
    QString                     w_portName;
    QSerialPort::DataBits       w_dataBits;
    QSerialPort::Parity         w_parity;
    QSerialPort::StopBits       w_stopBits;
    QSerialPort::FlowControl    w_flowControl;
    QSerialPort::Direction      w_direction;

    void initialize();


    void openSerialPort();
    void closeSerialPort();

    void enableButtons();
    void disableButtons();
    void enableSendLine();
    void disableSendLine();

    void getSerialPortInfo();

public slots:

    void readData();
    void writeData();

    void on_baudBox_currentIndexChanged(int);
    void on_portnameBox_currentIndexChanged(int);
    void on_databitsBox_currentIndexChanged(int);
    void on_parityBox_currentIndexChanged(int);
    void on_stopbitsBox_currentIndexChanged(int);
    void on_directionBox_currentIndexChanged(int);
    void on_flowcontrolBox_currentIndexChanged(int);

    void on_openPortButton_clicked();
    void on_closePortButton_clicked();
    void on_reloadButton_clicked();
};

#endif // MAINWINDOW_H
