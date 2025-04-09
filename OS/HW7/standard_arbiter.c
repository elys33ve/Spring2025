

/*
standard_arbiter.c :  The program should be a modified version of standard_noprotection.c that adds code to implement the arbiter (waiter) solution.
*/

/*
standard_noprotection.c: 
This program implements the standard dining philosophers problem as described in the book and in the Wikipedia article.  
Specifically, in THIS version, a hungry philosopher will attempt to get the chopsticks immediately to the left and right of the seating position.  
Each philosopher is implemented as a separate pthread. 
This version also has a “finite number of meals”.  All threads will end when that number of meals have been consumed. 
In the default version of the code I’m giving you, there are 10 philosophers and 10 million meals available. 
As there is no deadlock protection. This code will DEFINITELY deadlock before all 10 million meals are consumed.
*/

