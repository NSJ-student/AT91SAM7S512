#ifndef _PROJECT_H
#define _PROJECT_H

#include "AT91SAM7S256.h"

// 2008.09.10.
#ifndef __ASSEMBLY__

#include "lcd.h"
#include "aic.h"
#include "dbgu.h"

#define MCK 	48000000

void default_fiq_handler();
void default_irq_handler();
void default_spurious_handler();

#endif
// 2008.09.10.

#endif  // _PROJECT_H
