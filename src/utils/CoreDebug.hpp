#ifndef COREDEBUG_H
#define COREDEBUG_H

#include <stdint.h> 

void reportErrorMessage(const char *str, uint16_t end);
void reportMessageNonError(const char *str);

#endif