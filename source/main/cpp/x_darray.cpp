#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_debug.h"
#include "xbase/x_context.h"
#include "xbase/x_darray.h"
#include "xbase/x_math.h"

namespace xcore
{
    bool __set_capacity(void*& items, u32 sizeof_item, u32 cur_size, u32& cur_capacity, u32 new_capacity, u32 max_capacity)
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
            ASSERT(new_capacity < max_capacity);

            const u32 desired_size = sizeof_item * new_capacity;
            {
                void*    newitems;
                alloc_t* alloc = context_t::runtime_alloc();
                if (items == nullptr)
                {
                    newitems = alloc->allocate(desired_size);
                }
                else
                {
                    newitems = reallocate(alloc, items, cur_size, desired_size);
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

}; // namespace xcore
