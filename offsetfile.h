#ifndef OFFSETFILE_H
#define OFFSETFILE_H

#include <QObject>
#include <QSharedPointer>
#include <QMap>
#include <QDebug>

enum OffsetType { OFFSET_X, OFFSET_Y, OFFSET_Z, OFFSET_TEMPERATURE , BASE_TEMPERATURE };

class OffsetFile
{
public:
    OffsetFile(QString filename);

    void setDeviceOffset(int devId, OffsetType type, double offset);
    double getDeviceOffset(int devId, OffsetType type);

private:

    struct Offsets
    {
        Offsets() :
            x_offset(0),
            y_offset(0),
            z_offset(0),
            temperature_offset(0),
            base_temperature(0){
        }

        double x_offset;
        double y_offset;
        double z_offset;
        double temperature_offset;
        double base_temperature;
    };

    QString filename;
    QMap<int, Offsets> offsets;

    bool open();
    bool save_file();

};

#endif // OFFSETFILE_H
