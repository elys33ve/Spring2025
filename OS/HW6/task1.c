// Every lock we would want to use would be defined by a "lock variable" that keeps track of 
// if the lock is "locked" or "open". In it's most simple form, the lock variable would be
// a Binary flag that could be set "locked" or "open" as needed. When a process is started, and
// before any threads are run, all locks would be set to "open", as no thread is yet in a critical 
// section. When thread eventually start running, by definition one of them will get to a critical 
// section before other ones did. The threads that got there first would lock the lock variable 
// associated with the critical section it got to and then go into the locked area itself. Other 
// processes that later come to the same critical section would see the locked lock variable and wait
// until the lock holding process opens the lock again. In this way, we can assure that only one
// process is "in" a critical section at a time.
//
// In actual code, we would declare a variable of "lock variable type" for every critical section.
// User code would use additional calls that we will create to manipulate the lock. For reasons we'll
// see very soon, just directly setting the locks using standard language constructs WILL NOT IN GENERAL
// PROVIDE THE PROTECTION WE NEED. WE'LL EXPLORE WHY IN THIS HOMEWORK ASSIGNMENT.
//
// Let's conceptually "declare" a lock variable

lock_variable_type lock_variable;

// Now, we need ways to set the lock variable to "locked". Such a lock() function would require the following
// properties and behaviors.
//
// a) IF the lock variable is set, the function SETS the lock variable and returns,
//    presumably to run enter the critical section code that starts right after the 
//    call to lock().  If the lock variable is already locked, the function does not 
//    return.  In fact, it the whole thread goes into some kind of “wait” state and is
//    not allowed to progress past the call to lock().  Presumably some OTHER thread 
//    set the lock, and when it releases it, any OTHER threads that had run lock() 
//    calls and were waiting on it would be free to “grab” it themselves (I.E. set the 
//    lock and proceed into their critical section).  Naturally, only one “waiting” 
//    thread could do this… and in general, the one that wins (gets the lock) is
//    selected in some arbitrary way not predictable by the programmer.
//
// b) This one’s important.  The “lock” operation, that is, how the flag’s state is
//    changed from unset to set, is ATOMIC.  That is, the process of setting the flag
//    CANNOT be interrupted by any other attempt, by any other process, to set it or unset it.  
// If we cannot trust the lock mechanism to be free of race conditions, there is no way we
//    can use it to create protections from race conditions :)
//
// Let's show a conceptual function prototype for a lock function:

void lock(&lock_variable)

// The release() function would simply “unset” the lock_variable flag.  A release() 
// MUST be called at the end of a critical section, otherwise, other threads that 
// wanted to enter their own critical sections would never get the chance. Note that
// making the "release" or "unset" function atomic is not at all wrong, but as we will see, 
// in many cases it's not necessary, so for efficiency reasons, we don't worry about it.
// Let's look at a function prototype.

void release(&lock_variable)


// --------------------------------------------------------------------------------------

// I'm rendering some code in purple. In this case I'm using purple to mean... this code 
// is bad. Don't you go using this code....

// First, let's define a typename for our lock variable type. In this case, I'm just going
// to use an int as my basic flag type, but renamed into "lock_variable_type" for clarity.
// I'll define a value of 1 as "locked" and a value of 0 as "unlocked"

typedef int lock_variable_type

// Ok, here goes a bad... and actually really kind of is dangerous implementation of lock:
// First, let's write something to lock a lock by just setting it to 1

inline void lock(lock_variable_type *lock_variable)
{  while (*lock_variable);
   *lock_variable = 1;
}

// and here is a not as bad but still bad enough implementation of release (unlock)...
inline void release(lock_variable_type *lock_variable)
{ *lock_variable = 0;
}

// Just for sake of completeness, let's also make an "initialize lock" call that can be
// used in main() (the master thread) to initialize all locks to "unlocked" so we know
// everything is unlocked before any threads start.

inline void init_lock(lock_variable_type *lock_variable)
{ *lock_variable = 0;
}