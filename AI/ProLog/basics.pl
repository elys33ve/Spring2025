% working_directory(_, 'C:/Users/Fiona/Documents/UC/___PATH___').


% --- Facts: define relationships and data

% People and their ages
person(john, 30).
person(mary, 25).
person(peter, 40).

% Likes relationships
likes(john, mary).
likes(mary, books).
likes(peter, programming).

% Animals
animal(dog).
animal(cat).
animal(bird).


% --- Rules

% A person is old if they are over 35
old(X) :- person(X, Age), Age > 35.

% A person is young if they are under 30
young(X) :- person(X, Age), Age < 30.

% A person is a friend of someone if they like that person.
friend(X, Y) :- likes(X, Y), person(X,_),person(Y,_).


% --- Queries

% Is john 30 years old?
% ?- person(john, 30).

% Who is old?
% ?- old(X).

% Who does john like?
% ?- likes(john, X).

% Is a cat an animal?
% ?- animal(cat).

% Who is a friend of mary?
% ?- friend(X,mary).


% --- Lists

% A list of fruits
fruits([apple, banana, orange]).

% A rule to check if an item is in a list
member_of(X, [X|_]).
member_of(X, [_|T]) :- member_of(X, T).

% member_of
% ?- member_of(banana, [apple, banana, orange]).
% ?- member_of(grape, [apple, banana, orange]).


% --- Arithmetic

% A rule to add two numbers
add(X, Y, Z) :- Z is X + Y.

% add
% ?- add(5, 3, Result).



