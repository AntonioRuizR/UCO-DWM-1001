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
**             Date: 15.04.24                                                     **
**          Version: 0.9.2                                                        **
************************************************************************************/

#include "dialog_alarm_dist.h"
#include "ui_dialog_alarm_dist.h"

Dialog_alarm_dist::Dialog_alarm_dist(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_alarm_dist)
{
    ui->setupUi(this);
    setWindowTitle(tr("Configuración: Alarma"));
}

Dialog_alarm_dist::~Dialog_alarm_dist()
{
    delete ui;
}

void Dialog_alarm_dist::load_data_alarm(QList<float> received_alarm_data){

    ui->doubleSpinBox->setValue(received_alarm_data[0]);
    ui->doubleSpinBox_2->setValue(received_alarm_data[1]);
    ui->doubleSpinBox_3->setValue(received_alarm_data[3]);
    ui->doubleSpinBox_4->setValue(received_alarm_data[2]);
    ui->comboBox->setCurrentIndex(received_alarm_data[4]);
    if(received_alarm_data[5]==1){
        ui->checkBox->setChecked(true);
    }
}

QList<float> Dialog_alarm_dist::alarm_data(){

    QList<float> datosalarma;

    datosalarma.append(ui->doubleSpinBox->value());
    datosalarma.append(ui->doubleSpinBox_2->value());
    datosalarma.append(ui->doubleSpinBox_3->value());
    datosalarma.append(ui->doubleSpinBox_4->value());
    datosalarma.append(ui->comboBox->currentIndex());
    return datosalarma;

}

void Dialog_alarm_dist::on_checkBox_toggled(bool checked)
{
    if(checked==1){
        alarm_on=1;
    }
    else{
        alarm_on=0;
    }
}

bool Dialog_alarm_dist::alarm_activation()
{
    return alarm_on;
}

void Dialog_alarm_dist::on_doubleSpinBox_valueChanged(double arg1)
{
    if(arg1<(ui->doubleSpinBox_2->value())){
        ui->doubleSpinBox->setValue(ui->doubleSpinBox_2->value()+0.1);
    }
}

void Dialog_alarm_dist::on_doubleSpinBox_2_valueChanged(double arg1)
{
    if(arg1>(ui->doubleSpinBox->value())){
        ui->doubleSpinBox_2->setValue(ui->doubleSpinBox->value()-0.1);
    }
}
