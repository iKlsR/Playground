#include "TextInput.hpp"

TextInput::TextInput(const QString &ph, QWidget *parent) : _placeHolder(ph), QLineEdit(ph, parent)
{
}

TextInput::~TextInput()
{

}