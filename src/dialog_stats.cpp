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

#include "dialog_stats.h"
#include "ui_dialog_stats.h"

Dialog_stats::Dialog_stats(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_stats)
{
    ui->setupUi(this);
    setWindowTitle(tr("Estadísticas Avanzadas"));
}

Dialog_stats::~Dialog_stats()
{
    delete ui;
}

void Dialog_stats::initialise_variables(){

    reset_values = 0;
    save_statistics=0;
}

void Dialog_stats::load_data_statistics_d1(QList<double> stats_d1){

    ui->label_7->setText(QString::number(stats_d1[0],'g',3));
    ui->label_12->setText(QString::number(stats_d1[1],'g',3));
    ui->label_8->setText(QString::number(stats_d1[2],'g',3));
    ui->label_11->setText(QString::number(stats_d1[3],'g',3));
    ui->label_9->setText(QString::number(stats_d1[4],'g',3));
    ui->label_10->setText(QString::number(stats_d1[5],'g',3));
    ui->label_13->setText(QString::number(stats_d1[6],'g',3));
    ui->label_15->setText(QString::number(stats_d1[7],'g',3));
    ui->label_25->setText(QString::number(stats_d1[8]));
}

void Dialog_stats::load_data_statistics_d2(QList<double> stats_d2){

    ui->label_40->setText(QString::number(stats_d2[0],'g',3));
    ui->label_46->setText(QString::number(stats_d2[1],'g',3));
    ui->label_36->setText(QString::number(stats_d2[2],'g',3));
    ui->label_38->setText(QString::number(stats_d2[3],'g',3));
    ui->label_34->setText(QString::number(stats_d2[4],'g',3));
    ui->label_39->setText(QString::number(stats_d2[5],'g',3));
    ui->label_35->setText(QString::number(stats_d2[6],'g',3));
    ui->label_47->setText(QString::number(stats_d2[7],'g',3));
    ui->label_26->setText(QString::number(stats_d2[8]));
}

void Dialog_stats::load_data_statistics_d3(QList<double> stats_d3){

    ui->label_56->setText(QString::number(stats_d3[0],'g',3));
    ui->label_62->setText(QString::number(stats_d3[1],'g',3));
    ui->label_52->setText(QString::number(stats_d3[2],'g',3));
    ui->label_54->setText(QString::number(stats_d3[3],'g',3));
    ui->label_50->setText(QString::number(stats_d3[4],'g',3));
    ui->label_55->setText(QString::number(stats_d3[5],'g',3));
    ui->label_51->setText(QString::number(stats_d3[6],'g',3));
    ui->label_63->setText(QString::number(stats_d3[7],'g',3));
    ui->label_27->setText(QString::number(stats_d3[8]));
}

void Dialog_stats::load_data_statistics_d4(QList<double> stats_d4){

    ui->label_72->setText(QString::number(stats_d4[0],'g',3));
    ui->label_78->setText(QString::number(stats_d4[1],'g',3));
    ui->label_68->setText(QString::number(stats_d4[2],'g',3));
    ui->label_70->setText(QString::number(stats_d4[3],'g',3));
    ui->label_66->setText(QString::number(stats_d4[4],'g',3));
    ui->label_71->setText(QString::number(stats_d4[5],'g',3));
    ui->label_67->setText(QString::number(stats_d4[6],'g',3));
    ui->label_79->setText(QString::number(stats_d4[7],'g',3));
    ui->label_28->setText(QString::number(stats_d4[8]));
}

void Dialog_stats::load_data_statistics_pos_x(QList<double> stats_pos_x){
    ui->label_medio_x->setText(QString::number(stats_pos_x[0],'g',3));
    ui->label_moda_x->setText(QString::number(stats_pos_x[1],'g',3));
    ui->label_std_x->setText(QString::number(stats_pos_x[2],'g',3));
    ui->label_rango_x->setText(QString::number(stats_pos_x[3],'g',3));
    ui->label_max_x->setText(QString::number(stats_pos_x[4],'g',3));
    ui->label_min_x->setText(QString::number(stats_pos_x[5],'g',3));
    ui->label_percentil_25_x->setText(QString::number(stats_pos_x[6],'g',3));
    ui->label_percentil_75_x->setText(QString::number(stats_pos_x[7],'g',3));
}

void Dialog_stats::load_data_statistics_pos_y(QList<double> stats_pos_y){
    ui->label_medio_y->setText(QString::number(stats_pos_y[0],'g',3));
    ui->label_moda_y->setText(QString::number(stats_pos_y[1],'g',3));
    ui->label_std_y->setText(QString::number(stats_pos_y[2],'g',3));
    ui->label_rango_y->setText(QString::number(stats_pos_y[3],'g',3));
    ui->label_max_y->setText(QString::number(stats_pos_y[4],'g',3));
    ui->label_min_y->setText(QString::number(stats_pos_y[5],'g',3));
    ui->label_percentil_25_y->setText(QString::number(stats_pos_y[6],'g',3));
    ui->label_percentil_75_y->setText(QString::number(stats_pos_y[7],'g',3));
}

void Dialog_stats::load_data_statistics_pos_z(QList<double> stats_pos_z){
    ui->label_medio_z->setText(QString::number(stats_pos_z[0],'g',3));
    ui->label_moda_z->setText(QString::number(stats_pos_z[1],'g',3));
    ui->label_std_z->setText(QString::number(stats_pos_z[2],'g',3));
    ui->label_rango_z->setText(QString::number(stats_pos_z[3],'g',3));
    ui->label_max_z->setText(QString::number(stats_pos_z[4],'g',3));
    ui->label_min_z->setText(QString::number(stats_pos_z[5],'g',3));
    ui->label_percentil_25_z->setText(QString::number(stats_pos_z[6],'g',3));
    ui->label_percentil_75_z->setText(QString::number(stats_pos_z[7],'g',3));
}

void Dialog_stats::on_pushButton_clicked()
{
    reset_values = 1;

    //Distancia 1
    ui->label_7->setText("N/A");
    ui->label_12->setText("N/A");
    ui->label_8->setText("N/A");
    ui->label_11->setText("N/A");
    ui->label_9->setText("N/A");
    ui->label_10->setText("N/A");
    ui->label_13->setText("N/A");
    ui->label_15->setText("N/A");
    ui->label_25->setText("N/A");

    //Distancia 2
    ui->label_40->setText("N/A");
    ui->label_46->setText("N/A");
    ui->label_36->setText("N/A");
    ui->label_38->setText("N/A");
    ui->label_34->setText("N/A");
    ui->label_39->setText("N/A");
    ui->label_35->setText("N/A");
    ui->label_47->setText("N/A");
    ui->label_26->setText("N/A");

    //Distancia 3
    ui->label_56->setText("N/A");
    ui->label_62->setText("N/A");
    ui->label_52->setText("N/A");
    ui->label_54->setText("N/A");
    ui->label_50->setText("N/A");
    ui->label_55->setText("N/A");
    ui->label_51->setText("N/A");
    ui->label_63->setText("N/A");
    ui->label_27->setText("N/A");

    //Distancia 4
    ui->label_72->setText("N/A");
    ui->label_78->setText("N/A");
    ui->label_68->setText("N/A");
    ui->label_70->setText("N/A");
    ui->label_66->setText("N/A");
    ui->label_71->setText("N/A");
    ui->label_67->setText("N/A");
    ui->label_79->setText("N/A");
    ui->label_28->setText("N/A");

    //Posicion x
    ui->label_medio_x->setText("N/A");
    ui->label_moda_x->setText("N/A");
    ui->label_std_x->setText("N/A");
    ui->label_rango_x->setText("N/A");
    ui->label_max_x->setText("N/A");
    ui->label_min_x->setText("N/A");
    ui->label_percentil_25_x->setText("N/A");
    ui->label_percentil_75_x->setText("N/A");

    //Posicion y
    ui->label_medio_y->setText("N/A");
    ui->label_moda_y->setText("N/A");
    ui->label_std_y->setText("N/A");
    ui->label_rango_y->setText("N/A");
    ui->label_max_y->setText("N/A");
    ui->label_min_y->setText("N/A");
    ui->label_percentil_25_y->setText("N/A");
    ui->label_percentil_75_y->setText("N/A");

    //Posicion z
    ui->label_medio_z->setText("N/A");
    ui->label_moda_z->setText("N/A");
    ui->label_std_z->setText("N/A");
    ui->label_rango_z->setText("N/A");
    ui->label_max_z->setText("N/A");
    ui->label_min_z->setText("N/A");
    ui->label_percentil_25_z->setText("N/A");
    ui->label_percentil_75_z->setText("N/A");
}

bool Dialog_stats::reset_variables()
{
    return reset_values;
}

QString Dialog_stats::directory_folder_stat()
{
    QString dateandtime = QDateTime::currentDateTime().toString("dd_MM_yyyy hh_mm_ss");
    file_name = "Statistics " + dateandtime;
    complete_dir.append(save_folder + "/" + file_name + ".txt");
    return complete_dir;
}

void Dialog_stats::on_pushButton_2_clicked()
{
    save_folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),                                                   "/home",
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    ui->pushButton_2->setEnabled(false);
    save_statistics=1;
}

bool Dialog_stats::enable_data_saving()
{
    return save_statistics;
}

void Dialog_stats::load_data_statistics_ids(QList<QString> info_ids){

    if(info_ids[0]=="1"){
        ui->label_17->setText(info_ids[1]);
    }
    else if(info_ids[0]=="2"){
        ui->label_17->setText(info_ids[1]);
        ui->label_18->setText(info_ids[2]);
    }
    else if(info_ids[0]=="3"){
        ui->label_17->setText(info_ids[1]);
        ui->label_18->setText(info_ids[2]);
        ui->label_19->setText(info_ids[3]);
    }
    else if(info_ids[0]=="4"){
        ui->label_17->setText(info_ids[1]);
        ui->label_18->setText(info_ids[2]);
        ui->label_19->setText(info_ids[3]);
        ui->label_20->setText(info_ids[4]);
    }
}
