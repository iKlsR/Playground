#ifndef UPDATER_HPP
#define UPDATER_HPP

#include <QObject>
#include <condition_variable>
#include <mutex>

#include "Playground.hpp"

class Updater : public QObject
{
    Q_OBJECT

public:
    explicit Updater(std::condition_variable *_job_ready, std::mutex *_mtx, Swapper *swapper);
    ~Updater();

    void start();
    void run();

private:
    std::condition_variable *job_ready;
    std::mutex *mtx;
    Swapper *swapper;
};

#endif // UPDATER_HPP
