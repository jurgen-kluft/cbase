#include "ccore/c_target.h"

#include "cbase/c_allocator.h"
#include "ccore/c_vmem.h"

namespace ncore
{
    class allocator_system_t : public alloc_t
    {
    public:
        allocator_system_t()
            : m_vmem(nullptr)
        {
        }

        void init(vmem_allocator_t* vmem)
        {
            if (m_vmem == nullptr)
            {
                m_vmem = vmem;
                m_vmem->reserve(1 * cGB);
            }
        }

        void exit()
        {
            if (m_vmem != nullptr)
            {
                m_vmem->release();
                m_vmem = nullptr;
            }
        }

        virtual void* v_allocate(u32 size, u32 alignment)
        {
            void* ptr = m_vmem->commit((int_t)size, alignment);
            return ptr;
        }

        virtual void v_deallocate(void* ptr)
        {
            // As the system allocator, we do not expect to deallocate memory
        }

        vmem_allocator_t* m_vmem;
    };

    vmem_allocator_t   sSystemVmem = {0};
    allocator_system_t sSystemAllocator;

    void     g_init_system_alloc() { sSystemAllocator.init(&sSystemVmem); }
    void     g_exit_system_alloc() { sSystemAllocator.exit(); }
    alloc_t* g_get_system_alloc() { return &sSystemAllocator; }

};  // namespace ncore
