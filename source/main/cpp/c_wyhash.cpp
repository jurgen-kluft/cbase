#include "ccore/c_target.h"
#include "cbase/c_allocator.h"
#include "ccore/c_debug.h"
#include "cbase/c_hash.h"
#include "cbase/c_memory.h"
#include "cbase/c_random.h"
#include "cbase/c_wyhash.h"

namespace ncore
{
    namespace nhash
    {
        typedef u8  u8;
        typedef u32 uint32_t;
        typedef u64 u64;

// protections that produce different results:
// 1: normal valid behavior
// 2: extra protection against entropy loss (probability=2^-63), aka. "blind multiplication"
#define WYHASH_PROTECTION 1

// 0: normal version, slow on 32 bit systems
// 1: faster on 32 bit systems but produces different results, incompatible with wy2u0k function
#define WYHASH_32BIT_MUM 0

        // 128bit multiply function
#if (WYHASH_32BIT_MUM)
        static inline u64 _wyrot(u64 x) { return (x >> 32) | (x << 32); }
#endif

        static inline void _wymum(u64* A, u64* B)
        {
#if (WYHASH_32BIT_MUM)
            u64 hh = (*A >> 32) * (*B >> 32), hl = (*A >> 32) * (uint32_t)*B, lh = (uint32_t)*A * (*B >> 32), ll = (u64)(uint32_t)*A * (uint32_t)*B;
#    if (WYHASH_PROTECTION > 1)
            *A ^= _wyrot(hl) ^ hh;
            *B ^= _wyrot(lh) ^ ll;
#    else
            *A = _wyrot(hl) ^ hh;
            *B = _wyrot(lh) ^ ll;
#    endif
#else
            u64 ha = *A >> 32, hb = *B >> 32, la = (uint32_t)*A, lb = (uint32_t)*B, hi, lo;
            u64 rh = ha * hb, rm0 = ha * lb, rm1 = hb * la, rl = la * lb, t = rl + (rm0 << 32), c = t < rl;
            lo = t + (rm1 << 32);
            c += lo < t;
            hi = rh + (rm0 >> 32) + (rm1 >> 32) + c;
#    if (WYHASH_PROTECTION > 1)
            *A ^= lo;
            *B ^= hi;
#    else
            *A = lo;
            *B = hi;
#    endif
#endif
        }

        // multiply and xor mix function, aka MUM
        static inline u64 _wymix(u64 A, u64 B)
        {
            _wymum(&A, &B);
            return A ^ B;
        }

// read functions
#if (D_LITTLE_ENDIAN)
        static inline u64 _wyr8(const u8* p)
        {
            u64 v;
            nmem::memcpy(&v, p, 8);
            return v;
        }
        static inline u64 _wyr4(const u8* p)
        {
            uint32_t v;
            nmem::memcpy(&v, p, 4);
            return v;
        }
#else
        static inline u64 _wyr8(const u8* p)
        {
            u64 v;
            nmem::memcpy(&v, p, 8);
            return (((v >> 56) & 0xff) | ((v >> 40) & 0xff00) | ((v >> 24) & 0xff0000) | ((v >> 8) & 0xff000000) | ((v << 8) & 0xff00000000) | ((v << 24) & 0xff0000000000) | ((v << 40) & 0xff000000000000) | ((v << 56) & 0xff00000000000000));
        }
        static inline u64 _wyr4(const u8* p)
        {
            uint32_t v;
            nmem::memcpy(&v, p, 4);
            return (((v >> 24) & 0xff) | ((v >> 8) & 0xff00) | ((v << 8) & 0xff0000) | ((v << 24) & 0xff000000));
        }
#endif
        static inline u64 _wyr3(const u8* p, uint_t k) { return (((u64)p[0]) << 16) | (((u64)p[k >> 1]) << 8) | p[k - 1]; }

#define _likely_(a)   (a)
#define _unlikely_(a) (a)

        // wyhash main function
        static inline u64 wyhash(const void* key, uint_t len, u64 seed, const u64* secret)
        {
            const u8* p = (const u8*)key;
            seed ^= *secret;
            u64 a, b;
            if (_likely_(len <= 16))
            {
                if (_likely_(len >= 4))
                {
                    a = (_wyr4(p) << 32) | _wyr4(p + ((len >> 3) << 2));
                    b = (_wyr4(p + len - 4) << 32) | _wyr4(p + len - 4 - ((len >> 3) << 2));
                }
                else if (_likely_(len > 0))
                {
                    a = _wyr3(p, len);
                    b = 0;
                }
                else
                    a = b = 0;
            }
            else
            {
                uint_t i = len;
                if (_unlikely_(i > 48))
                {
                    u64 see1 = seed, see2 = seed;
                    do
                    {
                        seed = _wymix(_wyr8(p) ^ secret[1], _wyr8(p + 8) ^ seed);
                        see1 = _wymix(_wyr8(p + 16) ^ secret[2], _wyr8(p + 24) ^ see1);
                        see2 = _wymix(_wyr8(p + 32) ^ secret[3], _wyr8(p + 40) ^ see2);
                        p += 48;
                        i -= 48;
                    } while (_likely_(i > 48));
                    seed ^= see1 ^ see2;
                }
                while (_unlikely_(i > 16))
                {
                    seed = _wymix(_wyr8(p) ^ secret[1], _wyr8(p + 8) ^ seed);
                    i -= 16;
                    p += 16;
                }
                a = _wyr8(p + i - 16);
                b = _wyr8(p + i - 8);
            }
            return _wymix(secret[1] ^ len, _wymix(a ^ secret[1], b ^ seed));
        }

        // the default secret parameters
        // static const u64 _wyp[4] = {0xa0761d6478bd642full, 0xe7037ed1a0b428dbull, 0x8ebc6af09c88c6e3ull, 0x589965cc75374cc3ull};

        // a useful 64bit-64bit mix function to produce deterministic pseudo random numbers that can pass BigCrush and PractRand
        //         static inline u64 wyhash64(u64 A, u64 B)
        //         {
        //             A ^= 0xa0761d6478bd642full;
        //             B ^= 0xe7037ed1a0b428dbull;
        //             _wymum(&A, &B);
        //             return _wymix(A ^ 0xa0761d6478bd642full, B ^ 0xe7037ed1a0b428dbull);
        //         }

        // The wyrand PRNG that pass BigCrush and PractRand
        // Referenced externally in c_base.cpp
        u64 wyrand(u64* seed)
        {
            *seed += 0xa0761d6478bd642full;
            return _wymix(*seed, *seed ^ 0xe7037ed1a0b428dbull);
        }

        void wyrand(u64* seed, u8* buffer, u32 size)
        {
            u64 s = *seed;
            u32 i = 0;
            while (i < size)
            {
                s += 0xa0761d6478bd642full;
                u64 const l  = _wymix(s, s ^ 0xe7037ed1a0b428dbull);
                u8 const* lp = (u8 const*)&l;
                for (u32 j = 0; j < 8 && i < size; j++, i++)
                    buffer[i] = lp[j];
            }
            *seed = s;
        }

        // convert any 64 bit pseudo random numbers to uniform distribution [0,1). It can be combined with wyrand, wyhash64 or wyhash.
        //        static inline double wy2u01(u64 r)
        //        {
        //            const double _wynorm = 1.0 / (1ull << 52);
        //            return (r >> 12) * _wynorm;
        //        }

        // convert any 64 bit pseudo random numbers to APPROXIMATE Gaussian distribution. It can be combined with wyrand, wyhash64 or wyhash.
        //        static inline double wy2gau(u64 r)
        //        {
        //            const double _wynorm = 1.0 / (1ull << 20);
        //            return ((r & 0x1fffff) + ((r >> 21) & 0x1fffff) + ((r >> 42) & 0x1fffff)) * _wynorm - 3.0;
        //        }

#if (!WYHASH_32BIT_MUM)
        // fast range integer random number generation on [0,k) credit to Daniel Lemire. May not work when WYHASH_32BIT_MUM=1. It can be combined with wyrand, wyhash64 or wyhash.
        static inline u64 wy2u0k(u64 r, u64 k)
        {
            _wymum(&r, &k);
            return k;
        }
#endif

        // make your own secret
        static inline void make_secret(u64 seed, u64* secret)
        {
            u8 c[] = {15,  23,  27,  29,  30,  39,  43,  45,  46,  51,  53,  54,  57,  58,  60,  71,  75,  77,  78,  83,  85,  86,  89,  90,  92,  99,  101, 102, 105, 106, 108, 113, 114, 116, 120,
                      135, 139, 141, 142, 147, 149, 150, 153, 154, 156, 163, 165, 166, 169, 170, 172, 177, 178, 180, 184, 195, 197, 198, 201, 202, 204, 209, 210, 212, 216, 225, 226, 228, 232, 240};
            for (uint_t i = 0; i < 4; i++)
            {
                u8 ok;
                do
                {
                    ok        = 1;
                    secret[i] = 0;

                    for (uint_t j = 0; j < 64; j += 8)
                        secret[i] |= ((u64)c[wyrand(&seed) % sizeof(c)]) << j;

                    if (secret[i] % 2 == 0)
                    {
                        ok = 0;
                        continue;
                    }

                    for (uint_t j = 0; j < i; j++)
                    {
                        // manual popcount
                        u64 x = secret[j] ^ secret[i];
                        x -= (x >> 1) & 0x5555555555555555;
                        x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
                        x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
                        x = (x * 0x0101010101010101) >> 56;
                        if (x != 32)
                        {
                            ok = 0;
                            break;
                        }
                    }
                } while (!ok);
            }
        }

        /*  This is world's fastest hash map: 2x faster than bytell_hash_map.
            It does not store the keys, but only the hash/signature of keys.
            First we use pos=hash1(key) to approximately locate the bucket.
            Then we search signature=hash2(key) from pos linearly.
            If we find a bucket with matched signature we report the bucket
            Or if we meet a bucket whose signature=0, we report a new position to insert
            The signature collision probability is very low as we usually searched N~10 buckets.
            By combining hash1 and hash2, we acturally have 128 bit anti-collision strength.
            hash1 and hash2 can be the same function, resulting lower collision resistance but faster.
            The signature is 64 bit, but can be modified to 32 bit if necessary for save space.
            The above two can be activated by define WYHASHMAP_WEAK_SMALL_FAST
            simple examples:
            const	u64	size=213432;
            vector<wyhashmap_t>	idx(size);	//	allocate the index of fixed size. idx MUST be zeroed.
            vector<value_class>	value(size);	//	we only care about the index, user should maintain his own value vectors.
            string  key="dhskfhdsj"	//	the object to be inserted into idx
            u64	pos=wyhashmap(idx.data(), idx.size(), key.c_str(), key.size(), 1);	//	get the position and insert
            if(pos<size)	value[pos]++;	//	we process the vallue
            else	cerr<<"map is full\n";
            pos=wyhashmap(idx.data(), idx.size(), key.c_str(), key.size(), 0);	// just lookup by setting insert=0
            if(pos<size)	value[pos]++;	//	we process the vallue
            else	cerr<<"the key does not exist\n";
        */
        /*
        #ifdef	WYHASHMAP_WEAK_SMALL_FAST	// for small hashmaps whose size < 2^24 and acceptable collision
        typedef	uint32_t	wyhashmap_t;
        #else
        typedef	u64	wyhashmap_t;
        #endif
        static	inline	u64	wyhashmap(wyhashmap_t	*idx,	u64	idx_size,	const	void *key, u64	key_size,	u8	insert, u64 *secret){
            u64	i=1;	u64	h2;	wyhashmap_t	sig;
            do{	sig=h2=wyhash(key,key_size,i,secret);	i++;	}while(_unlikely_(!sig));
        #ifdef	WYHASHMAP_WEAK_SMALL_FAST
            u64	i0=wy2u0k(h2,idx_size);
        #else
            u64	i0=wy2u0k(wyhash(key,key_size,0,secret),idx_size);
        #endif
            for(i=i0;	i<idx_size&&idx[i]&&idx[i]!=sig;	i++);
            if(_unlikely_(i==idx_size)){
                for(i=0;	i<i0&&idx[i]&&idx[i]!=sig;  i++);
                if(i==i0)	return	idx_size;
            }
            if(!idx[i]){
                if(insert)	idx[i]=sig;
                else	return	idx_size;
            }
            return	i;
        }
        */

        static inline s32 wyhashmap(u64* idx, s32 idx_size, const void* key, s32 key_size, u8 insert, u64 const* secret)
        {
            u64 i = 1;
            u64 h2;
            u64 sig;
            do
            {
                sig = h2 = wyhash(key, (uint_t)key_size, (u32)i, secret);
                i++;
            } while (_unlikely_(!sig));

#ifdef WYHASHMAP_WEAK_SMALL_FAST
            u64 i0 = wy2u0k(h2, idx_size);
#else
            u64 i0 = wy2u0k(wyhash(key, (u32)key_size, 0, secret), (u64)idx_size);
#endif
            for (i = i0; i < (u64)idx_size && idx[i] && idx[i] != sig; i++) {}

            if (_unlikely_(i == (u64)idx_size))
            {
                for (i = 0; i < i0 && idx[i] && idx[i] != sig; i++)
                    ;
                if (i == i0)
                    return idx_size;
            }
            if (!idx[i])
            {
                if (insert)
                    idx[i] = sig;
                else
                    return idx_size;
            }
            return (s32)i;
        }

        wyreg::wyreg()
        {
            m_allocator = nullptr;
            m_size      = 0;
            m_count     = 0;
            m_index     = nullptr;
            m_secret[0] = 0;
            m_secret[1] = 0;
            m_secret[2] = 0;
            m_secret[3] = 0;
        }

        wyreg::~wyreg()
        {
            if (m_index)
            {
                m_allocator->deallocate(m_index);
                m_index = nullptr;
            }
        }

        void wyreg::init(alloc_t* allocator, s32 size, u64 seed)
        {
            m_allocator = allocator;
            m_size      = size;
            m_count     = 0;
            m_index     = (u64*)m_allocator->allocate((u32)sizeof(u64) * (u32)m_size);
            nmem::memset(m_index, 0, (s64)((s32)sizeof(u64) * m_size));
            make_secret(seed, m_secret);
        }

        bool wyreg::insert(void* key, s32 keysize, s32& pos)
        {
            pos = wyhashmap(m_index, m_size, key, keysize, 1, m_secret);
            if (pos < m_size)
            {
                m_count++;
                return true;
            }
            return false;
        }

        bool wyreg::find(void* key, s32 keysize, s32& pos) const
        {
            pos = wyhashmap(m_index, m_size, key, keysize, 0, m_secret);
            return pos < m_size;
        }

        bool wyreg::remove(void* key, s32 keysize, s32& pos)
        {
            pos = wyhashmap(m_index, m_size, key, keysize, 0, m_secret);
            if (pos < m_size)
            {
                // Do we need to 'move' matching 'keys' one position back?

                m_index[pos] = 0;

                m_count--;
                return true;
            }
            return false;
        }
    }  // namespace nhash
}  // namespace ncore
