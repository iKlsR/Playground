#ifndef PL_CHECKBOX_HPP
#define PL_CHECKBOX_HPP

#include <QCheckBox>

class CheckBox : public QCheckBox
{
    Q_OBJECT;

public:
    CheckBox(const QString &text,bool checked = false, QWidget *parent = Q_NULLPTR);
    ~CheckBox();

private:
    QString _text;
};

#endif // PL_CHECKBOX_HPP