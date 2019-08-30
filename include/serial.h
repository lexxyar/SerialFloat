#ifndef __SERIAL_H
#define __SERIAL_H

// #include <stddef.h>
#include <stdio.h>
#include "main.h"
#include <string.h>
#include <stdbool.h>

#define DEBUG_BUFFER_SIZE 512 //  Use a larger buffer size so that we don't interrupt USB processing.

#ifdef __cplusplus
extern "C" {
#endif

static char debugBuffer[DEBUG_BUFFER_SIZE + 1] = {0}; //  Buffer to hold output before flushing.
static volatile uint16_t debugBufferLength = 0;       //  Number of bytes in debug buffer.

extern UART_HandleTypeDef huart1;

void debug_append(const char *buffer, unsigned int length);
void debug_print_size_t(size_t l);
void debug_print_int(int i);
void debug_println_int(int i);
void debug_print_string(const char *s);
void debug_println_string(const char *s);
void debug_print_float(float f);
void debug_println_float(float f);
void debug_transmit();

#ifdef __cplusplus
}
#endif

#endif /* __SERIAL_H */