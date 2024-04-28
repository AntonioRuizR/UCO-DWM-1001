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

#ifndef DIALOG_DIST_GRAPH_CONFIG_H
#define DIALOG_DIST_GRAPH_CONFIG_H

#include <QDialog>

namespace Ui {
class Dialog_dist_graph_config;
}

class Dialog_dist_graph_config : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_dist_graph_config(QWidget *parent = nullptr);
    ~Dialog_dist_graph_config();

    bool autorange_var=0;
    QList<float> graph_data();
    void load_data(QList<float>);
    bool autorange();

private slots:
    void on_checkBox_toggled(bool checked);
    void on_uppervalue_valueChanged(double arg1);
    void on_lowervalue_valueChanged(double arg1);

private:
    Ui::Dialog_dist_graph_config *ui;
};

#endif // DIALOG_DIST_GRAPH_CONFIG_H
