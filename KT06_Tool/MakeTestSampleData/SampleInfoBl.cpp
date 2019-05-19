#include "SampleInfoBl.h"
#include "SqliteDbHelper.h"
#include <QDebug>
#include <QTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QByteArray>
#include <math.h>
#include <ctime>
#include <time.h>
#include <sys/time.h>


const QString COUNT_FIELDS[21] = {
    "WBC",
    "Lym","Mid","Gran",
    "Lym1","Mid1","Gran1",
    "RBC","HGB","HCT","MCV","MCH","MCHC",
    "RDWCV","RDWSD",
    "PLT","MPV","PDW","PCT","PLCC","PLCR"
};


SampleInfoBl::SampleInfoBl(QObject *parent) : QObject(parent)
{

}

bool SampleInfoBl::deleteAllSampleInfo()
{
    //1.Delete DB
    static QString delSqlArr[] = {
        "DELETE FROM SampleInfo ",
        "DELETE FROM SampleInfoOrg ",
        "DELETE FROM MicroscopyInfoTable ",
        "DELETE FROM HistoReferGroup"
    };
    const static int delSqlSize = sizeof(delSqlArr) / sizeof(delSqlArr[0]);

    SqliteDbHelper::Instance()->startTransaction();
    QSqlQuery query(SqliteDbHelper::Instance()->getDB());
    bool isError = false;
    for (int delSqlIndex = 0; delSqlIndex < delSqlSize; ++delSqlIndex)
    {
        if (!query.exec(delSqlArr[delSqlIndex]))
        {
            isError = true;
            break;
        }
    }

    if (isError)
    {
        qDebug() << query.lastError().text();
        SqliteDbHelper::Instance()->rollback();
        return false;
    }
    else
    {
        SqliteDbHelper::Instance()->commit();
        return true;
    }
}

bool SampleInfoBl::insertSampleInfo(QString sampleNum)
{
   int sampleId = -1;
    const static QString INSERT_SQL =
            " INSERT INTO SampleInfo( "
            " Age,AgeType,Sex,PatientType,ReferGroupId,ReferGroup, "
            " Name,MedicalRecordNum,Department,BedNum,SampleNum, "
            " Inspection,Inspector,Examiner,Remarks,Birth, "
            " TestTime,SampleTime,InspecTime,AnalysisMode,SampleMode, "
            " ResultStateExamine,ResultStatePrint,ResultStatecommunication, "
            " WBC, "
            " Lym,Mid,Gran, "
            " Lym1,Mid1,Gran1, "
            " RBC,HGB,HCT,MCV,MCH,MCHC,RDWCV,RDWSD, "
            " PLT,MPV,PDW,PCT,PLCC,PLCR, "
            " ThresholdFlag,ModifyFlag,AlarmFlag, "
            " SpecialInfoPath,SampleMapDatePath,SamplemapOtherDatePath, "
            " RbcHisGraph,RbcHisGraphbegin,RbcHisGraphend, "
            " PltHisGraph,PltHisGraphbegin,PltHisGraphend, "
            " WbcHisGraph,WbcHisGraphbegin,WbcHisGraphend,WbcHisGraphDiv1,WbcHisGraphDiv2) "
            " VALUES ( "
            " :Age, :AgeType, :Sex, :PatientType,:ReferGroupId, :ReferGroup, "
            " :Name, :MedicalRecordNum, :Department, :BedNum, :SampleNum, "
            " :Inspection,:Inspector,:Examiner,:Remarks,:Birth, "
            " :TestTime,:SampleTime,:InspecTime,:AnalysisMode,:SampleMode, "
            " :ResultStateExamine,:ResultStatePrint,:ResultStatecommunication, "
            " :WBC, "
            " :Lym,:Mid,:Gran, "
            " :Lym1,:Mid1,:Gran1, "
            " :RBC,:HGB,:HCT,:MCV,:MCH,:MCHC,:RDWCV,:RDWSD, "
            " :PLT,:MPV,:PDW,:PCT,:PLCC,:PLCR, "
            " :ThresholdFlag,:ModifyFlag,:AlarmFlag, "
            " :SpecialInfoPath,:SampleMapDatePath,:SamplemapOtherDatePath, "
            " :RbcHisGraph,:RbcHisGraphbegin,:RbcHisGraphend, "
            " :PltHisGraph,:PltHisGraphbegin,:PltHisGraphend, "
            " :WbcHisGraph,:WbcHisGraphbegin,:WbcHisGraphend,:WbcHisGraphDiv1,:WbcHisGraphDiv2) ";
    QSqlQuery query(SqliteDbHelper::Instance()->getDB());
    query.prepare(INSERT_SQL);
    //1.Bind Detail
    query.bindValue(":Age", -1);
    query.bindValue(":AgeType", 1);
    query.bindValue(":Sex", 0);
    query.bindValue(":PatientType", 0);
    query.bindValue(":ReferGroupId", 1);
    query.bindValue(":ReferGroup", QObject::trUtf8("通用"));
    query.bindValue(":Name", "");
    query.bindValue(":MedicalRecordNum", "");
    query.bindValue(":Department", "");
    query.bindValue(":BedNum", "");
    query.bindValue(":SampleNum", sampleNum);
    query.bindValue(":Inspection", "");
    query.bindValue(":Inspector", "");
    query.bindValue(":Examiner", "");
    query.bindValue(":Remarks", "");
    query.bindValue(":Birth", "1900-01-01");


    QDateTime testTime = generateTime();
    query.bindValue(":TestTime", testTime);
    query.bindValue(":SampleTime", "1900-01-01T00:00:00");
    query.bindValue(":InspecTime", "1900-01-01T00:00:00");
    query.bindValue(":AnalysisMode", 1);
    query.bindValue(":SampleMode", 1);
    query.bindValue(":ResultStateExamine", 0);
    query.bindValue(":ResultStatePrint", 0);
    query.bindValue(":ResultStatecommunication", 0);

    this->setTestData();
    //2.Bind Count
    for (int countIdx = 0; countIdx < 21; ++countIdx)
    {
        query.bindValue(":" + COUNT_FIELDS[countIdx], m_DbResult[countIdx]);
    }
    query.bindValue(":ThresholdFlag", 0);
    query.bindValue(":ModifyFlag", 0);
    query.bindValue(":AlarmFlag" , 0);

    QByteArray wbcHisGraph = QByteArray::fromHex("00000000000000000000000000010205090f161f29343f4a545d666e767e879098a1aab3bbc3cbd3d9dfe3e6e8e8e6e3ded8d0c7bdb2a69b8f847970675f58534e4a4745444444454648494b4d4e505355585c5f63676c7074787c8185898d9094989ca1a6abb1b7bdc3c8ced3d7dcdfe3e6e9ebedeff0f2f3f4f6f7f9fbfcfdfefffefefcfaf6f2ede8e3ddd8d2cdc8c3bdb8b2aca6a19b95908a847d77716b65605b56514d494643403e3b393634312e2b292623211f1d1b1a1817161514131211100f0e0e0d0d0c0c0c0c0c0c0b0b0b0b0b0b0b0b0b0a0a090807060504030201010000000000000000000000000000000000000000000000000000000000");
    QByteArray rbcHisGraph = QByteArray::fromHex("0000000108132130383e44494d4d4b4b4b4b4d4d4d4c4b4846423f3d3c3b3835323333333535343333343435353535343437383e41454d525a606368696d7277828b949ca4a9adb4bbc3ccd3dbe3e4e6eaeff7fdfffdfcf6f3ece7e4e2dfd7d1ccc8c0bbb4afaba29d948c867e7770666058504b4644423e3a35312d282622201e1a191615141312100e0e0d0c0d0c0b0a0a0808070606050505050505050505040303030303030303030303020303020303030303030202020202020101010101010101010101010101010101010100000000010100000001010100000000000000000000000000000101010000000000000000000000000000000101010100");
    QByteArray pltHisGraph = QByteArray::fromHex("000f3f99e8ffd0905e3e2c231d1916131211100f0f0d0c0b0b0c0b0b0b0a0a0a0a0a090909090909090909090a0a0a0a0a0a0908080808070707060607060502");

    for(int i = 0; i < 256; i++)
    {
        int temp = rand()%10;
        if(temp == 0){
            temp == 1;
        }
        double factor = temp/10.0;
        int value = 0;

        if(i < 128)
        {
            value = pltHisGraph[i];
            value = (int)(factor*value);
            pltHisGraph[i] = value;
        }

        value = wbcHisGraph[i];
        value = (int)(factor*value);
        wbcHisGraph[i] = value;

        value = rbcHisGraph[i];
        value = (int)(factor*value);
        rbcHisGraph[i] = value;

    }





    //3.Bind Graph
    query.bindValue(":SpecialInfoPath", "");
    query.bindValue(":SampleMapDatePath", "");
    query.bindValue(":SamplemapOtherDatePath", "");
    query.bindValue(":RbcHisGraph", rbcHisGraph);
    query.bindValue(":RbcHisGraphbegin", 30);
    query.bindValue(":RbcHisGraphend", 227);
    query.bindValue(":PltHisGraph", pltHisGraph);
    query.bindValue(":PltHisGraphbegin", 5);
    query.bindValue(":PltHisGraphend", 60);
    query.bindValue(":WbcHisGraph", wbcHisGraph);
    query.bindValue(":WbcHisGraphbegin", 13);
    query.bindValue(":WbcHisGraphend", 225);
    query.bindValue(":WbcHisGraphDiv1", 49);
    query.bindValue(":WbcHisGraphDiv2", 67);

    if (query.exec())
    {
        if (query.exec(" SELECT MAX(ID) AS MAX_ID FROM SampleInfo "))
        {
            if (query.next())
            {
                int maxIdIndex = query.record().indexOf("MAX_ID");
                sampleId = query.value(maxIdIndex).toInt();
            }
        }
        else
        {
            qDebug() << query.lastError().text();
        }
        return true;
    }
    else
    {
         qDebug() << query.lastError().text();
    }
    sampleId = -1;
    return false;
}

void SampleInfoBl::setTestData()
{
    float arr[21] = {7.40,4.04,1.99,0.68,54.6,26.9,9.2,4.32,127,40.8,94.4,29.4,311,16.7,54.5,235,11.2,15.5,0.270,34.9,82};

    //生成模拟的测试数据
    QVector<QString> testParam;
    testParam.clear();
    for(int i = 0;i < 21;i++)
    {
        QString para = QString::number((arr[i] + getRandomData(2)),'f',2);
        testParam.append(para);
    }

    memset(countResult,0x00, 21);

    //存入计数结果
    for(int i = 0; i < 21; i++)
    {
        countResult[i] = testParam[i].toDouble();
    }

    countResult[4] = 60;
    countResult[5] = 20;
    countResult[6] = 20;

    countResult[1] = countResult[0] * 0.60;
    countResult[2] = countResult[0] * 0.20;
    countResult[3] = countResult[0] * 0.20;

    memset(m_DbResult,0x00, 21);
    for(int i = 0; i < 21; i++)
    {
        m_DbResult[i] = (int)(countResult[i]*1000.0);
    };
}

QDateTime SampleInfoBl::generateTime()
{
    int timeBegin,timeNow,t;
    QDateTime time=QDateTime::currentDateTime();//获取当前时间
    QDateTime timeFinal;
    timeNow=time.toTime_t();//时间转换为时间戳
    timeBegin=1483536993;//https://tool.lu/timestamp/这个时间戳转换网站
    t=rand()%(timeNow-timeBegin); //获取一个随机数
    t=timeNow-t;//当前时间减去随机数得到目标随机时间的时间戳
    timeFinal=QDateTime::fromTime_t(t);//时间！
    timeFinal= timeFinal.addDays(-1*(rand()%30));
    timeFinal = timeFinal.addDays(1*(rand()%30));
    timeFinal = timeFinal.addSecs(rand()%1000000);
    return timeFinal;
}

float SampleInfoBl::getRandomData(float num)
{
    float tmp;
    struct timeval tpstart;
    gettimeofday(&tpstart,NULL);
    srand(tpstart.tv_usec);
    tmp = 1+(float) ((num +100.0)*rand()/(RAND_MAX+1.0));
    return (tmp/80);
}
