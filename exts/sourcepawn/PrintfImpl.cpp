///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2014-2018, PALANDesign Hannover, Germany
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and (v)snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources. These routines are thread
//        safe and reentrant!
//        Use this instead of the bloated standard/newlib printf cause these use
//        malloc for printf (and may not be thread safe).
//
///////////////////////////////////////////////////////////////////////////////

#include "spmod.hpp"

// ntoa conversion buffer size, this must be big enough to hold
// one converted numeric number including padded zeros (created on stack)
#define PRINTF_NTOA_BUFFER_SIZE    32U

// ftoa conversion buffer size, this must be big enough to hold
// one converted float number including padded zeros (created on stack)
#define PRINTF_FTOA_BUFFER_SIZE    32U

///////////////////////////////////////////////////////////////////////////////

// internal flag definitions
#define FLAGS_ZEROPAD   (1U << 0U)
#define FLAGS_LEFT      (1U << 1U)
#define FLAGS_PLUS      (1U << 2U)
#define FLAGS_SPACE     (1U << 3U)
#define FLAGS_HASH      (1U << 4U)
#define FLAGS_UPPERCASE (1U << 5U)
#define FLAGS_PRECISION (1U << 6U)
#define FLAGS_WIDTH     (1U << 7U)

// internal itoa format
static std::size_t _ntoa_format(char *buffer,
                        char *buf,
                        std::size_t len,
                        bool negative,
                        unsigned int base,
                        std::size_t maxlen,
                        unsigned int prec,
                        unsigned int width,
                        unsigned int flags)
{
    if (!maxlen)
        return 0;

    if (base > 16)
        return 0;

    // pad leading zeros
    while (!(flags & FLAGS_LEFT) && len < prec && len < PRINTF_NTOA_BUFFER_SIZE)
        buf[len++] = '0';

    while (!(flags & FLAGS_LEFT) && flags & FLAGS_ZEROPAD && len < width && len < PRINTF_NTOA_BUFFER_SIZE)
        buf[len++] = '0';

    // handle hash
    if (flags & FLAGS_HASH)
    {
        if ((len == prec || len == width) && len > 0)
        {
            len--;
            if (base == 16 && len > 0)
                len--;
        }
        if (base == 16 && !(flags & FLAGS_UPPERCASE) && len < PRINTF_NTOA_BUFFER_SIZE)
            buf[len++] = 'x';

        if (base == 16 && flags & FLAGS_UPPERCASE && len < PRINTF_NTOA_BUFFER_SIZE)
            buf[len++] = 'X';

        if (len < PRINTF_NTOA_BUFFER_SIZE)
            buf[len++] = '0';
    }

    // handle sign
    if (len == width && (negative || flags & FLAGS_PLUS || flags & FLAGS_SPACE))
        len--;

    if (len < PRINTF_NTOA_BUFFER_SIZE)
    {
        if (negative)
            buf[len++] = '-';

        else if (flags & FLAGS_PLUS)
            buf[len++] = '+';  // ignore the space if the '+' exists

        else if (flags & FLAGS_SPACE)
            buf[len++] = ' ';
    }
    // pad spaces up to given width
    std::size_t idx = 0;
    if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD))
    {
        for (std::size_t i = len; i < width && i < maxlen; ++i)
            buffer[idx++] = ' ';
    }

    // reverse string
    for (std::size_t i = 0; i < len && i < maxlen; ++i)
        buffer[idx++] = buf[len - i - 1];

    // append pad spaces up to given width
    if (flags & FLAGS_LEFT)
    {
        while (idx < width && idx < maxlen)
            buffer[idx++] = ' ';
    }
    return idx;
}

// internal itoa for 'long' type
static std::size_t _ntoa_long(char *buffer,
                        unsigned long value,
                        bool negative,
                        unsigned long base,
                        std::size_t maxlen,
                        unsigned int prec,
                        unsigned int width,
                        unsigned int flags)
{
    char buf[PRINTF_NTOA_BUFFER_SIZE];
    std::size_t len = 0;

    // write if precision != 0 and value is != 0
    if (!(flags & FLAGS_PRECISION) || value)
    {
        do
        {
            char digit = value % base;
            buf[len++] = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
            value /= base;
        } while (len < PRINTF_NTOA_BUFFER_SIZE && value);
    }

    return _ntoa_format(buffer, buf, len, negative, base, maxlen, prec, width, flags);
}

static std::size_t _ftoa(double value,
                    char *buffer,
                    std::size_t maxlen,
                    unsigned int prec,
                    unsigned int width,
                    unsigned int flags)
{
    char buf[PRINTF_FTOA_BUFFER_SIZE];
    std::size_t len  = 0;
    double diff = 0.0;

    // if input is larger than thres_max, revert to exponential
    const double thres_max = 0x7FFFFFFF;

    // test for negative
    bool negative = false;
    if (value < 0)
    {
        negative = true;
        value = -value;
    }

    // limit precision
    if (!(flags & FLAGS_PRECISION))
        prec = 6;  // by default, precesion is 6

    // precision of >= 10 can lead to overflow errors
    if (prec > 9)
        prec = 9;

    int whole = value;
    double tmp = (value - whole) * pow(10.0, prec);
    unsigned long frac = tmp;
    diff = tmp - frac;

    if (diff > 0.5)
    {
        ++frac;
        // handle rollover, e.g. case 0.99 with prec 1 is 1.0
        if (frac >= pow(10.0, prec))
        {
            frac = 0;
            ++whole;
        }
    }
    // if halfway, round up if odd, OR if last digit is 0
    else if (diff == 0.5 && (!frac || frac & 1))
        ++frac;

    // TBD: for very large numbers switch back to native sprintf for exponentials. anyone want to write code to replace this?
    // normal printf behavior is to print EVERY whole number digit which can be 100s of characters overflowing your buffers == bad
    if (value > thres_max)
        return 0;

    if (!prec)
    {
        diff = value - whole;
        // greater than 0.5, round up, e.g. 1.6 -> 2
        if (diff > 0.5)
            ++whole;

        // exactly 0.5 and ODD, then round up
        // 1.5 -> 2, but 2.5 -> 2
        else if (diff == 0.5 && whole & 1)
            ++whole;
    }
    else
    {
        unsigned int count = prec;
        // now do fractional part, as an unsigned number
        do
        {
            --count;
            buf[len++] = 48 + (frac % 10);
        } while (len < PRINTF_FTOA_BUFFER_SIZE && (frac /= 10));

        // add extra 0s
        while (len < PRINTF_FTOA_BUFFER_SIZE && count-- > 0)
            buf[len++] = '0';

        // add decimal
        if (len < PRINTF_FTOA_BUFFER_SIZE)
            buf[len++] = '.';
    }

    // do whole part, number is reversed
    while (len < PRINTF_FTOA_BUFFER_SIZE)
    {
        buf[len++] = 48 + (whole % 10);
        if (!(whole /= 10))
            break;
    }

    // pad leading zeros
    while (!(flags & FLAGS_LEFT) && flags & FLAGS_ZEROPAD && len < width && len < PRINTF_FTOA_BUFFER_SIZE)
        buf[len++] = '0';

    // handle sign
    if (len == width && (negative || flags & FLAGS_PLUS || flags & FLAGS_SPACE))
        len--;

    if (len < PRINTF_FTOA_BUFFER_SIZE)
    {
        if (negative)
            buf[len++] = '-';

        else if (flags & FLAGS_PLUS)
            buf[len++] = '+';  // ignore the space if the '+' exists

        else if (flags & FLAGS_SPACE)
            buf[len++] = ' ';
    }

    // pad spaces up to given width
    std::size_t idx = 0;
    if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD))
    {
        for (std::size_t i = len; i < width && i < maxlen; ++i)
            buffer[idx++] = ' ';
    }

    // reverse string
    for (std::size_t i = 0; i < len && i < maxlen; ++i)
        buffer[idx++] = buf[len - i - 1];

    // append pad spaces up to given width
    if (flags & FLAGS_LEFT)
    {
        while (idx < width && idx < maxlen)
            buffer[idx++] = ' ';
    }

    return idx;
}

unsigned int SPGlobal::formatString(char *buffer,
                                    std::size_t buffer_len,
                                    const char *format,
                                    SourcePawn::IPluginContext *ctx,
                                    const cell_t *params,
                                    std::size_t param) const
{
    unsigned int flags, width, precision, n;
    std::size_t idx = 0;

    // check if buffer is valid
    if (!buffer)
        return 0;

    // Check for bounds
    auto checkArgs = [ctx, params](std::size_t paramCheck)
    {
        if (static_cast<std::size_t>(params[0]) < paramCheck)
        {
            ctx->ReportError("String formatted incorrectly - parameter %u (total %u)", paramCheck, params[0]);
            return false;
        }
        return true;
    };

    while (idx < buffer_len && *format)
    {
        // format specifier?  %[flags][width][.precision][length]
        if (*format != '%')
        {
            // no
            buffer[idx++] = *format++;
            continue;
        }
        else
            format++; // yes, evaluate it

        // evaluate flags
        flags = 0;
        do
        {
            switch (*format)
            {
                case '0': flags |= FLAGS_ZEROPAD; format++; n = 1; break;
                case '-': flags |= FLAGS_LEFT;    format++; n = 1; break;
                case '+': flags |= FLAGS_PLUS;    format++; n = 1; break;
                case ' ': flags |= FLAGS_SPACE;   format++; n = 1; break;
                case '#': flags |= FLAGS_HASH;    format++; n = 1; break;
                default :                                   n = 0; break;
            }
        } while (n);

        // evaluate width field
        width = 0;
        if (isdigit(*format))
        {
            width = atoi(format);
            while (isdigit(*format))
                format++;
        }
        else if (*format == '*')
        {
            if (!checkArgs(param))
                return 0;

            cell_t *w;
            ctx->LocalToPhysAddr(params[param++], &w);
            if (*w < 0)
            {
                flags |= FLAGS_LEFT;    // reverse padding
                width = -*w;
            }
            else
                width = *w;

            format++;
        }

        // evaluate precision field
        precision = 0;
        if (*format == '.')
        {
            flags |= FLAGS_PRECISION;
            format++;
            if (isdigit(*format))
            {
                precision = atoi(format);
                while (isdigit(*format))
                    format++;
            }
            else if (*format == '*')
            {
                if (!checkArgs(param))
                    return 0;

                cell_t *prec;
                ctx->LocalToPhysAddr(params[param++], &prec);
                precision = *prec;

                format++;
            }
        }

        // evaluate specifier
        switch (*format)
        {
            case 'd':
            case 'i':
            case 'u':
            case 'x':
            case 'X':
            case 'o':
            case 'b':
            {
                // set the base
                unsigned int base;
                if (*format == 'x' || *format == 'X')
                    base = 16;

                else if (*format == 'o')
                    base = 8;

                else if (*format == 'b')
                {
                    base = 2;
                    flags &= ~FLAGS_HASH;   // no hash for bin format
                }
                else
                {
                    base = 10;
                    flags &= ~FLAGS_HASH;   // no hash for dec format
                }
                // uppercase
                if (*format == 'X')
                    flags |= FLAGS_UPPERCASE;

                // no plus or space flag for u, x, X, o, b
                if (*format != 'i' && *format != 'd')
                    flags &= ~(FLAGS_PLUS | FLAGS_SPACE);

                if (!checkArgs(param))
                    return 0;

                cell_t *value;
                ctx->LocalToPhysAddr(params[param++], &value);

                // convert the integer
                if (*format == 'i' || *format == 'd')
                {
                    // signed
                    idx += _ntoa_long(&buffer[idx],
                                    (*value > 0 ? *value : -*value),
                                    *value < 0,
                                    base,
                                    buffer_len - idx,
                                    precision,
                                    width,
                                    flags);
                }
                else
                {
                    // unsigned
                    idx += _ntoa_long(&buffer[idx],
                                    *value,
                                    false,
                                    base,
                                    buffer_len - idx,
                                    precision,
                                    width,
                                    flags);
                }
                format++;
                break;
            }
            case 'f':
            case 'F':
            {
                if (!checkArgs(param))
                    return 0;

                cell_t *value;
                ctx->LocalToPhysAddr(params[param++], &value);
                idx += _ftoa(sp_ctof(*value), &buffer[idx], buffer_len - idx, precision, width, flags);
                format++;
                break;
            }
            case 'c':
            {
                if (!checkArgs(param))
                    return 0;

                std::size_t l = 1;
                // pre padding
                if (!(flags & FLAGS_LEFT))
                {
                    while (idx < buffer_len && l++ < width)
                        buffer[idx++] = ' ';
                }
                // char output
                char *character;
                ctx->LocalToString(params[param++], &character);
                buffer[idx++] = *character;
                // post padding
                if (flags & FLAGS_LEFT)
                {
                    while (idx < buffer_len && l++ < width)
                        buffer[idx++] = ' ';
                }
                format++;
                break;
            }
            case 's':
            {
                if (!checkArgs(param))
                    return 0;

                char *p;
                ctx->LocalToString(params[param++], &p);
                std::size_t l = strlen(p);
                // pre padding
                if (flags & FLAGS_PRECISION)
                    l = (l < precision ? l : precision);

                if (!(flags & FLAGS_LEFT))
                {
                    while (idx < buffer_len && l++ < width)
                        buffer[idx++] = ' ';
                }
                // string output
                while (idx < buffer_len && *p && (!(flags & FLAGS_PRECISION) || precision--))
                    buffer[idx++] = *p++;

                // post padding
                if (flags & FLAGS_LEFT)
                {
                    while (idx < buffer_len && l++ < width)
                        buffer[idx++] = ' ';
                }
                format++;
                break;
            }

            case '%':
            {
                buffer[idx++] = '%';
                format++;
                break;
            }
            default:
            {
                buffer[idx++] = *format++;
                break;
            }
        }
    }
    // termination
    if (buffer_len)
        buffer[idx == buffer_len ? buffer_len - 1 : idx] = '\0';

    // return written chars without terminating \0
    return idx;
}
