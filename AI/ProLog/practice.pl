/*
"knowledge base" about animals. represent some basic information:
create "facts" that say things like "a cat is an animal" or "a dog is an animal." 
Then, you'll create a "rule" that allows you to ask, "Is this thing an animal?"

Facts: Define two or three different animals using a predicate (think of it as a label) like animal(something).
Rule: Create a rule called is_animal(X) that will be true if X matches one of your animal facts.

[practice].
animal(cat).
    true
animal(fish).
    false
is_animal(cat).
    true
is_animal(X).
    X = cat ;
    X = dog.

*/

% facts
animal(cat).
animal(dog).

% rule: check if X is an animal
is_animal(X) :- animal(X).


/*
Facts:
    Define people and their hobbies
    Define hobby categories
Rules:
    Create a rule called enjoys_category(Person, Category) that succeeds if a person enjoys a hobby that belongs to a specific category.
    Create a rule called shared_hobby(Person1, Person2, Hobby) that succeeds if two people share a common hobby.
*/

% --- Facts
% predicate - hobby(person_name, hobby_name).
hobby(name1, writing).
hobby(name2, singing).
hobby(name3, rockclimbing).

% predicate - category(hobby_name, category_name).
category(writing, art).
category(singing, art).
category(rockclimbing, sport).

% --- Rules
% if person enjoys hobby of specific category
enjoys_category(Person, Category) :-
    hobby(Person, Hobby), 
    category(Hobby, Category).

% if two people share a hobby
shared_hobby(Person1, Person2, Hobby) :-
    hobby(Person1, Hobby),
    hobby(Person2, Hobby),
    Person1 =/ Person2.

