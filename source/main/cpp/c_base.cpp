#include "cbase/c_target.h"
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
    class main_runes_allocator : public runes_alloc_t
    {
    public:
        s64 m_alloc_count;

        main_runes_allocator()
            : m_alloc_count(0)
        {
        }

        virtual runes_t allocate(s32 len, s32 cap, s32 type)
        {
            len = (len + (8 - 1)) & ~(8 - 1);
            if (len > cap)
                cap = len;
            cap = (cap + (8 - 1)) & ~(8 - 1);
            cap = cap + 7;

            s32 sizeofrune = 1;
            if (type == utf32::TYPE)
                sizeofrune = 4;
            else if (type == utf16::TYPE)
                sizeofrune = 2;

            alloc_t* sysalloc = context_t::system_alloc();

            runes_t r;
            r.m_type        = type;
            r.m_ascii.m_bos = (ascii::prune)sysalloc->allocate((cap + 1) * sizeofrune, sizeof(void*));
            r.m_ascii.m_str = r.m_ascii.m_bos;
            r.m_ascii.m_end = r.m_ascii.m_bos + len * sizeofrune;
            r.m_ascii.m_eos = r.m_ascii.m_bos + cap * sizeofrune;

            switch (type)
            {
            case ascii::TYPE:
                r.m_ascii.m_end[0] = '\0';
                r.m_ascii.m_eos[0] = '\0';
                break;
            case utf32::TYPE:
                r.m_utf32.m_end[0] = '\0';
                r.m_utf32.m_eos[0] = '\0';
                break;

            }

            m_alloc_count++;
            return r;
        }

        virtual void deallocate(runes_t& r)
        {
            if (r.m_ascii.m_bos != nullptr)
            {
                m_alloc_count--;
                alloc_t* sysalloc = context_t::system_alloc();
                sysalloc->deallocate(r.m_ascii.m_bos);
                r = runes_t();
            }
        }
    };

    static runes_alloc_t* get_runes_alloc()
    {
        static runes_alloc_t* s_main_runes_alloc_ptr = nullptr;
        if (s_main_runes_alloc_ptr == nullptr)
        {
            static main_runes_allocator s_main_runes_alloc;
            s_main_runes_alloc_ptr = &s_main_runes_alloc;
        }
        return s_main_runes_alloc_ptr;
    }

    extern u64 wyrand(u64* seed);

	class wyrand_t : public random_t
	{
	public:
        wyrand_t() : m_seed(0) {}
		virtual				~wyrand_t() {}
		virtual void		reset(s64 inSeed = 0);
		virtual u32			generate();

        XCORE_CLASS_PLACEMENT_NEW_DELETE
	private:
		u64			m_seed;
	};

    void wyrand_t::reset(s64 inSeed) { m_seed = inSeed; }
    u32 wyrand_t::generate() { return (u32)wyrand(&m_seed); }

} // namespace ncore

namespace cbase
{
    void init(ncore::s32 number_of_threads, ncore::s32 temporary_allocator_size)
    {
        ncore::alloc_t::init_system();
        ncore::console_t::init_default_console();
		ncore::context_t::init(number_of_threads, 16, ncore::alloc_t::get_system());
        ncore::context_t::register_thread(); // Should be called once from a created thread

#ifdef D_ASSERT
        ncore::asserthandler_t* assert_handler = ncore::asserthandler_t::sGetDefaultAssertHandler();
#else
        ncore::asserthandler_t* assert_handler = ncore::asserthandler_t::sGetReleaseAssertHandler();
#endif // D_ASSERT

        ncore::alloc_t*       system_allocator = ncore::alloc_t::get_system();
        ncore::runes_alloc_t* string_allocator = ncore::get_runes_alloc();

        // The assert handler, system and string allocator are thread safe
        for (ncore::s32 i = 0; i < number_of_threads; i++)
        {
            const ncore::s32 slot = i;

            ncore::u8* buffer_data = (ncore::u8*)system_allocator->allocate(temporary_allocator_size);
            ncore::alloc_t*  stack_allocator  = ncore::alloc_t::get_system()->construct<ncore::alloc_buffer_t>(buffer_data, temporary_allocator_size);
            ncore::random_t* random_generator = ncore::alloc_t::get_system()->construct<ncore::wyrand_t>();
            random_generator->reset((ncore::s64)random_generator); // randomize the seed

            ncore::context_t::set_assert_handler(assert_handler);
            ncore::context_t::set_system_alloc(system_allocator);
            ncore::context_t::set_runtime_alloc(system_allocator);
            ncore::context_t::set_frame_alloc(system_allocator);
            ncore::context_t::set_local_alloc(stack_allocator);
            ncore::context_t::set_string_alloc(string_allocator);
            ncore::context_t::set_random(random_generator);
        }
    }

    void exit()
    {
        ncore::alloc_t*        system_allocator = ncore::context_t::system_alloc();
        ncore::alloc_buffer_t* stack_allocator  = (ncore::alloc_buffer_t*)ncore::context_t::local_alloc();
        ncore::random_t*       random_generator = ncore::context_t::random();
        ncore::runes_alloc_t*  string_allocator = ncore::get_runes_alloc();

        ncore::context_t::set_system_alloc(nullptr);
        ncore::context_t::set_runtime_alloc(nullptr);
        ncore::context_t::set_frame_alloc(nullptr);
        ncore::context_t::set_local_alloc(nullptr);
        ncore::context_t::set_string_alloc(nullptr);
        ncore::context_t::set_random(nullptr);

        system_allocator->deallocate(random_generator);
        system_allocator->deallocate(stack_allocator->data());
        system_allocator->deallocate(stack_allocator);

        ncore::context_t::exit(system_allocator);
        ncore::alloc_t::exit_system();

        ncore::context_t::set_assert_handler(nullptr);
    }

}; // namespace cbase
