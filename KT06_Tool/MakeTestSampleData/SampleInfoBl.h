#ifndef SAMPLEINFOBL_H
#define SAMPLEINFOBL_H

#include <QObject>
#include <QString>
#include <QDateTime>

class SampleInfoBl : public QObject
{
    Q_OBJECT
public:
    explicit SampleInfoBl(QObject *parent = nullptr);


    bool deleteAllSampleInfo();
    bool insertSampleInfo(QString sampleNum);
    void setTestData();

    QDateTime generateTime();

signals:

public slots:


private:
   float getRandomData(float num);

   double countResult[21];  //计数结果
   int    m_DbResult[21];

};

#endif // SAMPLEINFOBL_H
