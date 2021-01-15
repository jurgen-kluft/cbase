# Scheduled code changes for all base packages

## xcode

Using Go to manage our packages and dependencies and generating Visual Studio projects as well as Tundra build files.

## xbase

Naming convention:

- namespaces start with x (e.g. xcore)
- struct/class ends with _t (e.g. alloc_t)
- you may use a namespace or static class

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

The package xvmem contains a virtual memory allocator.
Also, split of debugging allocators like memento to xdealloc

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

We need to redesign this a bit by limiting the feature set, design can consider multithreading but should by default be single-threaded.

- xfileinfo, xdirinfo
- xfilepath, xdirpath

## xdealloc

Progress = Alpha

- Leak Detector
- Use the allocator to allocate meta-data
- Memento

## xthread

- thread
- mutex, event, semaphore
- per thread a context holding
  - random number generator
  - string allocator
  - heap allocator

## xp2p

- peer to peer library
- connections are peer objects
- 1st version = TCP
- later we can look at UDP; like uTP or UDX

## xactor

Progress = Alpha

- simple actor framework
- slim/optimized
- maximum number of actors
- actors manage the messages they send and will get them send back as GC
- just sending messages

## xcmdline

Progress = v1.0

## xtext

- parsing/tokenizer
  - XML
  - JSON
  - Custom

## xlocal

Localization to support many languages easily in an app.

1. localization
2. string compression

## end

that's it... (for now)
