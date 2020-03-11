%Jorge Farinacci
%125008668
%csce 420
%June 24, 2019
%hw4pr2.pl




negation_as_failure(Node, [[Head | Rest1] | Rest2]) :-
    not(Node = Head),
    negation_as_failure(Node, Rest2).


my_topo_sort(Partialorder, Totalorder) :-
    flatten(Partialorder, Flattend),
    my_topo(Flattend, Partialorder, Totalorder).

my_topo([Node | Rest], Partialorder, Totalorder) :-

        (not(negation_as_failure(Node, Partialorder)) ->
            sort([Node | Rest],Totalorder)
          ; writeln('topological sort failed')
        ).
