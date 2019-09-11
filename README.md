 # lockless-queue
An unbounded generic lockless SPMC(Single Producer, multiple consumer) queue

Hi,
This is the documentation for a lockless, wait-free SPMC queue. You can link the library in your code and start replacing your thread unsafe queues with it. Please do a lot of testing and benchmarking before doing so, because lockfree doesn't always necessarily mean the "fastest", due to more contention.

If you are a complete noob to lockfree and wait-free data structures, then i have added some text on this subject matter for you. Just like you, i barely knew anything about lockfree. Infact, i used to think lock free and wait-free are the same things. Needless to say, i came a little far from that understanding now, but i still have 2390 Kms to go. I have tried to provide very brief insights into lockfree algorithms in below listed files. Concurrency, synchroniation, locks, thread-safety: Please note that if you are unfamiliar with any of these keywords, then the below information is probably just out of scope for you at this moment.  

Docs/Lockfree and other shenanighans


File Lockfree:
In this file, i am adding little introductions on all the pre-requisites that will go in maintaing/developing/understanding the code of this queue.

##What is lockfree?
A lock free algorithm protects a shared data structure through a non-blocking algorithm. The keyword here is non-blocking. The most important point to pick up from this explanation is that a thread will not be blocked by another thread. For example: consider using an STL std::queue with multiple threads. The code will look something like this:

mutex.lock()
q.enqueue(elem)
mutex.unlock();

Anytime, a thread acquires a lock, and other threads want to acquire the same lock, all of them will be blocked causing starvation, and they should be to achieve synchronization. And this kind of code, if badly done(we all have done it alteast once) can cause in major problems like deadlocks.
However, what if we could achieve synchronization with a guarantee that atleast one thread will always make progress. A lockfree data structure essentially does that. In a lockfree algorithm, each of your CPU core is always executing an instruction. 

What is wait-free?
Now imagine, if we could achieve synchronization without having to indefinitely suspend any thread at all? That is, all threads make progress, a complete state of altruism, so to say. Wait-free is a stronger condition which means that every thread is guaranteed to make progress over an arbitrary period of time, regardless of the timing/ordering of thread execution; and so we can say that the threads finish independently. All wait-free programs are lock-free. Reverse may not be true.
Also, these kind of algorithms are extremely difficult to write(ask me, i started with the courage to implement a completely unbounded MPMC queue, but here am i settling with SPMC for now). And that is not only attributed to the fact that wait-free algorithms are inherently difficult to write, but the fact that OS's memory allocator is not fundamentally wait-free. OS always relies on some kind of locking primitives to satisfy the calls to new() and delete() since it shares it's memory alloctors across processes. 

What does that tell us?
It's relatively more possible to write true lockfree and wait-free data structures for bounded solutions. For unbounded though, where you may need more memory at run time, they border on impossible, sort of. 

How can one do it without mutexes anyway?
A simple one word Acronymous answer to this question is "CAS", COMPARE and SWAP CPU instruction. Also, some(read alot) knowledge of memory barriers is required. 

## CAS
Almost all modern commercial CPUs now contain CAS instruction. It compares the contents of a memory location with a given value and, only if they are the same, modifies the contents of that memory location to a new given value. Since the advent of C++11, and the new atomics, C++ provides stanbdard CAS wrappers like this:
atomic_compare_exchange_weak( std::atomic<T>* obj, 
                                   typename std::atomic<T>::value_type* expected,
                                   typename std::atomic<T>::value_type desired ) noexcept;

CAS has been used in this queue to update things like head and tail. For example, in a standard Queue::enqueue operation, the most primitive thing that you need to do is to update your tail to point to the new node. It's this operation in enqueue that makes it thread unsafe. You can use CAS in a situation like this to check if your tail has not changed between read of the current position of tail and the present time and only then perform an overwrite. Since CAS is atomic, it's success will mean that you enqueued an element without leaving the queue in an inconsistent state and it's failure would mean, you gotta have to try it all over again. That simple, really.
The access to CAS using wrapper methods existed before also in both windows and Linux, however, what C++11 did is that now it is part of the standard. So, you can write platform independent lockfree code.

#CAS and the ABA problem
From Wikipedia:
In multithreaded computing, the ABA problem occurs during synchronization, when a location is read twice, has the same value for both reads, and "value is the same" is used to indicate "nothing has changed". However, another thread can execute between the two reads and change the value, do other work, then change the value back, thus fooling the first thread into thinking "nothing has changed" even though the second thread did work that violates that assumption.
And CAS is prone to exactly that, hence it can not be completely relied upon. So yeah, not that simple. However, there are solutions that exist for this. 

#Tagge Pointer 
I have used a tagged pointer to overcome this limitation. There are also bunch of new things like hazard pointers(that i still need to get my head around) which can also be used.

#Memory barriers
Ahhh, i wish i was an expert enough to explain this to anyone. The best i can do is to point you to a Herb Sutter talk. It's quite long, but it is the only way to even begin to understand what memory barriers are and how misunderstanding them can shake everything you ever knew about your code's flow of execution.
Basically, only guarantees in C++ standard pertaining to execution is "Thou shalt not change the behaviour of a single threaded program". On the name of Shiva and making your program more optimized, compilers, and even processors are allowed to change the order of any instruction they may want. This is the reason you start seeing unicorn scenarios("How is that value for my obj.counter even possible, this is sorcery, i am moving back home to start my own farm" kind of scenarios) as soon as you let bunch of threads go wild. Memory barriers, which is what mutexes and spinlocks enforce internally are strong checks on such optimization to make sure, that all your threads maintain sequential consistency.

I can't do bette than this on memory barriers. So here's that Herb Sutter talk:
https://www.youtube.com/watch?v=A8eCGOqgvH4


## So let's talk about the algorithm now.

<To be continued.>
