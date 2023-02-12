#ifndef UIBRIDGE_H
#define UIBRIDGE_H

#include <QObject>

class UiBridge : public QObject
{
    Q_OBJECT
public:
    UiBridge();

signals:
    void Signal_CurrentPosChange(int x , int y );
public slots:
    void slot_TargePosChange(int x , int y);
};

#endif // UIBRIDGE_H
