#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <QHash>
#include <QList>
#include <QString>
#include <QVariantList>

/**
* @brief Save the database query results
*/
class QueryResult
{
public:
    QueryResult();

    /**
    * @brief Set index for field name
    * @param fieldName Database field name
    * @param index field index
    * @return
    */
    void setFieldIndex(const QString &fieldName, int index);

    /**
    * @brief Get index of field name
    * @param fieldName Database field name
    * @return index
    */
    int getFieldIndex(const QString& fieldName) const;

    /**
    * @brief Add the database query record
    * @param record Database record
    * @return
    */
    void addRecord(const QVariantList& record);

    /**
    * @brief Get query record
    * @param recordIndex record index
    * @param row record result
    * @return true:has record; false:haven't record
    */
    bool getRecord(int recordIndex, QVariantList &record) const;

    /**
    * @brief Get query field value
    * @param recordIndex record index
    * @param fieldIndex field index
    * @param value field value
    * @return true:has record; false:haven't record
    */
    bool getRecordValue(int recordIndex, int fieldIndex, QVariant &value);

    /**
    * @brief Get query field value
    * @param recordIndex record index
    * @param fieldName field name
    * @param value field value
    * @return true:has record; false:haven't record
    */
    bool getRecordValue(int recordIndex, const QString &fieldName, QVariant &value);

    /**
    * @brief Get query field value
    * @param recordIndex record index
    * @param fieldIndex field index
    * @return value field value
    */
    QVariant getRecordValue(int recordIndex, int fieldIndex);

    /**
    * @brief Get query field value
    * @param recordIndex record index
    * @param fieldName field name
    * @return value field value
    */
    QVariant getRecordValue(int recordIndex, const QString &fieldName);

    /**
    * @brief Get field index map
    * @return field name mappint to index
    */
    const QHash<QString, int>& getFieldIndexMap() const;

    /**
    * @brief Get query record list
    * @return record list
    */
    const QList<QVariantList>& getRecordList() const;

    /**
    * @brief Get record count
    * @return record count
    */
    inline int getRecordCount() const
    {
        return this->recordList.size();
    }

    /**
    * @brief Get field count
    * @return field count
    */
    inline int getFieldCount() const
    {
        return this->fieldIndexMap.size();
    }

    /**
    * @brief Get field index
    * @return field index
    */
    static int getFieldIndex(const QHash<QString, int>& fieldIndexMap, const QString& fieldName);

    /**
     * @brief getRecordValue Get Record Value
     * @param fieldIndexMap Field Index Map
     * @param valueList Value List
     * @param fieldName Field Name
     * @return value
     */
    static QVariant getRecordValue(const QHash<QString, int>& fieldIndexMap, const QVariantList &valueList, const QString& fieldName);

private:
    // @brief field name mappint to index
    QHash<QString, int> fieldIndexMap;

    // @brief record list
    QList<QVariantList> recordList;
};

#endif // QUERYRESULT_H
