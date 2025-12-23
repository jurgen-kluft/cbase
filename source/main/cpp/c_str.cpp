#include "ccore/c_target.h"
#include "cbase/c_integer.h"
#include "cbase/c_memory.h"

#include "cbase/c_str.h"

namespace ncore
{
    struct blob8_t;

    // ------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------
    // TODO: Once we have a proper arena implementation, we can reduce
    //       the pointers to indices into the arenas.
    //       Basically reducing this structure to 16 bytes on a 64-bit system.
    struct str8_t
    {
        blob8_t* m_data;      // pointer to string data blob
        u16      m_from;      // starting index in blob (max blob size is 65536 bytes)
        u16      m_to;        // length of string
        str8_t*  m_previous;  // index of previous str8_t in arena, or INVALID_INDEX
        str8_t*  m_next;      // index of next str8_t in arena, or INVALID_INDEX
    };

    struct str8_db_t  // database of str8_t
    {
        arena_t* m_str8_arena;    // arena holding str8_t structs
        str8_t*  m_strings;       // array of str8_t pointers
        u32      m_string_count;  // number of str8_t pointers in the array
    };

    str8_t* str8db_alloc(str8_db_t* db)
    {
        // todo
        return nullptr;
    }

    // ------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------

    struct data8_db_t
    {
        arena_t** m_arenas;  // array of arenas holding string data (power of two sizes)
        u32       m_count;   // number of arenas holding string data
    };

    char* data8db_alloc(data8_db_t* db, u32 len)
    {
        // todo
        return nullptr;
    }

    // ------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------

    struct blob8_t
    {
        char const* m_string;     // pointer to string data
        u32         m_length;     // length of data in bytes
        u32         m_reference;  // reference count
    };

    struct blob8_db_t  // database of blob8_t
    {
        arena_t* m_blob8_arena;  // arena holding blob8_t structs
        blob8_t* m_blobs;        // array of blob8_t pointers
        u32      m_blob_count;   // number of blob8_t pointers in the array
    };

    blob8_t* blob8db_alloc(blob8_db_t* db, u32 len)
    {
        // todo
        return nullptr;
    }

    blob8_t* blob8db_alloc(blob8_db_t* db, char const* cstr, u32 len)
    {
        // todo
        return nullptr;
    }

    // ------------------------------------------------------------------------------------

    struct str8_data_t
    {
        blob8_db_t m_blob8_db;  // arena holding blob8_t structs
        str8_db_t  m_str8_db;   // arena holding str8_t structs
    };

    static str8_data_t s_str8_data;

    str8_t* str_make(u32 len)
    {
        // create a str8_t, if size == 0, then we will not point to any data blob
        // for any other size, we clamp it to the maximum blob size, since we can
        // always chain multiple str8_t structs together.
        str8_t* str = str8db_alloc(&s_str8_data.m_str8_db);

        if (len > 0) {}

        return str;
    }

}  // namespace ncore
