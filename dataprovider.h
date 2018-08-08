#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <QObject>
#include "datarecord.h"
#include "offsetfile.h"

class DataProvider : public QObject
{
    Q_OBJECT
public:
    explicit DataProvider(QObject *parent = NULL);

    bool loadFile(QString filename);

    const QMap<int, QList<DataRecord*> > Records();
    const QList<DataRecord*> SortedRecords();

    void setDeviceOffset(int devId, OffsetType type, double offset);

    double deviceXOffset(int devId);
    double deviceYOffset(int devId);
    double deviceZOffset(int devId);

    double deviceXScale(int devId);
    double deviceYScale(int devId);
    double deviceZScale(int devId);

    double deviceTempOffset(int devId);

   double deviceBaseTemp(int devId);

//    void autoOffsets(int referenceDev);

signals:
    void dataChanged();

private:
    QMap<int, QList<DataRecord*> > records;
    QList<DataRecord*> sorted_records;
    OffsetFile offsets;

    int reference_device_id;

    void sort_records();

};

#endif // DATAPROVIDER_H
