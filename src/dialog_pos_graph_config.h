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

#ifndef DIALOG_POS_GRAPH_CONFIG_H
#define DIALOG_POS_GRAPH_CONFIG_H

#include <QDialog>

namespace Ui {
class Dialog_pos_graph_config;
}

class Dialog_pos_graph_config : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_pos_graph_config(QWidget *parent = nullptr);
    ~Dialog_pos_graph_config();

    bool passive=0;
    bool passive_mode();
    QList<float> anchor_data();
    void load_data(QList<float>);

private slots:
    void on_pushButton_clicked();
    void on_doubleSpinBox_11_valueChanged(double arg1);
    void on_doubleSpinBox_12_valueChanged(double arg1);
    void on_doubleSpinBox_9_valueChanged(double arg1);
    void on_doubleSpinBox_10_valueChanged(double arg1);

private:
    Ui::Dialog_pos_graph_config *ui;
};

#endif // DIALOG_POS_GRAPH_CONFIG_H
