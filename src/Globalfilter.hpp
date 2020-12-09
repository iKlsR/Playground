#ifndef PL_GLOBALFILTER_HPP
#define PL_GLOBALFILTER_HPP

#include <QObject>

class GlobalFilter : public QObject
{
    Q_OBJECT

public:
    GlobalFilter() = default;
    virtual bool eventFilter(QObject *object, QEvent *event) override;
};

#endif // PL_GLOBALFILTER_HPP
