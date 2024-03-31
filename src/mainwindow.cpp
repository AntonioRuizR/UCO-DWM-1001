/***********************************************************************************
**                                                                                **
** UCO DWM1001-DEV, a software for the control of the DWM1001-DEV board           **
**    Copyright (C) 2024  Antonio Ruiz Ruiz                                       **
**                                                                                **
**    This program is free software: you can redistribute it and/or modify        **
**    it under the terms of the GNU General Public License as published by        **
**    the Free Software Foundation, either version 3 of the License, or any       **
**    later version.                                                              **
**                                                                                **
**    This program is distributed in the hope that it will be useful,             **
**    but WITHOUT ANY WARRANTY; without even the implied warranty of              **
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               **
**    GNU General Public License for more details.                                **
**                                                                                **
**    You should have received a copy of the GNU General Public License           **
**    along with this program.  If not, see <https://www.gnu.org/licenses/>.      **
**                                                                                **
************************************************************************************
**           Author: Antonio Ruiz Ruiz                                            **
**  Contact: antonioruizrruiz@gmail.com                                           **
**             Date: 10.03.24                                                     **
**          Version: 1.0.0                                                        **
************************************************************************************/

//Included classes

#include "mainwindow.h"
#include "dialog_alarm_dist.h"
#include "dialog_alarm_pos.h"
#include "dialog_deviceconfig.h"
#include "dialog_dist_graph_config.h"
#include "dialog_mediafilter.h"
#include "dialog_csv.h"
#include "dialog_pos_graph_config.h"
#include "dialog_stats.h"
#include "dialog_about.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include "console.h"
#include "qcustomplot.h"
#include <QStringRef>
#include <QTextCodec>
#include <QTimer>
#include <QElapsedTimer>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setting up
    //Initial GUI configuration:

    //Initializing current mode variables
    datareception_enabled=0;        //Variable that indicates that data reception has been enabled
    serial_message.clear();      //Clear the variable that saves the complete received message
    position_message.clear();   //Clear the variable that saves the complete received message (distance/position)

    //Detect and create language menu
    createLanguageMenu();

    //******GUI elements: initial settings******//
    ui->spinBox_sampletime->findChild<QLineEdit*>()->setReadOnly(true);
    ui->tabWidget->setEnabled(false);
    ui->spinBox_sampletime->setEnabled(false);
    ui->pushButton_disconnect->setEnabled(false);
    ui->frame_4->setEnabled(false);
    ui->textEdit->setReadOnly(true);
    ui->tabWidget->setCurrentIndex(0);
    ui->pushButton_distancemode->setEnabled(false);
    ui->pushButton_positionmode->setEnabled(false);
    ui->frame_6->setEnabled(false);

    //GPIO Tab initial settings
    ui->comboBox_GPIO8->setCurrentIndex(1);
    ui->comboBox_GPIO9->setCurrentIndex(1);
    ui->comboBox_GPIO10->setCurrentIndex(0);
    ui->comboBox_GPIO12->setCurrentIndex(0);
    ui->pushButton_GPIO10->setEnabled(false);
    ui->pushButton_GPIO12->setEnabled(false);

    //Tab initial configuration
    ui->actionStartDataSaving->setEnabled(false);
    ui->actionStopDataSaving->setEnabled(false);
    ui->actionDistanceGraphSettings->setEnabled(false);
    ui->actionDeviceSettings->setEnabled(false);
    ui->actionReset->setEnabled(false);
    ui->actionFactory_Reset->setEnabled(false);
    ui->actionPositionGraphSettings->setEnabled(false);
    ui->actionStatistics->setEnabled(false);

    //Labels
    ui->label_connection->setText(tr("<b><FONT COLOR='red' FONT SIZE = 4>DESCONECTADO</b></font>"));
    ui->label_alarm_dist->setText("");
    ui->label_alarm_pos->setText("");
    ui->label_alarm_dist->setFrameStyle(QFrame::NoFrame);
    ui->label_alarm_pos->setFrameStyle(QFrame::NoFrame);
    ui->textEdit->append(tr("\r\rAsegúrese de que la recepción de datos de la placa DWM1001-DEV se encuentre desactivada antes de realizar la conexión para un correcto funcionamiento del programa"));;

    //Available serial ports update button
    QIcon refreshButton(":/images/refresh_icon.png");
    ui->pushButton_USBportrefresh->setIcon(refreshButton);
    int height_refreshB = ui->pushButton_USBportrefresh->height();
    int width_refreshB = ui->pushButton_USBportrefresh->width();
    ui->pushButton_USBportrefresh->setIconSize(QSize(width_refreshB, height_refreshB)); // Ajusta este tamaño al que necesites

    //Window title
    setWindowTitle(tr("UCO DWM1001-DEV"));

    //Terminal/console initial settings
    ui->p_console->setEnabled(false);     //Terminal disabled until connection with the device

    //Serial port initial settings:
    p_serie = new QSerialPort;
    p_serie->setBaudRate(QSerialPort::Baud115200);
    p_serie->setDataBits(QSerialPort::Data8);
    p_serie->setParity(QSerialPort::NoParity);
    p_serie->setStopBits(QSerialPort::OneStop);
    p_serie->setFlowControl(QSerialPort::NoFlowControl);

    //Graph config:
    graph0_size=1.5;
    graph1_size=2;
    graph2_size=1;
    color_graph0=0;
    color_graph1=1;
    color_graph2=2;
    range_dist_y_max=10;
    range_dist_y_min=0;

    //Plot settings
    setupRealtimeDataDemo(ui->customPlot);
    setupRealtimeDataDemo3(ui->position_graph);

    //Signal-Function link
    connect(ui->p_console, &Console::getData, this, &MainWindow::writeData);        //Linking commands written in terminal with serial port write action
    connect(p_serie, &QSerialPort::readyRead, this, &MainWindow::readData);         //Linking serial port data received function to terminal
    connect(&timer_sampletime,SIGNAL(timeout()),this,SLOT(sampletimechange_wait()));           //Waiting time while new sample time is set
    connect(&timer_gpioconfig, SIGNAL(timeout()),this,SLOT(gpio_config()));                 //When timer ends, GPIO are configured
    connect(&timer_alarmdelay, SIGNAL(timeout()), this, SLOT(alarm_graph()));       //Timer associated with alarm activation delay
    connect(&tem_gpio8, SIGNAL(timeout()), this, SLOT(timer_gpio8()));      //Timer linked with GPIO 8 activation
    connect(&tem_gpio9, SIGNAL(timeout()), this, SLOT(timer_gpio9()));      //Timer linked with GPIO 9 activation
    connect(&tem_gpio10, SIGNAL(timeout()), this, SLOT(timer_gpio10()));    //Timer linked with GPIO 10 activation
    connect(&tem_gpio12, SIGNAL(timeout()), this, SLOT(timer_gpio12()));    //Timer linked with GPIO 12 activation
    connect(&timer_alarmdelay, SIGNAL(timeout()), this, SLOT(alarm_duration_f()));  //Timer associated with alarm duration
    connect(&timer_infofailure, SIGNAL(timeout()), this, SLOT(receive_info_failure()));       //If initial info  is not retrieved, a new attempt is made
    connect(&timer_opmodechange,SIGNAL(timeout()),this,SLOT(change_devicemode_wait()));                 //Waiting time while new operation mode is set
    connect(&timer_gpioclear, SIGNAL(timeout()), this, SLOT(gpio_clear_wait()));//While GPIO deactivation is in progress, read and clear serial port buffer.


    //Initial available serial ports analysis:
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(serialPortInfo.portName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**************************************************************
    WRITE/READ functions and send data to serial port function
***************************************************************/

//Function that sends messages manually written in console through serial port
void MainWindow::writeData(const QByteArray &data)
{
    p_serie->write(data);
    p_serie->flush();
    p_serie->waitForBytesWritten();
}

//Function that receives and process the received data from serial port
//Data is received and accumulated until the complete message is formed
//Depending on the GUI and device state, each message is procesed differently
//To prevent processing failure during GPIO deactivation, while it is in progress,
//this function only reads and clears serial port buffer
void MainWindow::readData()
{
    if(deactivate_GPIO==1){
        QByteArray temporal_data = p_serie->readAll();
        return;
    }
    if(datareception_enabled==1)
    {
        if(operation_mode==0){
            if (!serial_message.endsWith("\n")){
                const QByteArray data = p_serie->readAll();
                serial_message.append(data);
                ui->p_console->putData(data);
            }
            else{
                distance_analysis(serial_message);
                serial_message.clear();
            }
        }
        else{
            if (!position_message.endsWith("\n")){
                const QByteArray data = p_serie->readAll();
                position_message.append(data);
                ui->p_console->putData(data);
            }
            else{
                position_analysis(position_message);
                position_message.clear();
            }
        }
    }
    else if(receive_initial_info==1)
    {
        if (!info_message.contains(">")){
            const QByteArray data = p_serie->readAll();
            info_message.append(data);
            ui->p_console->putData(data);
        }
        if(info_message.contains(">") and (info_message.contains("tn") or info_message.contains("ani") or info_message.contains("an"))){
            receive_initial_info=0;
            receive_info();
            on_pushButton_distancemode_clicked();
        }

    }

    else{
        const QByteArray data = p_serie->readAll();
        ui->p_console->putData(data);
    }
}

//Function in GUI which sends data through serial port
//Each message is split up by sending the characters one by one
void MainWindow::send_message(QString command)
{
    for (int i=0; i<command.length(); i++){
        QStringRef subString(&command,i,1);
        QByteArray message = subString.toLocal8Bit();
        p_serie->write(message);
        p_serie->waitForBytesWritten(15);
        p_serie->waitForReadyRead(15);
    }
}


/************************************************************
    CONNECT/DISCONNECT and module's info related Functions
*************************************************************/

//When "Connect" button is clicked, connection with chosen serial port is established
//If the connection cannot be established, a failure message is shown
void MainWindow::device_connection()
{
    chosenPort = ui->comboBox->currentText();
    p_serie->setPortName(chosenPort);
    if (p_serie->open(QIODevice::ReadWrite)){

        ui->pushButton_disconnect->setEnabled(false);
        send_message("\r\r");
        p_serie->waitForBytesWritten(100);
        p_serie->waitForReadyRead(100);
        ui->scrollArea->setEnabled(true);
        ui->pushButton_connect->setEnabled(false);
        ui->p_console->setEnabled(true);
        ui->label_connection->setText(tr("<b><FONT COLOR='green' FONT SIZE = 4>CONECTADO</b></font>"));
        ui->spinBox_sampletime->setEnabled(true);
        ui->textEdit->setEnabled(true);
        ui->textEdit->setText(tr("Conexión realizada con éxito\r\n------------------\r\nCONFIGURANDO PUERTOS GPIO, ESPERE UN MOMENTO"));

        p_serie->waitForBytesWritten(100);
        timer_gpioconfig.start(1500);

        QMessageBox::information(this, tr("información"), tr("Recuerde configurar el dispositivo en función del modo de funcionamiento seleccionado"));
        return;
    }
    else{
        ui->label_connection->setText(tr("<b><FONT COLOR='red' FONT SIZE = 4>FALLIDO</b></font>"));
        ui->textEdit->append(tr("\r\nNo se la logrado establecer comunicación con el dispositivo escogido\r\nPruebe a escoger otro puerto USB"
                                "o compruebe si ya existe una conexión abierta con el dispositivo seleccionado"));
    }
}

//When clicked, it closes serial port connection and restarts GUI elements
void MainWindow::device_disconnection()
{
    savedata=0;
    ui->label_connection->setText(tr("<b><FONT COLOR='blue' FONT SIZE = 4>DESCONECTANDO</b></font>"));
    send_message("quit\r");
    p_serie->waitForReadyRead(500);
    ui->label_dist_value_1->setText("N/A");
    if (p_serie->isOpen()){
        p_serie->close();

        gpio_init_config=1;
        ui->pushButton_connect->setEnabled(true);
        ui->pushButton_disconnect->setEnabled(false);
        ui->p_console->setEnabled(false);
        ui->label_connection->setText(tr("<b><FONT COLOR='red' FONT SIZE = 4>DESCONECTADO</b></font>"));
        ui->textEdit->setText(tr("\r\nDesconectado con éxito"));
        ui->tabWidget->setEnabled(false);
        ui->frame_6->setEnabled(false);
        ui->frame_4->setEnabled(false);
        ui->label_funcmode->setText("N/A");
        ui->label_networkID->setText("N/A");
        ui->label_firm->setText("N/A");

        ui->actionStartDataSaving->setEnabled(false);
        ui->actionStopDataSaving->setEnabled(false);
        ui->actionDistanceGraphSettings->setEnabled(false);
        ui->actionDeviceSettings->setEnabled(false);
        ui->actionReset->setEnabled(false);
        ui->actionFactory_Reset->setEnabled(false);
        ui->actionPositionGraphSettings->setEnabled(false);

        //Graph clear

        ui->position_graph->clearItems();
        ui->position_graph->graph(0)->data()->clear();
        ui->position_graph->graph(1)->data()->clear();
        ui->position_graph->replot();

        //Alarms
        distance_alarm_plot=0;
        position_alarm_pos=0;
        ui->textEdit->clear();
        enable_statistics=0;
        reset_stats();
    }
    else{
        ui->label_connection->setText(tr("<b><FONT COLOR='red' FONT SIZE = 4>DESCONECTADO</b></font>"));
    }
}

//When a new sample time is set. A timer is activated until configuration is done
void MainWindow::sampletimechange_wait()
{
    ui->spinBox_sampletime->setEnabled(true);
    timer_sampletime.stop();
    ui->textEdit->append(tr("\r\n\r\nSe ha modificado el tiempo de muestreo del dispositivo"));
}

void MainWindow::change_devicemode_wait()
{
    QString shell_mode="\r\r";
    send_message(shell_mode);
    timer_opmodechange.stop();
    ui->textEdit->append(tr("\r\n\r\nMódulo Reiniciado."));
}

//Function that receives and process information about the connected device
void MainWindow::receive_info()
{
    if(info_message.contains(" tn ")){
        ui->label_funcmode->setText(tr("<b><FONT COLOR='green' FONT SIZE = 4>TAG</b></font>"));
        ui->plot_distance->setEnabled(true);
        ui->tab->setEnabled(true);
        if(operation_mode==1){
            ui->plot_distance->setEnabled(false);
        }
        passive_positioning=0;
        tag_positioning=1;
    }
    else if(info_message.contains(" ani ")){
        ui->label_funcmode->setText(tr("<b><FONT COLOR='red' FONT SIZE = 4>Ancla Iniciadora</b></font>"));
        ui->plot_distance->setEnabled(false);
        ui->tab_4->setEnabled(false);
        ui->tab->setEnabled(false);
        passive_positioning=0;
        tag_positioning=0;
    }
    else if(info_message.contains(" an ")){
        ui->label_funcmode->setText(tr("<b><FONT COLOR='blue' FONT SIZE = 4>Ancla</b></font>"));
        ui->plot_distance->setEnabled(true);
        ui->tab_4->setEnabled(true);
        ui->tab->setEnabled(false);
        passive_positioning=0;
        tag_positioning=0;
        if(operation_mode==0){
            ui->plot_distance->setEnabled(false);
        }
        if(info_message.contains("pasv")){
            passive_positioning=1;
            ui->label_funcmode->setText(tr("<b><FONT COLOR='blue' FONT SIZE = 4>Ancla pasiva</b></font>"));
        }
    }
    else{
        QMessageBox::information(this, "Advertencia", tr("No se ha logrado obtener correctamente la información del módulo, puede tratarse de un dispositivo no compatible"));
        return;
    }
    receive_initial_info=0;
    QStringList split_message;
    QString message_to_string = QTextCodec::codecForMib(106)->toUnicode(info_message);
    split_message = message_to_string.split("\r\n");
    QStringRef subString2(&split_message[2],29,5);
    QString network_id="";
    network_id.append(subString2);
    ui->label_networkID->setText(network_id);

    QStringRef subString3(&split_message[1],33,9);
    QString firm="";
    firm.append(subString3);
    ui->label_firm->setText(firm);
    info_message.clear();

    ui->frame_6->setEnabled(true);
    ui->frame_4->setEnabled(true);
    ui->actionDistanceGraphSettings->setEnabled(true);
    ui->actionDeviceSettings->setEnabled(true);
    ui->actionReset->setEnabled(true);
    ui->actionFactory_Reset->setEnabled(true);
    ui->actionPositionGraphSettings->setEnabled(true);
    ui->textEdit->append(tr("\r\n\r\nRecepción de información sobre el dispositivo completada"));
}

//When information is not received correctly. A new request is done
void MainWindow::receive_info_failure()
{
    if(receive_initial_info==1){
        send_message("\r");
    }
    return;
    timer_infofailure.stop();
}


/*******************************************
    FUNCTIONS ASSOCIATED WITH GUI ELEMENTS
********************************************/

//When refresh button clicked, available USB ports are retrieved
void MainWindow::on_pushButton_USBportrefresh_clicked()
{
    ui->comboBox->clear();
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(serialPortInfo.portName());
    }
    ui->textEdit->append(tr("\r\n\r\nLista de puertos USB disponibles actualizada"));
}

//When connection button is clicked, GUI tries to establish connection with the chosen device
void MainWindow::on_pushButton_connect_clicked()
{
    ui->label_connection->setText(tr("<b><FONT COLOR='blue' FONT SIZE = 4>CONECTANDO...</b></font>"));
    ui->menuLanguage->setEnabled(false);
    device_connection();
}

//When disconnection button is clicked, GUI closes the established serial port connection
void MainWindow::on_pushButton_disconnect_clicked()
{
    if(ui->plot_distance->isChecked()){
        QMessageBox::information(this, "Error", tr("Desactive la recepción de datos desde el módulo antes de desconectar"));
        return;
    }
    if (p_serie->isOpen()){
        device_disconnection();
        ui->menuLanguage->setEnabled(true);
    }
}

//Button in GUI which sets distance processing mode
void MainWindow::on_pushButton_distancemode_clicked()
{
    if(ui->plot_distance->isChecked()){
        QMessageBox::information(this, "Error", tr("Desactive la representación de datos para cambiar de modo"));
        return;
    }
    operation_mode=0;
    ui->tab->setEnabled(true);
    ui->tab_3->setEnabled(false);
    ui->pushButton_distancemode->setEnabled(false);
    ui->pushButton_positionmode->setEnabled(true);
    enable_statistics=0;
    if(ui->label_funcmode->text().contains("TAG")){
        ui->plot_distance->setEnabled(true);
    }
    else{
        ui->plot_distance->setEnabled(false);
    }
    ui->textEdit->append(tr("\r\n\r\nHa seleccionado el modo de funcionamiento orientado a la recepción de datos de distancia con el resto de módulos de la red.\r\n"
                            "Establezca el número de anclas disponibles en la red en la lista superior \"Número de medidas\""));
}

//Button in GUI which sets position processing mode
void MainWindow::on_pushButton_positionmode_clicked()
{
    if(alarm_activation==1){
        QMessageBox::information(this, "Error", tr("Desactive la alarma para cambiar de modo"));
        return;
    }
    if(ui->plot_distance->isChecked()){
        QMessageBox::information(this, "Error", tr("Desactive la recepción de datos para cambiar de modo"));
        return;
    }
    operation_mode=1;
    ui->tab->setEnabled(false);
    ui->tab_3->setEnabled(true);
    ui->pushButton_distancemode->setEnabled(true);
    ui->pushButton_positionmode->setEnabled(false);
    enable_statistics=0;
    if(!(ui->label_funcmode->text().contains("Iniciadora") or ui->label_funcmode->text().contains("Initiator")) ){
        ui->plot_distance->setEnabled(true);
    }
    else{
        ui->plot_distance->setEnabled(false);
    }
    ui->textEdit->append(tr("\r\n\r\nHa seleccionado el modo de funcionamiento orientado a la recepción de datos de posición del módulo conectado al PC.\r\n"
                            "Utilice la pestaña de posicionamiento deseada en función de si el módulo conectado es un ancla pasiva o un TAG"));
}

//When exit tab is clicked, if plotting is enabled, a warning message is shown.
//If not, if serial port is opened, it is closed and application shuts down
void MainWindow::on_actionExit_triggered()
{
    if(ui->plot_distance->isChecked()){
        QMessageBox::information(this, "Error", tr("Desactive la recepción de datos y desconecte el módulo antes de desconectar"));
        return;
    }
    if(p_serie->isOpen())device_disconnection();
    this->close();
}

//When exit button is clicked, if plotting is enabled, a warning message is shown.
//If not, if serial port is opened, it is closed and application shuts down
void MainWindow::closeEvent (QCloseEvent *event){
    if(ui->plot_distance->isChecked()){
        QMessageBox::information(this, "Error", tr("Desactive la recepción de datos y desconecte el módulo antes de cerrar la aplicación"));
        event->ignore();
        return;
    }
    if(p_serie->isOpen())device_disconnection();
    event->accept();
}

//When data reception is enabled (distance/position), corresponding command is sent to de device and the related tab is enabled
//Variable acting as a signal is activated -> GUI enters data reception mode
void MainWindow::on_plot_distance_toggled(bool checked)
{
    if (checked== true){
        ui->tab_4->setEnabled(false);
        ui->textEdit->append(tr("\r\n\r\nRecepción de datos activada\r\nSe comienza a representar los valores recibidos"));
        if(datareception_enabled==0){

            ui->actionDeviceSettings->setEnabled(false);
            ui->actionReset->setEnabled(false);
            ui->actionFactory_Reset->setEnabled(false);
            ui->actionExit->setEnabled(false);
            ui->spinBox_sampletime->setEnabled(false);
            ui->pushButton_distancemode->setEnabled(false);
            ui->pushButton_positionmode->setEnabled(false);

            if(operation_mode==1){
                send_message("lep\r");
                position_graph_enabled = 1;
            }
            else{
                send_message("lec\r");
            }
            datareception_enabled=1;
            ui->p_console->setEnabled(false);
            disconnect(ui->p_console, &Console::getData, this, &MainWindow::writeData);

            if(operation_mode==1){
                if((ui->label_funcmode->text().contains("Ancla")) or (ui->label_funcmode->text().contains("Anchor")) or (ui->label_funcmode->text().contains("TAG"))){
                    if (dataTimer.isActive()==false) dataTimer.start(125);
                }
                else{
                    QMessageBox::information(this, "Error", tr("Para solicitar el posicionamiento debe configurar el módulo como ancla pasiva o como TAG"));
                    return;
                    ui->plot_distance->setChecked(false);
                }
            }
            else{
                if(ui->label_funcmode->text().contains("TAG")){
                    if (dataTimer.isActive()==false) dataTimer.start(120);
                }
                else{
                    QMessageBox::information(this, "Error", tr("Para solicitar la distancia, configure el módulo como TAG "));
                    return;
                    ui->plot_distance->setChecked(false);
                }
            }
            ui->actionStatistics->setEnabled(true);
        }
    }
    else{
        if(distance_alarm_plot==1){
            ui->tab_4->setEnabled(false);
        }
        else{
            ui->tab_4->setEnabled(true);
        }
        if(operation_mode==1){
            send_message("lep\r");
            position_graph_enabled = 0;
        }
        else{
            send_message("lec\r");
        }
        dataTimer.stop();
        info_ids_1=0;
        info_ids_2=0;
        info_ids_3=0;
        info_ids_4=0;
        ui->textEdit->append(tr("\r\n\r\nRecepción de datos desactivada"));
        connect(ui->p_console, &Console::getData, this, &MainWindow::writeData);

        datareception_enabled=0;
        if(operation_mode==0){
            ui->pushButton_positionmode->setEnabled(true);
        }
        else{
            ui->pushButton_distancemode->setEnabled(true);
        }
        ui->p_console->setEnabled(true);
        ui->spinBox_sampletime->setEnabled(true);
        ui->actionDeviceSettings->setEnabled(true);
        ui->actionReset->setEnabled(true);
        ui->actionFactory_Reset->setEnabled(true);
        ui->actionExit->setEnabled(true);
        max_detected_devices=0;

        ui->label_dist_value_1->setText("N/A");
        ui->label_dist_value_2->setText("N/A");
        ui->label_dist_value_3->setText("N/A");
        ui->label_dist_value_4->setText("N/A");
        ui->label_mediafilter_1->setText("N/A");
        ui->label_mediafilter_2->setText("N/A");
        ui->label_mediafilter_3->setText("N/A");
        ui->label_mediafilter_4->setText("N/A");
        ui->label_29->setText("ID 1: ");
        ui->label_30->setText("ID 2: ");
        ui->label_41->setText("ID 3: ");
        ui->label_42->setText("ID 4: ");

        double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        ui->customPlot->graph(0)->data()->clear();
        ui->customPlot->graph(1)->data()->clear();
        ui->customPlot->graph(2)->data()->clear();
        ui->customPlot->graph(3)->data()->clear();
        ui->customPlot->graph(4)->data()->clear();
        ui->customPlot->graph(5)->data()->clear();
        ui->customPlot->graph(6)->data()->clear();

        realtimeDataSlot();

        ui->position_graph->graph(1)->data()->clear();
        ui->position_graph->replot();
    }
}

//Function associated with sample time change
void MainWindow::on_spinBox_sampletime_valueChanged(int arg1)
{
    if ((p_serie->isOpen())){
        QString sample_time;
        switch(arg1){
        case 100:  sample_time=("aurs 1 1\r");
            break;
        case 200:  sample_time=("aurs 2 2\r");
            break;
        case 300:  sample_time=("aurs 3 3\r");
            break;
        case 400:  sample_time=("aurs 4 4\r");
            break;
        case 500:  sample_time=("aurs 5 5\r");
            break;
        case 600:  sample_time=("aurs 6 6\r");
            break;
        case 700:  sample_time=("aurs 7 7\r");
            break;
        case 800:  sample_time=("aurs 8 8\r");
            break;
        case 900:  sample_time=("aurs 9 9\r");
            break;
        case 1000: sample_time=("aurs 10 10\r");
            break;
        }
        send_message(sample_time);
        ui->spinBox_sampletime->setEnabled(false);
        timer_sampletime.start(300);
    }
}

//Device reset function
void MainWindow::on_actionReset_triggered()
{
    if (ui->plot_distance->isChecked())
    {
        QMessageBox::information(this, "Error", tr("Desactive la recepción y graficación de los datos para reiniciar el dispositivo"));
        return;
    }
    send_message("reset\r");
    timer_opmodechange.start(1500);
    ui->textEdit->append(tr("\r\n\r\nReiniciando el dispositivo..."));
}

//Device factory reset function
void MainWindow::on_actionFactory_Reset_triggered()
{
    if (ui->plot_distance->isChecked())
    {
        QMessageBox::information(this, "Error", tr("Desactive la recepción y graficación de los datos para continuar"));
        return;
    }
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,"", tr("¿Estas seguro de restaurar valores de fábrica?\nSe perderá toda configuración guardada"),
                                                               QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes) {
        send_message("frst\r");
        ui->textEdit->append(tr("\r\n\r\nREALIZANDO REINICIO DE FÁBRICA, ESPERE UN MOMENTO..."));
        timer_opmodechange.start(2000);
        ui->label_networkID->setText("0x0000");
        QMessageBox::information(this, tr("información"), tr("Recuerde configurar el dispositivo como TAG y asignar el ID de la red correctamente"));
    }
}

//Data recording window settings
void MainWindow::on_actionDataSavingSettings_triggered()
{
    if(savedata==1){
        QMessageBox::information(this, tr("Error"), tr("Desactive la grabación de los datos para configurar el archivo .txt"));
        return;
    }
    else{
        Dialog_csv dialog;
        dialog.setModal(true);
        dialog.exec();
        if (dialog.result()==1){
            filename = dialog.folder_filename_csv();
            ui->actionStartDataSaving->setEnabled(true);
            ui->textEdit->append(tr("\r\n\r\nConfiguración de datos csv finalizada. El archivo se guardará en la siguiente ruta:\r\n"));
            ui->textEdit->append(filename);
        }
    }
}

//Start data recording
void MainWindow::on_actionStartDataSaving_triggered()
{
    savedata=1;
    ui->textEdit->append(tr("\r\rGrabando Valores"));
    ui->actionStartDataSaving->setEnabled(false);
    ui->actionDataSavingSettings->setEnabled(false);
    ui->actionStopDataSaving->setEnabled(true);
    ui->label_connection->setText(tr("<b><FONT COLOR='blue' FONT SIZE = 4>GRABANDO</b></font>"));
    ui->textEdit->append(tr("\r\n\r\nHa comenzado el grabado de los datos recibidos.\r\n"));
}

//Stop data recording
void MainWindow::on_actionStopDataSaving_triggered()
{
    savedata=0;
    ui->textEdit->append(tr("\r\nSe ha desactivado grabado de valores."));
    ui->actionStartDataSaving->setEnabled(true);
    ui->actionStopDataSaving->setEnabled(false);
    ui->actionDataSavingSettings->setEnabled(true);
    ui->label_connection->setText(tr("<b><FONT COLOR='green' FONT SIZE = 4>CONECTADO</b></font>"));
    distance_samples_ID1=0;
    distance_samples_ID2=0;
    distance_samples_ID3=0;
    distance_samples_ID4=0;
    position_samples=0;
}

//Position graph window settings
void MainWindow::on_actionPositionGraphSettings_triggered()
{
    QList<float> position0;
    position0.append(range_pos_x_min);
    position0.append(range_pos_x_max);
    position0.append(range_pos_y_min);
    position0.append(range_pos_y_max);
    position0.append(anchor_1_x);
    position0.append(anchor_2_x);
    position0.append(anchor_3_x);
    position0.append(anchor_4_x);
    position0.append(anchor_1_y);
    position0.append(anchor_2_y);
    position0.append(anchor_3_y);
    position0.append(anchor_4_y);

    Dialog_pos_graph_config dialog;
    dialog.load_data(position0);
    dialog.setModal(true);
    dialog.exec();

    if (dialog.result()==1){


        //ui->posicionamiento->graph(0)->data()->clear();
        QList<float> new_anchor_data = dialog.anchor_data();
        bool passive;
        anchor_1_x = new_anchor_data[0];
        anchor_1_y = new_anchor_data[1];
        anchor_2_x = new_anchor_data[2];
        anchor_2_y = new_anchor_data[3];
        anchor_3_x = new_anchor_data[4];
        anchor_3_y = new_anchor_data[5];
        anchor_4_x = new_anchor_data[6];
        anchor_4_y = new_anchor_data[7];
        range_pos_y_min = new_anchor_data[8];
        range_pos_y_max = new_anchor_data[9];
        range_pos_x_min = new_anchor_data[10];
        range_pos_x_max = new_anchor_data[11];

        pos_anchors();

        passive = dialog.passive_mode();
        if(passive==1){
            passive_config();
        }

    }
}

//Distance graph window settings
void MainWindow::on_actionDistanceGraphSettings_triggered()
{
    QList<float> graph0;
    graph0.append(range_dist_y_max);
    graph0.append(range_dist_y_min);
    graph0.append(graph0_size);
    graph0.append(graph1_size);
    graph0.append(graph2_size);

    graph0.append(graph4_size);
    graph0.append(graph5_size);
    graph0.append(graph6_size);

    Dialog_dist_graph_config dialog;
    dialog.load_data(graph0);
    dialog.setModal(true);
    dialog.exec();

    if (dialog.result()==1){

        QList<float> new_data = dialog.graph_data();

        range_dist_y_max = new_data[0];
        range_dist_y_min = new_data[1];
        graph0_size = new_data[2];
        graph1_size = new_data[3];
        graph2_size = new_data[4];

        color1=obtain_color(new_data[5]);
        color2=obtain_color(new_data[6]);
        color3=obtain_color(new_data[7]);

        graph4_size = new_data[8];
        graph5_size = new_data[9];
        graph6_size = new_data[10];

        color1_2=obtain_color(new_data[11]);
        color1_3=obtain_color(new_data[12]);
        color1_4=obtain_color(new_data[13]);

        QList<int> color_index;
        color_index.append(new_data[5]);
        color_index.append(new_data[11]);
        color_index.append(new_data[12]);
        color_index.append(new_data[13]);

        int current_color_index;
        for(int i = 0; i < color_index.size(); ++i) {
            current_color_index = color_index[i];
            switch(current_color_index){
            case 0:
                chosen_color_info="red";
                break;
            case 1:
                chosen_color_info="blue";
                break;
            case 2:
                chosen_color_info="darkGreen";
                break;
            case 3:
                chosen_color_info="gray";
                break;
            case 4:
                chosen_color_info="magenta";
                break;
            case 5:
                chosen_color_info="cyan";
                break;
            case 6:
                chosen_color_info="black";
                break;
            case 7:
                chosen_color_info="darkYellow";
                break;
            }
            if (i==0){
                ui->label_29->setText("ID 1: <b><font color='" + chosen_color_info + "'>" + id_1 + "</font>");
                chosen_color_d1=chosen_color_info;
            }
            else if(i==1){
                ui->label_30->setText("ID 2: <b><font color='" + chosen_color_info + "'>" + id_2 + "</font>");
                chosen_color_d2=chosen_color_info;
            }
            else if(i==2){
                ui->label_41->setText("ID 3: <b><font color='" + chosen_color_info + "'>" + id_3 + "</font>");
                chosen_color_d3=chosen_color_info;
            }
            else{
                ui->label_42->setText("ID 4: <b><font color='" + chosen_color_info + "'>" + id_4 + "</font>");
                chosen_color_d4=chosen_color_info;
            }
        }

        ui->customPlot->yAxis->setRange(range_dist_y_min,range_dist_y_max);

        //QPen graphPen;
        graphPen.setColor(color1); //Distance 1
        graphPen.setWidth(graph0_size);

        graphPen_2.setColor(color2);
        graphPen_2.setWidth(graph1_size);

        graphPenmaxmin.setColor(color3);
        graphPenmaxmin.setWidth(graph2_size);

        graphPen_3.setColor(color1_2); //Distance 2
        graphPen_3.setWidth(graph4_size);

        graphPen_4.setColor(color1_3); //Distance 3
        graphPen_4.setWidth(graph5_size);

        graphPen_5.setColor(color1_4); //Distance 4
        graphPen_5.setWidth(graph6_size);

        ui->customPlot->graph(0)->setPen(graphPen);
        ui->customPlot->graph(1)->setPen(graphPen_2);
        ui->customPlot->graph(2)->setPen(graphPenmaxmin);
        ui->customPlot->graph(3)->setPen(graphPenmaxmin);
        ui->customPlot->graph(4)->setPen(graphPen_3);
        ui->customPlot->graph(5)->setPen(graphPen_4);
        ui->customPlot->graph(6)->setPen(graphPen_5);

        auto_range=dialog.autorange();
    }
}

//Distance media filter settings
void MainWindow::on_pushButton_2_clicked()
{
    QList<float> info_bufer;
    info_bufer.append(bufer_size);
    info_bufer.append(mean_enable);
    info_bufer.append(mean_value_alarm_link);

    Dialog_mediafilter dialog;
    dialog.load_data_bufer(info_bufer);
    dialog.setModal(true);
    dialog.exec();

    if (dialog.result()==1){

        QList<float> new_info_bufer = dialog.bufer_data();

        bufer_size = new_info_bufer[0];
        mean_value_alarm_link=dialog.link_alarm();

        mean_enable=dialog.enable_mediafilter();
        if(mean_enable==0){
            ui->label_mediafilter_1->setText("N/A");
            ui->label_mediafilter_2->setText("N/A");
            ui->label_mediafilter_3->setText("N/A");
            ui->label_mediafilter_4->setText("N/A");
        }
        else{
            if(detected_devices==1){
                bufer_media=distance_value_1;
            }
            else if(detected_devices==2){
                bufer_media=distance_value_1;
                bufer_media_2=distance_value_2;
            }
            else if(detected_devices==3){
                bufer_media=distance_value_1;
                bufer_media_2=distance_value_2;
                bufer_media_3=distance_value_3;
            }
            else{
                bufer_media=distance_value_1;
                bufer_media_2=distance_value_2;
                bufer_media_3=distance_value_3;
                bufer_media_4=distance_value_4;
            }
            ui->textEdit->append(tr("\r\n\r\nFiltro de media activado."));
        }
    }
}

//Distance alarm settings
void MainWindow::on_pushButton_clicked()
{
    QList<float> received_alarm_data;
    received_alarm_data.append(max_value_alarm);
    received_alarm_data.append(min_value_alarm);
    received_alarm_data.append(alarm_delay);
    received_alarm_data.append(alarm_duration/1000);
    received_alarm_data.append(chosen_alarm);
    received_alarm_data.append(distance_alarm_plot);

    Dialog_alarm_dist dialog;
    dialog.load_data_alarm(received_alarm_data);
    dialog.setModal(true);
    dialog.exec();

    if (dialog.result()==1){
        double last_value_copy=distance_value_1;
        QList<float> new_alarm_data = dialog.alarm_data();

        max_value_alarm = new_alarm_data[0];
        min_value_alarm = new_alarm_data[1];
        alarm_delay = static_cast<int>(new_alarm_data[3]*1000);
        alarm_duration = static_cast<int>(new_alarm_data[2]*1000);
        chosen_alarm = static_cast<int>(new_alarm_data[4]);
        distance_alarm_plot = dialog.alarm_activation();
        dist_alarm_activation_request();
        if(distance_alarm_plot==1){
            ui->tab_4->setEnabled(false);
            ui->textEdit->append(tr("\r\n\r\nAlarma activada."));
        }
        else{
            ui->tab_4->setEnabled(true);
        }
    }
}

//DWM1001-DEV device window settings
void MainWindow::on_actionDeviceSettings_triggered()
{
    if (ui->plot_distance->isChecked())
    {
        QMessageBox::information(this, "Error", tr("Desactive la recepción y graficación de los datos para configurar"));
        return;
    }
    Dialog_deviceconfig dialog;
    dialog.setModal(true);
    dialog.exec();

    int func_mode=0;
    QString adress="";

    if (dialog.result()==1){
        func_mode = dialog.mode_processing();
        adress = dialog.network_processing();
        ui->plot_distance->setEnabled(false);
        if(adress!=""){
            network_config(adress);
            ui->label_networkID->setText(adress);
            p_serie->waitForReadyRead(80);
            ui->textEdit->append(tr("\r\n\r\nNuevo ID de red establecido"));
        }
        if(func_mode!=0){
            device_config(func_mode);
            p_serie->waitForBytesWritten(50);
            ui->textEdit->append(tr("\r\nModo de funcionamiento establecido"));
        }
    }
}

//Position media filter window settings
void MainWindow::on_pushButton_6_clicked()
{
    QList<float> info_bufer;
    info_bufer.append(bufer_size);
    info_bufer.append(mean_enable);
    info_bufer.append(mean_value_alarm_link);

    Dialog_mediafilter dialog;
    dialog.load_data_bufer(info_bufer);
    dialog.setModal(true);
    dialog.exec();

    if (dialog.result()==1){

        QList<float> new_bufer_info = dialog.bufer_data();

        bufer_size = new_bufer_info[0];

        mean_value_alarm_link=dialog.link_alarm();

        mean_enable=dialog.enable_mediafilter();
        if(mean_enable==0){
            ui->label_46->setText(tr("Desactivado"));
        }
        else{
            ui->label_46->setText(tr("<b><FONT COLOR='green' FONT SIZE = 4>Activado</b></font>"));
        }
    }
}

//Position alarm window settings
void MainWindow::on_pushButton_5_clicked()
{
    QList<float> received_alarm_pos_data;
    received_alarm_pos_data.append(pos_alarm_x_max);
    received_alarm_pos_data.append(pos_alarm_x_min);
    received_alarm_pos_data.append(pos_alarm_y_max);
    received_alarm_pos_data.append(pos_alarm_y_min);
    received_alarm_pos_data.append(pos_alarm_z_max);
    received_alarm_pos_data.append(pos_alarm_z_min);
    received_alarm_pos_data.append(chosen_alarm);
    received_alarm_pos_data.append(pos_alarm_delay);
    received_alarm_pos_data.append(pos_alarm_duration/1000);
    received_alarm_pos_data.append(position_alarm_pos);
    received_alarm_pos_data.append(pos_x_alarm_ena);
    received_alarm_pos_data.append(pos_y_alarm_ena);
    received_alarm_pos_data.append(pos_z_alarm_ena);

    Dialog_alarm_pos dialog;
    dialog.load_data_alarm(received_alarm_pos_data);
    dialog.setModal(true);
    dialog.exec();

    if (dialog.result()==1){

        QList<float> new_alarm_pos_data = dialog.alarm_data();

        pos_alarm_x_max = new_alarm_pos_data[0];
        pos_alarm_x_min = new_alarm_pos_data[1];
        pos_alarm_y_max = new_alarm_pos_data[2];
        pos_alarm_y_min = new_alarm_pos_data[3];
        pos_alarm_z_max = new_alarm_pos_data[4];
        pos_alarm_z_min = new_alarm_pos_data[5];
        chosen_alarm = static_cast<int>(new_alarm_pos_data[6]);
        pos_alarm_delay = static_cast<int>(new_alarm_pos_data[7]*1000);
        pos_alarm_duration = static_cast<int>(new_alarm_pos_data[8]*1000);
        position_alarm_pos = dialog.alarm_activation();
        pos_x_alarm_ena = dialog.alarm_x_enabled();
        pos_y_alarm_ena = dialog.alarm_y_enabled();
        pos_z_alarm_ena = dialog.alarm_z_enabled();

        dist_alarm_activation_request();
        ui->position_graph->clearItems();
        ui->position_graph->replot();
        if(position_alarm_pos==1){
            draw_alarm_pos();
        }
    }
}

void MainWindow::on_comboBox_GPIO8_activated(int index)
{
    if(index==0){
        send_message("gc 8\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO8->setEnabled(false);
    }
    else{
        send_message("gs 8\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        send_message("gt 8\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO8->setEnabled(true);
    }
}

void MainWindow::on_comboBox_GPIO9_activated(int index)
{
    if(index==0){
        send_message("gc 9\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO9->setEnabled(false);
    }
    else{
        send_message("gs 9\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        send_message("gt 9\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO9->setEnabled(true);
    }
}

void MainWindow::on_comboBox_GPIO10_activated(int index)
{
    if(index==0){
        send_message("gc 10\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO9->setEnabled(false);
    }
    else{
        send_message("gs 10\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        send_message("gt 10\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO9->setEnabled(true);
    }
}

void MainWindow::on_comboBox_GPIO12_activated(int index)
{
    if(index==0){
        send_message("gc 12\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO10->setEnabled(false);
    }
    else{
        send_message("gs 12\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        send_message("gt 12\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO10->setEnabled(true);
    }
}

void MainWindow::on_pushButton_GPIO8_clicked()
{
    if (ui->pushButton_GPIO8->text()=="Activar" or ui->pushButton_GPIO8->text()=="Activate"){
        send_message("gs 8\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO8->setText(tr("Desactivar"));

        double tem_gpio8d = (ui->doubleSpinBox_GPIO8->value())*1000;
        int tem_gpio8i = static_cast<int>(tem_gpio8d);
        if(tem_gpio8i==0){
            return;
        }
        deactivate_GPIO=8;
        ui->pushButton_GPIO8->setEnabled(false);
        tem_gpio8.start(tem_gpio8i);

    }
    else if (ui->pushButton_GPIO8->text()=="Desactivar" or ui->pushButton_GPIO8->text()=="Turn off"){
        send_message("gt 8\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO8->setText(tr("Activar"));
    }
}

void MainWindow::on_pushButton_GPIO9_clicked()
{
    if (ui->pushButton_GPIO9->text()=="Activar" or ui->pushButton_GPIO9->text()=="Activate"){
        send_message("gs 9\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO9->setText(tr("Desactivar"));

        double tem_gpio9d = (ui->doubleSpinBox_GPIO9->value())*1000;
        int tem_gpio9i = static_cast<int>(tem_gpio9d);
        if(tem_gpio9i==0){
            return;
        }
        deactivate_GPIO=9;
        ui->pushButton_GPIO9->setEnabled(false);
        tem_gpio9.start(tem_gpio9i);

    }
    else if (ui->pushButton_GPIO9->text()=="Desactivar" or ui->pushButton_GPIO9->text()=="Turn off"){
        send_message("gt 9\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO9->setText(tr("Activar"));
    }
}

void MainWindow::on_pushButton_GPIO10_clicked()
{
    if (ui->pushButton_GPIO10->text()=="Activar" or ui->pushButton_GPIO10->text()=="Activate"){
        send_message("gs 10\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO10->setText(tr("Desactivar"));

        double tem_gpio10d = (ui->doubleSpinBox_GPIO10->value())*1000;
        int tem_gpio10i = static_cast<int>(tem_gpio10d);
        if(tem_gpio10i==0){
            return;
        }
        deactivate_GPIO=10;
        ui->pushButton_GPIO10->setEnabled(false);
        tem_gpio10.start(tem_gpio10i);
    }

    else if (ui->pushButton_GPIO10->text()=="Desactivar" or ui->pushButton_GPIO10->text()=="Turn off"){
        send_message("gt 10\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO10->setText(tr("Activar"));
    }
}

void MainWindow::on_pushButton_GPIO12_clicked()
{
    if (ui->pushButton_GPIO12->text()=="Activar" or ui->pushButton_GPIO12->text()=="Activate"){
        send_message("gs 12\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO12->setText(tr("Desactivar"));

        double tem_gpio12d = (ui->doubleSpinBox_GPIO12->value())*1000;
        int tem_gpio12i = static_cast<int>(tem_gpio12d);
        if(tem_gpio12i==0){
            return;
        }
        deactivate_GPIO=10;
        ui->pushButton_GPIO12->setEnabled(false);
        tem_gpio12.start(tem_gpio12i);

    }
    else if (ui->pushButton_GPIO12->text()=="Desactivar" or ui->pushButton_GPIO12->text()=="Turn off"){
        send_message("gt 12\r");
        p_serie->waitForReadyRead(10);
        p_serie->waitForBytesWritten(10);
        ui->pushButton_GPIO12->setText(tr("Activar"));
    }
}

//Statistics window


void MainWindow::on_actionStatistics_triggered()
{
    if(enable_statistics==0){
        QMessageBox::information(this, tr("información"), tr("No se disponen los datos necesarios para el cálculo estadístico"));
        return;
    }
    disconnect(p_serie, &QSerialPort::readyRead, this, &MainWindow::readData);
    if(operation_mode==0){
        distance_stats_calc();
    }
    else{
        position_stats_calc();
    }
    Dialog_stats dialog;
    if(operation_mode==0){
        QList<QString> info_ids;
        QList<double> info_stats_d1;
        QList<double> info_stats_d2;
        QList<double> info_stats_d3;
        QList<double> info_stats_d4;
        int id_labels[4]={id_label_1,id_label_2,id_label_3,id_label_4};
        for(int i = 0; i < detected_devices; i++) {
            switch(id_labels[i]){
            case 1:

                info_ids.append("1");
                info_ids.append(initial_id_1);

                info_stats_d1.append(mediafilter_value_1);
                info_stats_d1.append(mode_1);
                info_stats_d1.append(std_1);
                info_stats_d1.append(range_1);
                info_stats_d1.append(max_value_1);
                info_stats_d1.append(min_value_1);
                info_stats_d1.append(percentil_25_1);
                info_stats_d1.append(percentil_75_1);
                info_stats_d1.append(received_data_quantity_1);
                dialog.load_data_statistics_d1(info_stats_d1);
                break;

            case 2:

                info_ids.clear();
                info_ids.append("2");
                info_ids.append(initial_id_1);
                info_ids.append(initial_id_2);
                info_stats_d2.append(mediafilter_value_2);
                info_stats_d2.append(mode_2);
                info_stats_d2.append(std_2);
                info_stats_d2.append(range_2);
                info_stats_d2.append(max_value_2);
                info_stats_d2.append(min_value_2);
                info_stats_d2.append(percentil_25_2);
                info_stats_d2.append(percentil_75_2);
                info_stats_d2.append(received_data_quantity_2);
                dialog.load_data_statistics_d2(info_stats_d2);

                break;

            case 3:

                info_ids.clear();
                info_ids.append("3");
                info_ids.append(initial_id_1);
                info_ids.append(initial_id_2);
                info_ids.append(initial_id_3);


                info_stats_d3.append(mediafilter_value_3);
                info_stats_d3.append(mode_3);
                info_stats_d3.append(std_3);
                info_stats_d3.append(range_3);
                info_stats_d3.append(max_value_3);
                info_stats_d3.append(min_value_3);
                info_stats_d3.append(percentil_25_3);
                info_stats_d3.append(percentil_75_3);
                info_stats_d3.append(received_data_quantity_3);
                dialog.load_data_statistics_d3(info_stats_d3);

                break;
            case 4:

                info_ids.clear();
                info_ids.append("4");
                info_ids.append(initial_id_1);
                info_ids.append(initial_id_2);
                info_ids.append(initial_id_3);
                info_ids.append(initial_id_4);


                info_stats_d4.append(mediafilter_value_4);
                info_stats_d4.append(mode_4);
                info_stats_d4.append(std_4);
                info_stats_d4.append(range_4);
                info_stats_d4.append(max_value_4);
                info_stats_d4.append(min_value_4);
                info_stats_d4.append(percentil_25_4);
                info_stats_d4.append(percentil_75_4);
                info_stats_d4.append(received_data_quantity_4);
                dialog.load_data_statistics_d4(info_stats_d4);

                break;
            }
        }
        dialog.load_data_statistics_ids(info_ids);
        ui->textEdit->append(tr("\r\n\r\nSe han calculado las estadísticas relativas a la recepción de datos de distancia. Pulse el botón de reiniciar valores para calcular"
                                "estadísticas a partir de este instante."));
    }
    else if (operation_mode==1){
        QList<double> info_stats_pos_x;
        info_stats_pos_x.append(mean_total_x);
        info_stats_pos_x.append(mode_x);
        info_stats_pos_x.append(std_x);
        info_stats_pos_x.append(range_x);
        info_stats_pos_x.append(max_x);
        info_stats_pos_x.append(min_x);
        info_stats_pos_x.append(percentil_25_x);
        info_stats_pos_x.append(percentil_75_x);
        dialog.load_data_statistics_pos_x(info_stats_pos_x);

        QList<double> info_stats_pos_y;
        info_stats_pos_y.append(mean_total_y);
        info_stats_pos_y.append(mode_y);
        info_stats_pos_y.append(std_y);
        info_stats_pos_y.append(range_y);
        info_stats_pos_y.append(max_y);
        info_stats_pos_y.append(min_y);
        info_stats_pos_y.append(percentil_25_y);
        info_stats_pos_y.append(percentil_75_y);
        dialog.load_data_statistics_pos_y(info_stats_pos_y);

        QList<double> info_stats_pos_z;
        info_stats_pos_z.append(mean_total_z);
        info_stats_pos_z.append(mode_z);
        info_stats_pos_z.append(std_z);
        info_stats_pos_z.append(range_z);
        info_stats_pos_z.append(max_z);
        info_stats_pos_z.append(min_z);
        info_stats_pos_z.append(percentil_25_z);
        info_stats_pos_z.append(percentil_75_z);
        dialog.load_data_statistics_pos_z(info_stats_pos_z);
        ui->textEdit->append(tr("\r\n\r\nSe han calculado las estadísticas relativas a la recepción de datos de posición. Pulse el botón de reiniciar valores para calcular"
                                "estadísticas a partir de este instante."));
    }
    connect(p_serie, &QSerialPort::readyRead, this, &MainWindow::readData);

    dialog.initialise_variables();

    dialog.setModal(true);
    dialog.exec();

    if (dialog.result()==1){

        if(dialog.enable_data_saving()==1){
            filename_est = dialog.directory_folder_stat();
            save_stats_data();
        }

        if(dialog.reset_variables()==1){
            reset_stats();
        }
    }
}

//About window
void MainWindow::on_actionAbout_triggered()
{
    Dialog_about dialog;
    dialog.setModal(true);
    dialog.exec();
}

void MainWindow::on_actionHelp_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Antoi11/DWM1001-DEV-GUI/blob/main/User%20Guide.pdf", QUrl::TolerantMode));
}

/**************************************
    DATA PROCESSING FUNCTIONS
***************************************/

//When data reception is enabled and a full message is received, this function splits and analyzes each message,
//obtaining relevant values, and calling alarm, media filter or data recording if requested
void MainWindow::distance_analysis(const QByteArray &data)
{
    //qDebug() << "Analisis distancia";
    QStringList received_message;
    QString DataAsString = QTextCodec::codecForMib(106)->toUnicode(data);
    received_message = DataAsString.split("\r\n");

    double id_dist_1=0;
    double id_dist_2=0;
    double id_dist_3=0;
    double id_dist_4=0;

    for (int i=0; i<received_message.size(); i++){
        QStringList comma_separated_message = received_message[i].split(",");
        int elementos_mensaje = comma_separated_message.size();

        if (elementos_mensaje == 8){

            multiple_measures=0;
            detected_devices=1;
            if(detected_devices>max_detected_devices){
                max_detected_devices = detected_devices;
            }

            id_1 = comma_separated_message[3];
            if(info_ids_1==0){
                if(info_ids_2==0 and info_ids_3==0 and info_ids_4==0){
                    initial_id_1 = id_1;
                }
                ui->label_29->setText("ID 1:  <b><font color='" + chosen_color_d1 + "'>" + initial_id_1 + "</font>");
                info_ids_1=1;
            }
            if (id_1==initial_id_1){
                distance_value_1 = comma_separated_message[7].toDouble();
                id_dist_1=distance_value_1;
                id_label_1=1;
            }
            else if (id_1==initial_id_2){
                distance_value_2 = comma_separated_message[7].toDouble();
                id_dist_1=distance_value_2;
                id_label_1=2;
            }
            else if (id_1==initial_id_3){
                distance_value_3 = comma_separated_message[7].toDouble();
                id_dist_1=distance_value_3;
                id_label_1=3;
            }
            else if (id_1==initial_id_4){
                distance_value_4 = comma_separated_message[7].toDouble();
                id_dist_1=distance_value_4;
                id_label_1=4;
            }

            if(savedata==1){
                save_distance_data(QDateTime::currentDateTime());
            }
        }

        else if (elementos_mensaje == 14){

            multiple_measures=1;
            detected_devices=2;
            if(detected_devices>max_detected_devices){
                max_detected_devices = detected_devices;
            }
            id_1 = comma_separated_message[3];
            id_2 = comma_separated_message[9];
            if (info_ids_2==0){
                if(info_ids_1==1 and info_ids_3==0 and info_ids_4==0){
                    QString ids_check[4]={id_1,id_2,id_3,id_4};
                    QList<QString> new_ids;
                    for(int i = 0; i < 4; i++) {
                        if((ids_check[i]!=initial_id_1)){
                            new_ids.append(ids_check[i]);
                        }
                    }
                    initial_id_2 = new_ids[0];
                }
                else{
                    initial_id_1 = id_1;
                    initial_id_2 = id_2;
                }
                ui->textEdit->append(tr("\r\nDetectados 2 módulos a representar\r\n"));
                ui->label_29->setText("ID 1:  <b><font color='" + chosen_color_d1 + "'>" + initial_id_1 + "</font>");
                ui->label_30->setText("ID 2:  <b><font color='" + chosen_color_d2 + "'>" + initial_id_2 + "</font>");
                info_ids_1=1;
                info_ids_2=1;
            }

            if(true){
                if (id_1==initial_id_1){
                    distance_value_1 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_1;
                    id_label_1=1;
                }
                else if (id_2==initial_id_1){
                    distance_value_1 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_1;
                    id_label_2=1;
                }
            }
            if(true){
                if (id_1==initial_id_2){
                    distance_value_2 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_2;
                    id_label_1=2;
                }
                else if (id_2==initial_id_2){
                    distance_value_2 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_2;
                    id_label_2=2;
                }
            }
            if(max_detected_devices==3 or max_detected_devices==4){
                if (id_1==initial_id_3){
                    distance_value_3 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_3;
                    id_label_1=3;
                }
                else if (id_2==initial_id_3){
                    distance_value_3 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_3;
                    id_label_2=3;
                }
            }
            if(max_detected_devices==4){
                if (id_1==initial_id_4){
                    distance_value_4 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_4;
                    id_label_1=4;
                }
                else if (id_2==initial_id_4){
                    distance_value_4 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_4;
                    id_label_2=4;
                }
            }

            if(savedata==1){
                save_distance_data(QDateTime::currentDateTime());
            }
        }
        else if (elementos_mensaje == 20 or elementos_mensaje == 25){
            multiple_measures=1;
            detected_devices=3;
            if(detected_devices>max_detected_devices){
                max_detected_devices = detected_devices;
            }
            id_1 = comma_separated_message[3];
            id_2 = comma_separated_message[9];
            id_3 = comma_separated_message[15];
            if (info_ids_3==0){
                if(info_ids_1==1 and info_ids_2==0 and info_ids_4==0){
                    QString ids_check[4]={id_1,id_2,id_3,id_4};
                    QList<QString> new_ids;
                    for(int i = 0; i < 4; i++) {
                        if((ids_check[i]!=initial_id_1)){
                            new_ids.append(ids_check[i]);
                        }
                    }
                    initial_id_2 = new_ids[0];
                    initial_id_3 = new_ids[1];
                }
                else if(info_ids_2==1 and info_ids_4==0){
                    QString ids_check[4]={id_1,id_2,id_3,id_4};
                    QList<QString> new_ids;
                    for(int i = 0; i < 4; i++) {
                        if((ids_check[i]!=initial_id_1) and (ids_check[i]!=initial_id_2)){
                            new_ids.append(ids_check[i]);
                        }
                    }
                    initial_id_3 = new_ids[0];
                }
                else{
                    initial_id_1 = id_1;
                    initial_id_2 = id_2;
                    initial_id_3 = id_3;
                }
                ui->textEdit->append(tr("\r\nDetectados 3 módulos a representar\r\n"));
                ui->label_29->setText("ID 1:  <b><font color='" + chosen_color_d1 + "'>" + initial_id_1 + "</font>");
                ui->label_30->setText("ID 2:  <b><font color='" + chosen_color_d2 + "'>" + initial_id_2 + "</font>");
                ui->label_41->setText("ID 3:  <b><font color='" + chosen_color_d3 + "'>" + initial_id_3 + "</font>");
                info_ids_1=1;
                info_ids_2=1;
                info_ids_3=1;
            }
            if(true){
                if (id_1==initial_id_1){
                    distance_value_1 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_1;
                    id_label_1=1;
                }
                else if (id_2==initial_id_1){
                    distance_value_1 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_1;
                    id_label_2=1;
                }
                else if (id_3==initial_id_1){
                    distance_value_1 = comma_separated_message[19].toDouble();
                    id_dist_3=distance_value_1;
                    id_label_3=1;
                }
            }
            if(true){
                if (id_1==initial_id_2){
                    distance_value_2 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_2;
                    id_label_1=2;
                }
                else if (id_2==initial_id_2){
                    distance_value_2 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_2;
                    id_label_2=2;
                }
                else if (id_3==initial_id_2){
                    distance_value_2 = comma_separated_message[19].toDouble();
                    id_dist_3=distance_value_2;
                    id_label_3=2;
                }
            }
            if(true){
                if (id_1==initial_id_3){
                    distance_value_3 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_3;
                    id_label_1=3;
                }
                else if (id_2==initial_id_3){
                    distance_value_3 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_3;
                    id_label_2=3;
                }
                else if (id_3==initial_id_3){
                    distance_value_3 = comma_separated_message[19].toDouble();
                    id_dist_3=distance_value_3;
                    id_label_3=3;
                }
            }
            if(max_detected_devices==4){
                qDebug() << max_detected_devices;
                if (id_1==initial_id_4){
                    distance_value_4 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_4;
                    id_label_1=4;
                }
                else if (id_2==initial_id_4){
                    distance_value_4 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_4;
                    id_label_2=4;
                }
                else if (id_3==initial_id_4){
                    distance_value_4 = comma_separated_message[19].toDouble();
                    id_dist_3=distance_value_4;
                    id_label_3=4;
                }
            }
            if(savedata==1){
                save_distance_data(QDateTime::currentDateTime());
            }
        }

        else if ((elementos_mensaje == 31) or (elementos_mensaje == 26)){

            multiple_measures=1;
            detected_devices=4;
            if(detected_devices>max_detected_devices){
                max_detected_devices = detected_devices;
            }
            id_1 = comma_separated_message[3];
            id_2 = comma_separated_message[9];
            id_3 = comma_separated_message[15];
            id_4 = comma_separated_message[21];
            if (info_ids_4==0){
                if(info_ids_1==1 and info_ids_2==0 and info_ids_3==0){
                    QString ids_check[4]={id_1,id_2,id_3,id_4};
                    QList<QString> new_ids;
                    for(int i = 0; i < 4; i++) {
                        if((ids_check[i]!=initial_id_1)){
                            new_ids.append(ids_check[i]);
                        }
                    }
                    initial_id_2 = new_ids[0];
                    initial_id_3 = new_ids[1];
                    initial_id_4 = new_ids[2];
                }
                else if(info_ids_2==1 and info_ids_3==0){
                    QString ids_check[4]={id_1,id_2,id_3,id_4};
                    QList<QString> new_ids;
                    for(int i = 0; i < 4; i++) {
                        if((ids_check[i]!=initial_id_1) and (ids_check[i]!=initial_id_2)){
                            new_ids.append(ids_check[i]);
                        }
                    }
                    initial_id_3 = new_ids[0];
                    initial_id_4 = new_ids[1];
                }
                else if(info_ids_3==1){
                    QString ids_check[4]={id_1,id_2,id_3,id_4};
                    for(int i = 0; i < 4; i++) {
                        if((ids_check[i]!=initial_id_1) and (ids_check[i]!=initial_id_2) and (ids_check[i]!=initial_id_3)){
                            initial_id_4 = ids_check[i];
                        }
                    }
                }
                else{
                    initial_id_1 = id_1;
                    initial_id_2 = id_2;
                    initial_id_3 = id_3;
                    initial_id_4 = id_4;
                }
                ui->textEdit->append(tr("\r\nDetectados 4 módulos a representar\r\n"));
                if (elementos_mensaje == 26){
                    ui->textEdit->append(tr("No se ha podido obtener la posición estimada\r\n"));
                }
                ui->label_29->setText("ID 1:  <b><font color='" + chosen_color_d1 + "'>" + initial_id_1 + "</font>");
                ui->label_30->setText("ID 2:  <b><font color='" + chosen_color_d2 + "'>" + initial_id_2 + "</font>");
                ui->label_41->setText("ID 3:  <b><font color='" + chosen_color_d3 + "'>" + initial_id_3 + "</font>");
                ui->label_42->setText("ID 4:  <b><font color='" + chosen_color_d4 + "'>" + initial_id_4 + "</font>");
                info_ids_1=1;
                info_ids_2=1;
                info_ids_3=1;
                info_ids_4=1;
            }
            if(true){
                if (id_1==initial_id_1){
                    distance_value_1 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_1;
                    id_label_1=1;
                }
                else if (id_2==initial_id_1){
                    distance_value_1 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_1;
                    id_label_2=1;
                }
                else if (id_3==initial_id_1){
                    distance_value_1 = comma_separated_message[19].toDouble();
                    id_dist_3=distance_value_1;
                    id_label_3=1;
                }
                else if(id_4==initial_id_1){
                    distance_value_1 = comma_separated_message[25].toDouble();
                    id_dist_4=distance_value_1;
                    id_label_4=1;
                }
            }
            if(true){
                if (id_1==initial_id_2){
                    distance_value_2 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_2;
                    id_label_1=2;
                }
                else if (id_2==initial_id_2){
                    distance_value_2 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_2;
                    id_label_2=2;
                }
                else if (id_3==initial_id_2){
                    distance_value_2 = comma_separated_message[19].toDouble();
                    id_dist_3=distance_value_2;
                    id_label_3=2;
                }
                else if(id_4==initial_id_2){
                    distance_value_2 = comma_separated_message[25].toDouble();
                    id_dist_4=distance_value_2;
                    id_label_4=2;
                }
            }
            if(true){
                if (id_1==initial_id_3){
                    distance_value_3 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_3;
                    id_label_1=3;
                }
                else if (id_2==initial_id_3){
                    distance_value_3 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_3;
                    id_label_2=3;
                }
                else if (id_3==initial_id_3){
                    distance_value_3 = comma_separated_message[19].toDouble();
                    id_dist_3=distance_value_3;
                    id_label_3=3;
                }
                else if (id_4==initial_id_3){
                    distance_value_3 = comma_separated_message[25].toDouble();
                    id_dist_4=distance_value_3;
                    id_label_4=3;
                }
            }
            if(true){
                if (id_1==initial_id_4){
                    distance_value_4 = comma_separated_message[7].toDouble();
                    id_dist_1=distance_value_4;
                    id_label_1=4;
                }
                else if (id_2==initial_id_4){
                    distance_value_4 = comma_separated_message[13].toDouble();
                    id_dist_2=distance_value_4;
                    id_label_2=4;
                }
                else if (id_3==initial_id_4){
                    distance_value_4 = comma_separated_message[19].toDouble();
                    id_dist_3=distance_value_4;
                    id_label_3=4;
                }
                else if (id_4==initial_id_4){
                    distance_value_4 = comma_separated_message[25].toDouble();
                    id_dist_4=distance_value_4;
                    id_label_4=4;
                }
            }
            if(savedata==1){
                save_distance_data(QDateTime::currentDateTime());
            }
        }
        if(mean_enable==1){
            media_calc(id_label_1, id_label_2, id_label_3, id_label_4);
        }
        distance_data_processing(id_dist_1, id_dist_2, id_dist_3, id_dist_4, id_label_1, id_label_2, id_label_3, id_label_4);
        if(alarm_activated==0 and distance_alarm_plot==1){
            check_distance_alarm();
        }
    }
    enable_statistics=1;
}

void MainWindow::media_calc(int id_label_1, int id_label_2, int id_label_3, int id_label_4)
{
    if(operation_mode==0){
        int id_labels[4]={id_label_1,id_label_2,id_label_3,id_label_4};
        for(int i = 0; i < detected_devices; i++) {
            switch(id_labels[i]){
            case 1:
                int k;
                if (bufer.size()<bufer_size){
                    bufer.append(distance_value_1);
                }
                else if (bufer.size()>bufer_size){
                    int delete_1 = bufer.size()-bufer_size+1;
                    for (int i=0; i<delete_1; i++)bufer.removeFirst();
                    bufer.append(distance_value_1);
                }
                else{
                    for (k=0; k<bufer_size-1; k++)bufer[k] = bufer[k+1];
                    bufer[bufer_size-1] = distance_value_1;
                }
                bufer_media=0;
                for (k=0; k<bufer.size();k++) bufer_media+=bufer[k];
                bufer_media = bufer_media/k;
                break;

            case 2:
                int k_2;
                if (bufer_2.size()<bufer_size){
                    bufer_2.append(distance_value_2);
                }
                else if (bufer_2.size()>bufer_size){
                    int delete_2 = bufer_2.size()-bufer_size+1;
                    for (int i=0; i<delete_2; i++)bufer_2.removeFirst();
                    bufer_2.append(distance_value_2);
                }
                else{
                    for (k_2=0; k_2<bufer_size-1; k_2++)bufer_2[k_2] = bufer_2[k_2+1];
                    bufer_2[bufer_size-1] = distance_value_2;
                }
                bufer_media_2=0;
                for (k_2=0; k_2<bufer_2.size();k_2++) bufer_media_2+=bufer_2[k_2];
                bufer_media_2 = bufer_media_2/k_2;
                break;

            case 3:
                int k_3;
                if (bufer_3.size()<bufer_size){
                    bufer_3.append(distance_value_3);
                }
                else if (bufer_3.size()>bufer_size){
                    int delete_3 = bufer_3.size()-bufer_size+1;
                    for (int j=0; j<delete_3; j++)bufer_3.removeFirst();
                    bufer_3.append(distance_value_3);
                }
                else{
                    for (k_3=0; k_3<bufer_size-1; k_3++)bufer_3[k_3] = bufer_3[k_3+1];
                    bufer_3[bufer_size-1] = distance_value_3;
                }
                bufer_media_3=0;
                for (k_3=0; k_3<bufer_3.size();k_3++) bufer_media_3+=bufer_3[k_3];
                bufer_media_3 = bufer_media_3/k_3;
                break;

            case 4:
                int k_4;
                if (bufer_4.size()<bufer_size){
                    bufer_4.append(distance_value_4);
                }
                else if (bufer_4.size()>bufer_size){
                    int delete_4 = bufer_4.size()-bufer_size+1;
                    for (int i=0; i<delete_4; i++)bufer_4.removeFirst();
                    bufer_4.append(distance_value_4);
                }
                else{
                    for (k_4=0; k_4<bufer_size-1; k_4++)bufer_4[k_4] = bufer_4[k_4+1];
                    bufer_4[bufer_size-1] = distance_value_4;
                }
                bufer_media_4=0;
                for (k_4=0; k_4<bufer_4.size();k_4++) bufer_media_4+=bufer_4[k_4];
                bufer_media_4 = bufer_media_4/k_4;
            }
        }
    }
}

void MainWindow::media_calc_pos()
{
    if(true){
        int k;
        if (bufer_x.size()<bufer_size){
            bufer_x.append(position_x);
        }
        else if (bufer_x.size()>bufer_size){
            int delete_x = bufer_x.size()-bufer_size+1;
            for (int i=0; i<delete_x; i++)bufer_x.removeFirst();
            bufer_x.append(position_x);
        }
        else{
            for (k=0; k<bufer_size-1; k++)bufer_x[k] = bufer_x[k+1];
            bufer_x[bufer_size-1] = position_x;
        }
        bufer_media_x=0;
        for (k=0; k<bufer_x.size();k++) bufer_media_x+=bufer_x[k];
        bufer_media_x = bufer_media_x/k;
    }

    if(true){
        int j;
        if (bufer_y.size()<bufer_size){
            bufer_y.append(position_y);
        }
        else if (bufer_y.size()>bufer_size){
            int delete_y = bufer_y.size()-bufer_size+1;
            for (int i=0; i<delete_y; i++)bufer_y.removeFirst();
            bufer_y.append(position_y);
        }
        else{
            for (j=0; j<bufer_size-1; j++)bufer_y[j] = bufer_y[j+1];
            bufer_y[bufer_size-1] = position_y;
        }
        bufer_media_y=0;
        for (j=0; j<bufer_y.size();j++) bufer_media_y+=bufer_y[j];
        bufer_media_y = bufer_media_y/j;
    }

    if(true){
        int h;
        if (bufer_z.size()<bufer_size){
            bufer_z.append(position_z);
        }
        else if (bufer_z.size()>bufer_size){
            int delete_z = bufer_z.size()-bufer_size+1;
            for (int i=0; i<delete_z; i++)bufer_z.removeFirst();
            bufer_z.append(position_z);
        }
        else{
            for (h=0; h<bufer_size-1; h++)bufer_z[h] = bufer_z[h+1];
            bufer_z[bufer_size-1] = position_z;
        }
        bufer_media_z=0;
        for (h=0; h<bufer_z.size();h++) bufer_media_z+=bufer_z[h];
        bufer_media_z = bufer_media_z/h;
    }
}

//When data reception is enabled and a full message is received, this function splits and analyzes each message,
//obtaining relevant values, and calling alarm, media filter or data recording if requested
void MainWindow::position_analysis(const QByteArray &data2){
    QStringList received_message;
    QString DataAsString = QTextCodec::codecForMib(106)->toUnicode(data2);                                           //Pasamos el mensaje recibido por el puerto serie a String UTF-8
    received_message = DataAsString.split("\r\n");                                                                    //Organizamos el mensaje recibido dividiéndolo cuando detecte un salto de carro
    if(passive_positioning==1){
        for (int i=0; i<received_message.size(); i++){
            QStringList comma_separated_message = received_message[i].split(",");
            int elementos_mensaje = comma_separated_message.size();
            if (elementos_mensaje == 8){
                position_x = comma_separated_message[3].toDouble();
                position_y = comma_separated_message[4].toDouble();
                position_z = comma_separated_message[5].toDouble();

                if(comma_separated_message[3]=="nan" or comma_separated_message[4]=="nan" or comma_separated_message[5]=="nan"){
                    position_x = pos_x_temp;
                    position_y = pos_y_temp;
                    position_z = pos_z_temp;
                }
                else{
                    pos_x_temp = position_x;
                    pos_y_temp = position_y;
                    pos_z_temp = position_z;
                }
            }
        }
    }
    else if (tag_positioning==1 or (position_graph_enabled == 1)){
        for (int i=0; i<received_message.size(); i++){
            QStringList comma_separated_message = received_message[i].split(",");
            int elementos_mensaje = comma_separated_message.size();
            if (elementos_mensaje == 5){
                position_x = comma_separated_message[1].toDouble();
                position_y = comma_separated_message[2].toDouble();
                position_z = comma_separated_message[3].toDouble();
            }
        }
    }
    if(mean_enable==1){
        media_calc_pos();
        ui->label_56->setText(QString::number(bufer_media_x,'g',3));
        ui->label_57->setText(QString::number(bufer_media_y,'g',3));
        ui->label_58->setText(QString::number(bufer_media_z,'g',3));
    }
    else{
        ui->label_56->setText(QString::number(position_x,'g',3));
        ui->label_57->setText(QString::number(position_y,'g',3));
        ui->label_58->setText(QString::number(position_z,'g',3));
    }

    position_data_samples++;
    position_data_processing();
    enable_statistics=1;

    if((alarm_activated == 0) and (position_alarm_pos == 1)){
        check_position_alarm();
    }

    if(savedata==1){
        save_position_data(QDateTime::currentDateTime(), position_x, bufer_media_x, position_y, bufer_media_y, position_z, bufer_media_z);
    }
}

//Position processing values needed to calculate statistics
void MainWindow::position_data_processing()
{
    position_list_x.append(position_x);
    if(position_x > max_x){
        max_x = position_x;
    }
    if(position_x < min_x){
        min_x = position_x;
    }
    accumulated_pos_x = accumulated_pos_x + position_x;
    mean_total_x = (accumulated_pos_x)/position_data_samples;

    position_list_y.append(position_y);
    if(position_y > max_y){
        max_y = position_y;
    }
    if(position_y < min_y){
        min_y = position_y;
    }
    accumulated_pos_y = accumulated_pos_y + position_y;
    mean_total_y = (accumulated_pos_y)/position_data_samples;

    position_list_z.append(position_z);
    if(position_z > max_z){
        max_z = position_z;
    }
    if(position_z < min_z){
        min_z = position_z;
    }
    accumulated_pos_z = accumulated_pos_z + position_z;
    mean_total_z = (accumulated_pos_z)/position_data_samples;
}

//Distance processing values needed to calculate statistics

void MainWindow::distance_data_processing(double id_dist_1, double id_dist_2, double id_dist_3, double id_dist_4, int id_label_1, int id_label_2, int id_label_3, int id_label_4)
{
    int id_labels[4]={id_label_1,id_label_2,id_label_3,id_label_4};
    double id_distances[4]={id_dist_1, id_dist_2, id_dist_3, id_dist_4};
    for(int i = 0; i < detected_devices; i++) {
        switch(id_labels[i]){
        case 1:
            distance_list_1.append(id_distances[i]);
            ui->label_dist_value_1->setText(QString::number(id_distances[i]));
            received_data_quantity_1++;
            if(mean_enable==1){
                ui->label_mediafilter_1->setText(QString::number(bufer_media,'g',3)); //Comprobar funcionamiento al mirar la función de filtromedia.
            }
            if(id_distances[i]>max_value_1){
                max_value_1=id_distances[i];
            }
            if(id_distances[i]<min_value_1){
                min_value_1=id_distances[i];
            }
            accumulated_dist_1=accumulated_dist_1+id_distances[i];
            mediafilter_value_1=(accumulated_dist_1)/received_data_quantity_1;  //Crear un numero_muestras para cada ID.
            break;

        case 2:
            distance_list_2.append(id_distances[i]);
            ui->label_dist_value_2->setText(QString::number(id_distances[i]));
            received_data_quantity_2++;
            if(mean_enable==1){
                ui->label_mediafilter_2->setText(QString::number(bufer_media_2,'g',3)); //Comprobar funcionamiento al mirar la función de filtromedia.
            }
            if(id_distances[i]>max_value_2){
                max_value_2=id_distances[i];
            }
            if(id_distances[i]<min_value_2){
                min_value_2=id_distances[i];
            }
            accumulated_dist_2=accumulated_dist_2+id_distances[i];
            mediafilter_value_2=(accumulated_dist_2)/received_data_quantity_2;  //Crear un numero_muestras para cada ID.
            break;

        case 3:
            distance_list_3.append(id_distances[i]);
            ui->label_dist_value_3->setText(QString::number(id_distances[i]));
            received_data_quantity_3++;
            if(mean_enable==1){
                ui->label_mediafilter_3->setText(QString::number(bufer_media_3,'g',3)); //Comprobar funcionamiento al mirar la función de filtromedia.
            }
            if(id_distances[i]>max_value_3){
                max_value_3=id_distances[i];
            }
            if(id_distances[i]<min_value_3){
                min_value_3=id_distances[i];
            }
            accumulated_dist_3=accumulated_dist_3+id_distances[i];
            mediafilter_value_3=(accumulated_dist_3)/received_data_quantity_3;  //Crear un numero_muestras para cada ID.
            break;

        case 4:
            distance_list_4.append(id_distances[i]);
            ui->label_dist_value_4->setText(QString::number(id_distances[i]));
            received_data_quantity_4++;
            if(mean_enable==1){
                ui->label_mediafilter_4->setText(QString::number(bufer_media_4,'g',3)); //Comprobar funcionamiento al mirar la función de filtromedia.
            }
            if(id_distances[i]>max_value_4){
                max_value_4=id_distances[i];
            }
            if(id_distances[i]<min_value_4){
                min_value_4=id_distances[i];
            }
            accumulated_dist_4=accumulated_dist_4+id_distances[i];
            mediafilter_value_4=(accumulated_dist_4)/received_data_quantity_4;  //Crear un numero_muestras para cada ID.
            break;
        }
        if(auto_range==1){
            double maximum_range;
            double minimum_range;
            maximum_range = fmax(fmax(max_value_1,max_value_2), fmax(max_value_3,max_value_4));
            minimum_range = fmin(fmin(min_value_1,min_value_2), fmin(min_value_3,min_value_4));
            if(minimum_range-4>0){
                ui->customPlot->yAxis->setRange(range_dist_y_min,maximum_range+2);
            }
            else{
                ui->customPlot->yAxis->setRange(0,maximum_range+2);
            }
        }
    }
}

void MainWindow::reset_stats()
{
    distance_list_1.clear();
    mediafilter_value_1=0;
    accumulated_dist_1=0;
    mode_1 = 0;
    std_1 = 0;
    range_1 = 0;
    max_value_1 = 0;
    min_value_1 = 1000;
    percentil_25_1 = 0;
    percentil_75_1 = 0;

    distance_list_2.clear();
    mediafilter_value_2=0;
    accumulated_dist_2=0;
    mode_2 = 0;
    std_2 = 0;
    range_2 = 0;
    max_value_2 = 0;
    min_value_2 = 1000;
    percentil_25_2 = 0;
    percentil_75_2 = 0;

    distance_list_3.clear();
    mediafilter_value_3=0;
    accumulated_dist_3=0;
    mode_3 = 0;
    std_3 = 0;
    range_3 = 0;
    max_value_3 = 0;
    min_value_3 = 1000;
    percentil_25_3 = 0;
    percentil_75_3 = 0;

    distance_list_4.clear();
    mediafilter_value_4=0;
    accumulated_dist_4=0;
    mode_4 = 0;
    std_4 = 0;
    range_4 = 0;
    max_value_4 = 0;
    min_value_4 = 1000;
    percentil_25_4 = 0;
    percentil_75_4 = 0;

    position_list_x.clear();
    mean_total_x=0;
    accumulated_pos_x=0;
    mode_x = 0;
    std_x = 0;
    range_x = 0;
    max_x = -1000;
    min_x = 1000;
    percentil_25_x = 0;
    percentil_75_x = 0;

    position_list_y.clear();
    mean_total_y=0;
    accumulated_pos_y=0;
    mode_y = 0;
    std_y = 0;
    range_y = 0;
    max_y = -1000;
    min_y = 1000;
    percentil_25_y = 0;
    percentil_75_y = 0;

    position_list_z.clear();
    mean_total_z=0;
    accumulated_pos_z=0;
    mode_z = 0;
    std_z = 0;
    range_z = 0;
    max_z = -1000;
    min_z = 1000;
    percentil_25_z = 0;
    percentil_75_z = 0;
    position_data_samples=0;
    received_data_quantity_1=0;
    received_data_quantity_2=0;
    received_data_quantity_3=0;
    received_data_quantity_4=0;
}

//Statistics calculation
void MainWindow::distance_stats_calc()
{
    //MODE D1
    int maxCount = 0;
    for (int i = 0; i < distance_list_1.count(); i++) {
        int count = distance_list_1.count(distance_list_1[i]);
        if (count > maxCount) {
            maxCount = count;
            mode_1 = distance_list_1[i];
        }
    }

    //STD D1
    for (int i = 0; i < distance_list_1.count(); i++) {
        std_1 += pow(distance_list_1[i] - mediafilter_value_1, 2);
    }
    std_1 = sqrt(std_1 / (distance_list_1.count() - 1));

    //RANGE D1
    range_1 = max_value_1 - min_value_1;

    //PERCENTILE D1
    QList<double> ordered_d1_values = distance_list_1;
    std::sort(ordered_d1_values.begin(), ordered_d1_values.end());

    int index_25 = qCeil((0.25 * ordered_d1_values.count()) - 1);

    if (index_25 == (0.25 * ordered_d1_values.count()) - 1) {
        percentil_25_1 = (ordered_d1_values[index_25] + ordered_d1_values[index_25 + 1]) / 2.0;
    } else {
        percentil_25_1 = ordered_d1_values[index_25];
    }

    int index_75 = qCeil((0.75 * ordered_d1_values.count()) - 1);

    if (index_75 == (0.75 * ordered_d1_values.count()) - 1) {
        percentil_75_1 = (ordered_d1_values[index_75] + ordered_d1_values[index_75 + 1]) / 2.0;
    } else {
        percentil_75_1 = ordered_d1_values[index_75];
    }

    //Two distance values

    if(detected_devices!=1){

        //MODE D1 D2

        maxCount = 0;
        for (int i = 0; i < distance_list_2.count(); i++) {
            int count_2 = distance_list_2.count(distance_list_2[i]);
            if (count_2 > maxCount) {
                maxCount = count_2;
                mode_2 = distance_list_2[i];
            }
        }

        //STD D1 D2

        for (int i = 0; i < distance_list_2.count(); i++) {
            std_2 += pow(distance_list_2[i] - mediafilter_value_2, 2);
        }
        std_2 = sqrt(std_2 / (distance_list_2.count() - 1));

        //RANGE D1 D2

        range_2 = max_value_2 - min_value_2;

        //PERCENTILE D1 D2

        QList<double> ordered_d2_values = distance_list_2;
        std::sort(ordered_d2_values.begin(), ordered_d2_values.end());

        int index_25_2 = qCeil((0.25 * ordered_d2_values.count()) - 1);

        if (index_25_2 == (0.25 * ordered_d2_values.count()) - 1) {
            percentil_25_2 = (ordered_d2_values[index_25_2] + ordered_d2_values[index_25_2 + 1]) / 2.0;
        } else {
            percentil_25_2 = ordered_d2_values[index_25_2];
        }

        int index_75_2 = qCeil((0.75 * ordered_d2_values.count()) - 1);

        if (index_75_2 == (0.75 * ordered_d2_values.count()) - 1) {
            percentil_75_2 = (ordered_d2_values[index_75_2] + ordered_d2_values[index_75_2 + 1]) / 2.0;
        } else {
            percentil_75_2 = ordered_d2_values[index_75_2];
        }

        //Three distance measurement
        if(detected_devices!=2){

            //MODE D1 D2 D3
            maxCount = 0;
            for (int i = 0; i < distance_list_3.count(); i++) {
                int count_3 = distance_list_3.count(distance_list_3[i]);
                if (count_3 > maxCount) {
                    maxCount = count_3;
                    mode_3 = distance_list_3[i];
                }
            }

            //STD D1 D2 D3
            for (int i = 0; i < distance_list_3.count(); i++) {
                std_3 += pow(distance_list_3[i] - mediafilter_value_3, 2);
            }
            std_3 = sqrt(std_3 / (distance_list_3.count() - 1));

            //RANGE D1 D2 D3
            range_3 = max_value_3 - min_value_3;

            //PERCENTILE D1 D2 D3
            QList<double> ordered_d3_values = distance_list_3;
            std::sort(ordered_d3_values.begin(), ordered_d3_values.end());

            int index_35_3 = qCeil((0.25 * ordered_d3_values.count()) - 1);

            if (index_35_3 == (0.25 * ordered_d3_values.count()) - 1) {
                percentil_25_3 = (ordered_d3_values[index_35_3] + ordered_d3_values[index_35_3 + 1]) / 2.0;
            } else {
                percentil_25_3 = ordered_d3_values[index_35_3];
            }

            int index_75_3 = qCeil((0.75 * ordered_d3_values.count()) - 1);

            if (index_75_3 == (0.75 * ordered_d3_values.count()) - 1) {
                percentil_75_3 = (ordered_d3_values[index_75_3] + ordered_d3_values[index_75_3 + 1]) / 2.0;
            } else {
                percentil_75_3 = ordered_d3_values[index_75_3];
            }

            //Four distance measurement
            if(detected_devices==4){

                //MODE D1 D2 D3 D4
                maxCount = 0;
                for (int i = 0; i < distance_list_4.count(); i++) {
                    int count_4 = distance_list_4.count(distance_list_4[i]);
                    if (count_4 > maxCount) {
                        maxCount = count_4;
                        mode_4 = distance_list_4[i];
                    }
                }

                //STD D1 D2 D3 D4
                for (int i = 0; i < distance_list_4.count(); i++) {
                    std_4 += pow(distance_list_4[i] - mediafilter_value_4, 2);
                }
                std_4 = sqrt(std_4 / (distance_list_4.count() - 1));

                //RANGE D1 D2 D3 D4
                range_4 = max_value_4 - min_value_4;

                //PERCENTILE D1 D2 D3 D4
                QList<double> ordered_d4_values = distance_list_4;
                std::sort(ordered_d4_values.begin(), ordered_d4_values.end());

                int index_45_4 = qCeil((0.25 * ordered_d4_values.count()) - 1);

                if (index_45_4 == (0.25 * ordered_d4_values.count()) - 1) {
                    percentil_25_4 = (ordered_d4_values[index_45_4] + ordered_d4_values[index_45_4 + 1]) / 2.0;
                } else {
                    percentil_25_4 = ordered_d4_values[index_45_4];
                }

                int index_75_4 = qCeil((0.75 * ordered_d4_values.count()) - 1);

                if (index_75_4 == (0.75 * ordered_d4_values.count()) - 1) {
                    percentil_75_4 = (ordered_d4_values[index_75_4] + ordered_d4_values[index_75_4 + 1]) / 2.0;
                } else {
                    percentil_75_4 = ordered_d4_values[index_75_4];
                }
            }
        }
    }
}

//Statistics calculation (Position)
void MainWindow::position_stats_calc()
{
    //MODA
    int maxCount_x = 0;
    for (int i = 0; i < position_list_x.count(); i++) {
        int count = position_list_x.count(position_list_x[i]);
        if (count > maxCount_x) {
            maxCount_x = count;
            mode_x = position_list_x[i];
        }
    }
    int maxCount_y = 0;
    for (int i = 0; i < position_list_y.count(); i++) {
        int count = position_list_y.count(position_list_y[i]);
        if (count > maxCount_y) {
            maxCount_y = count;
            mode_y = position_list_y[i];
        }
    }
    int maxCount_z = 0;
    for (int i = 0; i < position_list_z.count(); i++) {
        int count = position_list_z.count(position_list_z[i]);
        if (count > maxCount_z) {
            maxCount_z = count;
            mode_z = position_list_z[i];
        }
    }

    //STD
    for (int i = 0; i < position_list_x.count(); i++) {
        std_x += pow(position_list_x[i] - mean_total_x, 2);
    }
    std_x = sqrt(std_x / (position_list_x.count() - 1));

    for (int i = 0; i < position_list_y.count(); i++) {
        std_y += pow(position_list_y[i] - mean_total_y, 2);
    }
    std_y = sqrt(std_y / (position_list_y.count() - 1));

    for (int i = 0; i < position_list_z.count(); i++) {
        std_z += pow(position_list_z[i] - mean_total_z, 2);
    }
    std_z = sqrt(std_z / (position_list_z.count() - 1));

    //RANGE
    range_x = max_x - min_x;
    range_y = max_y - min_y;
    range_z = max_z - min_z;

    //PERCENTILE
    //Percentile X
    QList<double> ordered_posx_values = position_list_x;
    std::sort(ordered_posx_values.begin(), ordered_posx_values.end());

    int index_25_x = qCeil((0.25 * ordered_posx_values.count()) - 1);

    if (index_25_x == (0.25 * ordered_posx_values.count()) - 1) {
        percentil_25_x = (ordered_posx_values[index_25_x] + ordered_posx_values[index_25_x + 1]) / 2.0;
    }
    else {
        percentil_25_x = ordered_posx_values[index_25_x];
    }

    int index_75_x = qCeil((0.75 * ordered_posx_values.count()) - 1);

    if (index_75_x == (0.75 * ordered_posx_values.count()) - 1) {
        percentil_75_x = (ordered_posx_values[index_75_x] + ordered_posx_values[index_75_x + 1]) / 2.0;
    }
    else{
        percentil_75_x = ordered_posx_values[index_75_x];
    }

    //Percentile Y
    QList<double> ordered_posy_values = position_list_y;
    std::sort(ordered_posy_values.begin(), ordered_posy_values.end());

    int index_25_y = qCeil((0.25 * ordered_posy_values.count()) - 1);

    if (index_25_y == (0.25 * ordered_posy_values.count()) - 1) {
        percentil_25_y = (ordered_posy_values[index_25_y] + ordered_posy_values[index_25_y + 1]) / 2.0;
    }
    else {
        percentil_25_y = ordered_posy_values[index_25_y];
    }

    int index_75_y = qCeil((0.75 * ordered_posy_values.count()) - 1);

    if (index_75_y == (0.75 * ordered_posy_values.count()) - 1) {
        percentil_75_y = (ordered_posy_values[index_75_y] + ordered_posy_values[index_75_y + 1]) / 2.0;
    }
    else{
        percentil_75_y = ordered_posy_values[index_75_y];
    }

    //Percentile Z
    QList<double> ordered_posz_values = position_list_z;
    std::sort(ordered_posz_values.begin(), ordered_posz_values.end());

    int index_25_z = qCeil((0.25 * ordered_posz_values.count()) - 1);

    if (index_25_z == (0.25 * ordered_posz_values.count()) - 1) {
        percentil_25_z = (ordered_posz_values[index_25_z] + ordered_posz_values[index_25_z + 1]) / 2.0;
    }
    else {
        percentil_25_z = ordered_posz_values[index_25_z];
    }

    int index_75_z = qCeil((0.75 * ordered_posz_values.count()) - 1);

    if (index_75_z == (0.75 * ordered_posz_values.count()) - 1) {
        percentil_75_z = (ordered_posz_values[index_75_z] + ordered_posz_values[index_75_z + 1]) / 2.0;
    }
    else{
        percentil_75_z = ordered_posz_values[index_75_z];
    }
}

/**************************************
    CSV DATA RECORDING FUNCTIONS
**************************************/

//Data recording for 1 position value
void MainWindow::save_position_data(QDateTime hora, double position_x, double media_x, double position_y, double media_y, double position_z, double media_z)
{
    position_samples++;
    QString date = hora.toString("dd/MM/yyyy h:mm:ss.zzz");

    QFile file(filename);

    QString pos_alarm;
    if(alarm_activated==0){
        pos_alarm = "0";
    }
    else{
        pos_alarm = "1";
    }
    if(operation_mode==1){
        if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            QTextStream stream(&file);
            stream << position_samples << "," << date << "," << position_x << "," << position_y << "," << position_z << "," << pos_alarm << "\n";
        }
        file.close();
    }
}

//Data recording for distances
void MainWindow::save_distance_data(QDateTime hora)
{
    distance_samples_ID4++;
    QString date = hora.toString("dd/MM/yyyy h:mm:ss.zzz");
    QFile file(filename);
    QString alarm_status;
    if(alarm_activated==0){
        alarm_status = "0";
    }
    else{
        alarm_status = "1";
    }

    if(operation_mode==0){
        if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            int id_labels[4]={id_label_1,id_label_2,id_label_3,id_label_4};
            QTextStream stream(&file);
            stream << distance_samples_ID4 << "," << date << ",";
            for(int i = 0; i < detected_devices; i++) {
                switch(id_labels[i]){
                case 1:
                    stream << initial_id_1 << "," << distance_value_1 << ",";
                    break;

                case 2:
                    stream << initial_id_2 << "," << distance_value_2 << ",";
                    break;

                case 3:
                    stream << initial_id_3 << "," << distance_value_3 << ",";
                    break;

                case 4:
                    stream << initial_id_4 << "," << distance_value_4 << ",";
                    break;
                }
            }
            stream << alarm_status << "\n";
        }
        file.close();
    }
}

//Saving statistics function
void MainWindow::save_stats_data(){
    QFile file_est(filename_est);

    if(file_est.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream(&file_est);

        if(operation_mode==0){

            stream << "Distance measurement statistics:" << "\n\n" << "ID 1: " << initial_id_1 << "\n";
            stream << "   Mean value:  " << mediafilter_value_1 << "\n" << "   Mode:  " << mode_1 << "\n" << "   Standard deviation:  " << std_1 << "\n" << "   Range:  " << range_1 << "\n";
            stream << "   Max value:  " << max_value_1 << "\n" << "   Min value:  " << min_value_1 << "\n" << "   Percentile 25%:  " << percentil_25_1 << "\n" << "   Percentile 75%:  " << percentil_75_1 << "\n";

            if(detected_devices>1){

                stream << "\n" << "ID 2: " << initial_id_2 << "\n";
                stream << "   Mean value:  " << mediafilter_value_2 << "\n" << "   Mode:  " << mode_2 << "\n" << "   Standard deviation:  " << std_2 << "\n" << "   Range:  " << range_2 << "\n";
                stream << "   Max value:  " << max_value_2<< "\n" << "   Min value:  " << min_value_2 << "\n" << "   Percentile 25%:  " << percentil_25_2 << "\n" << "   Percentile 75%:  " << percentil_75_2 << "\n";

                if(detected_devices>2){

                    stream << "\n" << "ID 3: " << initial_id_3 << "\n";
                    stream << "   Mean value:  " << mediafilter_value_3 << "\n" << "   Mode:  " << mode_3 << "\n" << "   Standard deviation:  " << std_3 << "\n" << "   Range:  " << range_3 << "\n";
                    stream << "   Max value:  " << max_value_3 << "\n" << "   Min value:  " << min_value_3 << "\n" << "   Percentile 25%:  " << percentil_25_3 << "\n" << "   Percentile 75%:  " << percentil_75_3 << "\n";

                    if(detected_devices>3){

                        stream << "\n" << "ID 4: " << initial_id_4 << "\n";
                        stream << "   Mean value:  " << mediafilter_value_4 << "\n" << "   Mode:  " << mode_4 << "\n" << "   Standard deviation:  " << std_4 << "\n" << "   Range:  " << range_4 << "\n";
                        stream << "   Max value:  " << max_value_4 << "\n" << "   Min value:  " << min_value_4 << "\n" << "   Percentile 25%:  " << percentil_25_4 << "\n" << "   Percentile 75%:  " << percentil_75_4 << "\n";
                    }
                }
            }
        }
        else if (operation_mode==1){
            stream << "Positioning statistics:" << "\n\n";
            stream << "X - Axis:\n" << "   Medium value: " << mean_total_x << "\n" << "   Standard deviation:  " << std_x << "\n" << "   Max value:  " << max_x << "\n" << "   Min value:  " << min_x << "   Range:  " << range_x;
            stream << "   Percentile 25%:  " << percentil_25_x << "   Percentile 75%:  " << percentil_75_x << "\n\n";
            stream << "Y - Axis:\n" << "   Medium value: " << mean_total_y << "\n" << "   Standard deviation:  " << std_y << "\n" << "   Max value:  " << max_y << "\n" << "   Min value:  " << min_y << "   Range:  " << range_y;
            stream << "   Percentile 25%:  " << percentil_25_y << "   Percentile 75%:  " << percentil_75_y << "\n\n";
            stream << "Z - Axis:\n" << "   Medium value: " << mean_total_z << "\n" << "   Standard deviation:  " << std_z << "\n" << "   Max value:  " << max_z << "\n" << "   Min value:  " << min_z << "   Range:  " << range_z;
            stream << "   Percentile 25%:  " << percentil_25_z << "   Percentile 75%:  " << percentil_75_z << "\n\n";
        }
    }
    file_est.close();
}

/**************************************
*        QCUSTOMPLOT FUNCTIONS        *
***************************************/

//Set up function for distance graph
void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
    // Antialiasing disabled
    customPlot->setNotAntialiasedElements(QCP::aeAll);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    customPlot->xAxis->setTickLabelFont(font);
    customPlot->yAxis->setTickLabelFont(font);
    customPlot->legend->setFont(font);

    //Graph style config

    //Graph style distance 1
    //QPen graphPen;
    graphPen.setColor(Qt::red);
    graphPen.setWidthF(graph0_size);

    //Graph style distance 2
    //QPen graphPen_3;

    graphPen_3.setColor(Qt::blue);
    graphPen_3.setWidthF(graph0_size);

    //Graph style distance 3
    //QPen graphPen_4;

    graphPen_4.setColor(Qt::green);
    graphPen_4.setWidthF(graph0_size);

    //Graph style distance 4
    //QPen graphPen_5;

    graphPen_5.setColor(Qt::magenta);
    graphPen_5.setWidthF(graph0_size);

    //Graph style distance 5
    //QPen graphPen_6;

    graphPen_6.setColor(Qt::cyan);
    graphPen_6.setWidthF(graph0_size);

    //Graph style media filter (only for 1 distance)
    //QPen graphPen_2;

    graphPen_2.setColor(Qt::black);
    graphPen_2.setWidthF(graph1_size);

    //Graph style Alarm
    //QPen graphPenmaxmin;
    graphPenmaxmin.setColor(Qt::gray);
    graphPenmaxmin.setWidthF(graph0_size);

    //Graph distance

    customPlot->addGraph();
    customPlot->graph(0)->setPen(graphPen);
    customPlot->graph(0)->setName("Distancia");

    //Graph media filter:

    customPlot->addGraph();
    customPlot->graph(1)->setPen(graphPen_2);
    customPlot->graph(1)->setName("Media");

    //Alarm graph:

    customPlot->addGraph();
    customPlot->graph(2)->setPen(graphPenmaxmin);
    customPlot->graph(2)->setName("Máx y mín");

    customPlot->addGraph();
    customPlot->graph(3)->setPen(graphPenmaxmin);

    //Graph distance 2:

    customPlot->addGraph();
    customPlot->graph(4)->setPen(graphPen_3);
    customPlot->graph(4)->setName("Distancia 2");

    //Graph distance 3:

    customPlot->addGraph();
    customPlot->graph(5)->setPen(graphPen_4);
    customPlot->graph(5)->setName("Distancia 3");

    //Graph distance 4:

    customPlot->addGraph();
    customPlot->graph(6)->setPen(graphPen_5);
    customPlot->graph(6)->setName("Distancia 4");

    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);

    timeTicker->setDateTimeFormat("hh:mm:ss");
    timeTicker->setTickCount(2);
    timeTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssReadability);

    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(range_dist_y_min, range_dist_y_max);

    //When upper and right axis change, lower and left axis do the same
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // Setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
}

//Set up function for position graph
void MainWindow::setupRealtimeDataDemo3(QCustomPlot *position_graph)
{
    //Axis labels:
    position_graph->xAxis->setLabel(tr("Eje x"));
    position_graph->yAxis->setLabel(tr("Eje y"));

    //Antialiasing disabled

    //Graph style config:

    QPen graphPen_anchors;
    graphPen_anchors.setColor(Qt::blue);
    graphPen_anchors.setWidthF(4);

    QPen graphPen_tag;

    graphPen_tag.setColor(Qt::red);
    graphPen_tag.setWidthF(3);

    QPen graphPen_alarm;

    graphPen_alarm.setColor(Qt::gray);
    graphPen_alarm.setWidthF(2);

    //Anchor element graph settings:
    position_graph->addGraph();
    position_graph->graph(0)->setPen(graphPen_anchors);
    position_graph->graph(0)->setName("anchors");
    position_graph->graph(0)->setLineStyle(QCPGraph::lsNone);
    position_graph->graph(0)->setScatterStyle(QCPScatterStyle::ssCrossSquare);

    //TAG element graph settings:
    position_graph->addGraph();
    position_graph->graph(1)->setPen(graphPen_tag);
    position_graph->graph(1)->setName("TAG");
    position_graph->graph(1)->setLineStyle(QCPGraph::lsNone);
    position_graph->graph(1)->setScatterStyle(QCPScatterStyle::ssCircle);

    //Alarm graph:
    position_graph->addGraph();
    position_graph->graph(2)->setPen(graphPen_alarm);
    position_graph->graph(2)->setName("Alarm");

    position_graph->yAxis->setRange(range_pos_y_min, range_pos_y_max);
    position_graph->xAxis->setRange(range_pos_x_min, range_pos_x_max);

    //When upper and right axis change, lower and left axis do the same
    connect(position_graph->xAxis, SIGNAL(rangeChanged(QCPRange)), position_graph->xAxis2, SLOT(setRange(QCPRange)));
    connect(position_graph->yAxis, SIGNAL(rangeChanged(QCPRange)), position_graph->yAxis2, SLOT(setRange(QCPRange)));
}

//Plot function
void MainWindow::realtimeDataSlot()
{
    if(operation_mode==0){
        double key;
        QDateTime current_time = QDateTime::currentDateTime();
        key = current_time.toMSecsSinceEpoch() / 1000.0;

        static double lastPointKey = 0;
        if(true)
        {
            if (ui->plot_distance->isChecked()==true){

                int graph_index_1=0;
                double id_value_1;
                double id_media_1;
                int graph_index_2=0;
                double id_value_2;
                double id_media_2;
                int graph_index_3=0;
                double id_value_3;
                double id_media_3;
                int graph_index_4=0;
                double id_value_4;
                double id_media_4;

                if(detected_devices>0){

                    if(id_1==initial_id_1){
                        id_value_1=distance_value_1;
                        graph_index_1=0;
                        id_media_1=bufer_media;
                    }
                    else if(id_1==initial_id_2){
                        id_value_1=distance_value_2;
                        graph_index_1=4;
                        id_media_1=bufer_media_2;
                    }
                    else if(id_1==initial_id_3){
                        id_value_1=distance_value_3;
                        graph_index_1=5;
                        id_media_1=bufer_media_3;
                    }
                    else{
                        id_value_1=distance_value_4;
                        graph_index_1=6;
                        id_media_1=bufer_media_4;
                    }
                    if(detected_devices>1){

                        if(id_2==initial_id_1){
                            id_value_2=distance_value_1;
                            graph_index_2=0;
                            id_media_2=bufer_media;
                        }
                        else if(id_2==initial_id_2){
                            id_value_2=distance_value_2;
                            graph_index_2=4;
                            id_media_2=bufer_media_2;
                        }
                        else if(id_2==initial_id_3){
                            id_value_2=distance_value_3;
                            graph_index_2=5;
                            id_media_2=bufer_media_3;
                        }
                        else{
                            id_value_2=distance_value_4;
                            graph_index_2=6;
                            id_media_2=bufer_media_4;
                        }
                        if(detected_devices>2){

                            if(id_3==initial_id_1){
                                id_value_3=distance_value_1;
                                graph_index_3=0;
                                id_media_3=bufer_media;
                            }
                            else if(id_3==initial_id_2){
                                id_value_3=distance_value_2;
                                graph_index_3=4;
                                id_media_3=bufer_media_2;
                            }
                            else if(id_3==initial_id_3){
                                id_value_3=distance_value_3;
                                graph_index_3=5;
                                id_media_3=bufer_media_3;
                            }
                            else{
                                id_value_3=distance_value_4;
                                graph_index_3=6;
                                id_media_3=bufer_media_4;
                            }
                            if(detected_devices>3){

                                if(id_4==initial_id_1){
                                    id_value_4=distance_value_1;
                                    graph_index_4=0;
                                    id_media_4=bufer_media;
                                }
                                else if(id_4==initial_id_2){
                                    id_value_4=distance_value_2;
                                    graph_index_4=4;
                                    id_media_4=bufer_media_2;
                                }
                                else if(id_4==initial_id_3){
                                    id_value_4=distance_value_3;
                                    graph_index_4=5;
                                    id_media_4=bufer_media_3;
                                }
                                else{
                                    id_value_4=distance_value_4;
                                    graph_index_4=6;
                                    id_media_4=bufer_media_4;
                                }
                            }
                        }
                    }
                }

                if(multiple_measures==0){
                    ui->customPlot->graph(graph_index_1)->addData(key, id_value_1);
                    if (mean_enable==1){
                        ui->customPlot->graph(graph_index_1)->addData(key, id_media_1);
                    }
                }
                else if(multiple_measures==1){
                    /*if(fallo_mensaje==1){
                        fallo_mensaje=0;
                        return;
                    }*/
                    if(detected_devices==2){

                        if(mean_enable==1){
                            ui->customPlot->graph(graph_index_1)->addData(key, id_media_1);
                            ui->customPlot->graph(graph_index_2)->addData(key, id_media_2);
                        }
                        else{
                            ui->customPlot->graph(graph_index_1)->addData(key, id_value_1);
                            ui->customPlot->graph(graph_index_2)->addData(key, id_value_2);
                        }
                    }

                    else if(detected_devices==3){
                        if(mean_enable==1){
                            ui->customPlot->graph(graph_index_1)->addData(key, id_media_1);
                            ui->customPlot->graph(graph_index_2)->addData(key, id_media_2);
                            ui->customPlot->graph(graph_index_3)->addData(key, id_media_3);
                        }
                        else{
                            ui->customPlot->graph(graph_index_1)->addData(key, id_value_1);
                            ui->customPlot->graph(graph_index_2)->addData(key, id_value_2);
                            ui->customPlot->graph(graph_index_3)->addData(key, id_value_3);
                        }
                    }
                    else if(detected_devices==4){
                        if(mean_enable==1){
                            ui->customPlot->graph(graph_index_1)->addData(key, id_media_1);
                            ui->customPlot->graph(graph_index_2)->addData(key, id_media_2);
                            ui->customPlot->graph(graph_index_3)->addData(key, id_media_3);
                            ui->customPlot->graph(graph_index_4)->addData(key, id_media_4);
                        }
                        else{
                            ui->customPlot->graph(graph_index_1)->addData(key, id_value_1);
                            ui->customPlot->graph(graph_index_2)->addData(key, id_value_2);
                            ui->customPlot->graph(graph_index_3)->addData(key, id_value_3);
                            ui->customPlot->graph(graph_index_4)->addData(key, id_value_4);
                        }
                    }
                }
                lastPointKey = key;
            }
        }
        // make key axis range scroll with the data (at a constant range size):
        ui->customPlot->graph(0)->data()->removeBefore(key-32);
        ui->customPlot->graph(1)->data()->removeBefore(key-32);
        if (multiple_measures==1){
            ui->customPlot->graph(2)->data()->removeBefore(key-32);
            ui->customPlot->graph(3)->data()->removeBefore(key-32);
            ui->customPlot->graph(4)->data()->removeBefore(key-32);
            ui->customPlot->graph(5)->data()->removeBefore(key-32);
            ui->customPlot->graph(6)->data()->removeBefore(key-32);
        }
        ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
        ui->customPlot->replot();

        //If check alarm is requested
        if (distance_alarm_plot==1){

            //Max min plot reset
            ui->customPlot->graph(2)->data()->removeBefore(key+1);
            ui->customPlot->graph(3)->data()->removeBefore(key+1);

            //Plot max y min lines
            ui->customPlot->graph(2)->addData(key-8, max_value_alarm);
            ui->customPlot->graph(2)->addData(key+1, max_value_alarm);

            ui->customPlot->graph(3)->addData(key-8, min_value_alarm);
            ui->customPlot->graph(3)->addData(key+1, min_value_alarm);
        }
    }
    else{
        ui->position_graph->graph(1)->data()->clear();
        if(mean_enable==1){
            ui->position_graph->graph(1)->addData(bufer_media_x, bufer_media_y);
        }
        else{
            ui->position_graph->graph(1)->addData(position_x, position_y);
        }
        ui->position_graph->replot();
    }
}

//Asign index to color when requested
QColor MainWindow::obtain_color(float indice)
{
    int indice_int = static_cast<int>(indice);
    QColor chosen_color;
    switch(indice_int){
    case 0:
        chosen_color=Qt::red;
        break;
    case 1:
        chosen_color=Qt::blue;
        break;
    case 2:
        chosen_color=Qt::darkGreen;
        break;
    case 3:
        chosen_color=Qt::gray;
        break;
    case 4:
        chosen_color=Qt::magenta;
        break;
    case 5:
        chosen_color=Qt::cyan;
        break;
    case 6:
        chosen_color=Qt::black;
        break;
    case 7:
        chosen_color=Qt::darkYellow;
        break;
    }
    return chosen_color;
}

/**************************************
*           ALARM FUNCTIONS           *
***************************************/

//Function to check if received values activate alarm (up to 4 values)

void MainWindow::check_distance_alarm()
{
    int id_labels[4]={id_label_1,id_label_2,id_label_3,id_label_4};
    bool not_allowed_value = 0;
    for(int i = 0; i < detected_devices; i++) {
        double chosen_value;
        switch(id_labels[i]){
        case 1:
            if(mean_value_alarm_link){
                chosen_value = bufer_media;
            }
            else{
                chosen_value = distance_value_1;
            }

            if(chosen_value>max_value_alarm or chosen_value<min_value_alarm){
                not_allowed_value=1;
            }
            break;

        case 2:
            if(mean_value_alarm_link){
                chosen_value = bufer_media_2;
            }
            else{
                chosen_value = distance_value_2;
            }

            if(chosen_value>max_value_alarm or chosen_value<min_value_alarm){
                not_allowed_value=1;
            }
            break;
        case 3:
            if(mean_value_alarm_link){
                chosen_value = bufer_media_3;
            }
            else{
                chosen_value = distance_value_3;
            }

            if(chosen_value>max_value_alarm or chosen_value<min_value_alarm){
                not_allowed_value=1;
            }
            break;
        case 4:
            if(mean_value_alarm_link){
                chosen_value = bufer_media_4;
            }
            else{
                chosen_value = distance_value_4;
            }

            if(chosen_value>max_value_alarm or chosen_value<min_value_alarm){
                not_allowed_value=1;
            }
            break;
        }
    }
    if(not_allowed_value==1){
        if (alarm_activation==0){
            timer_alarmdelay.start(alarm_delay);
            alarm_activation=1;
        }
    }
    else{
        if (alarm_activation==1){
            timer_alarmdelay.stop();
            alarm_activation=0;
            ui->label_alarm_dist->setText("");
            ui->label_alarm_dist->setFrameStyle(QFrame::NoFrame);
        }
    }
}

//Function to check if received position value activate alarm
void MainWindow::check_position_alarm()
{
    bool alarm_act=0;

    if(pos_x_alarm_ena==1){
        if ((position_x > pos_alarm_x_max) or (position_x < pos_alarm_x_min)){
            alarm_act=1;
        }
    }
    if(pos_y_alarm_ena==1){
        if ((position_y > pos_alarm_y_max) or (position_y < pos_alarm_y_min)){
            alarm_act=1;
        }
    }
    if(pos_z_alarm_ena==1){
        if ((position_z > pos_alarm_z_max) or (position_z < pos_alarm_z_min)){
            alarm_act=1;
        }
    }

    if(alarm_act==1){
        if (alarm_activation==0){
            timer_alarmdelay.start(pos_alarm_delay);
            alarm_activation=1;
            return;
        }
        else{
            return;
        }
    }
    else{
        if (alarm_activation==1){
            timer_alarmdelay.stop();
            alarm_activation=0;
            ui->label_alarm_pos->setText("");
            ui->label_alarm_pos->setFrameStyle(QFrame::NoFrame);
        }
        return;
    }
}

//Function to plot alarm in position mode
void MainWindow::draw_alarm_pos()
{
    if(pos_x_alarm_ena==1 and pos_y_alarm_ena==1){
        ui->position_graph->clearItems();
        QCPItemRect *rectangle = new QCPItemRect(ui->position_graph);
        rectangle->setLayer("background");
        rectangle->topLeft->setCoords(pos_alarm_x_min,pos_alarm_y_max);
        rectangle->bottomRight->setCoords(pos_alarm_x_max,pos_alarm_y_min);
        rectangle->setBrush(QBrush(QColor("floralwhite")));
        rectangle->setClipAxisRect(ui->position_graph->axisRect(2));
        ui->position_graph->replot();
    }
    else if(pos_x_alarm_ena==1 and pos_y_alarm_ena==0){

        ui->position_graph->clearItems();
        QCPItemLine *line_1 = new QCPItemLine(ui->position_graph);
        line_1->start->setCoords(pos_alarm_x_max, range_pos_y_max+2);
        line_1->end->setCoords(pos_alarm_x_max, range_pos_y_min-2);

        QCPItemLine *line_2 = new QCPItemLine(ui->position_graph);
        line_2->start->setCoords(pos_alarm_x_min, range_pos_y_max+2);
        line_2->end->setCoords(pos_alarm_x_min, range_pos_y_min-2);
        ui->position_graph->replot();
    }
    else if(pos_x_alarm_ena==0 and pos_y_alarm_ena==1){

        ui->position_graph->clearItems();
        QCPItemLine *line_1 = new QCPItemLine(ui->position_graph);
        line_1->start->setCoords(range_pos_x_min-1, pos_alarm_y_min);
        line_1->end->setCoords(range_pos_x_max+1, pos_alarm_y_min);

        QCPItemLine *line_2 = new QCPItemLine(ui->position_graph);
        line_2->start->setCoords(range_pos_x_min-1, pos_alarm_y_max);
        line_2->end->setCoords(range_pos_x_max+1, pos_alarm_y_max);
        ui->position_graph->replot();
    }
    ui->textEdit->append(tr("\r\n\r\nZona segura establecida en el gráfico"));
}

//Alarm activation function - GPIO activation
void MainWindow::alarm_graph()
{

    previous_alarm = chosen_alarm;

    if(operation_mode==0){
        ui->label_alarm_dist->setText(tr("<b><FONT COLOR='green' FONT SIZE = 4>Alarma activada</b></font>"));
        ui->label_alarm_dist->setFrameStyle(QFrame::Box | QFrame::Raised);
    }
    else if (passive_positioning==1 or tag_positioning==1){
        ui->label_alarm_pos->setFrameStyle(QFrame::Box | QFrame::Raised);
        ui->label_alarm_pos->setText(tr("<b><FONT COLOR='green' FONT SIZE = 4>Alarma activada</b></font>"));
    }
    else{

    }

    switch (chosen_alarm){
    case 0:
        send_message("gs 8\r");
        break;
    case 1:
        send_message("gs 9\r");
        break;
    case 2:
        send_message("gs 10\r");
        break;
    case 3:
        send_message("gs 12\r");
        break;
    case 4:
        send_message("gs 13\r");
        break;
    case 5:
        send_message("gs 14\r");
        break;
    case 6:
        send_message("gs 15\r");
        break;
    case 7:
        send_message("gs 23\r");
        break;
    case 8:
        send_message("gs 27\r");
        break;
    }

    if(distance_alarm_plot==1){
        if(alarm_duration==0){
            alarm_activated=1;
            timer_alarmdelay.stop();
            p_serie->waitForReadyRead(10);
            p_serie->waitForBytesWritten(10);
            return;
        }
    }
    else if(position_alarm_pos==1){
        if(pos_alarm_duration==0){
            alarm_activated=1;
            timer_alarmdelay.stop();
            p_serie->waitForReadyRead(10);
            p_serie->waitForBytesWritten(10);
            return;
        }
    }
    alarm_activated=1;
    timer_alarmdelay.stop();

    if(distance_alarm_plot==1){
        timer_alarmdelay.start(alarm_duration);
    }
    else if(position_alarm_pos==1){
        timer_alarmdelay.start(pos_alarm_duration);
    }
    p_serie->waitForReadyRead(10);
    p_serie->waitForBytesWritten(10);
}

//When alarm timer ends, deactivates alarm
void MainWindow::alarm_duration_f()
{
    timer_alarmdelay.stop();
    clear_gpio();
    alarm_activated=0;
    if(operation_mode==0){
        ui->label_alarm_dist->setFrameStyle(QFrame::NoFrame);
        ui->label_alarm_dist->setText("");
    }
    else if (passive_positioning==1 or tag_positioning==1){
        ui->label_alarm_pos->setFrameStyle(QFrame::NoFrame);
        ui->label_alarm_pos->setText("");
    }
    else{
        ui->label_alarm_dist->setFrameStyle(QFrame::NoFrame);
        ui->label_alarm_dist->setText("");
        ui->label_alarm_pos->setFrameStyle(QFrame::NoFrame);
        ui->label_alarm_pos->setText("");
    }
}

//Deactivates GPIO
void MainWindow::clear_gpio(){
    deactivate_GPIO=1;
    timer_gpioclear.start(350);
    switch (previous_alarm)
    {
    case 0:
        send_message("gc 8\r");
        p_serie->waitForReadyRead(50);
        p_serie->waitForBytesWritten(50);
        if(distance_alarm_plot==1 or position_alarm_pos==1){
            ui->comboBox_GPIO8->setEnabled(false);
            ui->pushButton_GPIO8->setEnabled(false);
        }
        else{
            ui->comboBox_GPIO8->setEnabled(true);
            ui->pushButton_GPIO8->setEnabled(true);
        }
        break;

    case 1:
        send_message("gc 9\r");
        p_serie->waitForReadyRead(50);
        p_serie->waitForBytesWritten(50);
        if(distance_alarm_plot==1 or position_alarm_pos==1){
            ui->comboBox_GPIO9->setEnabled(false);
            ui->pushButton_GPIO9->setEnabled(false);
        }
        else{
            ui->comboBox_GPIO9->setEnabled(true);
            ui->pushButton_GPIO9->setEnabled(true);
        }
        break;

    case 2:
        send_message("gc 10\r");
        p_serie->waitForReadyRead(50);
        p_serie->waitForBytesWritten(50);
        if(distance_alarm_plot==1 or position_alarm_pos==1){
            ui->comboBox_GPIO10->setEnabled(false);
            ui->pushButton_GPIO10->setEnabled(false);
        }
        else{
            ui->comboBox_GPIO10->setEnabled(true);
            ui->pushButton_GPIO10->setEnabled(true);
        }
        break;

    case 3:
        send_message("gc 12\r");
        p_serie->waitForBytesWritten(50);
        if(distance_alarm_plot==1 or position_alarm_pos==1){
            ui->comboBox_GPIO12->setEnabled(false);
            ui->pushButton_GPIO12->setEnabled(false);
        }
        else{
            ui->comboBox_GPIO12->setEnabled(true);
            ui->pushButton_GPIO12->setEnabled(true);
        }
        break;

    case 4:
        send_message("gc 13\r");
        p_serie->waitForBytesWritten(50);
        break;
    case 5:
        send_message("gc 14\r");
        p_serie->waitForBytesWritten(50);
        break;
    case 6:
        send_message("gc 15\r");
        p_serie->waitForBytesWritten(50);
        break;
    case 7:
        send_message("gc 23\r");
        p_serie->waitForBytesWritten(50);
        break;
    case 8:
        send_message("gc 27\r");
        p_serie->waitForBytesWritten(50);
        break;
    }
}

void MainWindow::gpio_clear_wait()
{
    deactivate_GPIO=0;
    timer_gpioclear.stop();
}

//Alarm activation request (Distance)
void MainWindow::dist_alarm_activation_request()
{
    if(distance_alarm_plot==1){
        previous_alarm = chosen_alarm;
        clear_gpio();
    }
    else{
        clear_gpio();
        ui->customPlot->graph(2)->data()->clear();
        ui->customPlot->graph(3)->data()->clear();
        ui->customPlot->replot();
        alarm_activated=0;

        switch (previous_alarm)
        {
        case 0:
            if(ui->comboBox_GPIO8->currentIndex()==0){
                ui->pushButton_GPIO8->setEnabled(false);
            }
            break;

        case 1:
            if(ui->comboBox_GPIO9->currentIndex()==0){
                ui->pushButton_GPIO9->setEnabled(false);
            }
            break;

        case 2:
            if(ui->comboBox_GPIO10->currentIndex()==0){
                ui->pushButton_GPIO10->setEnabled(false);
            }
            break;
        case 3:
            if(ui->comboBox_GPIO12->currentIndex()==0){
                ui->pushButton_GPIO12->setEnabled(false);
            }
            break;
        }
    }
}

//Alarm activation request (Position)
void MainWindow::pos_alarm_activation_request()
{
    if(position_alarm_pos==1){
        previous_alarm = chosen_alarm;
        clear_gpio();
    }
    else{
        clear_gpio();
        alarm_activated=0;

        switch (previous_alarm)
        {
        case 0:
            if(ui->comboBox_GPIO8->currentIndex()==0){
                ui->pushButton_GPIO8->setEnabled(false);
            }
            break;

        case 1:
            if(ui->comboBox_GPIO9->currentIndex()==0){
                ui->pushButton_GPIO9->setEnabled(false);
            }
            break;

        case 2:
            if(ui->comboBox_GPIO10->currentIndex()==0){
                ui->pushButton_GPIO10->setEnabled(false);
            }
            break;
        case 3:
            if(ui->comboBox_GPIO12->currentIndex()==0){
                ui->pushButton_GPIO12->setEnabled(false);
            }
            break;
        }
    }
}

/***************************************
 *   DEVICE CONFIGURATION SETTINGS     *
 ***************************************/

//Initial GPIO config
void MainWindow::gpio_config()
{
    p_serie->waitForReadyRead(100);
    if (gpio_init_config==1){
        ui->tabWidget->setEnabled(false);

        send_message("gs 8\r");
        p_serie->waitForReadyRead(50);
        p_serie->waitForBytesWritten(50);

        send_message("gt 8\r");
        p_serie->waitForReadyRead(50);
        p_serie->waitForBytesWritten(50);

        send_message("gs 9\r");
        p_serie->waitForReadyRead(50);
        p_serie->waitForBytesWritten(50);

        send_message("gt 9\r");
        p_serie->waitForReadyRead(50);
        p_serie->waitForBytesWritten(50);

        send_message("gc 10\r");
        p_serie->waitForReadyRead(50);
        p_serie->waitForBytesWritten(50);

        send_message("gc 12\r");
        p_serie->waitForReadyRead(50);
        p_serie->waitForBytesWritten(50);

        gpio_init_config=0;
        timer_gpioconfig.stop();

        ui->tabWidget->setEnabled(true);
        ui->textEdit->append(tr("\r\n-------------\r\nCONFIGURACIÓN FINALIZADA"));

        ui->actionReset->setEnabled(true);
        ui->actionDataSavingSettings->setEnabled(true);
        ui->actionDistanceGraphSettings->setEnabled(true);
        ui->actionDeviceSettings->setEnabled(true);
        ui->actionFactory_Reset->setEnabled(true);

        receive_initial_info=1;
        send_message("si\r");
        timer_infofailure.start(2500);
        ui->textEdit->append(tr("\r\n\r\nGPIO configurados."));
        ui->pushButton_disconnect->setEnabled(true);
    }
}

//Fixed anchors plotting
void MainWindow::pos_anchors()
{
    ui->position_graph->graph(0)->data()->clear();
    ui->position_graph->graph(0)->addData(anchor_1_x, anchor_1_y);
    ui->position_graph->graph(0)->addData(anchor_2_x, anchor_2_y);
    ui->position_graph->graph(0)->addData(anchor_3_x, anchor_3_y);
    ui->position_graph->graph(0)->addData(anchor_4_x, anchor_4_y);
    ui->position_graph->yAxis->setRange(range_pos_y_min, range_pos_y_max);
    ui->position_graph->xAxis->setRange(range_pos_x_min, range_pos_x_max);
    ui->position_graph->replot();
}

//Set device mode as passive anchor
void MainWindow::passive_config()
{
    ui->plot_distance->setEnabled(false);
    send_message("nmp\r");
    ui->textEdit->setText(tr("Módulo configurado como ANCHOR PASIVO"));
    ui->tab_4->setEnabled(true);
    ui->tab->setEnabled(false);
    ui->label_funcmode->setText(tr("<b><FONT COLOR='blue' FONT SIZE = 4>Ancla Pasiva</b></font>"));
    passive_positioning=1;

    p_serie->flush();
    p_serie->waitForReadyRead(200);
    p_serie->waitForBytesWritten(200);
    timer_opmodechange.start(1500);
}

//Set Network ID function
void MainWindow::network_config(QString red){

    QString network_message="nis ";
    network_message.append(red);
    network_message.append("\r");
    send_message(network_message);
}

//Device mode configuration
void MainWindow::device_config(int modo)
{
    if(modo==1){
        ui->plot_distance->setEnabled(true);
        send_message("nma\r");
        ui->textEdit->setText(tr("Módulo configurado como ANCHOR"));
        ui->tab_4->setEnabled(true);
        ui->tab->setEnabled(false);
        ui->label_funcmode->setText(tr("<b><FONT COLOR='blue' FONT SIZE = 4>Ancla</b></font>"));
        if(operation_mode==1){
            ui->plot_distance->setEnabled(true);
        }
        else{
            ui->plot_distance->setEnabled(false);
        }
        passive_positioning=0;
        tag_positioning=0;
    }
    else if(modo==2){
        ui->plot_distance->setEnabled(false);
        send_message("nmi\r");
        ui->textEdit->setText(tr("Módulo configurado como INITIATOR ANCHOR"));
        ui->label_funcmode->setText(tr("<b><FONT COLOR='red' FONT SIZE = 4>Ancla Iniciadora</b></font>"));
        ui->tab_4->setEnabled(false);
        ui->tab->setEnabled(false);
        passive_positioning=0;
        tag_positioning=0;
    }
    else{
        send_message("nmt\r");
        ui->tab_4->setEnabled(true);
        ui->tab->setEnabled(true);
        ui->textEdit->setText(tr("Módulo configurado como TAG"));
        ui->label_funcmode->setText(tr("<b><FONT COLOR='green' FONT SIZE = 4>TAG</b></font>"));
        passive_positioning=0;
        tag_positioning=1;
        if(operation_mode==0){
            ui->plot_distance->setEnabled(true);
        }
        else{
            ui->plot_distance->setEnabled(false);
        }
    }
    p_serie->flush();
    p_serie->waitForReadyRead(200);
    p_serie->waitForBytesWritten(200);
    timer_opmodechange.start(1500);
}

//GPIO set to 0 when timer ends
void MainWindow::timer_gpio8()
{
    send_message("gt 8\r");
    p_serie->waitForReadyRead(10);
    p_serie->waitForBytesWritten(10);
    tem_gpio8.stop();
    ui->pushButton_GPIO8->setText(tr("Activar"));
    ui->pushButton_GPIO8->setEnabled(true);
}

//GPIO set to 0 when timer ends
void MainWindow::timer_gpio9()
{
    send_message("gt 9\r");
    p_serie->waitForReadyRead(10);
    p_serie->waitForBytesWritten(10);
    tem_gpio8.stop();
    ui->pushButton_GPIO9->setText(tr("Activar"));
    ui->pushButton_GPIO9->setEnabled(true);
}

//GPIO set to 0 when timer ends
void MainWindow::timer_gpio10()
{
    send_message("gt 10\r");
    p_serie->waitForReadyRead(10);
    p_serie->waitForBytesWritten(10);
    tem_gpio8.stop();
    ui->pushButton_GPIO10->setText(tr("Activar"));
    ui->pushButton_GPIO10->setEnabled(true);
}

//GPIO set to 0 when timer ends
void MainWindow::timer_gpio12()
{
    send_message("gt 12\r");
    p_serie->waitForReadyRead(10);
    p_serie->waitForBytesWritten(10);
    tem_gpio8.stop();
    ui->pushButton_GPIO12->setText(tr("Activar"));
    ui->pushButton_GPIO12->setEnabled(true);
}

/*************************************
*             TRANSLATE MENU         *
**************************************/

//Creation of the Language tab entries dynamically, dependent on the existing translations.
void MainWindow::createLanguageMenu(void) {
    QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
    langGroup->setExclusive(true);

    connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

    //Format systems language
    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

    m_langPath = QApplication::applicationDirPath();
    //qDebug() << m_langPath;
    m_langPath.append("/languages");
    QDir dir(m_langPath);
    QStringList fileNames = dir.entryList(QStringList("UCO_DWM1001_DEV_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i) {
        QString locale;
        locale = fileNames[i];
        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.lastIndexOf('_') + 1);

        QString lang = QLocale::languageToString(QLocale(locale).language());
        QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

        QAction *action = new QAction(ico, lang, this);
        action->setCheckable(true);
        action->setData(locale);

        ui->menuLanguage->addAction(action);
        langGroup->addAction(action);

        //Set default translators and language checked
        if (defaultLocale == locale) {
            action->setChecked(true);
        }
    }
}

//Function called every time, when a menu entry of the language menu is called
void MainWindow::slotLanguageChanged(QAction* action)
{
    if(0 != action) {
        loadLanguage(action->data().toString());
    }
}

void switchTranslator(QTranslator& translator, const QString& filename) {
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    QString path = QApplication::applicationDirPath();
    path.append("/languages/");
    if(translator.load(path + filename)) //Here Path and Filename has to be entered because the system didn't find the QM Files else
        qApp->installTranslator(&translator);
}

//Get translate file and load it
void MainWindow::loadLanguage(const QString& rLanguage) {
    if(m_currLang != rLanguage) {
        m_currLang = rLanguage;
        QLocale locale = QLocale(m_currLang);
        QLocale::setDefault(locale);
        //QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(m_translator, QString("UCO_DWM1001_DEV_%1.qm").arg(rLanguage));
        switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
        //ui->statusBar->showMessage(tr("Current Language changed to %1").arg(languageName));
        ui->label_alarm_dist->setText("");
        ui->label_alarm_pos->setText("");
        ui->label_alarm_dist->setFrameStyle(QFrame::NoFrame);
        ui->label_alarm_pos->setFrameStyle(QFrame::NoFrame);
    }
}

//Asign 2 language event changes to translation functions
void MainWindow::changeEvent(QEvent* event) {
    if(0 != event) {
        switch(event->type()) {
        //If a translator is loaded
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        //In case the system language changes:
        case QEvent::LocaleChange:
        {
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            loadLanguage(locale);
        }
        break;
        }
    }
    QMainWindow::changeEvent(event);
    ui->label_alarm_dist->setText("");
    ui->label_alarm_pos->setText("");
    ui->label_alarm_dist->setFrameStyle(QFrame::NoFrame);
    ui->label_alarm_pos->setFrameStyle(QFrame::NoFrame);
}

