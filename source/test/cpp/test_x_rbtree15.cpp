#include "xbase\private\x_int128.h"

#define DEBUG_RB15TREE
#include "xbase\private\x_rbtree15.h"

#include "xunittest\xunittest.h"

using namespace xcore;

class x_iidx_array_allocator : public x_iidx_allocator
{
	xbyte*			mObjectArray;
	xbyte*			mObjectArrayEnd;
	u32				mObjectArraySize;
	u32				mSizeOfObject;
	u32*				mFreeObjectList;
	u32				mAllocCount;

	enum { NILL_IDX = 0xffffffff };

	void		init_freelist()
	{
		u32* object_array = (u32*)mObjectArray;
		for (u32 i=1; i<mObjectArraySize; ++i)
		{
			*object_array = i;
			object_array += mSizeOfObject / 4;
		}
		*object_array = NILL_IDX;
		mFreeObjectList = (u32*)mObjectArray;
		mAllocCount = 0;
	}

public:
	x_iidx_array_allocator(void* _array, u32 num_objects, u32 size_of_object)
		: mObjectArray((xbyte*)_array)
		, mObjectArrayEnd((xbyte*)_array + (num_objects * size_of_object))
		, mObjectArraySize(num_objects)
		, mSizeOfObject(size_of_object)
		, mFreeObjectList(NULL)
		, mAllocCount(0)
	{
		init_freelist();
	}

	virtual const char*	name() const											{ return "indexed array allocator"; }

	virtual void*		allocate(xsize_t size, u32 align)
	{
		ASSERT(size < mSizeOfObject);
		void* p;
		iallocate(p);
		return p;
	}

	virtual void*		reallocate(void* old_ptr, xsize_t new_size, u32 new_alignment)
	{
		ASSERT(new_size <= mSizeOfObject);
		return old_ptr;
	}

	virtual void		deallocate(void* ptr)
	{
		u32 idx = to_idx(ptr);
		ideallocate(idx);
	}

	virtual void		release()
	{

	}

	virtual u32 iallocate( void*& p ) 
	{
		if (mFreeObjectList == NULL)
		{
			p = NULL;
			return NILL_IDX;
		}

		u32 idx = (u32)(((uptr)mFreeObjectList - (uptr)mObjectArray) / (uptr)mSizeOfObject);
		p = (void*)mFreeObjectList;

		u32 next_object = *mFreeObjectList;
		if (next_object != NILL_IDX)
			mFreeObjectList = (u32*)(mObjectArray + (next_object * mSizeOfObject));
		else
			mFreeObjectList = NULL;

		++mAllocCount;
		return idx;
	}

	virtual void* to_ptr( u32 idx ) const
	{
		if (idx == NILL_IDX)
			return NULL;
		ASSERT(mObjectArray!=NULL && idx < mObjectArraySize);
		void* p = (void*)(mObjectArray + (mSizeOfObject * idx));
		return p;
	}

	virtual void init() 
	{
		clear();
	}

	virtual u32 max_size() const
	{
		return mObjectArraySize;
	}

	virtual void ideallocate( u32 idx ) 
	{
		if (idx < mObjectArraySize)
		{
			u32* free_object = (u32*)(mObjectArray + (mSizeOfObject * idx));
			*free_object = to_idx(mFreeObjectList);
			mFreeObjectList = free_object;
			--mAllocCount;
		}
	}

	virtual void clear() 
	{
		init_freelist();
	}

	virtual u32 to_idx( void const* p ) const
	{
		ASSERT(mObjectArray!=NULL && mObjectArrayEnd!=NULL);
		if ((xbyte*)p >= mObjectArray && (xbyte*)p < mObjectArrayEnd)
		{
			u32 idx = (u32)(((uptr)p - (uptr)mObjectArray) / (uptr)mSizeOfObject);
			return idx;
		}
		else
		{
			return NILL_IDX;
		}
	}

	virtual u32 size() const
	{
		return mAllocCount;
	}
};

struct mynode : public xrbsnode15
{
	s32 mydata1;
	s32 mydata2;
};

inline u16		alloc_node(x_iidx_array_allocator* a, mynode*& p)
{
	void* vp;
	u32 idx = a->iallocate(vp);
	p = (mynode*)vp;
	return idx;
}


UNITTEST_SUITE_BEGIN(xrbtree15)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(one_node)
		{
			mynode	node_array[16];
			x_iidx_array_allocator a(node_array, 16, sizeof(mynode));

			mynode* nodep;
			u16 node = alloc_node(&a, nodep);
			nodep->clear(node);

			mynode* rootp;
			u16 root = alloc_node(&a, rootp);
			rootp->clear(root);
		}

		static void	add_to_tree(u16 root, u16 node, x_iidx_array_allocator* a)
		{
			mynode* rootp     = (mynode*)xrbnode15::to_ptr(a, root);
			mynode* endNodep  = rootp;
			u16     lastNode  = root;
			mynode* lastNodep = rootp;
			u16     curNode   = rootp->get_child(mynode::LEFT);
			mynode* curNodep  = (mynode*)xrbnode15::to_ptr(a, curNode);
			mynode* nodep     = (mynode*)xrbnode15::to_ptr(a, node);
			s32 s = mynode::LEFT;
			while (curNodep != endNodep)
			{
				lastNode  = curNode;
				lastNodep = curNodep;

				s32 c = 0;
				if (nodep->mydata1 < curNodep->mydata1)
					c = -1;
				else if (nodep->mydata1 > curNodep->mydata1)
					c =  1;

				if (c == 0)
					return;	// Duplicate
				s = (c < 0) ? mynode::LEFT : mynode::RIGHT;
				curNode  = curNodep->get_child(s);
				curNodep = (mynode*)xrbnode15::to_ptr(a, curNode);
			}

			rb15_attach_to(node, lastNode, s, a);
			rb15_insert_fixup(root, node, a);
			rb15_check(root, a);
		}

		UNITTEST_TEST(test_color)
		{
			mynode node;
			node.clear(0);
			node.set_color(mynode::RED);
			CHECK_TRUE(node.is_red());
			node.set_color(mynode::BLACK);
			CHECK_TRUE(node.is_black());
			node.set_color(mynode::RED);
			CHECK_TRUE(node.is_red());
		}

		UNITTEST_TEST(test_parent_side)
		{
			mynode node;
			node.clear(0);
			node.set_parent_side(mynode::LEFT);
			CHECK_EQUAL(mynode::LEFT, node.get_parent_side());
			node.set_parent_side(mynode::RIGHT);
			CHECK_EQUAL(mynode::RIGHT, node.get_parent_side());
		}

		UNITTEST_TEST(test_is_sibling)
		{
			mynode node;
			node.clear(0);
			CHECK_FALSE(node.is_sibling(0));
			node.set_left(1);
			node.set_parent(1);
			node.set_right(2);
			node.set_sibling(2);
			CHECK_TRUE(node.is_sibling(0));
		}

		UNITTEST_TEST(insert_remove_siblings)
		{
			mynode	node_array[16];
			x_iidx_array_allocator as(node_array, 16, sizeof(mynode));
			x_iidx_array_allocator* a = &as;

			mynode* rootp;
			u16 root = alloc_node(a, rootp);
			rootp->clear(root);
			CHECK_FALSE(rootp->is_sibling(root));

			// add 15 siblings
			for (s32 j=1; j<16; j++)
			{
				mynode* np;
				u16 n = alloc_node(a, np);
				np->clear(root);
				rootp->insert_sibling(root, n, root, a);
				CHECK_TRUE(np->is_sibling(root));
				CHECK_FALSE(rootp->is_sibling(root));
			}

			// Check the list
			CHECK_TRUE(rootp->has_sibling(root));
			u16 head = rootp->get_sibling();
			if (head != root)
			{
				u16 i    = 1;
				u16 si   = head;
				do
				{
					CHECK_EQUAL(i, si);
					mynode* sip = (mynode*)mynode::to_ptr(a, si);
					CHECK_TRUE(sip->is_sibling(root));
					si = sip->get_right();
					++i;
				} while (si != head);
			}

			// Remove all the siblings
			for (s32 j=1; j<16; j++)
			{
				mynode* jp = (mynode*)mynode::to_ptr(a, j);
				rootp->remove_sibling(j, root, a);
				jp->clear(root);
				a->ideallocate(j);
			}
		}

		UNITTEST_TEST(multiple_nodes)
		{
			mynode	node_array[16];
			for (s32 i=0; i<16; ++i)
			{
				node_array[i].clear(i);
				node_array[i].mydata1 = i;
				node_array[i].mydata2 = 100 + 30*i;
			}
			x_iidx_array_allocator a(node_array, 16, sizeof(mynode));

			mynode* rootp;
			u16 root = alloc_node(&a, rootp);

			for (s32 i=1; i<16; ++i)
			{
				mynode* nodep;
				u16 node = alloc_node(&a, nodep);
				CHECK_EQUAL(node, i);
				add_to_tree(root, i, &a);
			}

			s32 mydata = 1;
			u16 min_node = rb15_minimum(root, &a);
			u16 node = min_node;
			while (node != root)
			{
				mynode* nodep = (mynode*)xrbnode15::to_ptr(&a, node);
				CHECK_EQUAL(mydata, nodep->mydata1);
				++mydata;

				node = rb15_inorder(0, node, root, &a);
			}
		}
	}
}
UNITTEST_SUITE_END
