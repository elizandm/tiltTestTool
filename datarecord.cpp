#include "datarecord.h"

#define M_PI 3.14159265358979323846  /* pi */

static double x_offsets[] = {0, 0, 0, 0,    0.0174,     0.0174,     0.0134 ,    0.025,   0.0074,     0.0263 ,    0.0098  ,  0.0093,     0.024,      0.0165,     0.0255, 0.0255};
static double y_offsets[] = {0, 0, 0, 0,    -0.027,     -0.018,     -0.003 ,    -0.009,  -0.032,     -0.017,    -0.006  ,   -0.008,     -0.007,     -0.027,     -0.017, -0.017};
static double z_offsets[] = {0, 0, 0, 0,    -0.001244,  -0.001,     -0.00058 ,  -0.001,  -0.001,     -0.001 ,    -0.00052,  -0.00063,   -0.0001,    -0.0001,    -0.001, -0.001};


DataRecord::DataRecord(QString raw_data_str) :
    raw_data(raw_data_str),
    temp_offset(0),
    x_offset_deg(0),
    y_offset_deg(0),
    z_offset_deg(0),

      x_scale(0),
      y_scale(0),
      z_scale(0)
{
    extract_timestamp();
    extract_id();
    calculateData();
}

int DataRecord::DeviceID() const
{
    return devId;
}

QDateTime DataRecord::Timestamp() const
{
    return timestamp;
}

double DataRecord::xRotation() const
{
    return final_x_rotaton_deg;
}

double DataRecord::yRotation() const
{
    return final_y_rotaton_deg;
}

double DataRecord::xValue() const
{
    return Ax;
}

double DataRecord::yValue() const
{
    return Ay;
}

double DataRecord::zValue() const
{
    return Az;
}


double DataRecord::Temperature() const
{
    return temperature;
}

void DataRecord::setXoffset(double offset)
{
    //qDebug() << "devID" << devId<< "X " << offset ;
    x_offset_deg = offset;
    calculateData();
}

void DataRecord::setYoffset(double offset)
{
    y_offset_deg = offset;
    calculateData();
}

void DataRecord::setZoffset(double offset)
{
    z_offset_deg = offset;
    calculateData();
}

void DataRecord::setTempOffset(double offset)
{
    temp_offset = offset;
    calculateData();
}

void DataRecord::setBaseTemp(double offset)
{
    temp_base = offset;
    calculateData();
}

void DataRecord::setXscale(double offset)
{
    x_scale = offset;
    calculateData();
}

void DataRecord::setYscale(double offset)
{
    y_scale = offset;
    calculateData();
}

void DataRecord::setZscale(double offset)
{
    z_scale = offset;
    calculateData();
}


void DataRecord::extract_timestamp()
{
    QString date_section = raw_data.mid(0,10);
    QDate date = QDate::fromString(date_section, "yyyy-MM-dd");

    QString time_section = raw_data.mid(11,8);
    QTime time = QTime::fromString(time_section, "HH:mm:ss");

    timestamp = QDateTime(date, time);
}

void DataRecord::extract_id()
{
    QString str = raw_data.mid(83,2);
    bool ok;
    devId = str.toUInt(&ok, 16);

    if (!ok) {
        devId = -1;
        qDebug() << "Failed to get Id";
    }
}

void DataRecord::extract_x()
{
    QString str = raw_data.mid(91);

    QString xstr = str.mid(6,2) + str.mid(4,2);

    bool ok;
    int raw_x = xstr.toUInt(&ok, 16);

    if (!ok) {
        qDebug() << "Failed to get raw x";
        x = 0;
        return;
    }

    x = ((raw_x > (pow(2, 15) - 1)) ? raw_x - pow(2, 16) : raw_x);
}

void DataRecord::extract_y()
{
    QString str = raw_data.mid(91);

    QString ystr = str.mid(10,2) + str.mid(8,2);

    bool ok;
    int raw_y = ystr.toUInt(&ok, 16);

    if (!ok) {
        qDebug() << "Failed to get raw y";
        y = 0;
        return;
    }

    y = ((raw_y > (pow(2, 15) - 1)) ? raw_y - pow(2, 16) : raw_y);
}

void DataRecord::extract_z()
{
    QString str = raw_data.mid(91);

    QString zstr = str.mid(14,2) + str.mid(12,2);

    bool ok;
    int raw_z = zstr.toUInt(&ok, 16);

    if (!ok) {
        qDebug() << "Failed to get raw z";
        z = 0;
        return;
    }

    z = ((raw_z > (pow(2, 15) - 1)) ? raw_z - pow(2, 16) : raw_z);

}

void DataRecord::extract_temperature()
{
    QString str = raw_data.mid(91);

    QString temp_str = str.mid(30,2) + str.mid(28,2);

    bool ok;
    double raw_temp = temp_str.toUInt(&ok, 16);

    if (!ok) {
        qDebug() << "Failed to get raw temp";
        temperature = 0;
        return;
    }

    temperature = 25.0 + raw_temp/10.0+ temp_offset;
    //qDebug() << "temp offset "<< temp_offset;
}

void DataRecord::extract_rss()
{
    long long x_sqaured = pow(x,2);
    long long y_sqaured = pow(y,2);
    long long z_sqaured = pow(z,2);
    long long sqaure_total = x_sqaured + y_sqaured + z_sqaured;
    rss = sqrt(sqaure_total);
}

void DataRecord::calculate_a_xyz_values()
{
    Ax = x / rss;
    Ay = y / rss;
    Az = z / rss;
    //qDebug() << "device ID" << devId;
    //qDebug() << "prior    devID" << devId<< "X " << averageX[devId] <<"Y " <<averageY[devId]<<"Z " <<averageZ[devId]<<"i "<<averagei[devId];

    averageX[devId] = averageX[devId]+Ax;
    averageY[devId] = averageY[devId]+Ay;
    averageZ[devId] = averageZ[devId]+Az;
    averagei[devId] = averageX[devId]+1;
}

void DataRecord::calculate_rotations_on_x()
{
    //  1 | 0                   | 0
    //  0 | cos(xRadOffSet)     | sin(xRadOffSet)
    //  0 | -sin(xRadOffSet)    | cos(xRadOffSet)
    rot_on_x_x = Ax;
    rot_on_x_y = cos(x_offset_rad) * Ay + sin(x_offset_rad) * Az;
    rot_on_x_z = -sin(x_offset_rad) * Ay + cos(x_offset_rad) * Az;
}

void DataRecord::calculate_rotations_on_y()
{
    //  cos(yRadOffSet) | 0 | -sin(yRadOffSet)
    //  0               | 1 | 0
    //  sin(yRadOffSet) | 0 | cos(yRadOffSet)
    rot_on_y_x = cos(y_offset_rad) * rot_on_x_x + -sin(y_offset_rad) * rot_on_x_z;
    rot_on_y_y = rot_on_x_y;
    rot_on_y_z = sin(y_offset_rad) * rot_on_x_x + cos(y_offset_rad) * rot_on_x_z;
}

void DataRecord::calculate_rotations_on_z()
{
    //  cos(zRadOffSet)     | sin(zRadOffSet)   | 0
    //  -sin(zRadOffSet)    | cos(zRadOffSet)   | 0
    //  0                   | 0                 | 1
    rot_on_z_x = cos(z_offset_rad) * rot_on_y_x + sin(z_offset_rad) * rot_on_y_y;
    rot_on_z_y = -sin(z_offset_rad) * rot_on_y_x + cos(z_offset_rad) * rot_on_y_y;
    rot_on_z_z = rot_on_y_z;
}

void DataRecord::calculate_final_x_rot()
{
    double pow_z_rot = pow(rot_on_z_z, 2);
    double pow_x_rot = pow(rot_on_z_x, 2);

    final_x_rotaton_rad = tan( rot_on_z_y / pow_z_rot + 0.00001 * pow_x_rot  );
    final_x_rotaton_deg = final_x_rotaton_rad * 180 / M_PI;

}

void DataRecord::calculate_final_y_rot()
{
    double pow_y_rot = pow(rot_on_z_y, 2);
    double pow_z_rot = pow(rot_on_z_z, 2);
    final_y_rotaton_rad = tan( (-rot_on_z_x) / sqrt( pow_y_rot + pow_z_rot) );
    final_y_rotaton_deg = final_y_rotaton_rad * 180 / M_PI;
}



void DataRecord::calculate_final_Ax()
{
    Ax = (Ax+ x_offsets[devId])*(1+x_scale);

}

void DataRecord::calculate_final_Ay()
{
    Ay = (Ay + y_offsets[devId])*(1+y_scale);
    qDebug() << "dz" << y_scale;
}

void DataRecord::calculate_final_Az()
{
    Az = (Az+ z_offsets[devId])*(1+z_scale);
    qDebug() << "dz" << z_scale;


}



void DataRecord::calculateData()
{
    x_offset_rad = x_offset_deg * M_PI / 180.0;
    y_offset_rad = y_offset_deg * M_PI / 180.0;
    z_offset_rad = z_offset_deg * M_PI / 180.0;

    extract_x();
    extract_y();
    extract_z();
    extract_temperature();
    extract_rss();

    calculate_a_xyz_values();

    // Must be called in this order.
    calculate_rotations_on_x();
    calculate_rotations_on_y();
    calculate_rotations_on_z();

    calculate_final_x_rot();
    calculate_final_y_rot();

    calculate_final_Ax();
    calculate_final_Ay();
    calculate_final_Az();
}
