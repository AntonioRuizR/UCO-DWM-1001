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
