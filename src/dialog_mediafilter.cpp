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

#include "dialog_mediafilter.h"
#include "ui_dialog_mediafilter.h"

Dialog_mediafilter::Dialog_mediafilter(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_mediafilter)
{
    ui->setupUi(this);
    setWindowTitle(tr("Configuración: Filtro de media"));
}

Dialog_mediafilter::~Dialog_mediafilter()
{
    delete ui;
}

void Dialog_mediafilter::load_data_bufer(QList<float> info_bufer){

    ui->spinBox->setValue(info_bufer[0]);
    ui->checkBox_2->setChecked(info_bufer[1]);
    ui->checkBox->setChecked(info_bufer[2]);
}

QList<float> Dialog_mediafilter::bufer_data(){

    QList<float> bufer_size;

    bufer_size.append(ui->spinBox->value());
    return bufer_size;

}

void Dialog_mediafilter::on_checkBox_toggled(bool checked)
{
    if(checked==1){
        alarm_linked=1;
    }
    else{
        alarm_linked=0;
    }
}

bool Dialog_mediafilter::link_alarm()
{
    return alarm_linked;
}

void Dialog_mediafilter::on_checkBox_2_toggled(bool checked)
{
    if(checked==1){
        media_enabled=1;
    }
    else{
        media_enabled=0;
    }
}

bool Dialog_mediafilter::enable_mediafilter()
{
    return media_enabled;
}
