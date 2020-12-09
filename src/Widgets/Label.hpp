#ifndef PL_LABEL_HPP
#define PL_LABEL_HPP

#include <QLabel>

class Label : public QLabel
{
    Q_OBJECT;

public:
    Label(const QString &text, QWidget *parent = Q_NULLPTR);
    ~Label();

private:
    QString _text;
};

#endif // PL_LABEL_HPP