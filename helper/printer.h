#ifndef PRINTER_H
#define PRINTER_H

#include "ctype.h"
#include "stdarg.h"
#include <string.h>
#include <stdio.h>

class Printer
{
public:
    Printer();
    int vsprintf(char *buf, const char *fmt, va_list args);
    int sprintf(char *buf, const char *fmt...);
    int printf(const char *fmt...);
};

#endif // PRINTER_H
