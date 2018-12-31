#ifndef __LIBRARY_PRIVATE_H_
#define __LIBRARY_PRIVATE_H_
#include <stdint.h>

#include "khash.h"
#include "wrappedlibs.h"

typedef struct bridge_s bridge_t;
typedef struct kh_bridgemap_s kh_bridgemap_t;

typedef struct x86emu_s x86emu_t;
typedef void (*wrapper_t)(x86emu_t* emu, uintptr_t fnc);

typedef struct symbol2_s {
    wrapper_t    w;
    const char*  name;
} symbol2_t;

KHASH_MAP_DECLARE_STR(symbolmap, wrapper_t)
KHASH_MAP_DECLARE_STR(symbol2map, symbol2_t)
KHASH_MAP_DECLARE_STR(datamap, uint32_t)


#ifndef MAX_PATH
#define MAX_PATH 4096
#endif

typedef struct wlib_s {
    bridge_t    *bridge;
    void*       lib;        // dlopen result
    void*       priv;       // actual private
    void*       box86lib;   // ref to the dlopen on box86 itself from context
} wlib_t;

typedef struct library_s {
    char*               name;   // <> path
    int                 nbdot;  // nombre of "." after .so
    int                 type;   // 0: native(wrapped) 1: emulated(elf) -1: undetermined
    wrappedlib_fini_t   fini;
    wrappedlib_get_t    get;
    union {
        wlib_t  w;     
    }                   priv;  // private lib data
    bridge_t            *brige;
    kh_bridgemap_t      *bridgemap;
    kh_symbolmap_t      *symbolmap;
    kh_symbolmap_t      *mysymbolmap;
    kh_symbol2map_t     *symbol2map;
    kh_datamap_t        *datamap;
} library_t;

typedef struct dlprivate_s {
    library_t   **libs;
    int         lib_sz;
    int         lib_cap;
} dlprivate_t;

// type for map elements
typedef struct onesymbol_s {
    const char* name;
    wrapper_t   w;
    int         weak;
} onesymbol_t;
typedef struct onesymbol2_s {
    const char* name;
    wrapper_t   w;
    int         weak;
    const char* name2;
} onesymbol2_t;
typedef struct onedata_s {
    const char* name;
    uint32_t    sz;
    int         weak;
} onedata_t;

int getSymbolInMaps(library_t*lib, const char* name, uintptr_t *addr, uint32_t *size);  // Add bridges to functions

#endif //__LIBRARY_PRIVATE_H_