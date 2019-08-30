
/* Includes ------------------------------------------------------------------*/
#include "serial.h"

void debug_append(const char *buffer, unsigned int length)
{
    //  Append "length" number of bytes from "buffer" to the debug buffer.
    if (length >= DEBUG_BUFFER_SIZE)
    {
        return;
    } //  Don't allow logging of very long messages.
    if (debugBufferLength + length >= DEBUG_BUFFER_SIZE)
    {
        //  Erase the entire buffer.  Latest log is more important than old log.
        strcpy(debugBuffer, "[DROPPED]");
        debugBufferLength = 9;
        //  Still can't fit after clearing.  Quit.
        if (debugBufferLength + length >= DEBUG_BUFFER_SIZE)
        {
            return;
        }
    }
    //  Else append to the buffer.
    memcpy(&debugBuffer[debugBufferLength], buffer, length);
    debugBufferLength += length;
}
void debug_print_size_t(size_t l)
{
    //  We only print up to 10 digits, since 32 bits will give max 4,294,967,296.
    //  char buf[32]; sprintf(buf, "%u", l); debug_append(buf, strlen(buf)); return; ////  TODO

#define MAX_INT_LENGTH 10
    char buffer[MAX_INT_LENGTH + 1];
    int size = MAX_INT_LENGTH + 1;
    uint8_t prefixByZero = false;
    int length = 0;
    for (size_t divisor = 1000000000ul; divisor >= 1; divisor = divisor / 10)
    {
        char digit = '0' + (char)(l / divisor);
        if (digit > '9')
        {
            debug_print_string((char *)"(Overflow)");
            return;
        }
        if (digit > '0' || length > 0 || prefixByZero)
        {
            if (length < size)
            {
                buffer[length++] = digit;
            }
        }
        l = l % divisor;
    }
    if (length == 0)
    {
        buffer[length++] = '0';
    };
    if (length < size)
        buffer[length] = 0;
    buffer[size - 1] = 0; //  Terminate in case of overflow.

    debug_append(buffer, strlen(buffer));
}
void debug_print_int(int i)
{
    if (i == 0)
    {
        debug_append("0", 1);
    }
    else if (i >= 0)
    {
        debug_print_size_t((size_t)i);
    }
    else
    { // i < 0.
        debug_append("-", 1);
        debug_print_size_t((size_t)-i);
    }
    debug_transmit();
}
void debug_print_string(const char *s)
{
    if (s[0] == 0)
        return;
    debug_append(s, strlen(s));
    debug_transmit();
}
void debug_print_float(float f)
{
    //  Assume max 10 digits to the left of decimal point and 2 digits to the right.
    if (f == 0)
    {
        debug_append("0.00", 4);
        return;
    }
    else if (f < 0)
    {
        debug_append("-", 1);
        f = -f;
    }
    //  Print the digits left of the decimal point.
    debug_print_size_t((size_t)f);
    debug_append(".", 1);
    //  Print the 2 digits right of the decimal point.
    f = f * 10.0;
    debug_print_size_t(((size_t)f) % 10);
    f = f * 10.0;
    debug_print_size_t(((size_t)f) % 10);

    debug_transmit();
}

void debug_transmit(){
    HAL_UART_Transmit(&huart1, debugBuffer, debugBufferLength, 1000);
}

void debug_println_int(int i){
    debug_println_int(i);
    debug_print_string("\r\n");
}
void debug_println_string(const char *s){
    debug_print_string(s);
    debug_print_string("\r\n");
}
void debug_println_float(float f){
    debug_print_float(f);
    debug_print_string("\r\n");
}