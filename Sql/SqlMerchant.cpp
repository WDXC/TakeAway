#include "Sql/SqlMerchant.hpp"

SqlMerchant::SqlMerchant(SqlOps *ops)
{
    m_sql = std::shared_ptr<SqlOps>(ops);
}

SqlMerchant::~SqlMerchant()
{

}

bool SqlMerchant::handleInsert(const std::string &username, const std::string &mail, const std::string &pwd) {
    std::string query = "insert into sf_merchant (id, MerchantName, Email, SecretCode) values (?, ?, ?, ?)";
    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_null(0);
    m_sql->set_param_bind(1, username);
    m_sql->set_param_bind(2, mail);
    m_sql->set_param_bind(3, pwd);
    return m_sql->pre_excute();
}

bool SqlMerchant::handleUpdate(const std::string &mail, const std::string &pwd)
{
    std::string query = "update sf_merchant set SecretCode=? where MerchantName=?";
    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_bind(0, pwd);
    m_sql->set_param_bind(1, mail);
    return m_sql->pre_excute();
}

int SqlMerchant::queryMail(const std::string mail)
{
    std::string query = "select count(Email) from sf_merchant wherer Email=?";
    m_sql->set_param_bind(0, mail);
    if (!m_sql->query()) {
        return 0;
    }
    unsigned long len = 128;
    char* buf[32];
    m_sql->set_param_result(0, MYSQL_TYPE_INT24, *buf, sizeof (buf), &len);
    m_sql->get_result();
    m_sql->fetch_result();
    query = *buf;
    return stoi(query);
}

std::string SqlMerchant::handleQuery(const std::string &mail)
{
    std::string query = "select SecretCode from sf_merchant where Email=?";

    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_bind(0, mail);
    m_sql->query();
    unsigned long len = 128;
    char *buf[32];
    m_sql->set_param_result(0, MYSQL_TYPE_STRING, *buf, sizeof (buf), &len);
    m_sql->get_result();
    m_sql->fetch_result();
    query = *buf;
    return query;
}

bool SqlMerchant::handleDel(const std::string& mail) {
    std::string query = "delete from sf_merchant where Email=?";
    m_sql->initSql(m_sql->getStmt(), query);
    m_sql->set_param_bind(0, mail);
    return m_sql->pre_excute();
}
