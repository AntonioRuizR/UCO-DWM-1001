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

#include "dialog_dist_graph_config.h"
#include "ui_dialog_dist_graph_config.h"

Dialog_dist_graph_config::Dialog_dist_graph_config(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_dist_graph_config)
{
    ui->setupUi(this);
    setWindowTitle(tr("Configuración: Gráfica distancia"));
}

Dialog_dist_graph_config::~Dialog_dist_graph_config()
{
    delete ui;
}

void Dialog_dist_graph_config::load_data(QList<float> graph0){

    ui->uppervalue->setValue(graph0[0]);
    ui->lowervalue->setValue(graph0[1]);
    ui->tam_graph0->setValue(graph0[2]);
    ui->tam_graph1->setValue(graph0[3]);
    ui->tam_graph2->setValue(graph0[4]);

    ui->tam_graph0_2->setValue(graph0[5]);
    ui->tam_graph0_3->setValue(graph0[6]);
    ui->tam_graph0_4->setValue(graph0[7]);
}

QList<float> Dialog_dist_graph_config::graph_data(){

    QList<float> graph;

    graph.append(ui->uppervalue->value());
    graph.append(ui->lowervalue->value());
    graph.append(ui->tam_graph0->value());
    graph.append(ui->tam_graph1->value());
    graph.append(ui->tam_graph2->value());
    graph.append(ui->comboBox->currentIndex());
    graph.append(ui->comboBox_2->currentIndex());
    graph.append(ui->comboBox_3->currentIndex());
    graph.append(ui->tam_graph0_2->value());
    graph.append(ui->tam_graph0_3->value());
    graph.append(ui->tam_graph0_4->value());
    graph.append(ui->comboBox_4->currentIndex());
    graph.append(ui->comboBox_5->currentIndex());
    graph.append(ui->comboBox_6->currentIndex());

    return graph;
}


void Dialog_dist_graph_config::on_checkBox_toggled(bool checked)
{
    if(checked==1){
        autorange_var=1;
    }
    else{
        autorange_var=0;
    }
}

bool Dialog_dist_graph_config::autorange()
{
    return autorange_var;
}

void Dialog_dist_graph_config::on_uppervalue_valueChanged(double arg1)
{
    if( arg1 == ui->lowervalue->value() or arg1 < ui->lowervalue->value()){
        ui->uppervalue->setValue(ui->lowervalue->value()+0.1);
    }
}


void Dialog_dist_graph_config::on_lowervalue_valueChanged(double arg1)
{
    if( arg1 == ui->uppervalue->value() or arg1 > ui->uppervalue->value()){
        ui->lowervalue->setValue(ui->uppervalue->value()-0.1);
    }
}
