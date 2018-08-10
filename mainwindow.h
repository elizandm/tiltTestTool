#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include "dataprovider.h"
#include <QSharedPointer>
#include "graphcontroller.h"
#include <QDoubleSpinBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_browse_clicked();
    void on_btn_import_clicked();
    void on_doubleSpinBox_x_offset_valueChanged(double arg1);
    void on_doubleSpinBox_y_offset_valueChanged(double arg1);
    void on_doubleSpinBox_z_offset_valueChanged(double arg1);
    void on_doubleSpinBox_temp_offset_valueChanged(double arg1);
    void on_comboBox_device_currentIndexChanged(int index);

    void on_X_Scale_spin_valueChanged(double arg1);

    void on_Y_Scale_Spin_valueChanged(double arg1);

    void on_Z_Scale_Spin_valueChanged(double arg1);

    void on_checkBox_2_toggled(bool checked);

    void on_checkBox_3_toggled(bool checked);

    void on_checkBox_4_toggled(bool checked);

    void on_checkBox_6_toggled(bool checked);

    void on_checkBox_5_toggled(bool checked);

    void on_checkBox_7_toggled(bool checked);

    void on_checkBox_8_toggled(bool checked);

    void on_checkBox_9_toggled(bool checked);

    void on_checkBox_A_toggled(bool checked);

    void on_checkBox_B_toggled(bool checked);

    void on_checkBox_C_toggled(bool checked);

    void on_checkBox_D_toggled(bool checked);

    void on_checkBox_E_toggled(bool checked);

    void on_checkBox_F_toggled(bool checked);

private:
    Ui::MainWindow *ui;

    QSharedPointer<DataProvider> data_provider;
    QSharedPointer<GraphController> graph_controller;

    void update_dev_id_combobox();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
