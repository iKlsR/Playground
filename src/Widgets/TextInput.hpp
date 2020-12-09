#ifndef PL_TEXTINPUT_HPP
#define PL_TEXTINPUT_HPP

#include <QLineEdit>

class TextInput : public QLineEdit
{
    Q_OBJECT;

public:
    TextInput(const QString &_placeHolder = QString(), QWidget *parent = Q_NULLPTR);
    ~TextInput();

private:
    QString _placeHolder;
};

#endif // PL_TEXTINPUT_HPP