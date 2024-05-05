#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "cbase/c_base.h"
#include "cbase/c_buffer.h"
#include "cbase/c_console.h"
#include "cbase/c_context.h"
#include "cbase/c_debug.h"
#include "cbase/c_random.h"
#include "cbase/c_runes.h"

namespace ncore
{
    namespace nhash
    {
        extern void wyrand(u64* seed, u8* buffer, u32 size);
    }  // namespace nhash

    // Note: This is the default random generator that is used by context_t
    // The user is able to change the random generator by calling context_t::set_random()
    class wyrand_t : public random_t
    {
    public:
        wyrand_t()
            : m_seed(0)
        {
        }
        virtual ~wyrand_t() {}
        virtual void reset(s64 inSeed = 0);
        virtual void generate(u8* outData, u32 numBytes);

        DCORE_CLASS_PLACEMENT_NEW_DELETE
    private:
        u64 m_seed;
    };

    void wyrand_t::reset(s64 inSeed) { m_seed = inSeed; }
    void wyrand_t::generate(u8* outData, u32 numBytes) { nhash::wyrand(&m_seed, outData, numBytes); }

}  // namespace ncore

namespace cbase
{
    void init(ncore::s32 number_of_threads, ncore::s32 temporary_allocator_size)
    {
        ncore::init_system_alloc();

        ncore::console_t::init_default_console();
        ncore::context_t::init(number_of_threads, 16, ncore::get_system_alloc());
        ncore::context_t::register_thread();  // Should be called once from a created thread

        // The assert handler, system and string allocator are thread safe
        ncore::alloc_t*         system_allocator = ncore::get_system_alloc();
        ncore::asserthandler_t* assert_handler   = gSetAssertHandler();
        for (ncore::s32 i = 0; i < number_of_threads; i++)
        {
            const ncore::s32 slot = i;

            ncore::u8*       buffer_data      = (ncore::u8*)system_allocator->allocate(temporary_allocator_size);
            ncore::alloc_t*  stack_allocator  = ncore::get_system_alloc()->construct<ncore::alloc_buffer_t>(buffer_data, temporary_allocator_size);
            ncore::random_t* random_generator = ncore::get_system_alloc()->construct<ncore::wyrand_t>();
            random_generator->reset((ncore::s64)random_generator);  // randomize the seed

            ncore::context_t::set_assert_handler(assert_handler);
            ncore::context_t::set_system_alloc(system_allocator);
            ncore::context_t::set_runtime_alloc(system_allocator);
            ncore::context_t::set_frame_alloc(system_allocator);
            ncore::context_t::set_local_alloc(stack_allocator);
            ncore::context_t::set_random(random_generator);
        }
    }

    void exit()
    {
        ncore::alloc_t*        system_allocator = ncore::context_t::system_alloc();
        ncore::alloc_buffer_t* stack_allocator  = (ncore::alloc_buffer_t*)ncore::context_t::local_alloc();
        ncore::random_t*       random_generator = ncore::context_t::random();

        ncore::context_t::set_system_alloc(nullptr);
        ncore::context_t::set_runtime_alloc(nullptr);
        ncore::context_t::set_frame_alloc(nullptr);
        ncore::context_t::set_local_alloc(nullptr);
        ncore::context_t::set_random(nullptr);

        system_allocator->deallocate(random_generator);
        system_allocator->deallocate(stack_allocator->data());
        system_allocator->deallocate(stack_allocator);

        ncore::context_t::exit(system_allocator);
        ncore::exit_system_alloc();

        ncore::context_t::set_assert_handler(nullptr);
    }

};  // namespace cbase
