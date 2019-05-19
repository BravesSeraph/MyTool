#include "SqliteDbHelper.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QList>

#define showDebug(x) qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << ": " << (x)


bool SqliteDbHelper::openDB(QString dbName)
{
    if (m_dbName.compare(dbName) == 0)
    {
        if (m_bOpen == false)
        {
            m_DB.setDatabaseName(dbName);
            m_bOpen = m_DB.open();
        }
    }
    else
    {
        if (m_bOpen)
        {
            this->closeDB();
        }
        m_dbName = dbName;
        m_DB.setDatabaseName(m_dbName);
        m_bOpen = m_DB.open();

    }

    QString msg = m_bOpen ? "Success":"Failure";
    qDebug() << "Open DataBase:" << m_dbName << " " << msg;

    return m_bOpen;
}

bool SqliteDbHelper::closeDB()
{
    if (m_DB.isOpen())
    {
        m_DB.close();

    }
    m_bOpen = false;
    return m_bOpen;
}

bool SqliteDbHelper::updateTable( QString tableName, const QVariantHash& vhash)
{
    if (!m_bOpen)
    {
        return false;
    }

    if(tableName.isEmpty() || vhash.size() < 1)
    {
        return false;
    }

    QSqlQuery sqlQuery(m_DB);
    QString sql = QString("update ")+tableName+QString(" set ");

    QList<QString> fieldNames = vhash.keys();
    for (int i = 0; i < fieldNames.size(); i++)
    {
        sql +=  fieldNames.value(i);
        sql +=  QString(" = :");
        sql +=  fieldNames.value(i);

        if (i != fieldNames.size()-1)
        {
            sql += QString(" ,");
        }
    }
    sqlQuery.prepare(sql);
    //qDebug() << sql;
    foreach(QString itemKey,fieldNames)
    {
        sqlQuery.bindValue(":"+itemKey,vhash.value(itemKey));
    }
    bool bRet = sqlQuery.exec();
    if (!bRet)
    {

        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        qLog(strErr);

    }
    return bRet;
}

bool SqliteDbHelper::updateTableWithWhereExp( QString tableName,QString whereStr,
                                              const QVariantHash& updateFieldhash,
                                              const QVariantHash& whereFieldhash)
{
    if (!m_bOpen)
    {
        return false;
    }

    if(tableName.isEmpty() || updateFieldhash.size() < 1 || whereFieldhash.size() < 1)
    {
        return false;
    }

    QSqlQuery sqlQuery(m_DB);
    QString sql = QString("update ")+tableName+QString(" set ");

    QList<QString> fieldNames = updateFieldhash.keys();
    for (int i = 0; i < fieldNames.size(); i++)
    {
        sql +=  fieldNames.value(i);
        sql +=  QString(" = :");
        sql +=  fieldNames.value(i);

        if (i != fieldNames.size()-1)
        {
            sql += QString(" ,");
        }
    }
    //附加的判断条件
    sql.append(" " + whereStr);

    sqlQuery.prepare(sql);
    //update field bind
    foreach(QString itemKey,fieldNames)
    {
        sqlQuery.bindValue(":"+itemKey,updateFieldhash.value(itemKey));
    }
    //where field bind
    foreach(QString itemKey,whereFieldhash.keys())
    {
        sqlQuery.bindValue(":"+itemKey,whereFieldhash.value(itemKey));
    }
    bool bRet = sqlQuery.exec();
    if (!bRet)
    {
        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        qLog(strErr);
        //qDebug() << "[SqliteDbHelper]::updateTableWithWhereExp" << strErr;
    }
    return bRet;
}


bool SqliteDbHelper::updateTable(QString tableName, QString sqlExpr, const QVariantHash &vhash)
{
    if (!m_bOpen)
    {
        return false;
    }

    if (tableName.isEmpty() || sqlExpr.isEmpty() || vhash.isEmpty())
    {
        return false;
    }
    QSqlQuery sqlQuery(m_DB);
    sqlQuery.prepare(sqlExpr);

    QList<QString> fieldNames = vhash.keys();
    foreach(QString itemKey,fieldNames)
    {
        sqlQuery.bindValue(":"+itemKey,vhash.value(itemKey));
    }
    bool bRet = sqlQuery.exec();
    if (!bRet)
    {
        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        qLog(strErr);
    }
    return bRet;
}


bool SqliteDbHelper::queryTable(QString tableName,QString sqlExpr,
                                const QVariantHash& bindFieldHash, QList<QVariantHash>& resultLst)
{
    if (!m_bOpen)
    {
        return false;
    }

    if (tableName.isEmpty() || sqlExpr.isEmpty() || bindFieldHash.isEmpty())
    {
        return false;
    }
    QSqlQuery sqlQuery(m_DB);
    sqlQuery.prepare(sqlExpr);

    QList<QString> fieldNames = bindFieldHash.keys();
    foreach(QString itemKey,fieldNames)
    {
        sqlQuery.bindValue(":"+itemKey,bindFieldHash.value(itemKey));
    }
    bool bRet = sqlQuery.exec();
    if(bRet)
    {
        while (sqlQuery.next()) {

            //each row value
            QVariantHash rowHash;
            QSqlRecord trecord = sqlQuery.record();
            int fieldNum = trecord.count();
            for(int i = 0; i < fieldNum;i++) {
                rowHash.insert(trecord.fieldName(i),trecord.value(i));
            }
            resultLst.append(rowHash);
        }

    }
    else
    {
        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        qLog(strErr);
    }
    return bRet;
}

bool SqliteDbHelper::queryTable(QString tableName, QList<QVariantHash> &resultLst)
{
    if (!m_bOpen)
    {
        return false;
    }

    if (tableName.isEmpty())
    {
        return false;
    }
    QString sqlExpr = QString("select * from  %1").arg(tableName);
    QSqlQuery sqlQuery(m_DB);
    sqlQuery.prepare(sqlExpr);

    bool bRet = sqlQuery.exec();
    if(bRet)
    {
        while (sqlQuery.next()) {

            //each row value
            QVariantHash rowHash;
            QSqlRecord trecord = sqlQuery.record();
            int fieldNum = trecord.count();
            for(int i = 0; i < fieldNum;i++) {
                rowHash.insert(trecord.fieldName(i),trecord.value(i));
            }
            resultLst.append(rowHash);
        }

    }
    else
    {
        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        qLog(strErr);
    }
    return bRet;
}


bool SqliteDbHelper::queryTable(QString tableName, QString sqlExpr,QList<QVariantHash> &resultLst)
{
    if (!m_bOpen)
    {
        return false;
    }

    if (tableName.isEmpty() || sqlExpr.isEmpty())
    {
        return false;
    }
    QSqlQuery sqlQuery(m_DB);
    sqlQuery.prepare(sqlExpr);

    bool bRet = sqlQuery.exec();
    if(bRet)
    {
        while (sqlQuery.next()) {

            //each row value
            QVariantHash rowHash;
            QSqlRecord trecord = sqlQuery.record();
            int fieldNum = trecord.count();
            for(int i = 0; i < fieldNum;i++) {
                rowHash.insert(trecord.fieldName(i),trecord.value(i));
            }
            resultLst.append(rowHash);
        }

    }
    else
    {
        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        //qDebug() << "queryTable() " << strErr;
        qLog(strErr);
    }
    return bRet;
}


bool SqliteDbHelper::insertTable(QString tableName, const QVariantHash& bindFieldHash)
{
    if (!m_bOpen)
    {
        return false;
    }

    if (tableName.isEmpty() || bindFieldHash.isEmpty())
    {
        return false;
    }
    QString sqlExpr = QString("insert into %1 (").arg(tableName);
    QList<QString> fieldNames = bindFieldHash.keys();
    QString sqlFieldStr;
    QString sqlBindStr;
    foreach(QString strItem,fieldNames)
    {
        sqlFieldStr.append(QString("%1,").arg(strItem));
        sqlBindStr.append(QString(":%1,").arg(strItem));
    }
    sqlFieldStr = sqlFieldStr.mid(0,sqlFieldStr.length()-1);
    sqlBindStr = sqlBindStr.mid(0,sqlBindStr.length() - 1);

    sqlExpr.append(sqlFieldStr);
    sqlExpr.append(") values(");
    sqlExpr.append(sqlBindStr);
    sqlExpr.append(")");
    //qDebug() << "[SqlitDbHelper]" << sqlExpr;
    QSqlQuery sqlQuery(m_DB);
    sqlQuery.prepare(sqlExpr);


    //qDebug() << "[SqlitDbHelper] prepare" << true;
    //bind values
    foreach(QString itemK,fieldNames)
    {
        sqlQuery.bindValue(":"+itemK,bindFieldHash.value(itemK));

    }
    bool bRet = sqlQuery.exec();
    if (!bRet)
    {

        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        qLog(strErr);

    }
    return bRet;

}

bool SqliteDbHelper::insertTable(QString tableName, const QVariantHash &bindFieldHash, int &rowId)
{
    rowId = 0;

    if (!m_bOpen)
    {
        return false;
    }

    if (tableName.isEmpty() || bindFieldHash.isEmpty())
    {
        return false;
    }
    QString sqlExpr = QString("insert into %1 (").arg(tableName);
    QList<QString> fieldNames = bindFieldHash.keys();
    QString sqlFieldStr;
    QString sqlBindStr;
    foreach(QString strItem,fieldNames)
    {
        sqlFieldStr.append(QString("%1,").arg(strItem));
        sqlBindStr.append(QString(":%1,").arg(strItem));
    }
    sqlFieldStr = sqlFieldStr.mid(0,sqlFieldStr.length()-1);
    sqlBindStr = sqlBindStr.mid(0,sqlBindStr.length() - 1);

    sqlExpr.append(sqlFieldStr);
    sqlExpr.append(") values(");
    sqlExpr.append(sqlBindStr);
    sqlExpr.append(")");
    //qDebug() << "[SqlitDbHelper]" << sqlExpr;
    QSqlQuery sqlQuery(m_DB);
    sqlQuery.prepare(sqlExpr);


    //qDebug() << "[SqlitDbHelper] prepare" << true;
    //bind values
    foreach(QString itemK,fieldNames)
    {
        sqlQuery.bindValue(":"+itemK,bindFieldHash.value(itemK));

    }
    bool bRet = sqlQuery.exec();
    if (!bRet)
    {

        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        qLog(strErr);

    }
    else
    {
        if (sqlQuery.exec(" SELECT MAX(ID) AS MAX_ID FROM  " + tableName))
        {
            if (sqlQuery.next())
            {
                int maxIdIndex = sqlQuery.record().indexOf("MAX_ID");
                rowId = sqlQuery.value(maxIdIndex).toInt();
            }
        }
        else
        {
            qDebug() << "[SqliteDbHelper::insertTable]:" << sqlQuery.lastError().text();
        }
    }
    return bRet;

}


bool SqliteDbHelper::deleteTable(QString tableName, QString sqlExpr, const QVariantHash& bindFieldHash)
{
    if (!m_bOpen)
    {
        return false;
    }
    if (tableName.isEmpty() || sqlExpr.isEmpty() || bindFieldHash.isEmpty())
    {
        return false;
    }
    QSqlQuery sqlQuery(m_DB);

    sqlQuery.prepare(sqlExpr);


    QList<QString> fieldNames = bindFieldHash.keys();
    foreach(QString itemKey,fieldNames)
    {
        sqlQuery.bindValue(":"+itemKey,bindFieldHash.value(itemKey));

    }

    bool bRet = sqlQuery.exec();
    if (!bRet)
    {
        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        //qDebug() << strErr;
        qLog(strErr);
    }
    return bRet;

}

bool SqliteDbHelper::deleteTable(QString tableName)
{
    if (!m_bOpen)
    {
        return false;
    }
    if (tableName.isEmpty())
    {
        return false;
    }

    //delete whole table
    QString sqlExpr = "delete from " + tableName;
    QSqlQuery sqlQuery(m_DB);
    sqlQuery.prepare(sqlExpr);

    bool bRet = sqlQuery.exec();
    if (!bRet)
    {

        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        //qDebug() << strErr;
        qLog(strErr);

    }
    return bRet;


}

bool SqliteDbHelper::dropTable(QString tableName)
{
    if (!m_bOpen)
    {
        return false;
    }
    if (tableName.isEmpty())
    {
        return false;
    }

    //delete whole table
    QString sqlExpr = "DROP TABLE " + tableName;
    QSqlQuery sqlQuery(m_DB);
    sqlQuery.prepare(sqlExpr);

    bool bRet = sqlQuery.exec();
    if (!bRet)
    {

        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        //qDebug() << strErr;
        qLog(strErr);

    }
    return bRet;

}

bool SqliteDbHelper::queryData(
        const QString& querySql,
        const QMap<QString, QVariant> &paramMap,
        QueryResult &queryResult)
{
    if (!m_bOpen)
    {
        return false;
    }

    if (querySql.length() > 0)
    {
        QSqlQuery query(m_DB);
        query.prepare(querySql);
        for (QMap<QString, QVariant>::const_iterator paramItr = paramMap.begin(); paramMap.end() != paramItr; ++paramItr)
        {
            query.bindValue(paramItr.key(), paramItr.value());
        }

        if (query.exec())
        {
            QSqlRecord record = query.record();
            int fieldCount = record.count();
            for (int fieldIdx = 0; fieldIdx < fieldCount; ++fieldIdx)
            {
                QString fieldName = record.fieldName(fieldIdx);
                queryResult.setFieldIndex(fieldName, fieldIdx);
            }

            while (query.next())
            {
                QVariantList fieldList;
                for (int fieldIdx = 0; fieldIdx < fieldCount; ++fieldIdx)
                {
                    fieldList.push_back(query.value(fieldIdx));
                }
                queryResult.addRecord(fieldList);
            }
            return true;
        }
        else
        {
            QString strErr = query.lastError().text().toUtf8();
            showDebug(strErr);
        }
    }
    return false;
}


bool SqliteDbHelper::executeSqlExp(QString sqlExp)
{
    if (!m_bOpen)
    {
        return false;
    }

    if (sqlExp.isEmpty())
    {
        return false;
    }

    QSqlQuery sqlQuery(m_DB);
    sqlQuery.prepare(sqlExp);

    bool bRet = sqlQuery.exec();
    if (!bRet)
    {

        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        qLog(strErr);

    }
    return bRet;

}

bool SqliteDbHelper::executeSqlExp(QString sqlExpr, const QVariantHash &bindHash)
{
    if (!m_bOpen)
    {
        return false;
    }

    if ( sqlExpr.isEmpty() || bindHash.isEmpty())
    {
        return false;
    }
    QSqlQuery sqlQuery(m_DB);
    sqlQuery.prepare(sqlExpr);

    QList<QString> fieldNames = bindHash.keys();
    foreach(QString itemKey,fieldNames)
    {
        sqlQuery.bindValue(":"+itemKey,bindHash.value(itemKey));
    }
    bool bRet = sqlQuery.exec();
    if (!bRet)
    {

        QString strErr = sqlQuery.lastError().text().toUtf8();
        showDebug(strErr);
        qLog(strErr);

    }
    return bRet;
}

