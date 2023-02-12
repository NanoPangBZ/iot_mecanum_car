#include "UiBridge.h"

#include <QDebug>
#define DEBUG_Info(...) qDebug(__VA_ARGS__)

UiBridge::UiBridge()
{

}

void UiBridge::slot_TargePosChange(int x , int y)
{
    DEBUG_Info("X:%d \t Y:%d" , x , y );
}
