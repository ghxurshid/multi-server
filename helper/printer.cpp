#include <QDebug>
#include "printer.h"

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SMALL	32		/* Must be 32 == 0x20 */
#define SPECIAL	64		/* 0x */
#define __do_div(n, base) ({ \
int __res; \
__res = ((unsigned long) n) % (unsigned) base; \
n = ((unsigned long) n) / (unsigned) base; \
__res; })

static int skip_atoi(const char **s);
static QString number(QString str, long num, int base, int size, int precision, int type);

Printer::Printer()
{

}

QString Printer::vsprintf(const char *fmt, QStringList list)
{
    int len;
    unsigned long num;
    int base;
    QString s;
    int flags;		    /* flags to number() */
    int field_width;	/* width of output field */
    int precision;		/* min. # of digits for integers; max
                           number of chars for from string */
    int qualifier;		/* 'h', 'l', or 'L' for integer fields */

    QString str;

    for (; *fmt; ++fmt) {
        if (*fmt != '%') {
            str.append(*fmt);
            continue;
        }
        /* process flags */
        flags = 0;
          repeat:
        ++fmt;		/* this also skips first '%' */
        switch (*fmt) {
        case '-':
            flags |= LEFT;
            goto repeat;
        case '+':
            flags |= PLUS;
            goto repeat;
        case ' ':
            flags |= SPACE;
            goto repeat;
        case '#':
            flags |= SPECIAL;
            goto repeat;
        case '0':
            flags |= ZEROPAD;
            goto repeat;
        }
        /* get field width */
        field_width = -1;
        if (isdigit(*fmt))
            field_width = skip_atoi(&fmt);
        else if (*fmt == '*') {
            ++fmt;
            /* it's the next argument */
            field_width = (!list.isEmpty() ? list.takeFirst() : QString()).toInt(); // va_arg(args, int);
            if (field_width < 0) {
                field_width = -field_width;
                flags |= LEFT;
            }
        }
        /* get the precision */
        precision = -1;
        if (*fmt == '.') {
            ++fmt;
            if (isdigit(*fmt))
                precision = skip_atoi(&fmt);
            else if (*fmt == '*') {
                ++fmt;
                /* it's the next argument */
                precision = (!list.isEmpty() ? list.takeFirst() : QString()).toInt(); // va_arg(args, int);
            }
            if (precision < 0)
                precision = 0;
        }
        /* get the conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
            qualifier = *fmt;
            ++fmt;
        }
        /* default base */
        base = 10;
        switch (*fmt) {
        case 'c':
            if (!(flags & LEFT))
                while (--field_width > 0)
                    str.append(' ');
            str.append(static_cast<unsigned char>(((!list.isEmpty() ? list.takeFirst() : QString()).toInt()))); // va_arg(args, int);
            while (--field_width > 0)
                str.append(' ');
            continue;
        case 's':            
            s = (!list.isEmpty() ? list.takeFirst() : QString()).left(precision);//va_arg(args, char *);
            len = s.size();// strnlen(s, precision);
            if (!(flags & LEFT))
                while (len < field_width--)
                    str.append(' ');
            str.append(s);
            while (len < field_width--)
                str.append(' ');
            continue;
        case 'p':
//            if (field_width == -1) {
//                field_width = 2 * sizeof(void *);
//                flags |= ZEROPAD;
//            }
//            str = number(str,
//                     (unsigned long)va_arg(args, int), 16, // void* replaced to int
//                     field_width, precision, flags);
            continue;
        case 'n':
//            if (qualifier == 'l') {
//                long *ip = va_arg(args, long *);
//                *ip = (str - buf);
//            } else {
//                int *ip = va_arg(args, int *);
//                *ip = (str - buf);
//            }
            continue;
        case '%':
            str.append('%');
            continue;
            /* integer number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;
        case 'x':
            flags |= SMALL;
        case 'X':
            base = 16;
            break;
        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;
        default:
            str.append('%');
            if (*fmt)
                str.append(*fmt);
            else
                --fmt;
            continue;
        }
        if (qualifier == 'l')
            num = (!list.isEmpty() ? list.takeFirst() : QString()).toULong(); // va_arg(args, unsigned long);
        else if (qualifier == 'h') {
            num = (!list.isEmpty() ? list.takeFirst() : QString()).toUShort(); //(unsigned short)va_arg(args, int);
            if (flags & SIGN)
                num = (short)num;
        } else if (flags & SIGN)
            num = (!list.isEmpty() ? list.takeFirst() : QString()).toInt();// va_arg(args, int);
        else
            num = (!list.isEmpty() ? list.takeFirst() : QString()).toUInt(); //va_arg(args, unsigned int);
        str = number(str, num, base, field_width, precision, flags);
    }

    return str;
}
QString Printer::sprintf(const char *fmt, QStringList list)
{ 
    return vsprintf(fmt, list);
}
int Printer::printf(const char *fmt, QStringList list)
{
    qDebug() << vsprintf(fmt, list);
}

static int skip_atoi(const char **s)
{
    int i = 0;
    while (isdigit(**s))
        i = i * 10 + *((*s)++) - '0';
    return i;
}

static QString number(QString str, long num, int base, int size, int precision, int type)
{
    /* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
    static const char digits[] = "0123456789ABCDEF"; /* "GHIJKLMNOPQRSTUVWXYZ"; */
    char tmp[66];
    char c, sign, locase;
    int i;
    /* locase = 0 or 0x20. ORing digits or letters with 'locase'
     * produces same digits or (maybe lowercased) letters */
    locase = (type & SMALL);
    if (type & LEFT)
        type &= ~ZEROPAD;
    if (base < 2 || base > 16)
        return nullptr;
    c = (type & ZEROPAD) ? '0' : ' ';
    sign = 0;
    if (type & SIGN) {
        if (num < 0) {
            sign = '-';
            num = -num;
            size--;
        } else if (type & PLUS) {
            sign = '+';
            size--;
        } else if (type & SPACE) {
            sign = ' ';
            size--;
        }
    }
    if (type & SPECIAL) {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }
    i = 0;
    if (num == 0)
        tmp[i++] = '0';
    else
        while (num != 0)
            tmp[i++] = (digits[__do_div(num, base)] | locase);
    if (i > precision)
        precision = i;
    size -= precision;
    if (!(type & (ZEROPAD + LEFT)))
        while (size-- > 0)
            str.append(' ');
    if (sign)
        str.append(sign);
    if (type & SPECIAL) {
        if (base == 8)
            str.append('0');
        else if (base == 16) {
            str.append('0');
            str.append(('X' | locase));
        }
    }
    if (!(type & LEFT))
        while (size-- > 0)
            str.append(c);
    while (i < precision--)
        str.append('0');
    while (i-- > 0)
        str.append(tmp[i]);
    while (size-- > 0)
        str.append(' ');
    return str;
}
