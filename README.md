# Buddy

A simple buddy allocator under Apache License 2.0.

No more than 60 lines of code for malloc/free function.

No recursive function.

The code is simple and straightforward as long as you understand what a buddy allocator is and what it is for.

No thorough tests yet. Feel free to test it and bug reports will be greatly appreciated.

I will use it in some microcontroller (MCU) projects, and will keep the code targeting this domain. So thread-safety won't be addressed forever.

The assumptions are:

1. Single threaded.
2. No other memory allocator available. The memory pool will be either statically allocated (prefered), or allocated on stack using `alloca()` if available.

# Explanation

There are many web pages explaining the buddy memory allocator. I found this page is particularly helpful with working codes.

http://homepage.cs.uiowa.edu/~jones/opsys/notes/27.shtml

The sample code works, but recursive functions are used. So I rewrite it.

Another modification is `free()`. The sample code requires the caller providing the size of allocated block. It is used to calculate the block order - in buddy's terminology - of the mem block to be freed. This is inconvenient in real world code.

Exactly *ONE BYTE RIGHT BEFORE* the allocated chunk is used to store the `order`.

```C

      4DEADBEEFDEADBEEF....
--------------------------------------
      ^^
      ||
     /  \
  order  block pointer pointing to here,
         which is assured for 4-byte or 
         higher order alignment.  
```

It's value is the round-up of log<sub>2</sub>(size) rather than the size itself, so one byte is enough.

Under this design, there is no way to know the exact size passed to `malloc()` during allocation, but for my application, there is no need to know this. Since I choose the buddy algorithm, I don't mind the internal fragmentation. And 1 byte (as `uint8_t`) is very economical for MCU applications.
