#ifndef GLOBALAPI_SQLITEDBHELPER_H
#define GLOBALAPI_SQLITEDBHELPER_H

#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QVariantHash>
#include <QDebug>
#include "QueryResult.h"

#ifdef LOG_DBLOG
#define qLog(x) qDebug(x)
#else
#define qLog(x)
#endif

class SqliteDbHelper
{

private:
    SqliteDbHelper()
    {
         m_DB = QSqlDatabase::addDatabase("QSQLITE");
         openDB("KT06.db");
    }

public:
    ~SqliteDbHelper()
    {
        if (m_bOpen)
        {
            m_DB.close();
        }

    }

    static SqliteDbHelper* Instance()
    {
        static SqliteDbHelper mdbHelper;
        return &mdbHelper;
    }

    bool openDB(QString dbName);
    bool closeDB();

    bool isOpen()
    {
        return m_bOpen;
    }


    QSqlDatabase   getDB()
    {
        return m_DB;
    }

    /**
     * @brief updateTable 更新数据库中的表
     * @param tableName 表名称
     * @param vhash  需要更新的表的字段
     * @return
     */
    bool updateTable(QString tableName,const QVariantHash& vhash);

    /**
     * @brief updateTable 更新数据库中的表
     * @param tableName 表名称
     * @param whereStr SQL where 子句条件判断
     * @param updateFieldhash  语句中需要绑定的所有更新变量字段
     * @param whereFieldhash  语句中需要绑定的条件变量字段
     * @return
     */
    bool updateTableWithWhereExp( QString tableName,QString whereStr,
                                  const QVariantHash& updateFieldhash,
                                  const QVariantHash& whereFieldhash);

    /**
     * @brief updateTable 更新数据库中的表
     * @param tableName 表名称
     * @param sqlExpr SQL表达式语句
     * @param vhash  需要更新的表的字段
     * @return
     */
    bool updateTable(QString tableName,QString sqlExpr,const QVariantHash& vhash);

    /**
     * @brief queryTable 查询数据表
     * @param tableName 表名称
     * @param sqlExpr 查询SQL表达式语句
     * @param bindFieldHash 查询语句中需要绑定的字段
     * @param resultLst 查询的结果集合
     * @return
     */
    bool queryTable(QString tableName, QString sqlExpr, const QVariantHash& bindFieldHash, QList<QVariantHash>& resultLst);

    /**
     * @brief queryTable 查询数据表
     * @param tableName 表名称
     * @param resultHash 查询的结果集合
     * @return
     */
    bool queryTable(QString tableName,QList<QVariantHash>& resultLst);

    /**
     * @brief queryTable zhijie caozuo数据表
     * @param tableName
     * @param sqlExpr sql sentences with no bind fields
     * @param resultLst result list
     * @return
     */
    bool queryTable(QString tableName, QString sqlExpr, QList<QVariantHash> &resultLst);


    /**
     * @brief insertTable 插入数据到数据表
     * @param tableName 表名称
     * @param bindFieldHash 单行插入数据
     * @return
     */
    bool insertTable(QString tableName, const QVariantHash &bindFieldHash);

    /**
     * @brief insertTable 插入数据到数据表
     * @param tableName 表名称
     * @param bindFieldHash 单行插入数据
     * @param rowId 插入后表返回的自增长ID
     * @return
     */
    bool insertTable(QString tableName, const QVariantHash &bindFieldHash, int &rowId);


    /**
     * @brief queryTable 查询数据表
     * @param tableName 表名称
     * @param sqlExpr 查询SQL表达式语句
     * @param bindFieldHash 删除SQL语句中需要绑定的字段
     * @return
     */
    bool deleteTable(QString tableName, QString sqlExpr, const QVariantHash& bindFieldHash);

    /**
     * @brief deleteTable 删除表数据
     * @param tableName 表名称
     * @return
     */
    bool deleteTable(QString tableName);

    /**
     * @brief dropTable 删除表
     * @param tableName 表名称
     * @return
     */
    bool dropTable(QString tableName);

    /**
    * @brief Query Data
    * @param querySql query string (select ...)
    * @param paramMap bind parameter map
    * @param queryResult save query result
    * @return true:query successful; false:query failed
    */
    bool queryData(const QString& querySql, const QMap<QString, QVariant> &paramMap, QueryResult &queryResult);

    /**
     * @brief executeSqlExp
     * @param sqlExp
     * @return
     */
    bool executeSqlExp(QString sqlExp);

    /**
     * @brief executeSqlExp
     * @param sqlExpr
     * @param bindHash
     * @return
     */
    bool executeSqlExp(QString sqlExpr, const QVariantHash &bindHash);

    //启动事务
    void startTransaction()
    {
        m_DB.transaction();
    }

    //提交事务
    void commit()
    {
        m_DB.commit();
    }

    //回滚事务
    void rollback()
    {
        m_DB.rollback();
    }





private:

    QString         m_dbName;
    bool            m_bOpen;
    QSqlDatabase    m_DB;

};

#endif // GLOBALAPI_SQLITEDBHELPER_H
