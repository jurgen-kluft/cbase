# Scheduled code changes for all base packages

## xcode

~~Support Tundra build system~~ DONE

## xbase

~~Split of containers into xcore/xgenerics~~ DONE

Simplify the whole string usage, currently we have code to handle ascii/utf8/utf16/utf32. Can we just use ascii and utf32 and remove all other code except the conversion from/to. Console, Log, printf/sprintf, sscanf etc.. should all just use a standard low-level string container that is ascii and utf32. It doesn't matter if there is dynamic allocation.

Refactor to follow the below rules:

- namespaces start with x (e.g. xcore)
- struct/class ends with _t (e.g. alloc_t)
- you may use a namespace or static class

example:
class xu16
{
public:
  typedef u16 uxx;
  static bool ispo2(uxx value);
  static s32  clz(uxx value);
  static s32  ctz(uxx value);
  static uxx  floorpo2(uxx value);
};
class xu32
{
public:
  typedef u32 uxx;
  static bool ispo2(uxx value);
  static s32  clz(uxx value);
  static s32  ctz(uxx value);
  static uxx  floorpo2(uxx value);
};
class xu64
{
public:
  typedef u64 uxx;
  static bool ispo2(uxx value);
  static s32  clz(uxx value);
  static s32  ctz(uxx value);
  static uxx  floorpo2(uxx value);
};

u32 size = 10;
u32 sizepo2 = xu32.floorpo2(size);

## xcore (typeless/raw containers)

- ~~list~~ DONE
- ~~array~~ DONE
- ~~stack~~ DONE
- ~~queue~~ DONE
- ~~map~~ DONE
- ~~set~~ DONE

## xgenerics

- ``list<T>``
- ``array<T>``
- ``stack<T>``
- ``queue<T>``
- ``map<K,V>``
- ``set<V>``

## xstring (Alpha)

string class (utf32)

## xallocator

We need a virtual memory based allocator :smile:
Also, split of debugging allocators like memento to xmemory

## xatomic

Do we still need these lock free data structures ?

## xhash (Alpha)

Just a simple interface here and a couple of hash candidates. This is just hashing, no encryption.

## xmerkle

Utilities to construct, update use merkle trees.

## xcrypto (AES)

AES-256

## xrandom (Alpha)

A couple of random number generators

- mersenne twister
- and ?

## xtime (Beta)

Package for dealing with date and time

## xfilesystem (Proto)

We need to redesign this a bit by limiting the feature set, design can consider multithreading but should by default be single-threaded

- xfile, xdir
- xfileinfo, xdirinfo
- xfilepath, xdirpath

## xmemory

- Leak Detector
- Use the allocator to allocate meta-data

## xthread

- thread
- mutex, event, semaphore
- per thread
  - random number generator
  - string allocator
  - heap allocator

## xp2p

- peer to peer library
- connections are peer objects
- 1st version = TCP
- later we can look at UDP; like uTP or UDX

## xactor

- erlang like actor framework
- slim/optimized
- maximum number of actors
- actors manage the messages they send and will get them send back as GC
- just sending messages

## xcmdline

trim down to the bare minimum, remove the ugly opt library.

## xtext

- text reader/writer
- parsing/tokenizer
  - <https://github.com/Sakrac/struse>
  - XML
  - JSON
  - Custom

## xlocal

Localization to support many languages easily in an app.

1. localization
2. string compression

## end

that's it... (for now)
