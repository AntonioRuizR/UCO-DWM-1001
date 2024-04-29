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

#include "dialog_pos_graph_config.h"
#include "ui_dialog_pos_graph_config.h"

Dialog_pos_graph_config::Dialog_pos_graph_config(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_pos_graph_config)
{
    ui->setupUi(this);
    setWindowTitle(tr("Configuración: Gráfica de posicionamiento"));
}

Dialog_pos_graph_config::~Dialog_pos_graph_config()
{
    delete ui;
}

void Dialog_pos_graph_config::load_data(QList<float> position0){

    ui->doubleSpinBox_11->setValue(position0[0]);
    ui->doubleSpinBox_12->setValue(position0[1]);
    ui->doubleSpinBox_9->setValue(position0[2]);
    ui->doubleSpinBox_10->setValue(position0[3]);
    ui->doubleSpinBox->setValue(position0[4]);
    ui->doubleSpinBox_2->setValue(position0[5]);
    ui->doubleSpinBox_3->setValue(position0[6]);
    ui->doubleSpinBox_4->setValue(position0[7]);
    ui->doubleSpinBox_5->setValue(position0[8]);
    ui->doubleSpinBox_6->setValue(position0[9]);
    ui->doubleSpinBox_7->setValue(position0[10]);
    ui->doubleSpinBox_8->setValue(position0[11]);
}

QList<float> Dialog_pos_graph_config::anchor_data(){

    QList<float> anchors;

    anchors.append(ui->doubleSpinBox->value());
    anchors.append(ui->doubleSpinBox_5->value());
    anchors.append(ui->doubleSpinBox_2->value());
    anchors.append(ui->doubleSpinBox_6->value());
    anchors.append(ui->doubleSpinBox_3->value());
    anchors.append(ui->doubleSpinBox_7->value());
    anchors.append(ui->doubleSpinBox_4->value());
    anchors.append(ui->doubleSpinBox_8->value());

    anchors.append(ui->doubleSpinBox_9->value());
    anchors.append(ui->doubleSpinBox_10->value());
    anchors.append(ui->doubleSpinBox_11->value());
    anchors.append(ui->doubleSpinBox_12->value());

    return anchors;
}

void Dialog_pos_graph_config::on_pushButton_clicked()
{
    passive=1;
}

bool Dialog_pos_graph_config::passive_mode()
{
    return passive;
}

void Dialog_pos_graph_config::on_doubleSpinBox_11_valueChanged(double arg1)
{
    if(arg1 > ui->doubleSpinBox_12->value() or arg1 == ui->doubleSpinBox_12->value()){
        ui->doubleSpinBox_11->setValue(ui->doubleSpinBox_12->value()-0.1);
    }
}


void Dialog_pos_graph_config::on_doubleSpinBox_12_valueChanged(double arg1)
{
    if(arg1 < ui->doubleSpinBox_11->value() or arg1 == ui->doubleSpinBox_11->value()){
        ui->doubleSpinBox_12->setValue(ui->doubleSpinBox_11->value()+0.1);
    }
}


void Dialog_pos_graph_config::on_doubleSpinBox_9_valueChanged(double arg1)
{
    if(arg1 > ui->doubleSpinBox_10->value() or arg1 == ui->doubleSpinBox_10->value()){
        ui->doubleSpinBox_9->setValue(ui->doubleSpinBox_10->value()-0.1);
    }
}


void Dialog_pos_graph_config::on_doubleSpinBox_10_valueChanged(double arg1)
{
    if(arg1 < ui->doubleSpinBox_9->value() or arg1 == ui->doubleSpinBox_9->value()){
        ui->doubleSpinBox_10->setValue(ui->doubleSpinBox_9->value()+0.1);
    }
}
