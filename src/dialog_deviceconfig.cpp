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

#include "dialog_deviceconfig.h"
#include "ui_dialog_deviceconfig.h"

Dialog_deviceconfig::Dialog_deviceconfig(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_deviceconfig)
{
    ui->setupUi(this);
    setWindowTitle(tr("Configuración del módulo DWM1001"));
}

Dialog_deviceconfig::~Dialog_deviceconfig()
{
    delete ui;
}

void Dialog_deviceconfig::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    func_mode=1;
}

void Dialog_deviceconfig::on_pushButton_2_clicked()
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(true);
    func_mode=2;
}

void Dialog_deviceconfig::on_pushButton_3_clicked()
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(false);
    func_mode=3;
}

void Dialog_deviceconfig::on_pushButton_4_clicked()
{
    if(ui->lineEdit->text()!=""){
        network=ui->lineEdit->text();
        ui->label->setText(tr("Dirección escogida: "));
        ui->label_2->setText(network);
    }
}

int Dialog_deviceconfig::mode_processing()
{
    return func_mode;
}

QString Dialog_deviceconfig::network_processing()
{
    return network;
}
