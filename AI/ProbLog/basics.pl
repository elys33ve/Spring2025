/* https://dtai.cs.kuleuven.be/problog/tutorial/basic/01_coins.html */

% Probabilistic facts
0.5::heads1.
0.6::heads2.

/* Basic ProbLog: Probabilistic facts and clauses */
% Rules
twoHeads :- heads1, heads2.

%query(heads1).
%query(heads2).
%query(twoHeads).


/* Noisy-or: Multiple rules for the same head */
% Rules:
someHeads :- heads1.
someHeads :- heads2.

%query(someHeads).


/* First Order */
% Probabilistic facts:
0.6::lands_heads(_).

% Background information:
coin(c1).
coin(c2).
coin(c3).
coin(c4).

% Rules:
heads(C) :- coin(C), lands_heads(C).
someHeads :- heads(_).

%query(someHeads).

/* Probabilistic clauses */
% Probabilistic facts:
0.6::heads(C) :- coin(C).

% Background information:
coin(c1).
coin(c2).
coin(c3).
coin(c4).

% Rules:
someHeads :- heads(_).

query(someHeads).