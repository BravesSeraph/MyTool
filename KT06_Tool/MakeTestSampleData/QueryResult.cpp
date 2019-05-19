#include "QueryResult.h"
#include <QDebug>

QueryResult::QueryResult()
{
}

void QueryResult::setFieldIndex(const QString &fieldName, int index)
{
    this->fieldIndexMap.insert(fieldName, index);
}

int QueryResult::getFieldIndex(const QString& fieldName) const
{
    QHash<QString, int>::const_iterator fieldItr = fieldIndexMap.find(fieldName);
    if (fieldIndexMap.end() == fieldItr)
    {
        return -1;
    }
    return fieldItr.value();
}

void QueryResult::addRecord(const QVariantList& record)
{
    this->recordList.push_back(record);
}

bool QueryResult::getRecord(int recordIndex, QVariantList &record) const
{
    if (recordIndex >= 0 && recordIndex < this->recordList.size())
    {
        record = this->recordList.at(recordIndex);
        return true;
    }
    return false;
}

bool QueryResult::getRecordValue(int recordIndex, int fieldIndex, QVariant &value)
{
    if ((recordIndex >= 0 && recordIndex < this->recordList.size())
        && (fieldIndex >=0 && fieldIndex < this->fieldIndexMap.size()))
    {
        value = this->recordList.at(recordIndex).at(fieldIndex);
        return true;
    }
    return false;
}

bool QueryResult::getRecordValue(int recordIndex, const QString &fieldName, QVariant &value)
{
    return getRecordValue(recordIndex, getFieldIndex(fieldName), value);
}

const QHash<QString, int>& QueryResult::getFieldIndexMap() const
{
    return this->fieldIndexMap;
}

const QList<QVariantList>& QueryResult::getRecordList() const
{
    return this->recordList;
}

int QueryResult::getFieldIndex(const QHash<QString, int>& fieldIndexMap, const QString& fieldName)
{
    QHash<QString, int>::const_iterator fieldItr = fieldIndexMap.find(fieldName);
    if (fieldIndexMap.end() == fieldItr)
    {
        return -1;
    }
    return fieldItr.value();
}

QVariant QueryResult::getRecordValue(const QHash<QString, int>& fieldIndexMap, const QVariantList &valueList, const QString& fieldName)
{
    int fieldIndex = QueryResult::getFieldIndex(fieldIndexMap, fieldName);
    if ((fieldIndex >= 0) && fieldIndex < valueList.size())
    {
        return valueList.at(fieldIndex);
    }
    else
    {
        qDebug() << "Field Index Error. Index:" << fieldIndex << ";Name:" << fieldName;
    }
    return QVariant();
}

/**
* @brief Get query field value
* @param recordIndex record index
* @param fieldIndex field index
* @return value field value
*/
QVariant QueryResult::getRecordValue(int recordIndex, int fieldIndex)
{
    QVariant value;
    this->getRecordValue(recordIndex, fieldIndex, value);
    return value;
}

/**
* @brief Get query field value
* @param recordIndex record index
* @param fieldName field name
* @return value field value
*/
QVariant QueryResult::getRecordValue(int recordIndex, const QString &fieldName)
{
    QVariant value;
    this->getRecordValue(recordIndex, fieldName, value);
    return value;
}
