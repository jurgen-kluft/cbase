#ifndef __CBASE_ALLOCATOR_H__
#define __CBASE_ALLOCATOR_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "ccore/c_allocator.h"
#include "ccore/c_debug.h"
#include "cbase/c_context.h"
#include "cbase/c_memory.h"

namespace ncore
{
    class buffer_t;

    void     init_system_alloc();
    void     exit_system_alloc();
    alloc_t* get_system_alloc();

    inline void* reallocate(alloc_t* alloc, void* ptr, u32 size, u32 new_size, u32 alignment = sizeof(void*))
    {
        void* newptr = alloc->allocate(new_size, alignment);
        nmem::memcpy(newptr, ptr, size);
        alloc->deallocate(ptr);
        return newptr;
    }

    // fixed-size allocator
    class fsa_t
    {
    public:
        inline u32   allocsize() const { return v_allocsize(); }
        inline void* allocate() { return v_allocate(); }
        inline void  deallocate(void* ptr) { v_deallocate(ptr); }

        template <typename T, typename... Args>
        T* construct(Args... args)
        {
            ASSERT(sizeof(T) <= allocsize());
            void* mem    = v_allocate();
            T*    object = new (mem) T(args...);
            return object;
        }

        template <typename T>
        void destruct(T* p)
        {
            p->~T();
            v_deallocate(p);
        }

    protected:
        virtual u32   v_allocsize() const = 0;
        virtual void* v_allocate()        = 0;
        virtual void  v_deallocate(void*) = 0;

        virtual ~fsa_t() {}
    };

    class fsa_to_alloc_t : public fsa_t
    {
    public:
        fsa_to_alloc_t()
            : m_allocator(nullptr)
            , m_size(0)
        {
        }
        fsa_to_alloc_t(u32 size, alloc_t* allocator)
            : m_allocator(allocator)
            , m_size(size)
        {
        }

    protected:
        virtual u32   v_size() const { return m_size; }
        virtual void* v_allocate() { return m_allocator->allocate(m_size, sizeof(void*)); }
        virtual void  v_deallocate(void* p) { m_allocator->deallocate(p); }

    private:
        alloc_t* m_allocator;
        u32      m_size;
    };

    // The dexer interface, 'pointer to index' and 'index to pointer'
    class dexer_t
    {
    public:
        inline void* idx2ptr(u32 index)  { return v_idx2ptr(index); }
        inline u32   ptr2idx(void const* ptr) const { return v_ptr2idx(ptr); }

        template <typename T>
        inline T* idx2obj(u32 index)
        {
            return static_cast<T*>(v_idx2ptr(index));
        }
        template <typename T>
        inline u32 obj2idx(T const* ptr) const
        {
            return v_ptr2idx(ptr);
        }

    protected:
        virtual void* v_idx2ptr(u32 index) = 0;
        virtual u32   v_ptr2idx(void const* ptr) const = 0;
    };

    class fsadexed_t : public fsa_t, public dexer_t
    {
    public:
        fsadexed_t() {}
        ~fsadexed_t() {}
    };

    // Global new and delete
    template <typename T, typename... Args>
    T* g_New(Args... args)
    {
        void* mem    = context_t::runtime_alloc()->allocate(sizeof(T));
        T*    object = new (mem) T(args...);
        return object;
    }

    template <typename T>
    void g_Delete(T* p)
    {
        p->~T();
        context_t::runtime_alloc()->deallocate(p);
    }

    template <class T>
    class cdtor_default_t
    {
    public:
        inline T* construct(void* mem) const { return static_cast<T*>(mem); }
        inline T* copy_construct(void* mem, T const* other) const
        {
            T* item = (T*)(mem);
            *item   = *other;
            return item;
        }
        inline void destruct(T* obj) const {}
    };

    template <class T>
    class cdtor_placement_new_t
    {
    public:
        inline T*   construct(void* mem) const { return new (mem) T(); }
        inline T*   copy_construct(void* mem, T const* other) const { return new (mem) T(*other); }
        inline void destruct(T* obj) const { obj->~T(); }
    };

    class dexed_array_t : public dexer_t
    {
    public:
        dexed_array_t()
            : m_data(nullptr)
            , m_sizeof(0)
            , m_countof(0)
        {
        }
        dexed_array_t(void* array_item, u32 sizeof_item, u32 countof_item);

        DCORE_CLASS_PLACEMENT_NEW_DELETE
    protected:
        virtual void* v_idx2ptr(u32 index) const;
        virtual u32   v_ptr2idx(void* ptr) const;

    private:
        void* m_data;
        u32   m_sizeof;
        u32   m_countof;
    };

    class fsadexed_array_t : public fsadexed_t
    {
    public:
        fsadexed_array_t();
        fsadexed_array_t(void* array_item, u32 sizeof_item, u32 countof_item);
        ~fsadexed_array_t() {}

        DCORE_CLASS_PLACEMENT_NEW_DELETE

    protected:
        virtual void* v_allocate();
        virtual void  v_deallocate(void*);
        virtual u32   v_allocsize() const;
        virtual void* v_idx2ptr(u32 index) const;
        virtual u32   v_ptr2idx(void* ptr) const;

    private:
        void* m_data;
        u32   m_sizeof;
        u32   m_countof;
        u32   m_freelist;
        u32   m_freeindex;
    };

};  // namespace ncore

#endif  ///< __CBASE_ALLOCATOR_H__
