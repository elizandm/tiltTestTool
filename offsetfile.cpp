#include "offsetfile.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Default offsets... (Probs delete later or save as json?).
//static double temp_offsets[] = {0, 0, 1.4, 0.1, 0.2, -0.1, 0.9, 0.8, 0.7, 0.6, -0.3, -0.5, 0.7, 0.3, 0.3, 0.2};
//static double x_offsets[] = {0, 0, 0, 1.53, -0.08, 0.475, 1.35 , 1.05, -0.26, 0.53 , 1.25  , 0.65, 1.15, 1.13, -0.02, 0.57 };
//static double y_offsets[] = {0, 0, 0, 0.31, -0.65, -0.7 , -0.53, -1.2, -0.17, -1.18, -0.335, -0.7, -0.2, -1.1, -0.74, -1.26};
//static double z_offsets[] = {0, 0, 0, 0.27, 0.6  , 0.5  , -0.6 , -1  , -0.1 , 0.3  , -0.15 , -0.1, 0.0 , 0.0 , 0.0  , 0.0  };

OffsetFile::OffsetFile(QString filename) :
    filename(filename)
{
    qDebug() << "file" << filename;
    open();
}

void OffsetFile::setDeviceOffset(int devId, OffsetType type, double offset)
{
    if (!offsets.contains(devId))
        offsets.insert(devId, Offsets());

    if (type == OFFSET_X)
        offsets[devId].x_offset = offset;

    if (type == OFFSET_Y)
        offsets[devId].y_offset = offset;

    if (type == OFFSET_Z)
        offsets[devId].z_offset = offset;

    if (type == OFFSET_TEMPERATURE)
        offsets[devId].temperature_offset = offset;

    if (type == BASE_TEMPERATURE)
        offsets[devId].base_temperature = offset;

    save_file();
}

double OffsetFile::getDeviceOffset(int devId, OffsetType type)
{
    if (!offsets.contains(devId))
        return 0;

    if (type == OFFSET_X)
        return offsets.value(devId).x_offset;

    if (type == OFFSET_Y)
        return offsets.value(devId).y_offset;

    if (type == OFFSET_Z)
        return offsets.value(devId).z_offset;

    if (type == OFFSET_TEMPERATURE)
        return offsets.value(devId).temperature_offset;

    if (type == BASE_TEMPERATURE)
        return offsets.value(devId).base_temperature;

    return 0;
}

bool OffsetFile::open()
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    if (doc.isEmpty() || !doc.isArray())
        return false;

    // Array of QJsonObjects...

    foreach (QJsonValue c, doc.array()) {
        QJsonObject obj = c.toObject();
        Offsets os;
        int id = obj.value("DeviceID").toInt();
        os.x_offset = obj.value("xOffset").toDouble();
        os.y_offset = obj.value("yOffset").toDouble();
        os.z_offset = obj.value("zOffset").toDouble();
        os.temperature_offset = obj.value("tempOffset").toDouble();
        os.base_temperature = obj.value("basetemp").toDouble();
        offsets.insert(id, os);
    }

    return true;
}

bool OffsetFile::save_file()
{
    qDebug() << "Saving";

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "or not";
        return false;
    }

    QJsonArray array;

    QMapIterator<int, Offsets> i(offsets);
    while (i.hasNext()) {
        i.next();

        QJsonObject obj;
        obj.insert("DeviceID", i.key());
        obj.insert("xOffset", i.value().x_offset);
        obj.insert("yOffset", i.value().y_offset);
        obj.insert("zOffset", i.value().z_offset);
        obj.insert("tempOffset", i.value().temperature_offset);
        array.append(obj);
    }

    QJsonDocument doc(array);

    file.write(doc.toJson());
    file.close();

    return true;
}
