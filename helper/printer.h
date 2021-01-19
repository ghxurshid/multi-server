#ifndef PRINTER_H
#define PRINTER_H

#include "ctype.h"
#include "stdarg.h"
#include <string.h>
#include <stdio.h>

#include <QString>
#include <QStringList>

class Printer
{
public:
    Printer();
    QString vsprintf(const char *fmt, QStringList list);
    QString sprintf(const char *fmt, QStringList list);
    int printf(const char *fmt, QStringList list);
};

#endif // PRINTER_H
