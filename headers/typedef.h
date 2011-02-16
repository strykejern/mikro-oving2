#ifndef TYPEDEF_HEADER_INCLUDED
#define TYPEDEF_HEADER_INCLUDED

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t BITFIELD;

#define DEBOUNCE() SLEEP(10000)
#define SLEEP( TIME ) { uint32_t i; for( i = TIME; i--; ); }

#endif	/* TYPEDEF_HEADER_INCLUDED */
