#ifndef GRAPHCONTROLLER_H
#define GRAPHCONTROLLER_H

#include <QObject>
#include <QMainWindow>
#include <QScopedPointer>
#include <QSharedPointer>
#include "dataprovider.h"

#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QLayout>

QT_CHARTS_USE_NAMESPACE

class GraphController : public QObject
{
    Q_OBJECT
public:
    explicit GraphController(QSharedPointer<DataProvider> dataProvider, QObject *parent = NULL);
    ~GraphController();

    void ShowXShiftGraph();
    void ShowYShiftGraph();
    void ShowXGraph();
    void ShowYGraph();
    void ShowZGraph();
    void showTemperatureShiftGraph();

    void autoGenOffsets();
    int plot_on[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};




private slots:
    void updateGraphs();

private:

    // QMainWindow <-- QChartView <-- QChart <-- QLineSeries(1 per dev)
    enum GraphType { GRAPH_X_DRIFT, GRAPH_Y_DRIFT, GRAPH_X, GRAPH_Y, GRAPH_Z, GRAPH_TEMPERATURE };

    QSharedPointer<DataProvider> data_provider;

    QChart *x_graph;
    QChart *y_graph;
    QChart *z_graph;

    QChart *y_drift_graph;
    QChart *x_drift_graph;
    QChart *temperature_graph;

    QMainWindow *x_window;
    QMainWindow *y_window;
    QMainWindow *z_window;

    QMainWindow *x_drift_window;
    QMainWindow *y_drift_window;
    QMainWindow *temperature_window;


    void update_graph(QChart *graph, GraphType graphType, QString x_axis_name);

    void setup_graph(QString title, QMainWindow *window);

};

#endif // GRAPHCONTROLLER_H
