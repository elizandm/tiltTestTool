#ifndef DATARECORD_H
#define DATARECORD_H

#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <array>


class DataRecord
{
public:
    explicit DataRecord(QString raw_data_str);

    int DeviceID() const;
    QDateTime Timestamp() const;
    double xRotation() const;
    double yRotation() const;
    double xValue() const;
    double yValue() const;
    double zValue() const;
    double Temperature() const;

    void setXoffset(double offset);
    void setYoffset(double offset);
    void setZoffset(double offset);
    void setTempOffset(double offset);

    void setBaseTemp(double offset);


private:
    QString raw_data;
    QDateTime timestamp;
    int devId;
    double temperature;

    // Four values are manually calculated.
    double temp_offset;
    double temp_base;
    double x_offset_deg;
    double y_offset_deg;
    double z_offset_deg;
    double x_offset_rad;
    double y_offset_rad;
    double z_offset_rad;


    double x;
    double y;
    double z;
    double Ax;
    double Ay;
    double Az;
    double rss;

    //double averageX[16];
    //double averageY[16];
    //double averageZ[16];
    //double averagei[16];
    double averageX[16];
    double averageY[16];
    double averageZ[16];
    double averagei[16];


    // Rotation around x values
    double rot_on_x_x;
    double rot_on_x_y;
    double rot_on_x_z;

    // Rotation around x values
    double rot_on_y_x;
    double rot_on_y_y;
    double rot_on_y_z;

    // Rotation around x values
    double rot_on_z_x;
    double rot_on_z_y;
    double rot_on_z_z;

    // Poor names but don't know...
    double final_x_rotaton_deg;
    double final_y_rotaton_deg;
    double final_x_rotaton_rad;
    double final_y_rotaton_rad;

    // Private functions
    void extract_timestamp();
    void extract_id();
    void extract_x();
    void extract_y();
    void extract_z();
    void extract_temperature();
    void extract_rss();

    void calculate_a_xyz_values();
    void calculate_rotations_on_x();
    void calculate_rotations_on_y();
    void calculate_rotations_on_z();

    void calculate_offset_x();
    void calculate_offset_y();
    void calculate_offset_z();

    void calculate_final_x_rot();
    void calculate_final_y_rot();


    void calculate_final_Ax();
    void calculate_final_Ay();
    void calculate_final_Az();


    void calculateData();
};

#endif // DATARECORD_H
