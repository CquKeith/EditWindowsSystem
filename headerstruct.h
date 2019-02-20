#ifndef HEADERSTRUCT_H
#define HEADERSTRUCT_H

#include <time.h>
#include <windows.h>
#include <QObject>

typedef struct s_DateTime
{
    ushort year;
    ushort month;
    ushort day;
    ushort hour;
    ushort minute;
    ushort second;
    ushort millionSecond;

    QString toQStirng(){
//        return QString::number("%1-%2-%3 %4:%5:%6:%7").arg(year,month,day,hour,minute,second,millionSecond);
        return QString::number(hour)+":"+QString::number(minute)+":"+QString::number(second)+":"+QString::number(millionSecond);
    }

}s_DateTime;



#endif // HEADERSTRUCT_H
