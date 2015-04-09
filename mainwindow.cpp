#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    getSerialPortInfo();
    initialize();//you can NOT call this before getSerialPortInfo

}

MainWindow::~MainWindow()
{
    closeSerialPort(); //just in case it is still open
    delete ui;
}

void MainWindow::initialize(){

    ui->baudBox->setCurrentIndex(3);
    ui->databitsBox->setCurrentIndex(3);

    w_baudRate = QSerialPort::Baud9600;
    w_portName = ui->portnameBox->currentText();
    w_dataBits = QSerialPort::Data8;
    w_parity   = QSerialPort::NoParity;
    w_stopBits = QSerialPort::OneStop;
    w_direction= QSerialPort::Input;

    disableSendLine();
    enableButtons();
    ui->statusBar->showMessage(tr("Not connected"));
}

void MainWindow::openSerialPort(){

    ui->statusBar->showMessage(tr("Connecting..."));
    serial->setBaudRate(w_baudRate);
    serial->setPortName(w_portName);
    serial->setDataBits(w_dataBits);
    serial->setParity(w_parity);
    serial->setStopBits(w_stopBits);
    serial->setFlowControl(w_flowControl);

    //open depends on the current value of w_direction
    QIODevice::OpenMode direct;
    switch(w_direction){
    case QSerialPort::Input:
        direct = QIODevice::ReadOnly;
        break;
    case QSerialPort::Output:
        direct = QIODevice::WriteOnly;
        break;
    case QSerialPort::ReadWrite:
        direct = QIODevice::ReadWrite;
        break;
    default:
        direct = QIODevice::ReadOnly;
    }

    if(serial->open(direct)){
        ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                               .arg(w_portName).arg(w_baudRate).arg(w_dataBits)
                                               .arg(w_parity).arg(w_stopBits).arg(w_flowControl));
        disableButtons();
        //if( w_direction == )


    }else{
        qDebug()<<"Error:"<<serial->error();
        //QMessageBox::critical(this, tr("Error"), serial->errorString());
    }
}

void MainWindow::closeSerialPort(){
    ui->statusBar->showMessage(tr("Closing..."));
    if(serial->isOpen()){
        serial->close();
        ui->statusBar->clearMessage();
        enableButtons(); //port settings buttons
    }
    ui->statusBar->showMessage(tr("Not connected"));
}

void MainWindow::disableButtons(){
    //used to disable buttons when a port is open
    ui->baudBox->setEnabled(false);
    ui->databitsBox->setEnabled(false);
    ui->directionBox->setEnabled(false);
    ui->flowcontrolBox->setEnabled(false);
    ui->parityBox->setEnabled(false);
    ui->portnameBox->setEnabled(false);
    ui->stopbitsBox->setEnabled(false);
    ui->reloadButton->setEnabled(false);

    ui->openPortButton->setEnabled(false);
    ui->closePortButton->setEnabled(true);

    if(serial->isOpen() && (w_direction==QSerialPort::Output || w_direction == QSerialPort::ReadWrite)){
        enableSendLine();
    }else{
        disableSendLine();
    }
}


void MainWindow::enableButtons(){
    //used to enable buttons when port is closed
    ui->baudBox->setEnabled(true);
    ui->databitsBox->setEnabled(true);
    ui->directionBox->setEnabled(true);
    ui->flowcontrolBox->setEnabled(true);
    ui->parityBox->setEnabled(true);
    ui->portnameBox->setEnabled(true);
    ui->stopbitsBox->setEnabled(true);
    ui->reloadButton->setEnabled(true);

    ui->openPortButton->setEnabled(true);
    ui->closePortButton->setEnabled(false);

    disableSendLine();

}

void MainWindow::enableSendLine(){
    ui->sendLine->setEnabled(true);
    ui->sendLineButton->setEnabled(true);
}

void MainWindow::disableSendLine(){
    ui->sendLine->setEnabled(false);
    ui->sendLineButton->setEnabled(false);
}

void MainWindow::getSerialPortInfo(){
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<qint32> rates = QSerialPortInfo::standardBaudRates();
    ui->portnameBox->clear();
    for(int i=0; i<ports.size(); i++){
        qDebug()<<ports[i].portName();
        qDebug()<<rates;
        ui->portnameBox->addItem(ports[i].portName());
    }
}

void MainWindow::readData(){
    QByteArray data = serial->readLine();
    ui->outputPane->putData(data);
}

void MainWindow::writeData(){
    QString outText = ui->sendLine->text();
    QByteArray output = QByteArray(outText.toLocal8Bit());
    if(serial->isOpen()){
        serial->write(output+'\n');
        serial->waitForBytesWritten(-1);

        ui->outputPane->putData('>>>> '+output+'\n');
        qDebug()<<output<<"...written";
    }else{
        qDebug()<<"port not open";
    }
}


void MainWindow::on_sendLineButton_clicked(){
    writeData();
    ui->sendLine->clear();
}

void MainWindow::on_sendLine_returnPressed(){
    writeData();
    ui->sendLine->clear();
}

void MainWindow::on_reloadButton_clicked(){
    getSerialPortInfo();
}

void MainWindow::on_openPortButton_clicked(){
    openSerialPort();
}

void MainWindow::on_closePortButton_clicked(){
    closeSerialPort();
}

void MainWindow::on_baudBox_currentIndexChanged(int idx){
    switch(idx){
    case 0: w_baudRate=QSerialPort::Baud1200;
        break;
    case 1: w_baudRate=QSerialPort::Baud2400;
        break;
    case 2: w_baudRate=QSerialPort::Baud4800;
        break;
    case 3: w_baudRate=QSerialPort::Baud9600;
        break;
    case 4: w_baudRate=QSerialPort::Baud19200;
        break;
    case 5: w_baudRate=QSerialPort::Baud38400;
        break;
    case 6: w_baudRate=QSerialPort::Baud57600;
        break;
    case 7: w_baudRate=QSerialPort::Baud115200;
        break;
    }

    qDebug()<<"baudrate = "<<w_baudRate;
}

void MainWindow::on_portnameBox_currentIndexChanged(int idx){
    w_portName = ui->portnameBox->currentText();
    qDebug()<<"port name = "<<w_portName;
}

void MainWindow::on_databitsBox_currentIndexChanged(int idx){
    switch(idx){
    case 0: w_dataBits=QSerialPort::Data5;
        break;
    case 1: w_dataBits=QSerialPort::Data6;
        break;
    case 2: w_dataBits=QSerialPort::Data7;
        break;
    case 3: w_dataBits=QSerialPort::Data8;
        break;
    }

    qDebug()<<"data bits = "<<w_dataBits;
}

void MainWindow::on_parityBox_currentIndexChanged(int idx){
    switch(idx){
    case 0: w_parity=QSerialPort::NoParity;
        break;
    case 1: w_parity=QSerialPort::EvenParity;
        break;
    case 2: w_parity=QSerialPort::OddParity;
        break;
    case 3: w_parity=QSerialPort::SpaceParity;
        break;
    case 4: w_parity=QSerialPort::MarkParity;
        break;
    }

    qDebug()<<"parity = "<<w_parity;
}

void MainWindow::on_stopbitsBox_currentIndexChanged(int idx){
    switch(idx){
    case 0: w_stopBits=QSerialPort::OneStop;
        break;
    case 1: w_stopBits=QSerialPort::TwoStop;
        break;
    }

    qDebug()<<"stop bits = "<<w_stopBits;
}

void MainWindow::on_directionBox_currentIndexChanged(int idx){
    switch(idx){
    case 0: w_direction=QSerialPort::Input;
        disableSendLine();
        sendLineEnabled = false;
        break;
    case 1: w_direction=QSerialPort::Output;
        //enableSendLine();
        sendLineEnabled = true;
        break;
    case 2: w_direction=QSerialPort::AllDirections;
        //enableSendLine();
        sendLineEnabled = true;
        break;
    }

    qDebug()<<"direction = "<<w_direction;
}

void MainWindow::on_flowcontrolBox_currentIndexChanged(int idx){
    switch(idx){
    case 0: w_flowControl=QSerialPort::NoFlowControl;
        break;
    case 1: w_flowControl=QSerialPort::HardwareControl;
        break;
    case 2: w_flowControl=QSerialPort::SoftwareControl;
        break;
    }

    qDebug()<<"flow control = "<<w_flowControl;
}


