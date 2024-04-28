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
**             Date: 25.04.24                                                     **
**          Version: 1.0.0                                                        **
************************************************************************************/

#ifndef DIALOG_STATS_H
#define DIALOG_STATS_H

#include <QDialog>
#include <QFileDialog>
#include <QDateTime>

namespace Ui {
class Dialog_stats;
}

class Dialog_stats : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_stats(QWidget *parent = nullptr);
    ~Dialog_stats();

    void load_data_statistics_d1(QList<double>);
    void load_data_statistics_d2(QList<double>);
    void load_data_statistics_d3(QList<double>);
    void load_data_statistics_d4(QList<double>);

    void load_data_statistics_pos_x(QList<double>);
    void load_data_statistics_pos_y(QList<double>);
    void load_data_statistics_pos_z(QList<double>);

    void load_data_statistics_ids(QList<QString>);

    void initialise_variables();
    bool reset_variables();
    bool enable_data_saving();
    QString directory_folder_stat();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Dialog_stats *ui;

    bool reset_values = 0;
    QString save_folder;
    QString file_name;
    QString complete_dir;
    bool save_statistics=0;
};

#endif // DIALOG_STATS_H
