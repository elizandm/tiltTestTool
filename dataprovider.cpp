#include "dataprovider.h"

DataProvider::DataProvider(QObject *parent) :
    QObject(parent),
    reference_device_id(-1),
    offsets("offsets.json")
{
}

bool DataProvider::loadFile(QString filename)
{
    QFile file(filename);
    QStringList data_lines;

    if (!file.exists() || !file.open(QFile::ReadOnly))
        return false;

    qDeleteAll(sorted_records);
    sorted_records.clear();
    records.clear();

    QString data(file.readAll());
    QStringList all_lines = data.split((QRegExp("\n|\r\n|\r")));

    foreach (QString line, all_lines) {
        if (line.contains("RF N Packet: ") && line.contains("01045411")) {
            data_lines.append(line);

            DataRecord *record = new DataRecord(line);
            int devId = record->DeviceID();

            if (!records.contains(devId))
                records.insert(devId, QList<DataRecord*>());

            record->setXoffset(offsets.getDeviceOffset(devId, OFFSET_X));
            record->setYoffset(offsets.getDeviceOffset(devId, OFFSET_Y));
            record->setZoffset(offsets.getDeviceOffset(devId, OFFSET_Z));
            record->setTempOffset(offsets.getDeviceOffset(devId, OFFSET_TEMPERATURE));
            record->setTempOffset(offsets.getDeviceOffset(devId, SCALE_X));
            record->setTempOffset(offsets.getDeviceOffset(devId, SCALE_Y));
            record->setTempOffset(offsets.getDeviceOffset(devId, SCALE_Z));
            records[devId].append(record);
            sorted_records.append(record);
        }
    }

    sort_records();

    emit dataChanged();
    return true;
}

const QMap<int, QList<DataRecord *> > DataProvider::Records()
{
    return records;
}

const QList<DataRecord *> DataProvider::SortedRecords()
{
    return sorted_records;
}

void DataProvider::setDeviceOffset(int devId, OffsetType type, double offset)
{
    if (!records.contains(devId))
        return;

    foreach (DataRecord *rec, records.value(devId)) {

        if (type == OFFSET_X)
            rec->setXoffset(offset);
        if (type == OFFSET_Y)
            rec->setYoffset(offset);
        if (type == OFFSET_Z)
            rec->setZoffset(offset);
        if (type == OFFSET_TEMPERATURE)
            rec->setTempOffset(offset);
        if (type == BASE_TEMPERATURE)
            rec->setBaseTemp(offset);
        if (type == SCALE_X)
            rec->setXscale(offset);
        if (type == SCALE_Y)
            rec->setYscale(offset);
        if (type == SCALE_Z)
            rec->setZscale(offset);
    }

    offsets.setDeviceOffset(devId, type, offset);
    emit dataChanged();
}

double DataProvider::deviceXOffset(int devId)
{
    return offsets.getDeviceOffset(devId, OFFSET_X);
}

double DataProvider::deviceYOffset(int devId)
{
    return offsets.getDeviceOffset(devId, OFFSET_Y);
}

double DataProvider::deviceZOffset(int devId)
{
    return offsets.getDeviceOffset(devId, OFFSET_Z);
}

double DataProvider::deviceTempOffset(int devId)
{
    return offsets.getDeviceOffset(devId, OFFSET_TEMPERATURE);
}

double DataProvider::deviceXScale(int devId)
{
    return offsets.getDeviceOffset(devId, SCALE_X);

}

double DataProvider::deviceYScale(int devId)
{
    return offsets.getDeviceOffset(devId, SCALE_Y);

}
double DataProvider::deviceZScale(int devId)
{
    return offsets.getDeviceOffset(devId, SCALE_Z);
}

bool PComp(DataRecord *a, DataRecord *b)
{
   return (a->Timestamp()) < (b->Timestamp());
}

void DataProvider::sort_records()
{
    foreach (QList<DataRecord*> dev_recs, records.values()) {
        std::sort(dev_recs.begin(), dev_recs.end(), PComp);
    }

    std::sort(sorted_records.begin(), sorted_records.end(), PComp);
}
