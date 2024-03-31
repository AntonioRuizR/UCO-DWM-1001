#ifndef DIALOG_ALARM_POS_H
#define DIALOG_ALARM_POS_H

#include <QDialog>

namespace Ui {
class Dialog_alarm_pos;
}

class Dialog_alarm_pos : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_alarm_pos(QWidget *parent = nullptr);
    ~Dialog_alarm_pos();

    void load_data_alarm(QList<float>);

    QList<float> alarm_data();

    bool alarm_on=0;
    bool alarm_pos_on=0;
    bool alarm_activation();

    bool alarm_x_enabled();
    bool alarm_y_enabled();
    bool alarm_z_enabled();

    bool alarm_x_ena=0;
    bool alarm_y_ena=0;
    bool alarm_z_ena=0;

private slots:

    void on_doubleSpinBox_valueChanged(double arg1);
    void on_doubleSpinBox_2_valueChanged(double arg1);
    void on_doubleSpinBox_5_valueChanged(double arg1);
    void on_doubleSpinBox_6_valueChanged(double arg1);
    void on_doubleSpinBox_7_valueChanged(double arg1);
    void on_doubleSpinBox_8_valueChanged(double arg1);
    void on_checkBox_toggled(bool checked);
    void on_checkBox_2_toggled(bool checked);
    void on_checkBox_3_toggled(bool checked);
    void on_checkBox_4_toggled(bool checked);

private:
    Ui::Dialog_alarm_pos *ui;
};

#endif // DIALOG_ALARM_POS_H
