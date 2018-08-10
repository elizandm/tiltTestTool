#include "graphcontroller.h"

GraphController::GraphController(QSharedPointer<DataProvider> dataProvider, QObject *parent) :
    QObject(parent),
    data_provider(dataProvider),
    temperature_graph(new QChart()),

    x_graph(new QChart()),
    y_graph(new QChart()),
    z_graph(new QChart()),

    x_drift_graph(new QChart()),
    y_drift_graph(new QChart()),

    x_window(new QMainWindow()),
    y_window(new QMainWindow()),
    z_window(new QMainWindow()),

    x_drift_window(new QMainWindow()),
    y_drift_window(new QMainWindow()),
    temperature_window(new QMainWindow())
{
    setup_graph("(THETA) X Axis ROTATION Title Graph", x_drift_window);
    setup_graph("(PHI) Y Axis ROTATION Title Graph", y_drift_window);

    setup_graph("X Axis Title Graph", x_window);
    setup_graph("Y Axis Title Graph", y_window);
    setup_graph("Z Axis Title Graph", z_window);

    setup_graph("Temperature Change Graph", temperature_window);

    if (data_provider) {
        connect(data_provider.data(), SIGNAL(dataChanged()), this, SLOT(updateGraphs()));
    }
}

GraphController::~GraphController()
{
    x_drift_window->close();
    y_drift_window->close();

    x_window->close();
    y_window->close();
    z_window->close();

    temperature_window->close();

    delete x_drift_window;
    delete y_drift_window;

    delete x_window;
    delete y_window;
    delete z_window;

    delete temperature_window;
}

void GraphController::ShowXShiftGraph()
{
    update_graph(x_drift_graph, GRAPH_X_DRIFT, "Rotation (°)");

    x_drift_graph->setTitle("NWSM X Axis ROTATION Drift Graph(THETA)");

    QChartView *chartView = new QChartView(x_drift_graph);
    chartView->setRenderHint(QPainter::Antialiasing);
    x_drift_window->setCentralWidget(chartView);
    x_drift_window->show();
}

void GraphController::ShowYShiftGraph()
{
    update_graph(y_drift_graph, GRAPH_Y_DRIFT, "Rotation (°)");
    y_drift_graph->setTitle("NWSM Y Axis ROTATION Drift Graph(PHI)");
    QChartView *chartView = new QChartView(y_drift_graph);
    chartView->setRenderHint(QPainter::Antialiasing);
    y_drift_window->setCentralWidget(chartView);
    y_drift_window->show();
}

void GraphController::showTemperatureShiftGraph()
{
    update_graph(temperature_graph, GRAPH_TEMPERATURE, "Temperature (°C)");
    temperature_graph->setTitle("NWSM Temperature Graph");
    QChartView *chartView = new QChartView(temperature_graph);
    chartView->setRenderHint(QPainter::Antialiasing);
    temperature_window->setCentralWidget(chartView);
    temperature_window->show();
}

void GraphController::ShowXGraph()
{
    update_graph(x_graph, GRAPH_X, "G");
    x_graph->setTitle("NWSM X Axis Drift Graph");
    QChartView *chartView = new QChartView(x_graph);
    chartView->setRenderHint(QPainter::Antialiasing);
    x_window->setCentralWidget(chartView);
    x_window->show();
}

void GraphController::ShowYGraph()
{
    update_graph(y_graph, GRAPH_Y, "G");
    y_graph->setTitle("NWSM Y Axis Drift Graph");
    QChartView *chartView = new QChartView(y_graph);
    chartView->setRenderHint(QPainter::Antialiasing);
    y_window->setCentralWidget(chartView);
    y_window->show();
}

void GraphController::ShowZGraph()
{
    update_graph(z_graph, GRAPH_Z, "G");
    z_graph->setTitle("NWSM Z Axis Drift Graph");
    QChartView *chartView = new QChartView(z_graph);
    chartView->setRenderHint(QPainter::Antialiasing);
    z_window->setCentralWidget(chartView);
    z_window->show();
}


void GraphController::autoGenOffsets()
{
    //QMap<int, QList<DataRecord *> > records = data_provider->Records();

    QList<DataRecord*> records = data_provider-> SortedRecords();

   // qDebug() << "found next"<< records<< " under key ";
        qDebug() << "found next"<< records<< " under key ";

    //QMapIterator<int, QList<DataRecord *> > i(records); //he is doing that here
    //while (i.hasNext()) {
      //  i.next();
        //qDebug() << "found next"<< i.next()[1]<< " under key ";


        //if (i.key() == 2)
          //  qDebug() << "Got here " << " under key ";

        //QList<DataRecord *> dev_recs = i.value();
   // }

}

void GraphController::updateGraphs()
{
    update_graph(temperature_graph, GRAPH_TEMPERATURE, "Temperature (°C)");
    update_graph(x_drift_graph, GRAPH_X_DRIFT, "Rotation (°)");
    update_graph(y_drift_graph, GRAPH_Y_DRIFT, "Rotation (°)");

    update_graph(x_graph, GRAPH_X, "Rotation (°)");
    update_graph(y_graph, GRAPH_Y, "Rotation (°)");
    update_graph(z_graph, GRAPH_Z, "Rotation (°)");

}

void GraphController::update_graph(QChart *graph, GraphController::GraphType graphType, QString x_axis_name)
{
    QList<DataRecord*> records = data_provider->SortedRecords();
    QDateTime oldestRec = records.first()->Timestamp(); // Zero.
    QMap<int, QLineSeries*> graphData;

    foreach (DataRecord *record, records) {
        //By changing this you can turn plots on and off
        /*
         * (record->DeviceID() == 3 ||record->DeviceID() == 2 ||
                record->DeviceID() == 4 ||record->DeviceID() == 5 ||
                record->DeviceID() == 6 ||record->DeviceID() == 7 ||
                record->DeviceID() == 8 ||record->DeviceID() == 9 ||
                record->DeviceID() == 10 ||record->DeviceID() == 11 ||
                record->DeviceID() == 13 ||record->DeviceID() == 12 ||
                record->DeviceID() == 14 || record->DeviceID() == 15)*/
        if (record->DeviceID() == 3 ||record->DeviceID() == 5 ||
                record->DeviceID() == 6 ||record->DeviceID() == 4 ||
                record->DeviceID() == 10 || record->DeviceID() == 15) {
            if (!graphData.contains(record->DeviceID())&& plot_on[record->DeviceID()-1]==record->DeviceID()) {//check if series exists in array
                    graphData.insert(record->DeviceID(), new QLineSeries());
                    graphData[record->DeviceID()]->setName(QString("Dev %1").arg(record->DeviceID()));

            }

        QLineSeries *series = graphData.value(record->DeviceID());
        double x = oldestRec.secsTo(record->Timestamp()) / 3600;

        double y;

        if (graphType == GRAPH_X_DRIFT)
            y = record->xRotation();

        else if (graphType == GRAPH_Y_DRIFT)
            y = record->yRotation();

        if (graphType == GRAPH_X)
            y = record->xValue();

        else if (graphType == GRAPH_Y)
            y = record->yValue();

        if (graphType == GRAPH_Z)
            y = record->zValue();

        else if (graphType == GRAPH_TEMPERATURE)
            y = record->Temperature();

        *series << QPointF((qreal)x,(qreal)y);
        }
    }

    graph->removeAllSeries();

    foreach (QLineSeries *series, graphData.values()) {
        graph->addSeries(series);
    }

    // Needed to update scales!
    graph->createDefaultAxes();
    graph->axisY()->setTitleText(x_axis_name);
    graph->axisX()->setTitleText(QString("Hours since %1").arg(oldestRec.toString()));
}

void GraphController::setup_graph(QString title, QMainWindow *window)
{
    window->setWindowTitle(title);
    window->resize(1000, 600);
    window->grabGesture(Qt::PanGesture);
    window->grabGesture(Qt::PinchGesture);
}
