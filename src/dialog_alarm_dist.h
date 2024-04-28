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

#ifndef DIALOG_ALARM_DIST_H
#define DIALOG_ALARM_DIST_H

#include <QDialog>

namespace Ui {
class Dialog_alarm_dist;
}

class Dialog_alarm_dist : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_alarm_dist(QWidget *parent = nullptr);
    ~Dialog_alarm_dist();

    void load_data_alarm(QList<float>);
    QList<float> alarm_data();
    bool alarm_on=0;
    bool alarm_activation();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_doubleSpinBox_2_valueChanged(double arg1);

private:
    Ui::Dialog_alarm_dist *ui;
};

#endif // DIALOG_ALARM_DIST_H
