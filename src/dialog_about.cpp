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

#include "dialog_about.h"
#include "ui_dialog_about.h"
#include <QPixmap>
#include <QDesktopServices>

Dialog_about::Dialog_about(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_about)
{
    ui->setupUi(this);
    setWindowTitle(tr("Acerca de:"));

    QPixmap logo_uco(":/images/uco.png");
    QPixmap logo_epsc(":/images/epsc.png");
    QPixmap logo_prinia(":/images/prinia_logo.jpg");
    int width_logo_uco = ui->label_logouco->width();
    int height_logo_uco = ui->label_logouco->height();
    int width_logo_epsc = ui->label_logoepsc->width();
    int height_logo_epsc = ui->label_logoepsc->height();
    int width_logo_prinia = ui->label_5->width();
    int height_logo_prinia = ui->label_5->height();
    ui->label_logouco->setPixmap(logo_uco.scaled(width_logo_uco,height_logo_uco,Qt::KeepAspectRatio));
    ui->label_logoepsc->setPixmap(logo_epsc.scaled(width_logo_epsc,height_logo_epsc,Qt::KeepAspectRatio));
    ui->label_5->setPixmap(logo_prinia.scaled(width_logo_prinia,height_logo_prinia,Qt::KeepAspectRatio));

    ui->label_3->setText("<a href=\"https://www.researchgate.net/profile/Mario-Ruz-2\">Mario L. Ruz Ruiz</a>"); // El enlace que quieres abrir
    ui->label_3->setOpenExternalLinks(true);

    ui->label_2->setText("<a href=\"https://www.researchgate.net/profile/Antonio-Ruiz-Ruiz-3\">Antonio Ruiz Ruiz</a>"); // El enlace que quieres abrir
    ui->label_2->setOpenExternalLinks(true);

    ui->label_4->setText("<a href=\"https://www.researchgate.net/profile/Juan-Garrido-26\">Juan Garrido Jurado</a>"); // El enlace que quieres abrir
    ui->label_4->setOpenExternalLinks(true);

    ui->label->setText(tr("Aplicación desarrollada en la Escuela Politécnica Superior de Córdoba de la " "<a href=\"https://www.uco.es\">Universidad de Córdoba</a> (España)."));
    ui->label->setOpenExternalLinks(true);
}

Dialog_about::~Dialog_about()
{
    delete ui;
}
