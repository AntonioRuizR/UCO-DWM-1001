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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qelapsedtimer.h"
#include "qtimer.h"
#include <QMainWindow>
#include "qcustomplot.h"
#include <QFileDialog>
#include <QSerialPort>
#include <QMessageBox>
#include <QPen>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QSerialPort;
class Console;

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

protected slots:
    // this slot is called by the language menu actions
    void slotLanguageChanged(QAction* action);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    void setupRealtimeDataDemo2(QCustomPlot *customPlot);
    void setupRealtimeDataDemo3(QCustomPlot *customPlot);

    void device_connection();
    void device_disconnection();
    void closeEvent (QCloseEvent *event);

private slots:

    // GUI objects:

    //void on_pushButton_clicked();
    //void on_pushButton_2_clicked();
    //void on_pushButton_3_clicked();
    //void on_spinBox_3_valueChanged(int arg1);

    // Connection:

    void writeData(const QByteArray &data);
    void readData();
    void sampletimechange_wait();
    void gpio_config();
    void device_config(int);
    void passive_config();
    void network_config(QString);
    //void informacion_sys();
    void send_message(QString);
    void change_devicemode_wait();
    void receive_info();
    void receive_info_failure();

    // Graph functions:

    void distance_analysis(const QByteArray &data);
    void position_analysis(const QByteArray &data2);
    void realtimeDataSlot();

    void media_calc();
    void media_calc_pos();

    void distance_data_processing(double, double, double, double);
    void position_data_processing();
    void distance_stats_calc();
    void position_stats_calc();
    void reset_stats();

    void save_distance_data(QDateTime hora);
    void save_position_data(QDateTime hora);
    void save_stats_data();

    void check_distance_alarm();
    void check_position_alarm();
    void alarm_graph();
    void alarm_duration_f();
    void dist_alarm_activation_request();
    void pos_alarm_activation_request();
    void draw_alarm_pos();

    //void desactivar_recepcion();
    void clear_gpio();
    QColor obtain_color(float);
    void gpio_clear_wait();

    void timer_gpio8();
    void timer_gpio9();
    void timer_gpio10();
    void timer_gpio12();
    void pos_anchors();

    //    void on_comboBox_3_currentIndexChanged(int index);

    //void on_plot_distancia_toggled(bool checked);

    //void on_pushButton_3_clicked();

    //void on_tabWidget_tabBarClicked(int index);

    //void on_pushButton_reiniciovalores_clicked();

    //void on_actionConfiguraci_n_CSV_triggered();

    //void on_actionFactory_Reset_triggered();

    //void on_comboBox_GPIO8_activated(int index);

    //void on_comboBox_GPIO9_activated(int index);

    //void on_comboBox_GPIO10_activated(int index);

    //void on_comboBox_GPIO12_activated(int index);

    //void on_pushButton_GPIO8_clicked();

    //void on_pushButton_GPIO9_clicked();

    //void on_pushButton_GPIO10_clicked();

    //void on_pushButton_GPIO12_clicked();

    //void on_checkBox_2_stateChanged(int arg1);

    //void on_checkBox_2_toggled(bool checked);

    //void on_plotAlarma_toggled(bool checked);

    //void on_plotAlarma_clicked(bool checked);

    //void on_doubleSpinBox_alarmmax_valueChanged(double arg1);

    //void on_doubleSpinBox_alarmmin_valueChanged(double arg1);

    //void on_actionReset_2_triggered();

    //void on_pushButton_4_clicked();

    //void on_pushButton_5_clicked();

    //void on_pushButton_6_clicked();

    //void on_actionAcerca_de_triggered();

    //void on_plotAlarma_2_clicked(bool checked);

    //void on_pushButton_4_clicked();

    //void on_comboBox_4_currentIndexChanged(int index);

    //void on_checkBox_3_clicked(bool checked);

    void on_pushButton_USBportrefresh_clicked();

    void on_pushButton_connect_clicked();

    void on_pushButton_disconnect_clicked();

    void on_pushButton_distancemode_clicked();

    void on_pushButton_positionmode_clicked();

    void on_actionExit_triggered();

    void on_plot_distance_toggled(bool checked);

    void on_spinBox_sampletime_valueChanged(int arg1);

    void on_actionDistanceGraphSettings_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_actionReset_triggered();

    void on_actionFactory_Reset_triggered();

    void on_actionDataSavingSettings_triggered();

    void on_actionStartDataSaving_triggered();

    void on_actionStopDataSaving_triggered();

    void on_actionPositionGraphSettings_triggered();

    void on_actionDeviceSettings_triggered();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_comboBox_GPIO8_activated(int index);

    void on_comboBox_GPIO9_activated(int index);

    void on_comboBox_GPIO10_activated(int index);

    void on_comboBox_GPIO12_activated(int index);

    void on_pushButton_GPIO8_clicked();

    void on_pushButton_GPIO9_clicked();

    void on_pushButton_GPIO10_clicked();

    void on_pushButton_GPIO12_clicked();

    void on_actionStatistics_triggered();

    //void on_comboBox_nummedidas_activated(int index);

    void on_actionAbout_triggered();

    //void on_pushButton_3_clicked();

    void on_actionHelp_triggered();

    void on_actionHistogram_triggered();

    void on_actionReiniciar_los_datos_recibidos_triggered();

private:
    // loads a language by the given language shortcur (e.g. de, en)
    void loadLanguage(const QString& rLanguage);

    // creates the language menu dynamically from the content of m_langPath
    void createLanguageMenu(void);


    Ui::MainWindow *ui;
    QTranslator m_translator; // contains the translations for this application
    QTranslator m_translatorQt; // contains the translations for qt
    QString m_currLang; // contains the currently loaded language
    QString m_langPath; // Path of language files. This is always fixed to /languages.

    QSerialPort *p_serie;

    // Inicializar variables

    //bool grabar=0;
    bool savedata=0;

    //bool multiples_mediciones=0;
    bool multiple_measures=0;

    bool info_ids_1=0;
    bool info_ids_2=0;
    bool info_ids_3=0;
    bool info_ids_4=0;

    //QByteArray mensaje_serie;
    QByteArray serial_message;
    QByteArray position_message;
    //QByteArray mensaje_bienvenida;
    bool receive_initial_info=0;
    QByteArray info_message;
    QString filename="A:/Documentos/valores_csv.txt";
    QString filename_est;
    //QStringList Mensaje_info;
    //QByteArray mensaje_info;
    QString chosenPort;
    //Console *p_consola = nullptr;
    QString network;

    QTimer timer_sampletime;
    QTimer dataTimer;
    QTimer posTimer;
    QTimer timer_gpioconfig;
    QTimer timer_alarmdelay;
    QTimer timer_alarmduration;
    QTimer timer_opmodechange;
    QTimer timer_infofailure;
    QTimer timer_gpioclear;
    bool deactivate_GPIO=0;

    int distance_samples_ID1=0; //Esto parece que no hace nada
    int distance_samples_ID2=0;
    int distance_samples_ID3=0;
    int distance_samples_ID4=0; //Sólo se usa esta variable (analizar)
    int position_samples=0;
    //int tiempo_activacion;
    //int fallo_mensaje=0;

    //QElapsedTimer timer_medida;

    QTimer tem_gpio8;
    QTimer tem_gpio9;
    QTimer tem_gpio10;
    QTimer tem_gpio12;
    int desac_gpio=0;

    //bool escala_tiempo=0;
    bool datareception_enabled;
    //bool solicitud_info;
    //bool modo=0;
    //bool modo_anterior=0;
    bool passive_positioning=0; //NUEVO
    bool tag_positioning=0; //NUEVO

    bool operation_mode=0;

    bool enable_statistics=0;
    //QList<float> valor_distancia_actual;
    double distance_value_1;
    double distance_value_2;
    double distance_value_3;
    double distance_value_4;
    //double valor_distancia_5;
    //double valor_distancia_6;
    double mediafilter_value_1;
    double mediafilter_value_2;
    double mediafilter_value_3;
    double mediafilter_value_4;
    double accumulated_dist_1=0;
    double accumulated_dist_2=0;
    double accumulated_dist_3=0;
    double accumulated_dist_4=0;
    double position_data_samples=0;
    double received_data_quantity_1=0;
    double received_data_quantity_2=0;
    double received_data_quantity_3=0;
    double received_data_quantity_4=0;
    double min_value_1=1000;
    double max_value_1=0;
    double min_value_2=1000;
    double max_value_2=0;
    double min_value_3=1000;
    double max_value_3=0;
    double min_value_4=1000;
    double max_value_4=0;

    double mode_1=0;
    double std_1=0;
    double range_1=0;
    double percentil_25_1=0;
    double percentil_75_1=0;

    double mode_2=0;
    double std_2=0;
    double range_2=0;
    double percentil_25_2=0;
    double percentil_75_2=0;

    double mode_3=0;
    double std_3=0;
    double range_3=0;
    double percentil_25_3=0;
    double percentil_75_3=0;

    double mode_4=0;
    double std_4=0;
    double range_4=0;
    double percentil_25_4=0;
    double percentil_75_4=0;

    //double media_total_x=0;
    //double media_total_y=0;
    //double media_total_z=0;
    double mode_x=0;
    double mode_y=0;
    double mode_z=0;
    double std_x=0;
    double std_y=0;
    double std_z=0;
    double max_x=-1000;
    double max_y=-1000;
    double max_z=-1000;
    double min_x=1000;
    double min_y=1000;
    double min_z=1000;
    double range_x=0;
    double range_y=0;
    double range_z=0;
    double accumulated_pos_x=0;
    double accumulated_pos_y=0;
    double accumulated_pos_z=0;
    double mean_total_x=0;
    double mean_total_y=0;
    double mean_total_z=0;
    double percentil_25_x=0;
    double percentil_75_x=0;
    double percentil_25_y=0;
    double percentil_75_y=0;
    double percentil_25_z=0;
    double percentil_75_z=0;


    QList<double> distance_list_1;
    QList<double> distance_list_2;
    QList<double> distance_list_3;
    QList<double> distance_list_4;

    QList<double> position_list_x;
    QList<double> position_list_y;
    QList<double> position_list_z;

    //double valor_filtro_max=0;
    //double valor_filtro_min=100;
    //double numero_muestras_filtro=0;
    //double valor_filtro_suma=0;
    //double valor_filtro_media;
    int detected_devices = 0;
    int max_detected_devices = 0;

    //bool espera_m=0;
    bool gpio_init_config=1;
    //bool men_bienvenida=1;
    bool alarm_activation=0;
    bool alarm_activated=0;
    //bool cambio_ui=0;
    int previous_alarm=0;
    int chosen_alarm=0;
    bool distance_alarm_plot=0;
    bool position_alarm_pos=0;

    bool auto_range=0;

    bool mean_value_alarm_link=0;

    int bufer_size=1;

    double bufer_media;
    double bufer_media_2;
    double bufer_media_3;
    double bufer_media_4;
    double bufer_media_5;
    double bufer_media_6;

    //float bufer_tam=0;
    bool mean_enable=0;
    QList<double> bufer;
    QList<double> bufer_2;
    QList<double> bufer_3;
    QList<double> bufer_4;
    QList<double> bufer_5;
    QList<double> bufer_6;

    double bufer_media_x;
    QList<double> bufer_x;
    double bufer_media_y;
    QList<double> bufer_y;
    double bufer_media_z;
    QList<double> bufer_z;

    QString id_1;
    QString id_2;
    QString id_3;
    QString id_4;
    QString initial_id_1;
    QString initial_id_2;
    QString initial_id_3;
    QString initial_id_4;

    int id_label_1=0;
    int id_label_2=0;
    int id_label_3=0;
    int id_label_4=0;

    //double posicion_total;
    double position_x;
    double position_y;
    double position_z;

    double pos_x_temp;
    double pos_y_temp;
    double pos_z_temp;

    float pos_alarm_x_min;
    float pos_alarm_x_max;
    float pos_alarm_y_min;
    float pos_alarm_y_max;
    float pos_alarm_z_min;
    float pos_alarm_z_max;
    int pos_alarm_delay=0;
    int pos_alarm_duration=0;
    bool pos_x_alarm_ena=0;
    bool pos_y_alarm_ena=0;
    bool pos_z_alarm_ena=0;

    float axisYmax;
    float max_value_alarm;
    float min_value_alarm;
    //bool start_alarm=0;
    int alarm_delay=0;
    int alarm_duration=0;

    float graph0_size=1.50;
    float graph1_size=1.50;
    float graph2_size=1.50;
    //float graph3_size=1.50;
    float graph4_size=1.50;
    float graph5_size=1.50;
    float graph6_size=1.50;
    int color_graph0;
    int color_graph1;
    int color_graph2;

    QColor color1;
    QColor color1_2;
    QColor color1_3;
    QColor color1_4;
    QColor color2;
    QColor color3;
    QString chosen_color_info="";

    QString chosen_color_d1="red";
    QString chosen_color_d2="blue";
    QString chosen_color_d3="darkgreen";
    QString chosen_color_d4="magenta";

    //int index_color_d1;
    //int indice_color_d2;
    //int indice_color_d3;
    //int indice_color_d4;
    int indice_color_fm;
    int indice_color_al;

    QPen graphPen;
    QPen graphPen_2;
    QPen graphPen_3;
    QPen graphPen_4;
    QPen graphPen_5;
    QPen graphPen_6;
    QPen graphPenmaxmin;

    double range_dist_y_max;
    double range_dist_y_min;

    double range_pos_y_min=0;
    double range_pos_y_max=10;
    double range_pos_x_min=0;
    double range_pos_x_max=10;

    double anchor_1_x;
    double anchor_1_y;
    double anchor_2_x;
    double anchor_2_y;
    double anchor_3_x;
    double anchor_3_y;
    double anchor_4_x;
    double anchor_4_y;

    bool position_graph_enabled=0;
};
#endif // MAINWINDOW_H
