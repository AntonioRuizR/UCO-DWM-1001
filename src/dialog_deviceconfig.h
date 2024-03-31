/***********************************************************************************
**                                                                                **
** <DWM1001-DEV APP, a GUI for the control of the DWM1001-DEV development board>  **
**    Copyright (C) <2024>  <Antonio Ruiz Ruiz>                                   **
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

#ifndef DIALOG_DEVICECONFIG_H
#define DIALOG_DEVICECONFIG_H

#include <QDialog>

namespace Ui {
class Dialog_deviceconfig;
}

class Dialog_deviceconfig : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_deviceconfig(QWidget *parent = nullptr);
    ~Dialog_deviceconfig();

    int func_mode=0;
    QString network;
    int mode_processing();
    QString network_processing();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::Dialog_deviceconfig *ui;
};

#endif // DIALOG_DEVICECONFIG_H
