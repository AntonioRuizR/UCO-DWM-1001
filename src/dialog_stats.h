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

    //void load_data_estadisticas_d1(QList<double>);
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
