QT -= gui

CONFIG += c++14 console mysql
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
               $$PWD/include/Netlib/Base \
               $$PWD/include/Netlib/Log \
               $$PWD/include/Netlib/Net \
               $$PWD/include/Netlib/rapidjson \
               $$PWD/include/Netlib/Socket \
               $$PWD/include/Netlib/Thread \
               $$PWD/include/Netlib/Timer \

HEADERS += \
         $$PWD/Http/HttpRequest.hpp \
         $$PWD/Http/HttpResponse.hpp \
         $$PWD/Http/HttpServer.hpp \
         $$PWD/Http/HttpContext.hpp \
         $$PWD/Http/HttpDataHandle.hpp \
         $$PWD/Http/HttpCallback.hpp \
         $$PWD/Sql/SqlUserData.hpp \
         $$PWD/Sql/SqlConnPool.hpp \
         $$PWD/Sql/SqlHandler.hpp \
         $$PWD/Sql/SqlOps.hpp \
         $$PWD/Sql/SqlMerchant.hpp \
         $$PWD/Sql/SqlGoods.hpp \
         $$PWD/Email/MailSend.hpp\


SOURCES += \
        sql_main.cpp \
        http_main.cpp \
        $$PWD/Email/MailSend.cpp \
        $$PWD/Http/HttpContext.cpp \
        $$PWD/Http/HttpResponse.cpp \
        $$PWD/Http/HttpServer.cpp \
        $$PWD/Http/HttpDataHandle.cpp \
        $$PWD/Http/HttpCallback.cpp \
        $$PWD/Sql/SqlUserData.cpp \
        $$PWD/Sql/SqlConnPool.cpp \
        $$PWD/Sql/SqlHandler.cpp \
        $$PWD/Sql/SqlOps.cpp \
        $$PWD/Sql/SqlMerchant.cpp


LIBS += $$PWD/lib/libNet.a  -lpthread -lmysqlclient

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
