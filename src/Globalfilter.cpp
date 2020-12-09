#include <QKeyEvent>
#include "Globalfilter.hpp"

bool GlobalFilter::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape)  exit(0);
    }

    return false;
}
