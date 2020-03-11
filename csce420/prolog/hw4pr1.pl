%Jorge Farinacci
%125008668
%csce 420
%June 24, 2019
%hw4pr1.pl


my_max([], -2147483648).
my_max([x|[]],x).
my_max([Head|Tail], M) :- my_max(Tail, Tail_max), 
    (Head >= Tail_max -> M = Head ; M = Tail_max).


my_sort([Sorted],[Sorted]).
my_sort(Unsorted, Sorted):-
    my_max(Unsorted,Elem),
    select(Elem,Unsorted,Unsorted2),
    my_sort(Unsorted2,Sorted2),
    append([Elem],Sorted2,Sorted),!.

