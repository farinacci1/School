%Jorge Farinacci
%125008668
%csce 420
%June 24,2019
%hw4pr3.pl

:- discontiguous spouse/2, parent/2, sibling/2.

%sex
man(george).
man(spencer).
man(phillip).
man(charles).
man(andrew).
man(mark).
man(edward).
man(william).
man(harry).
man(peter).
man(eugenie).
man(james).
woman(kydd).
woman(mum).
woman(elizabeth).
woman(margaret).
woman(anne).
woman(diana).
woman(sarah).
woman(sophie).
woman(zara).
woman(beatrice).
woman(louise).
%George and Mum
spouse(mum,george).
%children of George and Mum
parent(george,elizabeth).
parent(george, margaret).
parent(mum,margaret).
parent(mum,elizabeth).
%spencer and kydd
spouse(spencer,kydd).
%children of spencer and kydd
parent(spencer,diana).
parent(kydd,diana).
%elizabeth and phillip
spouse(elizabeth,philip).
%children of elizabeth and phillip
parent(elizabeth,charles).
parent(elizabeth,anne).
parent(elizabeth,andrew).
parent(elizabeth,edward).
parent(phillip,charles).
parent(phillip,anne).
parent(phillip, andrew).
parent(phillip,edward).

%Diana and Charles
spouse(diana,charles).
%children of Diana and Charles
parent(charles, william).
parent(charles,harry).
parent(diana,william).
parent(diana,harry).

%Anne and Mark
spouse(anne,mark).
%children of anne and mark
parent(anne,peter).
parent(mark,peter).
parent(anne,zara).
parent(mark,zara).

%Andrew and Sarah
spouse(sarah,andrew).
%children of anne and mark
parent(sarah,beatrice).
parent(andrew,beatrice).
parent(sarah,eugenie).
parent(andrew,eugenie).

%Edward and Sophie
spouse(sophie,edward).
%children of anne and mark
parent(sophie,louise).
parent(edward,louise).
parent(sophie,james).
parent(edward,james).

%sibling relationships
sibling(elizabeth,margaret).
sibling(charles,anne).
sibling(charles,andrew).
sibling(charles,edward).
sibling(anne,andrew).
sibling(anne,edward).
sibling(andrew,edward).




father(X,Y)         :- man(X), parent(X,Y).
mother(X,Y)         :- woman(X), parent(X,Y).
child(X,Y)          :- parent(Y,X).
grandparent(X,Y)    :- parent(X,Z),parent(Z,Y).
grandchild(X,Y)     :- grandparent(Y,X).
sibling(X,Y)        :- sibling(Y,X),parent(Z,X), parent(Z,Y), X \= Y.
brother(X,Y)        :- man(X), sibling(X,Y).
sister(X,Y)         :- woman(X), sibling(X,Y).
spouse(X,Y)         :- spouse(Y,X).
wife(X,Y)           :- woman(X), spouse(X,Y).
husband(X,Y)        :- man(X), spouse(X,Y).
aunt(X,Y)           :- woman(X), sister(X,Z), parent(Z,Y).
uncle(X,Y)          :- man(X), brother(X,Z), parent(Z,Y).
ancestor(X,Y)       :- parent(X,Z),ancestor(Z,Y).
brother_in_law(X,Y) :- spouse(Y,Z), brother(X,Z).
sister_in_law(X,Y)  :- spouse(Y,Z), sister(X,Z).
cousin(X,Y)         :- parent(Z,X),parent(W,Y), sibling(Z,W).
second_cousin(X,Y)  :- parent(Z,X), cousin(Z,W), parent(W,Y).
