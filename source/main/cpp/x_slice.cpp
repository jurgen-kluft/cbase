#include "xbase/x_target.h"
#include "xbase/x_slice.h"

namespace xcore
{
	slice_data::slice_data()
	{
		mAllocator = NULL;
		mRefCount = 0;
		mSize = 0;
		mData = NULL;
	}

	slice_data::slice_data(u32 size)
	{
		mAllocator = NULL;
		mRefCount = 0;
		mSize = size;
		mData = NULL;
	}

	slice_data::slice_data(xbyte* data, u32 size)
	{
		mAllocator = NULL;
		mRefCount = 0;
		mSize = size;
		mData = data;
	}

	slice_data	sNull;

	slice_data* 	slice_data::incref()
	{ 
		if (mAllocator != NULL) 
			mRefCount++; 
		return (slice_data*)this; 
	}
	slice_data* 	slice_data::incref() const
	{ 
		if (mAllocator != NULL) 
			mRefCount++; 
		return (slice_data*)this;
	}

	slice_data*		slice_data::decref()
	{
		if (mAllocator == NULL)
			return this;

		s32 const refs = mRefCount;
		if (refs == 0)
			return &sNull;
		if (refs == 1)
		{
			mAllocator->deallocate(this);
			return &sNull;
		}

		mRefCount--;
		return this;
	}

	slice_data*		slice_data::alloc(x_iallocator* allocator, s32& tosize)
	{
		tosize = x_intu::align(tosize, 16);
		slice_data* data = (slice_data*)allocator->allocate(tosize + sizeof(slice_data), 16);
		data->mRefCount = 1;
		data->mSize = tosize;
		data->mAllocator = allocator;
		data->mData = (xbyte*)data + sizeof(slice_data);
		return data;
	}

	slice_data*	slice_data::resize(s32 from, s32 to)
	{
		slice_data* data = &sNull;
		if (mAllocator != NULL)
		{
			s32 tosize = x_intu::align(to - from, 16);
			data = (slice_data*)mAllocator->allocate(tosize + sizeof(slice_data), 16);
			data->mAllocator = mAllocator;
			data->mRefCount = 1;
			data->mSize = tosize;
			data->mData = (xbyte*)data + sizeof(slice_data);
			u32 const size2copy = x_intu::min(tosize, mSize);
			xmem_utils::memcpy(data->mData, this->mData + from, size2copy);
			decref();
		}
		return data;
	}

	slice_data	slice_data::sNull;


	slice::slice()
	{
		mData = NULL;
		mFrom = 0;
		mTo = 0;
	}

	slice::slice(slice_data* blk, s32 from, s32 to)
	{
		mData = blk->incref();
		mFrom = from;
		mTo = to;
	}

	slice::slice(slice const& other)
	{
		mData = other.mData->incref();
		mFrom = other.mFrom;
		mTo = other.mTo;
	}

	slice::slice(x_iallocator* allocator, s32 size)
	{
		mData = slice_data::alloc(allocator, size);
		mFrom = 0;
		mTo = size;
	}

	slice::~slice()
	{
		mData->decref();
		mFrom = 0;
		mTo = 0;
	}

	void			slice::alloc(slice& slice, x_iallocator* allocator, s32 tosize) 
	{ 
		slice.mData = slice_data::alloc(allocator, tosize); 
		slice.mFrom = 0; 
		slice.mTo = tosize; 
	}

	void			slice::resize(s32 len) 
	{ 
		mData = mData->resize(mFrom, mFrom + len); 
		mFrom = 0; 
		mTo = len; 
	}

	s32				slice::length() const 
	{ 
		return mTo - mFrom; 
	}

	slice			slice::view(u32 from, u32 to) const
	{
		slice s;
		if ((mFrom + from) < mTo && (mFrom + to) <= mTo)
		{
			s.mData = mData->incref();
			s.mFrom = mFrom + from;
			s.mTo = mFrom + to;
			return s;
		}
		s.mData = &slice_data::sNull;
		return s;
	}

	void			slice::release() 
	{ 
		mData = mData->decref(); 
	}



	// ----------------------------------------------------------------------------------------
	//   TEST FUNCTION
	// ----------------------------------------------------------------------------------------
	template<typename T>
	class range_iter_t;

	template<typename T>
	struct from_t
	{
		from_t(T from) : m_from(from) {}
		T m_from;
	};
	template<typename T>
	struct until_t
	{
		until_t(T until) : m_until(until) {}
		T m_until;
	};
	template<typename T>
	struct step_t
	{
		step_t(T s) : m_step(s) {}
		T m_step;
	};

	struct count_t
	{
		count_t(s32 c) : m_count(c) {}
		s32 m_count;
	};

	template<typename T>
	from_t<T>	from(T value) { return from_t<T>(value); }
	template<typename T>
	until_t<T>	until(T value) { return until_t<T>(value); }
	template<typename T>
	step_t<T>	step(T value) { return step_t<T>(value); }

	count_t		count(s32 value) { return value; }


	template<typename T>
	class range_t
	{
	public:
		inline		range_t(until_t<T> _until);		
		inline		range_t(from_t<T> _from, until_t<T> _until);
		inline		range_t(from_t<T> _from, count_t _count);
		inline		range_t(range_t<T> const& range) ;

		s32			index() const	{ return m_index; }
		T			item() const	{ return m_current; }

		enum EState { STATE_START, STATE_ITER, STATE_END };
		enum EMode { MODE_INIT, MODE_ITER };

		bool		forward()
		{
			switch (iter.m_state) {
		 	case range_t<T>::STATE_START:
				m_index = 0;
				m_current = m_from;
				m_state = STATE_ITER;
				break;
			case range_t<T>::STATE_ITER:
				iter.m_index += 1;
				iter.m_current += iter.m_step;
				if (iter.m_current >= iter.m_to) {
					iter.m_state = STATE_END;
				}
				break;
			case STATE_END:
				return false;
			}
			return true;
		}

		bool		backward()
		{
			switch (iter.m_state) {
		 	case range_t<T>::STATE_START:
				m_index = 0;
				m_current = m_to - m_step;
				m_state = STATE_ITER;
				break;
			case range_t<T>::STATE_ITER:
				iter.m_index += 1;
				iter.m_current -= iter.m_step;
				if (iter.m_current <= iter.m_from) {
					iter.m_state = STATE_END;
				}
				break;
			case STATE_END:
				return false;
			}
			return true;
		}

		T			m_from;
		T			m_to;
		s32			m_state;
		s32			m_index;
		T			m_current;
		T			m_step;
	};

	
	template<typename T>
	inline range_t::range_t(until_t<T> u)
		: m_state(STATE_START)
		, m_index(0)
		, m_step(1)
	{
		m_from = 0;
		m_to = _until.m_until;
		m_current = m_from;
	}

	template<typename T>
	inline range_t::range_t(from_t<T> _from, until_t<T> _until)
		: m_state(STATE_START)
		, m_index(0)
		, m_step(1)
	{
		m_from = _from.m_from;
		m_to = _until.m_until;
		m_current = m_from;
	}

	template<typename T>
	inline range_t::range_t(from_t<T> _from, count_t _count)
		: m_state(STATE_START)
		, m_index(0)
		, m_step(1)
	{
		m_from = _from.m_from;
		m_to = m_from + _count;
		m_current = m_from;
	}

	template<typename T>
	bool		forward(range_t<T>& iter)
	{
		return iter.forward();
	}

	template<typename T>
	bool		backward(range_t<T>& iter)
	{
		return iter.backward();
	}

	static void test_t(x_iallocator* allocator)
	{
		memory mem(allocator);

		array_t<s32> integers;
		make(mem, integers, 100, 0);

		// New Version: 99 -> 0
		range_t<s32> iter(from(0), count(100));
		while (forward(iter)) {
			append(integers, iter.item());
		}
		// 0 -> 99
		sort(integers);

		f32 f = 0.0f;
		map_t<s32, f32> int_to_float;
		make(mem, int_to_float, 100);

		auto iter = integers.begin();
		while (iterate(iter, integers)) {
			int_to_float[*iter] = f;
			f += 0.1f;
		}

		auto int_0_1 = integers(0, 2);

		s32 int0 = int_0_1[0];
		s32 int1 = int_0_1[1];

		queue_t<s32> q;
		make(mem, q, 100);

		q.push(0);
		q.push(1);
		q.push(2);
		q.push(3);
		
		s32 qi;
		q.pop(qi);
	}

}

