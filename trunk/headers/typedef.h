#ifndef TYPEDEF_HEADER_INCLUDED
#define TYPEDEF_HEADER_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


//Type definition for a 32 bit field
typedef uint32_t BITFIELD;

//Debouncing and waiting macros
#define DEBOUNCE() SLEEP(10000)
#define SLEEP( TIME ) { uint32_t i; for( i = TIME; i--; ); }

//Macro to get array length
#define ARRAY_SIZE( ARRAY )	( sizeof( ARRAY ) / ARRAY[0] )

#endif	/* TYPEDEF_HEADER_INCLUDED */
