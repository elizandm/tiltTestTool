#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>

#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QLayout>
#include <QtGlobal>

#include <QtCore/QtMath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    data_provider(new DataProvider()),
    graph_controller(new GraphController(data_provider)),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_browse_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open record"), QString(), tr("Text files (*.txt *.text)"));

    ui->lineEdit_import_file->setText(fileName);
}


void MainWindow::on_btn_import_clicked()
{
    QString filename = ui->lineEdit_import_file->text();

    if (!data_provider->loadFile(filename))
        QMessageBox::information(this, "Import failed", "File doesn't exists");

    update_dev_id_combobox();
    graph_controller->autoGenOffsets();

    graph_controller->showTemperatureShiftGraph();
    graph_controller->ShowXShiftGraph();
    graph_controller->ShowYShiftGraph();

    graph_controller->ShowXGraph();
    graph_controller->ShowYGraph();
    graph_controller->ShowZGraph();

}


void MainWindow::update_dev_id_combobox()
{
    QList<int> recIds = data_provider->Records().keys();

    ui->comboBox_device->clear();

    foreach (int id, recIds) {
        ui->comboBox_device->addItem(QString("Device %1").arg(id), id);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    graph_controller.reset();
    QMainWindow::closeEvent(event);
}

void MainWindow::on_doubleSpinBox_x_offset_valueChanged(double arg1)
{
    int id = ui->comboBox_device->currentData().toInt();
    data_provider->setDeviceOffset(id, OFFSET_X, arg1);
}

void MainWindow::on_doubleSpinBox_y_offset_valueChanged(double arg1)
{
    int id = ui->comboBox_device->currentData().toInt();
    data_provider->setDeviceOffset(id, OFFSET_Y, arg1);
}

void MainWindow::on_doubleSpinBox_z_offset_valueChanged(double arg1)
{
    int id = ui->comboBox_device->currentData().toInt();
    data_provider->setDeviceOffset(id, OFFSET_Z, arg1);
    qDebug() << "nnnne "<<arg1;
}

void MainWindow::on_doubleSpinBox_temp_offset_valueChanged(double arg1)
{
    int id = ui->comboBox_device->currentData().toInt();
    data_provider->setDeviceOffset(id, OFFSET_TEMPERATURE, arg1);
}

void MainWindow::on_comboBox_device_currentIndexChanged(int)
{
    int id = ui->comboBox_device->currentData().toInt();
    ui->doubleSpinBox_x_offset->setValue(data_provider->deviceXOffset(id));
    ui->doubleSpinBox_y_offset->setValue(data_provider->deviceYOffset(id));
    ui->doubleSpinBox_z_offset->setValue(data_provider->deviceZOffset(id));
    ui->doubleSpinBox_temp_offset->setValue(data_provider->deviceTempOffset(id));

    ui->X_Scale_spin->setValue(data_provider->deviceXScale(id));
    ui->Y_Scale_Spin->setValue(data_provider->deviceYScale(id));
    ui->Z_Scale_Spin->setValue(data_provider->deviceZScale(id));
}


void MainWindow::on_X_Scale_spin_valueChanged(double arg1)
{
    int id = ui->comboBox_device->currentData().toInt();
    data_provider->setDeviceOffset(id, SCALE_X, arg1);
    qDebug() << "xscale "<<arg1;

}

void MainWindow::on_Y_Scale_Spin_valueChanged(double arg1)
{
    int id = ui->comboBox_device->currentData().toInt();
    data_provider->setDeviceOffset(id, SCALE_Y, arg1);
    qDebug() << "yscale "<<arg1;

}

void MainWindow::on_Z_Scale_Spin_valueChanged(double arg1)
{
    int id = ui->comboBox_device->currentData().toInt();
    data_provider->setDeviceOffset(id, SCALE_Z, arg1);
    qDebug() << "zscale "<<arg1;

}

void MainWindow::on_checkBox_2_toggled(bool checked)
{
    qDebug() << "box 2 "<<checked;

}

void MainWindow::on_checkBox_3_toggled(bool checked)
{
    qDebug() << "box 3 "<<checked;

}

void MainWindow::on_checkBox_4_toggled(bool checked)
{
    qDebug() << "box 4 "<<checked;
}

void MainWindow::on_checkBox_5_toggled(bool checked)
{
    qDebug() << "box 5 "<<checked;
}

void MainWindow::on_checkBox_6_toggled(bool checked)
{
    qDebug() << "box 6 "<<checked;
}



void MainWindow::on_checkBox_7_toggled(bool checked)
{

}

void MainWindow::on_checkBox_8_toggled(bool checked)
{

}

void MainWindow::on_checkBox_9_toggled(bool checked)
{

}

void MainWindow::on_checkBox_A_toggled(bool checked)
{

}

void MainWindow::on_checkBox_B_toggled(bool checked)
{

}

void MainWindow::on_checkBox_C_toggled(bool checked)
{

}

void MainWindow::on_checkBox_D_toggled(bool checked)
{

}

void MainWindow::on_checkBox_E_toggled(bool checked)
{

}

void MainWindow::on_checkBox_F_toggled(bool checked)
{

}
