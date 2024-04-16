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

#ifndef DIALOG_CSV_H
#define DIALOG_CSV_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class Dialog_csv;
}

class Dialog_csv : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_csv(QWidget *parent = nullptr);
    ~Dialog_csv();

    //QString rutaynombre_csv();
    QString folder_filename_csv();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog_csv *ui;
    QString folder_directory_csv;
    QString filename_csv;
    QString complete_folder_filename;
};

#endif // DIALOG_CSV_H
