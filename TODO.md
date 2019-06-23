# Scheduled code changes for all base packages

## xcode

~~Support Tundra build system~~ DONE

## xbase

~~Split of containers into xcore/xgenerics~~ DONE

Simplify the whole string usage, currently we have code to handle ascii/utf8/utf16/utf32. Can we just use utf32 and remove all other code except the conversion from/to. Console, Log, printf/sprintf, sscanf etc.. should all just use a standard low-level string container that is utf32. It doesn't matter if there is dynamic allocation.
  
## xcore (typeless/raw containers)

- ~~list~~ DONE
- ~~array~~ DONE
- ~~stack~~ DONE
- ~~queue~~ DONE
- ~~tree~~ DONE
- custom trie-tree map

## xgenerics

- ``list<T>``
- ``array<T>``
- ``stack<T>``
- ``queue<T>``
- ``map<K,V>``
- custom trie-tree map

## xstring

string class (utf32) (Alpha version)

## xallocator

We need a virtual memory based allocator :smile:

## xatomic

Do we still need these lock free data structures ?

## xhash

Just a simple interface here and a couple of hash candidates. This is just hashing, no encryption.

## xmerkle

Utilities to construct, update use merkle trees.

## xcrypto (AES)

AES-256

## xrandom

A couple of random number generators

- mersenne twister
- and ?

## xtime

Package for dealing with date and time

## xfilesystem

We need to redesign this a bit by limiting the feature set, design can consider multithreading but should by default be single-threaded

- xfile, xdir
- xfileinfo, xdirinfo
- xfilepath, xdirpath

## xmemory

- Leak Detector
- Use the allocator to allocate the meta-data for the xrbtree

## xthread

- thread
- mutex, event, semaphore
- per thread
  - random number generator
  - allocator

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

that's it...
