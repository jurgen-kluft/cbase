#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_base.h"
#include "xbase/x_buffer.h"
#include "xbase/x_console.h"
#include "xbase/x_context.h"
#include "xbase/x_debug.h"
#include "xbase/x_random.h"
#include "xbase/x_runes.h"

namespace xcore
{
    class main_runes_allocator : public runes_alloc_t
    {
    public:
        alloc_t* m_allocator;
        s64 m_alloc_count;

        main_runes_allocator(alloc_t* system_alloc)
            : m_allocator(system_alloc)
            , m_alloc_count(0)
        {
        }

        virtual runes_t allocate(s32 len, s32 cap, s32 type)
        {
            if (len > cap)
                cap = len;

            s32 sizeofrune = 1;
            if (type == utf32::TYPE)
                sizeofrune = 4;
            else if (type == utf16::TYPE)
                sizeofrune = 2;

            runes_t r;
            r.m_type                         = type;
            r.m_ascii.m_bos          = (ascii::prune)m_allocator->allocate(cap * sizeofrune, sizeof(void*));
            r.m_ascii.m_end          = r.m_ascii.m_bos + len * sizeofrune;
            r.m_ascii.m_eos          = r.m_ascii.m_bos + (cap - 1) * sizeofrune;
            r.m_ascii.m_end[0]       = '\0';
            r.m_ascii.m_end[cap - 1] = '\0';

            m_alloc_count++;
            return r;
        }

        virtual void deallocate(runes_t& r)
        {
            if (r.m_ascii.m_bos != nullptr)
            {
                m_alloc_count--;
                m_allocator->deallocate(r.m_ascii.m_bos);
                r = runes_t();
            }
        }
    };

    static runes_alloc_t* get_runes_alloc()
    {
        static runes_alloc_t* s_main_runes_alloc_ptr = nullptr;
        if (s_main_runes_alloc_ptr == nullptr)
        {
            static main_runes_allocator s_main_runes_alloc(xcore::alloc_t::get_system());
            s_main_runes_alloc_ptr = &s_main_runes_alloc;
        }
        return s_main_runes_alloc_ptr;
    }

    extern u64 wyrand(u64* seed);

	class wyrand_t : public random_t
	{
	public:
		virtual				~wyrand_t() {}
		virtual void		reset(s64 inSeed = 0);
		virtual u32			generate();

        XCORE_CLASS_PLACEMENT_NEW_DELETE
	private:
		u64			m_seed;
	};

    void wyrand_t::reset(s64 inSeed) { m_seed = inSeed; }
    u32 wyrand_t::generate() { return (u32)wyrand(&m_seed); }

} // namespace xcore

namespace xbase
{
    void init(xcore::s32 number_of_threads, xcore::s32 temporary_allocator_size)
    {
        xcore::alloc_t::init_system();
        xcore::console_t::init_default_console();
		xcore::context_t::init(number_of_threads, 16, xcore::alloc_t::get_system());
        xcore::context_t::register_thread(); // Should be called once from a created thread

#ifdef X_ASSERT
        xcore::asserthandler_t* assert_handler = xcore::asserthandler_t::sGetDefaultAssertHandler();
#else
        xcore::asserthandler_t* assert_handler = xcore::asserthandler_t::sGetReleaseAssertHandler();
#endif // X_ASSERT

        xcore::alloc_t*       system_allocator = xcore::alloc_t::get_system();
        xcore::runes_alloc_t* string_allocator = xcore::get_runes_alloc();

        // The assert handler, system and string allocator are thread safe
        for (xcore::s32 i = 0; i < number_of_threads; i++)
        {
            const xcore::s32 slot = i;

            xcore::xbyte* buffer_data = (xcore::xbyte*)system_allocator->allocate(temporary_allocator_size);
            xcore::alloc_t*  stack_allocator  = xcore::alloc_t::get_system()->construct<xcore::alloc_buffer_t>(buffer_data, temporary_allocator_size);
            xcore::random_t* random_generator = xcore::alloc_t::get_system()->construct<xcore::wyrand_t>();
            random_generator->reset((xcore::s64)random_generator); // randomize the seed

            xcore::context_t::set_assert_handler(assert_handler);
            xcore::context_t::set_system_alloc(system_allocator);
            xcore::context_t::set_stack_alloc(stack_allocator);
            xcore::context_t::set_string_alloc(string_allocator);
            xcore::context_t::set_random(random_generator);
        }
    }

    void exit()
    {
        xcore::alloc_t*        system_allocator = xcore::context_t::system_alloc();
        xcore::alloc_buffer_t* stack_allocator  = (xcore::alloc_buffer_t*)xcore::context_t::stack_alloc();
        xcore::random_t*       random_generator = xcore::context_t::random();
        xcore::runes_alloc_t*  string_allocator = xcore::get_runes_alloc();

        xcore::context_t::set_system_alloc(nullptr);
        xcore::context_t::set_stack_alloc(nullptr);
        xcore::context_t::set_string_alloc(nullptr);
        xcore::context_t::set_random(nullptr);

        system_allocator->deallocate(random_generator);
        system_allocator->deallocate(stack_allocator->data());
        system_allocator->deallocate(stack_allocator);

        xcore::context_t::exit(system_allocator);
        xcore::alloc_t::exit_system();

        xcore::context_t::set_assert_handler(nullptr);
    }

}; // namespace xbase
