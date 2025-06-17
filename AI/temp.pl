pred1(_, [], []).
pred1(E, [E|T], T1):- 
           !, 
            pred1(E,  T, T1).
 
pred1(E, [H|T], [H|T1]):- 
           pred1(E, T, T1).
