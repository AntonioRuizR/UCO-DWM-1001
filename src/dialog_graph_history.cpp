#include "dialog_graph_history.h"
#include "ui_dialog_graph_history.h"

Dialog_graph_history::Dialog_graph_history(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_graph_history)
{
    ui->setupUi(this);
    setWindowTitle(tr("Historial de mediciones"));
    setupRealtimeDataDemo();
}

Dialog_graph_history::~Dialog_graph_history()
{
    delete ui;
}

void Dialog_graph_history::setupRealtimeDataDemo()
{
    //Graph style distance 1
    graphPen_1.setColor(Qt::red);
    graphPen_1.setWidthF(2);

    //Graph style distance 2

    graphPen_2.setColor(Qt::blue);
    graphPen_2.setWidthF(2);

    //Graph style distance 3

    graphPen_3.setColor(Qt::green);
    graphPen_3.setWidthF(2);

    //Graph style distance 4

    graphPen_4.setColor(Qt::magenta);
    graphPen_4.setWidthF(2);

    //Graph selected graph

    graphPen_selected.setColor(Qt::black);
    graphPen_selected.setWidthF(2.5);

    //Graph distance 1
    ui->graph_h->addGraph();
    ui->graph_h->graph(0)->setPen(graphPen_1);
    ui->graph_h->graph(0)->setName("Distance 1");

    //Graph distance 2

    ui->graph_h->addGraph();
    ui->graph_h->graph(1)->setPen(graphPen_2);
    ui->graph_h->graph(1)->setName("Distance 2");

    //Graph distance 3

    ui->graph_h->addGraph();
    ui->graph_h->graph(2)->setPen(graphPen_3);
    ui->graph_h->graph(2)->setName("Distance 3");

    //Graph distance 4

    ui->graph_h->addGraph();
    ui->graph_h->graph(3)->setPen(graphPen_4);
    ui->graph_h->graph(3)->setName("Distance 4");

    //Axis label

    ui->graph_h->xAxis->setLabel(tr("Número de medidas"));
    ui->graph_h->yAxis->setLabel(tr("Distancia estimada"));

    //Graph configuration
    ui->graph_h->setInteraction(QCP::iRangeDrag, true);
    ui->graph_h->setInteraction(QCP::iRangeZoom, true);
    ui->graph_h->setInteraction(QCP::iSelectPlottables, true);
    ui->graph_h->setInteraction(QCP::iMultiSelect, true);

    ui->graph_h->graph(0)->setSelectable(QCP::stDataRange);
    ui->graph_h->graph(1)->setSelectable(QCP::stDataRange);
    ui->graph_h->graph(2)->setSelectable(QCP::stDataRange);
    ui->graph_h->graph(3)->setSelectable(QCP::stDataRange);

    //Interactions

    //connect(ui->graph_h, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->graph_h, SIGNAL(selectionChangedByUser()), this, SLOT(graph_interaction()));
}

void Dialog_graph_history::setupRealtimeDataDemo3(double range_pos_y_min, double range_pos_y_max, double range_pos_x_min, double range_pos_x_max)
{
    range_pos_x_max_c = range_pos_x_max;
    range_pos_x_min_c = range_pos_x_min;
    range_pos_y_max_c = range_pos_y_max;
    range_pos_y_min_c = range_pos_y_min;

    //Axis labels:
    ui->pos_h->xAxis->setLabel(tr("Eje x"));
    ui->pos_h->yAxis->setLabel(tr("Eje y"));

    //Antialiasing disabled

    //Graph style config:

    QPen graphPen_anchors;
    graphPen_anchors.setColor(Qt::red);
    graphPen_anchors.setWidthF(3);

    QPen graphPen_tag;

    graphPen_tag.setColor(Qt::blue);
    graphPen_tag.setWidthF(2);

    QPen graphPen_alarm;

    graphPen_alarm.setColor(Qt::gray);
    graphPen_alarm.setWidthF(2);

    //Anchor element graph settings:
    ui->pos_h->addGraph();
    ui->pos_h->graph(0)->setPen(graphPen_anchors);
    ui->pos_h->graph(0)->setName("anchors");
    ui->pos_h->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->pos_h->graph(0)->setScatterStyle(QCPScatterStyle::ssCrossSquare);

    //TAG element graph settings:
    ui->pos_h->addGraph();
    ui->pos_h->graph(1)->setPen(graphPen_tag);
    ui->pos_h->graph(1)->setName("TAG");
    ui->pos_h->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->pos_h->graph(1)->setScatterStyle(QCPScatterStyle::ssCircle);

    //Alarm graph:
    ui->pos_h->addGraph();
    ui->pos_h->graph(2)->setPen(graphPen_alarm);
    ui->pos_h->graph(2)->setName("Alarm");

    ui->pos_h->yAxis->setRange(range_pos_y_min_c, range_pos_y_max_c);
    ui->pos_h->xAxis->setRange(range_pos_x_min_c, range_pos_x_max_c);

    //When upper and right axis change, lower and left axis do the same
    connect(ui->pos_h->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->pos_h->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->pos_h->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->pos_h->yAxis2, SLOT(setRange(QCPRange)));

    ui->pos_h->setInteraction(QCP::iRangeDrag, true);
    ui->pos_h->setInteraction(QCP::iRangeZoom, true);
}

void Dialog_graph_history::load_distances_1(QList<double>list_d1, double max_d1, QString id_1, double num_d1)
{
    QVector<double> received_data_quantity_1(list_d1.count());
    for (int i = 0; i < list_d1.count(); ++i) {
        received_data_quantity_1[i] = i + 1;
    }
    data_list_d1 = list_d1;
    max_dist1 = max_d1;
    ui->graph_h->graph(0)->setData(received_data_quantity_1,data_list_d1);
    ui->label->setText("ID 1: <b><font color='red'>" + id_1 + "</font>");
    ui->label_5->setText(tr("Medidas:  ") + QString::number(num_d1));
    //ui->graph_h->xAxis->setRange(0,num_d1);
}

void Dialog_graph_history::load_distances_2(QList<double>list_d2, double max_d2, QString id_2, double num_d2)
{
    QVector<double> received_data_quantity_2(list_d2.count());
    for (int i = 0; i < list_d2.count(); ++i) {
        received_data_quantity_2[i] = i + 1;
    }
    data_list_d2 = list_d2;
    max_dist2 = max_d2;
    ui->graph_h->graph(1)->setData(received_data_quantity_2,data_list_d2);
    ui->label_2->setText("ID 2: <b><font color='blue'>" + id_2 + "</font>");
    ui->label_6->setText(tr("Medidas:  ")+ QString::number(num_d2));
}

void Dialog_graph_history::load_distances_3(QList<double>list_d3, double max_d3, QString id_3, double num_d3)
{
    QVector<double> received_data_quantity_3(list_d3.count());
    for (int i = 0; i < list_d3.count(); ++i) {
        received_data_quantity_3[i] = i + 1;
    }
    data_list_d3 = list_d3;
    max_dist3 = max_d3;
    ui->graph_h->graph(2)->setData(received_data_quantity_3,data_list_d3);
    ui->label_3->setText("ID 3: <b><font color='green'>" + id_3 + "</font>");
    ui->label_7->setText(tr("Medidas:  ")+ QString::number(num_d3));
}

void Dialog_graph_history::load_distances_4(QList<double>list_d4, double max_d4, QString id_4, double num_d4)
{
    QVector<double> received_data_quantity_4(list_d4.count());
    for (int i = 0; i < list_d4.count(); ++i) {
        received_data_quantity_4[i] = i + 1;
    }
    data_list_d4 = list_d4;
    max_dist4 = max_d4;
    ui->graph_h->graph(3)->setData(received_data_quantity_4,data_list_d4);
    ui->label_4->setText("ID 4: <b><font color='magenta'>" + id_4 + "</font>");
    ui->label_8->setText(tr("Medidas:  ")+ QString::number(num_d4));
}

void Dialog_graph_history::load_coordinates(QList<double>pos_x_list, QList<double>pos_y_list, QList<double>pos_z_list)
{
    QVector<double> received_data_quantity_pos(pos_x_list.count());
    ui->label_19->setText(QString::number(pos_x_list.count()));
    data_pos_x = pos_x_list;
    data_pos_y = pos_y_list;
    data_pos_z = pos_z_list;
    for (int i = 0; i < pos_x_list.count(); ++i) {
        received_data_quantity_pos[i] = i + 1;
        ui->pos_h->graph(1)->addData(data_pos_x[i], data_pos_y[i]);
    }
    ui->pos_h->replot();
}

void Dialog_graph_history::load_anchors(double anchor_1_x, double anchor_1_y, double anchor_2_x, double anchor_2_y, double anchor_3_x, double anchor_3_y, double anchor_4_x, double anchor_4_y)
{
    ui->pos_h->graph(0)->data()->clear();
    ui->pos_h->graph(0)->addData(anchor_1_x, anchor_1_y);
    ui->pos_h->graph(0)->addData(anchor_2_x, anchor_2_y);
    ui->pos_h->graph(0)->addData(anchor_3_x, anchor_3_y);
    ui->pos_h->graph(0)->addData(anchor_4_x, anchor_4_y);
    ui->pos_h->replot();
}

void Dialog_graph_history::max_range_graph(double d1, double d2, double d3, double d4, double num_d1, double num_d2, double num_d3, double num_d4)
{

    maximum_y_range = fmax(fmax(d1,d2), fmax(d3,d4));
    ui->graph_h->yAxis->setRange(0,qCeil(maximum_y_range));
    maximum_x_range = fmax(fmax(num_d1,num_d2), fmax(num_d3,num_d4));
    ui->graph_h->xAxis->setRange(0,maximum_x_range);
}

void Dialog_graph_history::on_pushButton_clicked()
{
    ui->graph_h->rescaleAxes();
    ui->graph_h->yAxis->setRange(0,qCeil(maximum_y_range));
    ui->graph_h->xAxis->setRange(0,maximum_x_range);
    ui->graph_h->replot();
}

bool Dialog_graph_history::reset_variables()
{
    return reset_values;
}


void Dialog_graph_history::on_pushButton_2_clicked()
{
    reset_values = 1;
    ui->pushButton_2->setEnabled(false);
    ui->graph_h->clearGraphs();
}


void Dialog_graph_history::on_checkBox_1_toggled(bool checked)
{
    if(checked == 1){
        ui->graph_h->graph(0)->setVisible(true);
    }
    else{
        ui->graph_h->graph(0)->setVisible(false);
    }
    ui->graph_h->replot();
}

void Dialog_graph_history::on_checkBox_2_toggled(bool checked)
{
    if(checked == 1){
        ui->graph_h->graph(1)->setVisible(true);
    }
    else ui->graph_h->graph(1)->setVisible(false);
    ui->graph_h->replot();
}

void Dialog_graph_history::on_checkBox_3_toggled(bool checked)
{
    if(checked == 1){
        ui->graph_h->graph(2)->setVisible(true);
    }
    else ui->graph_h->graph(2)->setVisible(false);
    ui->graph_h->replot();
}

void Dialog_graph_history::on_checkBox_4_toggled(bool checked)
{
    if(checked == 1){
        ui->graph_h->graph(3)->setVisible(true);
    }
    else ui->graph_h->graph(3)->setVisible(false);
    ui->graph_h->replot();
}

void Dialog_graph_history::graph_interaction()
{
    QCPDataSelection selection;
    if(ui->graph_h->graph(0)->selected()){
        selection = ui->graph_h->graph(0)->selection();
    }
    else if(ui->graph_h->graph(1)->selected()){
        selection = ui->graph_h->graph(1)->selection();
    }
    else if(ui->graph_h->graph(2)->selected()){
        selection = ui->graph_h->graph(2)->selection();
    }
    else if(ui->graph_h->graph(3)->selected()){
        selection = ui->graph_h->graph(3)->selection();
    }

    double sum = 0;
    double max = 0;
    double min =5000;

    foreach (QCPDataRange dataRange, selection.dataRanges())
    {
        QCPGraphDataContainer::const_iterator begin = ui->graph_h->graph(0)->data()->at(dataRange.begin()); // get range begin iterator from index
        QCPGraphDataContainer::const_iterator end = ui->graph_h->graph(0)->data()->at(dataRange.end()); // get range end iterator from index
        for (QCPGraphDataContainer::const_iterator it=begin; it!=end; ++it)
        {
            // iterator "it" will go through all selected data points, as an example, we calculate the value average
            sum += it->value;
            if(it->value > max){
                max = it->value;
            }
            if(it->value < min){
                min = it->value;
            }
        }
    }
    double average = sum/selection.dataPointCount();
    ui->label_9->setText(QString::number(average,'f',2));
    if(max==0){
        ui->label_12->setText("nan");
    }
    else{
        ui->label_12->setText(QString::number(max,'f',2));
    }


    if(min==5000){
        ui->label_14->setText("nan");
    }
    else{
        ui->label_14->setText(QString::number(min,'f',2));
    }
    ui->label_16->setText(QString::number(selection.dataPointCount(),'f',0));

    //código que muestre el valor XY del punto seleccionado
}

void Dialog_graph_history::on_pushButton_4_clicked()
{
    reset_values = 1;
    ui->pushButton_4->setEnabled(false);
    ui->pos_h->clearGraphs();
}


void Dialog_graph_history::on_pushButton_3_clicked()
{
    ui->pos_h->rescaleAxes();
    ui->pos_h->yAxis->setRange(range_pos_y_min_c, range_pos_y_max_c);
    ui->pos_h->xAxis->setRange(range_pos_x_min_c, range_pos_x_max_c);
    ui->pos_h->replot();
}

