#include "ccore/c_target.h"
#include "cbase/c_wyhash.h"
#include "cbase/c_random.h"

namespace ncore
{
    void wyrand_t::reset(s64 inSeed) { m_seed = (u64)inSeed; }
    u32  wyrand_t::rand32() { return (u32)nhash::wyrand(&m_seed); }
    u64  wyrand_t::rand64() { return nhash::wyrand(&m_seed); }
    void wyrand_t::generate(u8* outData, u32 numBytes) { nhash::wyrand(&m_seed, outData, numBytes); }

}  // namespace ncore
