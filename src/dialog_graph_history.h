#ifndef DIALOG_GRAPH_HISTORY_H
#define DIALOG_GRAPH_HISTORY_H

#include "qcustomplot.h"
#include <QDialog>

namespace Ui {
class Dialog_graph_history;
}

class Dialog_graph_history : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_graph_history(QWidget *parent = nullptr);
    ~Dialog_graph_history();

    void setupRealtimeDataDemo();
    void setupRealtimeDataDemo3(double, double, double, double);
    //void setupRealtimeDataDemo2(QCustomPlot *customPlot);
    void load_distances_1(QList<double>, double, QString, double);
    void load_distances_2(QList<double>, double, QString, double);
    void load_distances_3(QList<double>, double, QString, double);
    void load_distances_4(QList<double>, double, QString, double);
    void load_coordinates(QList<double>, QList<double>, QList<double>);
    void load_anchors(double, double, double, double, double, double, double, double);
    void max_range_graph(double, double, double, double, double, double, double, double);
    bool reset_variables();

    //void mousePress();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_1_toggled(bool checked);
    void on_checkBox_2_toggled(bool checked);
    void on_checkBox_3_toggled(bool checked);
    void on_checkBox_4_toggled(bool checked);

    void graph_interaction();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Dialog_graph_history *ui;

    QPen graphPen_1;
    QPen graphPen_2;
    QPen graphPen_3;
    QPen graphPen_4;
    QPen graphPen_selected;

    //QVector<double> received_data_quantity_1;
    //QVector<double> received_data_quantity_2;
    //QVector<double> received_data_quantity_3;
    //QVector<double> received_data_quantity_4;

    QVector<double> data_list_d1;
    QList<double> data_list_d2;
    QList<double> data_list_d3;
    QList<double> data_list_d4;

    QList<double> data_pos_x;
    QList<double> data_pos_y;
    QList<double> data_pos_z;

    double max_dist1=0;
    double max_dist2=0;
    double max_dist3=0;
    double max_dist4=0;

    QString id_label_1;
    QString id_label_2;
    QString id_label_3;
    QString id_label_4;

    double maximum_y_range;
    double maximum_x_range;

    double range_pos_y_min_c;
    double range_pos_y_max_c;
    double range_pos_x_min_c;
    double range_pos_x_max_c;

    bool reset_values = 0;

};

#endif // DIALOG_GRAPH_HISTORY_H
