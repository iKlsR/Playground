#include "CheckBox.hpp"

CheckBox::CheckBox(const QString &text, bool checked, QWidget *parent) : _text(text), QCheckBox(" " + text, parent)
{
    auto check = checked ? Qt::Checked : Qt::Unchecked;
    setCheckState(check);
    setAccessibleName("checkbox_widget");
}

CheckBox::~CheckBox()
{

}