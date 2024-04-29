/***********************************************************************************
**                                                                                **
** UCO DWM-GUI, a software for the control of the DWM1001-DEV board               **
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
**             Date: 29.04.24                                                     **
**          Version: 1.0.0                                                        **
************************************************************************************/

#include "dialog_alarm_pos.h"
#include "ui_dialog_alarm_pos.h"

Dialog_alarm_pos::Dialog_alarm_pos(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_alarm_pos)
{
    ui->setupUi(this);
}

Dialog_alarm_pos::~Dialog_alarm_pos()
{
    delete ui;
}

void Dialog_alarm_pos::load_data_alarm(QList<float> received_alarm_data){

    //Eje X
    ui->doubleSpinBox->setValue(received_alarm_data[0]);
    ui->doubleSpinBox_2->setValue(received_alarm_data[1]);
    ui->checkBox_2->setChecked(received_alarm_data[10]);

    //Eje Y
    ui->doubleSpinBox_5->setValue(received_alarm_data[2]);
    ui->doubleSpinBox_6->setValue(received_alarm_data[3]);
    ui->checkBox_3->setChecked(received_alarm_data[11]);

    //Eje Z
    ui->doubleSpinBox_7->setValue(received_alarm_data[4]);
    ui->doubleSpinBox_8->setValue(received_alarm_data[5]);
    ui->checkBox_4->setChecked(received_alarm_data[12]);

    //GPIO election
    ui->comboBox->setCurrentIndex(received_alarm_data[6]);

    //Temporizadores
    ui->doubleSpinBox_4->setValue(received_alarm_data[7]);
    ui->doubleSpinBox_3->setValue(received_alarm_data[8]);

    if(received_alarm_data[9]==1){
        ui->checkBox->setChecked(true);
    }
}

QList<float> Dialog_alarm_pos::alarm_data(){

    QList<float> alarm_pos_data;
    alarm_pos_data.append(ui->doubleSpinBox->value());
    alarm_pos_data.append(ui->doubleSpinBox_2->value());
    alarm_pos_data.append(ui->doubleSpinBox_5->value());
    alarm_pos_data.append(ui->doubleSpinBox_6->value());
    alarm_pos_data.append(ui->doubleSpinBox_7->value());
    alarm_pos_data.append(ui->doubleSpinBox_8->value());
    alarm_pos_data.append(ui->comboBox->currentIndex());
    alarm_pos_data.append(ui->doubleSpinBox_4->value());
    alarm_pos_data.append(ui->doubleSpinBox_3->value());

    return alarm_pos_data;

}

bool Dialog_alarm_pos::alarm_activation()
{
    return alarm_on;
}

bool Dialog_alarm_pos::alarm_x_enabled()
{
    return alarm_x_ena;
}

bool Dialog_alarm_pos::alarm_y_enabled()
{
    return alarm_y_ena;
}

bool Dialog_alarm_pos::alarm_z_enabled()
{
    return alarm_z_ena;
}

void Dialog_alarm_pos::on_doubleSpinBox_valueChanged(double arg1)
{
    if(arg1<(ui->doubleSpinBox_2->value())){
        ui->doubleSpinBox->setValue(ui->doubleSpinBox_2->value()+0.1);
    }
}


void Dialog_alarm_pos::on_doubleSpinBox_2_valueChanged(double arg1)
{
    if(arg1>(ui->doubleSpinBox->value())){
        ui->doubleSpinBox_2->setValue(ui->doubleSpinBox->value()-0.1);
    }
}


void Dialog_alarm_pos::on_doubleSpinBox_5_valueChanged(double arg1)
{
    if(arg1<(ui->doubleSpinBox_6->value())){
        ui->doubleSpinBox_5->setValue(ui->doubleSpinBox_6->value()+0.1);
    }
}


void Dialog_alarm_pos::on_doubleSpinBox_6_valueChanged(double arg1)
{
    if(arg1>(ui->doubleSpinBox_5->value())){
        ui->doubleSpinBox_6->setValue(ui->doubleSpinBox_5->value()-0.1);
    }
}


void Dialog_alarm_pos::on_doubleSpinBox_7_valueChanged(double arg1)
{
    if(arg1<(ui->doubleSpinBox_8->value())){
        ui->doubleSpinBox_7->setValue(ui->doubleSpinBox_8->value()+0.1);
    }
}


void Dialog_alarm_pos::on_doubleSpinBox_8_valueChanged(double arg1)
{
    if(arg1>(ui->doubleSpinBox_7->value())){
        ui->doubleSpinBox_8->setValue(ui->doubleSpinBox_7->value()-0.1);
    }
}


void Dialog_alarm_pos::on_checkBox_toggled(bool checked)
{
    if(checked==1){
        alarm_on=1;
    }
    else{
        alarm_on=0;
    }
}


void Dialog_alarm_pos::on_checkBox_2_toggled(bool checked)
{
    if(checked==1){
        alarm_x_ena=1;
    }
    else{
        alarm_x_ena=0;
    }
}


void Dialog_alarm_pos::on_checkBox_3_toggled(bool checked)
{
    if(checked==1){
        alarm_y_ena=1;
    }
    else{
        alarm_y_ena=0;
    }
}


void Dialog_alarm_pos::on_checkBox_4_toggled(bool checked)
{
    if(checked==1){
        alarm_z_ena=1;
    }
    else{
        alarm_z_ena=0;
    }
}
