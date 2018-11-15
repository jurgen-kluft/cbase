# xcode

~~Support Tundra build system~~

# xbase

~~Split of containers into xcore/xgenerics~~

# Finalize
  * per thread: (move this to xthread package)
  * local storage API
  * debug/assert system
  * local allocator
  * heap allocator
  * random generator
  * temporary string allocator

# xcore (typeless/raw containers)
* ~~list~~
* ~~array~~
* ~~stack~~
* ~~queue~~
* pqueue
* map

# xgenerics
* list<T>
* vector<T>
* stack<T>
* queue<T>
* pqueue<T>
* map<K,V>

# xstring
string class (utf32)

# xallocator
    Can we do something with new/delete in terms of determining where to allocate?
    Heap: new (xheap_alloc) object
    Thread / Stack: new (xstack_alloc) object

# xatomic

# xhash

# xmerkle

# xcrypto (AES)

# xrandom

# xtime
# xfilesystem
We need to redesign this a bit by limiting the feature set
Design can consider multithreading but should by default be single-threaded
* xfile, xdir
* xfileinfo, xdirinfo
* xfilepath
* xdirpath

# xmemory
- Leak Detector
- Use the allocator to allocate the meta-data for the xrbtree

# xthread
* thread
* mutex, event, semaphore

# xp2p
- peer to peer library
- connections are peer objects
- 1st version = TCP
- later we can look at UDP; like uTP or UDX

# xactor
- erlang like actor framework
- slim/optimized
- maximum number of actors
- actors manage the messages they send and will get them send back as GC
- just sending messages

# xcmdline
trim down to the bare minimum, remove the ugly opt library

# xtext
- support for localization manager (with compression ?)
- text reader/writer
- parsing/tokenizer
​