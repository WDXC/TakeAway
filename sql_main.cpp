#include "Sql/SqlHandler.hpp"

#include <iostream>

using namespace std;

//int main() {
//    std::unique_ptr<SqlConnPool> obj = std::make_unique<SqlConnPool>(g_sqlMgr::instance());
//    if (!obj->Init("127.0.0.1", 3306, "root", "password", "TakeAway", 2)) {
//        return 0;
//    }
//    std::unique_ptr<SqlHandler> res_obj = std::make_unique<SqlHandler>(std::move(obj));
//    if (res_obj->dealInsert()) {
//        cout << "ok" << endl;
//    } else {
//        cout << "don't ok" << endl;
//    }
//    cout << res_obj->dealQuery() << endl;
//}
