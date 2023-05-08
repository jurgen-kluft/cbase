#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "ccore/c_debug.h"
#include "cbase/c_context.h"
#include "cbase/c_darray.h"
#include "cbase/c_limits.h"

namespace ncore
{
    class default_array_capacity_handler : public array_capacity_handler_t
    {
    public:
        virtual bool set_capacity(void*& items, u32 sizeof_item, u32 cur_size, u32& cur_capacity, u32 new_capacity);
        virtual u32  max_capacity() const { return type_t<u32>::max() >> 1; }
    };

    array_capacity_handler_t* g_get_default_array_capacity_handler()
    {
        static default_array_capacity_handler s_default_array_capacity_handler;
        return &s_default_array_capacity_handler;
    }

    bool default_array_capacity_handler::set_capacity(void*& items, u32 sizeof_item, u32 cur_size, u32& cur_capacity, u32 new_capacity)
    {
        if (new_capacity == cur_capacity)
            return true;

        if (new_capacity == 0)
        {
            if (items != nullptr)
            {
                alloc_t* alloc = context_t::runtime_alloc();
                alloc->deallocate(items);
                cur_capacity = 0;
                items        = nullptr;
            }
        }
        else if (new_capacity > cur_capacity)
        {
            ASSERT(cur_size <= cur_capacity);
            ASSERT(new_capacity < max_capacity());

            {
                void*    newitems;
                alloc_t* alloc = context_t::runtime_alloc();
                if (items == nullptr)
                {
                    newitems = alloc->allocate(new_capacity*sizeof_item);
                }
                else
                {
                    newitems = reallocate(alloc, items, cur_size*sizeof_item, new_capacity*sizeof_item);
                }

                if (!newitems)
                {
                    return false;
                }

                items = newitems;
            }

            cur_capacity = static_cast<u32>(new_capacity);
        }
        else
        {
            // shrink
            return false;
        }
        return true;
    }

}; // namespace ncore
