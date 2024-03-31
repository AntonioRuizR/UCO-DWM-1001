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

#include "dialog_csv.h"
#include "ui_dialog_csv.h"

//CSV Data recording window settings
Dialog_csv::Dialog_csv(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_csv)
{
    ui->setupUi(this);
    setWindowTitle(tr("Configuración CSV"));
}

QString Dialog_csv::folder_filename_csv()
{
    folder_directory_csv = ui->lineEdit_ruta->text();
    filename_csv = ui->lineEdit_nombre->text();
    if((folder_directory_csv=="") || (filename_csv=="")){
        complete_folder_filename = QDir::currentPath() + "/" + "Test.csv";
        return complete_folder_filename;
    }
    else{
        complete_folder_filename.append(folder_directory_csv + "/" + filename_csv + ".csv");
        return complete_folder_filename;
    }
}

Dialog_csv::~Dialog_csv()
{
    delete ui;
}

void Dialog_csv::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),                                                   "/home",
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_ruta->setText(dir);
}
