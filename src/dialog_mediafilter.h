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

#ifndef DIALOG_MEDIAFILTER_H
#define DIALOG_MEDIAFILTER_H

#include <QDialog>

namespace Ui {
class Dialog_mediafilter;
}

class Dialog_mediafilter : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_mediafilter(QWidget *parent = nullptr);
    ~Dialog_mediafilter();

    bool alarm_linked=0;

    bool media_enabled=0;

    QList<float> bufer_data();

    bool link_alarm();

    bool enable_mediafilter();

    void load_data_bufer(QList<float>);

private slots:
    void on_checkBox_toggled(bool checked);

    void on_checkBox_2_toggled(bool checked);

private:
    Ui::Dialog_mediafilter *ui;
};

#endif // DIALOG_MEDIAFILTER_H
