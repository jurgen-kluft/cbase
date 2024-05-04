# Scheduled code changes for all base packages

## ccode

Using Go to manage our packages and dependencies and generating Visual Studio projects as well as Tundra build files. [Done]

## Style & Naming Conventions

- package starts with c (e.g. cbase, ccore, cstring, callocator, chash, crandom, cfilesystem) [Done]
- namespaces start with n (e.g. ncore, nrunes) [Done]
- struct/class ends with _t (e.g. alloc_t) [Done]
- global functions start with g_ (e.g. g_min, g_max, g_po2) [Done]
- static functions start with s_ (e.g. s_min, s_max, s_po2) [Done]

## cstring (Alpha)

string class (UCS-2, UTF-16)

## csuperalloc

The package csuperalloc contains a virtual memory allocator.

- split of debugging allocators like memento to cdealloc

## catomic

Do we still need these lock free data structures ?

## chash (Alpha)

Just a simple interface here and a couple of hash candidates.
This is just hashing, no encryption.

## cmerkle

Utilities to construct, update and use merkle trees.

## ccrypto (AES)

AES-256

## crandom (Alpha)

A couple of random number generators

- mersenne twister
- and ?

## ctime (Beta)

Package for dealing with date and time

## cfilesystem (Proto)

We need to redesign this a bit by limiting the feature set, design can consider multithreading but should by default be single-threaded.

- filesystem_t
- fileinfo_t, dirinfo_t
- filepath_t, dirpath_t

## cdealloc

Progress = Alpha

- Leak Detector
- Use the allocator to allocate meta-data
- Memento

## cthread

- thread
- mutex, event, semaphore
- per thread a context holding (this is part of cbase)
  - random number generator
  - string allocator
  - heap allocator

## cp2p

- peer to peer library
- connections are peer objects
- 1st version = TCP
- later we can look at UDP; like uTP or UDX

## cactor

Progress = Alpha

- simple actor framework
- slim/optimized
- maximum number of actors
- actors manage the messages they send and will get them send back as GC
- just sending messages

## ccmdline

Very usable.

## ctext

Does contain a working 'manual' parser class.

- parsing/tokenizer
  - XML
  - JSON
  - Custom

## clocal

Localization to support many languages easily in an app.

1. localization
2. string compression

## end

that's it... (for now)
