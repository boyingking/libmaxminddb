#ifndef MAXMINDDB_H
#define MAXMINDDB_H

#ifdef __cplusplus
extern "C" {
#endif
#define _GNU_SOURCE
#include <sys/types.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#define MMDB_DTYPE_EXT (0)
#define MMDB_DTYPE_PTR (1)
#define MMDB_DTYPE_UTF8_STRING (2)
#define MMDB_DTYPE_DOUBLE (3)
#define MMDB_DTYPE_BYTES (4)
#define MMDB_DTYPE_UINT16 (5)
#define MMDB_DTYPE_UINT32 (6)
#define MMDB_DTYPE_MAP (7)      /* HASH */
#define MMDB_DTYPE_INT32 (8)
#define MMDB_DTYPE_UINT64 (9)
#define MMDB_DTYPE_UINT128 (10)
#define MMDB_DTYPE_ARRAY (11)
#define MMDB_DTYPE_CONTAINER (12)
#define MMDB_DTYPE_END_MARKER (13)
#define MMDB_DTYPE_BOOLEAN (14)
#define MMDB_DTYPE_FLOAT (15)

#define MMDB_DTYPE_MAX (MMDB_DTYPE_FLOAT)

/* GEOIPDB flags */
#define MMDB_MODE_MMAP (1)
#define MMDB_MODE_MEMORY_CACHE (2)
#define MMDB_MODE_MASK (7)

/* GEOIPDB err codes */
#define MMDB_SUCCESS (0)
#define MMDB_FILE_OPEN_ERROR (1)
#define MMDB_CORRUPT_DATABASE (2)
#define MMDB_INVALID_DATABASE (3)
#define MMDB_IO_ERROR (4)
#define MMDB_OUT_OF_MEMORY (5)
#define MMDB_UNKNOWN_DATABASE_FORMAT (6)
#define MMDB_INVALID_DATA (7)

/* */
#define MMDB_DEBUG (0)

#if MMDB_DEBUG
#define MMDB_DBG_CARP(...) fprintf(stderr, __VA_ARGS__ );
#else
#define MMDB_DBG_CARP(...)
#endif
    /* This is a pointer into the data section for a given IP address lookup */
    typedef struct MMDB_entry_s {
        struct MMDB_s *mmdb;
        uint32_t offset;
    } MMDB_entry_s;

    typedef struct MMDB_lookup_result_s {
        MMDB_entry_s entry;
        uint16_t netmask;
    } MMDB_lookup_result_s;

    typedef struct MMDB_entry_data_s {
        /* return values */
        union {
            uint32_t pointer;
            const char *utf8_string;
            double double_value;
            const uint8_t *bytes;
            uint16_t uint16;
            uint32_t uint32;
            int32_t int32;
            uint64_t uint64;
            uint8_t uint128[16];
            bool boolean;
            float float_value;
        };
        /* This is a 0 if a given entry cannot be found. This can only happen
         * when a call to MMDB_(v)get_value() asks for hash keys or array
         * indices that don't exist. */
        uint32_t offset;
        /* This is the next entry in the data section, but it's really only
         * relevant for entries that part of a larger map or array
         * struct. There's no good reason for an end user to look at this
         * directly. */
        uint32_t offset_to_next;
        /* This is only valid for strings, utf8_strings or binary data */
        uint32_t data_size;
        /* This is an MMDB_DTYPE_* constant */
        uint32_t type;
    } MMDB_entry_data_s;

    /* This is the return type when someone asks for all the entry data in a map or array */
    typedef struct MMDB_entry_data_list_s {
        MMDB_entry_data_s entry_data;
        struct MMDB_entry_data_list_s *next;
    } MMDB_entry_data_list_s;

    typedef struct MMDB_description_s {
        const char *language;
        const char *description;
    } MMDB_description_s;

    typedef struct MMDB_metadata_s {
        uint32_t node_count;
        uint16_t record_size;
        uint16_t ip_version;
        char *database_type;
        struct {
            size_t count;
            const char **names;
        } languages;
        uint16_t binary_format_major_version;
        uint16_t binary_format_minor_version;
        uint64_t build_epoch;
        struct {
            size_t count;
            MMDB_description_s **descriptions;
        } description;
    } MMDB_metadata_s;

    typedef struct MMDB_s {
        uint32_t flags;
        char *filename;
        ssize_t file_size;
        const uint8_t *file_content;
        const uint8_t *data_section;
        uint16_t full_record_byte_size;
        uint16_t depth;
        MMDB_metadata_s metadata;
    } MMDB_s;

    /* --prototypes automatically generated by dev-bin/regen-prototypes.pl - don't remove this comment */
    extern MMDB_lookup_result_s *MMDB_lookup(MMDB_s *mmdb, const char *ipstr,
                                             int *gai_error, int *mmdb_error);
    extern uint16_t MMDB_open(const char *filename, uint32_t flags, MMDB_s *mmdb);
    extern void MMDB_close(MMDB_s *mmdb);
    extern int MMDB_get_value(MMDB_entry_s *start, MMDB_entry_data_s *entry_data, ...);
    extern int MMDB_vget_value(MMDB_entry_s *start, MMDB_entry_data_s *entry_data,
                               va_list params);
    extern int MMDB_get_entry_data_list(MMDB_entry_s *start,
                                        MMDB_entry_data_list_s **entry_data_list);
    extern MMDB_entry_data_list_s *MMDB_alloc_entry_data_list(void);
    extern void MMDB_free_entry_data_list(MMDB_entry_data_list_s *freeme);
    extern const char *MMDB_lib_version(void);
    extern int MMDB_dump(MMDB_entry_data_list_s *entry_data_list, int indent);
    extern const char *MMDB_strerror(uint16_t error_code);
    /* --prototypes end - don't remove this comment-- */

#ifdef __cplusplus
}
#endif
#endif                          /* MAXMINDDB_H */
