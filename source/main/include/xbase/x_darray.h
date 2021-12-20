#ifndef __XBASE_DARRAY_H__
#define __XBASE_DARRAY_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
    // only for types that support bitwise move
    // size is the number of actual items, less or equal to the capacity
    // capacity = maximum number of items
    //
    // --------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------
    // We could create a map<K,V> where the K and V items are being sourced from a darray.
    // Then the api for the map can become:
    //
    // map(); // this will create standard darray's for k, v and tree nodes
    // map(k_darray, v_darray, tree_node_darray);
    //
    // bool map::insert(u32 k_index, u32 v_index); // This will use the entries from the k_darray and v_darray
    // bool map::insert(u32 k_index, const V& value); // This will add K to k_darray and V to v_darray
    // bool map::insert(const K& key, u32 v_index); // This will add K to k_darray and use V from the v_darray
    // bool map::insert(const K& key, const V& value); // This will add K to k_darray and V to v_darray
    //
    // bool map::find(u32 k_index, u32& v_index) or
    // bool map::find(const K* key, u32& v_index) where key* should be an entry in the k_darray
    // bool map::find(const K& key, u32& v_index) where key  can be a separate value
    //
    // bool map::remove(k_index);
    // bool map::remove(const K& key);
    //
    //
    // --------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------
    // A vector_t could also use the darray interface
    //
    //
    // --------------------------------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------------------------------
    // note: virtual memory can also create a darray that supports growing
    //

    const s32 darray_offset_size        = -4;
    const s32 darray_offset_capacity    = -8;

    typedef bool (*set_capacity_fn)(void*& darray, u32 sizeof_item, u32 new_item_count);
    void* __create_darray(u32 sizeof_item, u32 initial_size, u32 initial_capacity, u32 maximum_capacity, set_capacity_fn set_capacity = nullptr);
    void  __destroy_darray(void*& darray);

    bool       __set_size(void* darray, u32 size);
    inline u32 __get_size(void* darray)
    {
        xbyte const* header = (xbyte const*)darray;
        return *((u32*)&header[darray_offset_size]);
    }
    bool       __set_cap(void*& darray, u32 sizeof_item, u32 new_capacity);
    inline u32 __get_cap(void* darray)
    {
        xbyte const* header = (xbyte const*)darray;
        return *((u32*)&header[darray_offset_capacity]);
    }
    bool  __add_item(void*& darray, u32 sizeof_item, void* item);
    bool  __set_item(void* darray, u32 sizeof_item, u32 index, void* item);
    void* __get_item(void* darray, u32 sizeof_item, u32 index);
    bool  __equal_items(void* darray, u32 sizeof_item, u32 lhs_index, u32 rhs_index);
    s32   __compare_items(void* darray, u32 sizeof_item, u32 lhs_index, u32 rhs_index);

    template <typename T> inline T* create_darray(u32 initial_size, u32 initial_capacity, u32 maximum_capacity, set_capacity_fn set_capacity = nullptr)
    {
        return (T*)__create_darray(sizeof(T), initial_size, initial_capacity, maximum_capacity, set_capacity);
    }
    template <typename T> inline void destroy_darray(T*& darray) { __destroy_darray((void*&)darray); }
    template <typename T> inline bool set_size(T* darray, u32 size) { return __set_size((void*)darray, size); }
    template <typename T> inline u32  get_size(T* darray) { return __get_size((void*)darray); }
    template <typename T> inline bool set_cap(T*& darray, u32 new_capacity) { return __set_cap((void*&)darray, sizeof(T), new_capacity); }
    template <typename T> inline u32  get_cap(T* darray) { return __get_cap((void*)darray); }
    template <typename T> inline bool add_item(T*& darray, T const& item)
    {
        u32 const index = __get_size(darray);
        __set_size(darray, index + 1);
        darray[index] = item;
		return true;
    }
    template <typename T> inline bool set_item(T* darray, u32 index, T const& item) { darray[index] = item; }
    template <typename T> inline T*   get_item(T* darray, u32 index) { return &darray[index]; }

    template <typename T> inline bool equal_items(T* darray, u32 lhs_index, u32 rhs_index)
    {
        T* lhs = &darray[lhs_index];
        T* rhs = &darray[rhs_index];
        return *lhs == *rhs;
    }

    template <typename T> inline s32 compare_items(T* darray, u32 lhs_index, u32 rhs_index)
    {
        T* lhs = &darray[lhs_index];
        T* rhs = &darray[rhs_index];
        if (*lhs < *rhs)
            return -1;
        else if (*lhs > *rhs)
            return -1;
        return 0;
    }

}; // namespace xcore

#endif // __XBASE_CARRAY_H__
