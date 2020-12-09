#ifndef PL_INPUT_HPP
#define PL_INPUT_HPP

#include <QHash>
#include <Qt>

class Input
{
public:
    Input() = default;
    static QHash<int, bool> keyStates;
    static bool keyPressed(Qt::Key key);
    static bool keyReleased(Qt::Key key);
    static void resetKeys();
};

#endif // PL_INPUT_HPP
