#include "xbase/x_target.h"
#include "xbase/x_allocator.h"
#include "xbase/x_base.h"
#include "xbase/x_buffer.h"
#include "xbase/x_console.h"
#include "xbase/x_debug.h"

namespace xbase
{
#ifdef X_ASSERT
    void x_Init()
    {
        xcore::alloc_t::init_system();
        xcore::console_t::init_default_console();
        xcore::asserthandler_t::sRegisterHandler(NULL); // This will initialize the default handler
    }

    void x_Exit()
    {
        xcore::asserthandler_t::sRegisterHandler(NULL); // This will initialize the default handler
    }
#else
    void x_Init()
    {
        xcore::alloc_t::init_system();
        xcore::console_t::init_default_console();
    }

    void x_Exit() {}
#endif
}; // namespace xbase
