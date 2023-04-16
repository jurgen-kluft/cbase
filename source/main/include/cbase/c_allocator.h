#ifndef __CBASE_ALLOCATOR_H__
#define __CBASE_ALLOCATOR_H__
#include "cbase/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "cbase/c_debug.h"
#include "cbase/c_context.h"
#include "cbase/c_memory.h"

namespace ncore
{
    class buffer_t;

    class alloc_t
    {
    public:
        static void     init_system();
        static void     exit_system();
        static alloc_t* get_system();

        void* allocate(u32 size, u32 alignment) { return v_allocate(size, alignment); }
        void* allocate(u32 size) { return v_allocate(size, sizeof(void*)); }
        u32   deallocate(void* ptr) { return v_deallocate(ptr); }
        void  release() { v_release(); }

        template <typename T, typename... Args> T* construct(Args... args)
        {
            void* mem    = v_allocate(sizeof(T), sizeof(void*));
            T*    object = new (mem) T(args...);
            return object;
        }

        template <typename T, typename... Args> T* obtain_array(u32 count)
        {
            void* mem = v_allocate(sizeof(T) * count, sizeof(void*));
            return (T*)mem;
        }

        template <typename T, typename... Args> T* placement(u32 EXTRA = 0, Args... args)
        {
            void* mem    = v_allocate(sizeof(T) + EXTRA, sizeof(void*));
            T*    object = new (mem) T(args...);
            return object;
        }

        template <typename T> void destruct(T* p)
        {
            p->~T();
            v_deallocate(p);
        }

        template <typename T> void release_array(T* p) { v_deallocate(p); }

    protected:
        virtual void* v_allocate(u32 size, u32 align) = 0; // Allocate memory with alignment
        virtual u32   v_deallocate(void* p)           = 0; // Deallocate/Free memory
        virtual void  v_release()                     = 0;

        virtual ~alloc_t() {}
    };

    inline void* reallocate(alloc_t* alloc, void* ptr, u32 size, u32 new_size, u32 alignment = sizeof(void*))
    {
        void* newptr = alloc->allocate(new_size, alignment);
        nmem::memcpy(newptr, ptr, size);
        alloc->deallocate(ptr);
        return newptr;
    }

    // class new and delete
#define DCORE_CLASS_PLACEMENT_NEW_DELETE                   \
    void* operator new(ncore::uint_t num_bytes, void* mem) \
    {                                                      \
        return mem;                                        \
    }                                                      \
    void operator delete(void* mem, void*)                 \
    {                                                      \
    }                                                      \
    void* operator new(ncore::uint_t num_bytes) noexcept   \
    {                                                      \
        return nullptr;                                    \
    }                                                      \
    void operator delete(void* mem)                        \
    {                                                      \
    }

#define DCORE_CLASS_NEW_DELETE(get_allocator_func, align)             \
    void* operator new(ncore::uint_t num_bytes, void* mem)            \
    {                                                                 \
        return mem;                                                   \
    }                                                                 \
    void operator delete(void* mem, void*)                            \
    {                                                                 \
    }                                                                 \
    void* operator new(ncore::uint_t num_bytes)                       \
    {                                                                 \
        ASSERT(num_bytes < (ncore::uint_t)2 * 1024 * 1024 * 1024);    \
        return get_allocator_func()->allocate((u32)num_bytes, align); \
    }                                                                 \
    void operator delete(void* mem)                                   \
    {                                                                 \
        get_allocator_func()->deallocate(mem);                        \
    }

#define DCORE_CLASS_ARRAY_NEW_DELETE(get_allocator_func, align)       \
    void* operator new[](ncore::uint_t num_bytes)                     \
    {                                                                 \
        ASSERT(num_bytes < (ncore::uint_t)2 * 1024 * 1024 * 1024);    \
        return get_allocator_func()->allocate((u32)num_bytes, align); \
    }                                                                 \
    void operator delete[](void* mem)                                 \
    {                                                                 \
        get_allocator_func()->deallocate(mem);                        \
    }

    // helper functions
    inline void* g_advance_ptr(void* ptr, uint_t size) { return (void*)((ptr_t)ptr + size); }
    inline void* g_align_ptr(void* ptr, u32 alignment) { return (void*)(((ptr_t)ptr + (alignment - 1)) & ~((ptr_t)alignment - 1)); }
    inline ptr_t g_diff_ptr(void* ptr, void* next_ptr) { return (ptr_t)((ptr_t)next_ptr - (ptr_t)ptr); }
    inline bool  g_ptr_in_range(void* buffer, uint_t size, void* ptr)
    {
        ptr_t begin  = (ptr_t)buffer;
        ptr_t end    = begin + size;
        ptr_t cursor = (ptr_t)ptr;
        return cursor >= begin && cursor < end;
    }

    // fixed-size allocator
    class fsa_t
    {
    public:
        inline u32   size() const { return v_size(); }
        inline void* allocate() { return v_allocate(); }
        inline u32   deallocate(void* ptr) { return v_deallocate(ptr); }
        inline void  release() { v_release(); }

        template <typename T, typename... Args> T* construct(Args... args)
        {
            ASSERT(sizeof(T) <= size());
            void* mem    = v_allocate();
            T*    object = new (mem) T(args...);
            return object;
        }

        template <typename T> void destruct(T* p)
        {
            p->~T();
            v_deallocate(p);
        }

    protected:
        virtual u32   v_size() const      = 0;
        virtual void* v_allocate()        = 0;
        virtual u32   v_deallocate(void*) = 0;
        virtual void  v_release()         = 0;

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
        virtual u32   v_deallocate(void* p) { return m_allocator->deallocate(p); }
        virtual void  v_release() { return m_allocator->release(); }

    private:
        alloc_t* m_allocator;
        u32      m_size;
    };

    // The dexer interface, 'pointer to index' and 'index to pointer'
    class dexer_t
    {
    public:
        inline void* idx2ptr(u32 index) const { return v_idx2ptr(index); }
        inline u32   ptr2idx(void* ptr) const { return v_ptr2idx(ptr); }

        template <typename T> inline T*  idx2obj(u32 index) const { return static_cast<T*>(v_idx2ptr(index)); }
        template <typename T> inline u32 obj2idx(T* ptr) const { return v_ptr2idx(ptr); }

    protected:
        virtual void* v_idx2ptr(u32 index) const = 0;
        virtual u32   v_ptr2idx(void* ptr) const = 0;
    };

    class fsadexed_t : public fsa_t, public dexer_t
    {
    public:
        fsadexed_t() {}
        ~fsadexed_t() {}
    };

    // Global new and delete
    template <typename T, typename... Args> T* g_New(Args... args)
    {
        void* mem    = context_t::runtime_alloc()->allocate(sizeof(T));
        T*    object = new (mem) T(args...);
        return object;
    }

    template <typename T> void g_Delete(T* p)
    {
        p->~T();
        context_t::runtime_alloc()->deallocate(p);
    }

    class alloc_buffer_t : public alloc_t
    {
        static inline u8* align_ptr(u8* ptr, ptr_t align) { return (u8*)(((ptr_t)ptr + (align - 1)) & ~(align - 1)); }

        u8* m_base;
        u8* m_ptr;
        u8* m_end;
        s64 m_cnt;

    public:
        alloc_buffer_t(u8* buffer, s64 length);

        u8* data() { return m_base; }

        DCORE_CLASS_PLACEMENT_NEW_DELETE

    protected:
        virtual void* v_allocate(u32 size, u32 align)
        {
            if (m_ptr < m_end && align_ptr(m_ptr + size, align) <= m_end)
            {
                u8* storage = m_ptr;
                m_ptr       = align_ptr(m_ptr + size, sizeof(void*));
                m_cnt += 1;
                return storage;
            }
            return nullptr;
        }

        virtual u32 v_deallocate(void* p)
        {
            if (p != nullptr)
            {
                ASSERT(m_cnt > 0);
                m_cnt -= 1;
                if (m_cnt == 0)
                    m_ptr = m_base;
            }
            return 0;
        }

        virtual void v_release()
        {
            m_base = nullptr;
            m_ptr  = nullptr;
            m_end  = nullptr;
        }
    };

    // Allocate a one or more objects in-place
    class allocinplace_t : public alloc_t
    {
        u8* m_base;
        u8* m_data;
        u64 m_size;

    public:
        inline allocinplace_t(u8* data, u64 size)
            : m_base(data)
            , m_data(data)
            , m_size(size)
        {
        }

    protected:
        virtual void* v_allocate(u32 size, u32 align);
        virtual u32   v_deallocate(void* p);
        virtual void  v_release();
    };

    template <u32 L> class inplace_t
    {
        enum
        {
            SIZE = (L + 3) / 4
        };
        u64 m_memory[SIZE];

    public:
        inline inplace_t() {}

        allocinplace_t               allocator() const { return allocinplace_t((u8*)m_memory, (u64)SIZE * 8); }
        inline u64                   size() const { return (u64)SIZE * 8; }
        template <class T> inline T* object() { return (T*)(m_memory); }
    };

    template <class T> class cdtor_default_t
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

    template <class T> class cdtor_placement_new_t
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
        virtual u32   v_deallocate(void*);
        virtual u32   v_size() const;
        virtual void* v_idx2ptr(u32 index) const;
        virtual u32   v_ptr2idx(void* ptr) const;
        virtual void  v_release();

    private:
        void* m_data;
        u32   m_sizeof;
        u32   m_countof;
        u32   m_freelist;
        u32   m_freeindex;
    };

    namespace talloc
    {
        // frame/tag allocation
        // should this just switch the global and heap allocators on context_t?
        // when you end a frame all memory in that frame is released
        // so for each tag should we use a virtual memory chunk that grows/shrinks?

        typedef u32     tag_id_t;
        extern tag_id_t begin(const char* name);
        extern void     end(tag_id_t id);
    }; // namespace talloc

}; // namespace ncore

#endif ///< __CBASE_ALLOCATOR_H__
