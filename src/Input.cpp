#include "Input.hpp"

QHash<int, bool> Input::keyStates = QHash<int, bool>();

bool Input::keyPressed(Qt::Key key)
{
    if (!keyStates.contains(key)) return false;
    return keyStates[key];
}

bool Input::keyReleased(Qt::Key key)
{
    return !keyPressed(key);
}

void Input::resetKeys()
{
    keyStates = QHash<int, bool>();
}
