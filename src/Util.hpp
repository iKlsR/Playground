#ifndef UTIL_HPP
#define UTIL_HPP

#include <algorithm>
#include <QSharedPointer>

// #ifdef _WIN32
//    #include <io.h>
//    #define access    _access_s
// #else
//    #include <unistd.h>
// #endif

// bool file_exists(const std::string &fileName) {
//     return access(fileName.c_str(), 0) == 0;
// }

// template<class T> bool
// shared_connect(const QSharedPointer<T> &sender,
//                const char *signal,
//                const QObject *receiver,
//                const char *method,
//                Qt::ConnectionType type = Qt::AutoConnection)
// {
//     return QObject::connect(sender.data(), signal, receiver, method, type);
// }

#endif // UTIL_HPP
