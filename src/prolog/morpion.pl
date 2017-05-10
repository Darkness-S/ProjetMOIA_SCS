:- dynamic board/1.
:- retractall(board(_)).
:- assert(board([_Z1,_Z2,_Z3,_Z4,_Z5,_Z6,_Z7,_Z8,_Z9,
		 _A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9,
		 _K1,_K2,_K3,_K4,_K5,_K6,_K7,_K8,_K9])). 

%%%%%
%%  Generate possible marks on a free spot on the board.
%%  Use mark(+,+,-X,-Y) to query/generate possible moves (X,Y).
%%%%%
mark(Player, [X|_],1,1) :- var(X), X=Player.
mark(Player, [_,X|_],1,2) :- var(X), X=Player.
mark(Player, [_,_,X|_],1,3) :- var(X), X=Player.
mark(Player, [_,_,_,X|_],1,4) :- var(X), X=Player.
mark(Player, [_,_,_,_,X|_],1,5) :- var(X), X=Player.
mark(Player, [_,_,_,_,_,X|_],1,6) :- var(X), X=Player.
mark(Player, [_,_,_,_,_,_,X|_],1,7) :- var(X), X=Player.
mark(Player, [_,_,_,_,_,_,_,X|_],1,8) :- var(X), X=Player.
mark(Player, [_,_,_,_,_,_,_,_,X|_],1,9) :- var(X), X=Player.
mark(Player, [_,_,_,_,_,_,_,_,_,X|_],2,1) :- var(X), X=Player.
mark(Player, [_,_,_,_,_,_,_,_,_,_,X|_],2,2) :- var(X), X=Player.
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,X|_],2,3) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,X|_],2,4) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],2,5) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],2,6) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],2,7) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],2,8) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],2,9) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],3,1) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],3,2) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],3,3) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],3,4) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],3,5) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],3,6) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],3,7) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],3,8) :- var(X), X=Player. 
mark(Player, [_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X|_],3,9) :- var(X), X=Player. 

%%%%%
%%  Move 
%%%%%
move(P,(1,1),[X1|R],[P|R]) :- var(X1).
move(P,(2,1),[X1,X2|R],[X1,P|R]) :- var(X2).
move(P,(3,1),[X1,X2,X3|R],[X1,X2,P|R]) :- var(X3).
move(P,(1,2),[X1,X2,X3,X4|R],[X1,X2,X3,P|R]) :- var(X4).
move(P,(2,2),[X1,X2,X3,X4,X5|R],[X1,X2,X3,X4,P|R]) :- var(X5).
move(P,(3,2),[X1,X2,X3,X4,X5,X6|R],[X1,X2,X3,X4,X5,P|R]) :- var(X6).
move(P,(1,3),[X1,X2,X3,X4,X5,X6,X7|R],[X1,X2,X3,X4,X5,X6,P|R]) :- var(X7).
move(P,(2,3),[X1,X2,X3,X4,X5,X6,X7,X8|R],[X1,X2,X3,X4,X5,X6,X7,P|R]) :- var(X8).
move(P,(3,3),[X1,X2,X3,X4,X5,X6,X7,X8,X9|R],[X1,X2,X3,X4,X5,X6,X7,X8,P|R]) :- var(X9).

%%%%%
%%  Record a move: record(+,+,+).
%%%%%
record(Player,X,Y) :- 
   retract(board(B)), 
   mark(Player,B,X,Y),
   assert(board(B)).
   
%%%%%
%% Calculate the value of a position, o maximizes, x minimizes.
%%%%%
value(Board,100) :- win(Board,o), !.
value(Board,-100) :- win(Board,x), !.
value(Board,E) :- 
   findall(o,open(Board,o),MAX), 
   length(MAX,Emax),      % # lines open to o
   findall(x,open(Board,x),MIN), 
   length(MIN,Emin),      % # lines open to x
   E is Emax - Emin.
   
%%%%% 
%%  A winning line is ALREADY bound to Player. 
%%  win(+Board,+Player) is true or fail.
%%    e.g., win([P,P,P|_],P).  is NOT correct, because could bind 
%%%%%
win([Z1,Z2,Z3|_],P) :- Z1==P, Z2==P, Z3==P.
win([_,_,_,Z1,Z2,Z3|_],P) :-  Z1==P, Z2==P, Z3==P.
win([_,_,_,_,_,_,Z1,Z2,Z3],P) :-  Z1==P, Z2==P, Z3==P.
win([_,_,_,_,_,_,_,_,_,A1,A2,A3|_],P) :- A1==P, A2==P, A3==P. 
win([_,_,_,_,_,_,_,_,_,_,_,_,A1,A2,A3|_],P) :- A1==P, A2==P, A3==P. 
win([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,A1,A2,A3|_],P) :- A1==P, A2==P, A3==P. 
win([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,K1,K2,K3|_],P) :- K1==P, K2==P, K3==P. 
win([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,K1,K2,K3|_],P) :- K1==P, K2==P, K3==P. 
win([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,K1,K2,K3|_],P) :- K1==P, K2==P, K3==P. 

%%%%%
%%  A line is open if each position is either free or equals the Player
%%%%%
open([Z1,Z2,Z3|_],Player) :- (var(Z1) | Z1 == Player),(var(Z2) | Z2 == Player), (var(Z3) | Z3 == Player).
open([_,_,_,Z1,Z2,Z3|_],Player) :- (var(Z1) | Z1 == Player),(var(Z2) | Z2 == Player), (var(Z3) | Z3 == Player).
open([_,_,_,_,_,_,Z1,Z2,Z3],Player) :- (var(Z1) | Z1 == Player),(var(Z2) | Z2 == Player), (var(Z3) | Z3 == Player).
open([Z1,_,_,Z2,_,_,Z3,_,_],Player) :- (var(Z1) | Z1 == Player),(var(Z2) | Z2 == Player), (var(Z3) | Z3 == Player).
open([_,Z1,_,_,Z2,_,_,Z3,_],Player) :- (var(Z1) | Z1 == Player),(var(Z2) | Z2 == Player), (var(Z3) | Z3 == Player).
open([_,_,Z1,_,_,Z2,_,_,Z3],Player) :- (var(Z1) | Z1 == Player),(var(Z2) | Z2 == Player), (var(Z3) | Z3 == Player).
open([Z1,_,_,_,Z2,_,_,_,Z3],Player) :- (var(Z1) | Z1 == Player),(var(Z2) | Z2 == Player), (var(Z3) | Z3 == Player).
open([_,_,Z1,_,Z2,_,Z3,_,_],Player) :- (var(Z1) | Z1 == Player),(var(Z2) | Z2 == Player), (var(Z3) | Z3 == Player).

%%%%%
%% Calculate the value of a position, o maximizes, x minimizes.
%%%%%
value(Board,100) :- win(Board,o), !.
value(Board,-100) :- win(Board,x), !.
value(Board,E) :- 
   findall(o,open(Board,o),MAX), 
   length(MAX,Emax),      % # lines open to o
   findall(x,open(Board,x),MIN), 
   length(MIN,Emin),      % # lines open to x
   E is Emax - Emin.
   
   alpha_beta(_Player,0,Position,_Alpha,_Beta,_NoMove,Value) :- 
   value(Position,Value).

alpha_beta(Player,D,Position,Alpha,Beta,Move,Value) :- 
   D > 0, 
   findall((X,Y),mark(Player,Position,X,Y),Moves), 
   Alpha1 is -Beta, % max/min
   Beta1 is -Alpha,
   D1 is D-1, 
   evaluate_and_choose(Player,Moves,Position,D1,Alpha1,Beta1,nil,(Move,Value)).

evaluate_and_choose(Player,[Move|Moves],Position,D,Alpha,Beta,Record,BestMove) :-
   move(Player,Move,Position,Position1), 
   other_player(Player,OtherPlayer),
   alpha_beta(OtherPlayer,D,Position1,Alpha,Beta,_OtherMove,Value),
   Value1 is -Value,
   cutoff(Player,Move,Value1,D,Alpha,Beta,Moves,Position,Record,BestMove).
evaluate_and_choose(_Player,[],_Position,_D,Alpha,_Beta,Move,(Move,Alpha)).

cutoff(_Player,Move,Value,_D,_Alpha,Beta,_Moves,_Position,_Record,(Move,Value)) :- 
   Value >= Beta, !.
cutoff(Player,Move,Value,D,Alpha,Beta,Moves,Position,_Record,BestMove) :- 
   Alpha < Value, Value < Beta, !, 
   evaluate_and_choose(Player,Moves,Position,D,Value,Beta,Move,BestMove).
cutoff(Player,_Move,Value,D,Alpha,Beta,Moves,Position,Record,BestMove) :- 
   Value =< Alpha, !, 
   evaluate_and_choose(Player,Moves,Position,D,Alpha,Beta,Record,BestMove).

other_player(o,x).
other_player(x,o).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% For testing, use h(+,+) to record human move,
%%% supply coordinates. Then call c (computer plays).
%%% Use s to show board.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
h(X,Y) :- record(x,X,Y), showBoard.

c :- 
   board(B), 
   alpha_beta(o,2,B,-200,200,(X,Y),_Value), % <=== NOTE
   record(o,X,Y), showBoard.

showBoard :- 
   board([Z1,Z2,Z3,Z4,Z5,Z6,Z7,Z8,Z9,
   	  A1,A2,A3,A4,A5,A6,A7,A8,A9,
   	  K1,K2,K3,K4,K5,K6,K7,K8,K9]), 
   write('    '),mark(Z1),mark(Z2),mark(Z3),write(' '),
   		 mark(Z4),mark(Z5),mark(Z6),write(' '),
   		 mark(Z7),mark(Z8),mark(Z9),write(' '),nl,
   write('    '),mark(A1),mark(A2),mark(A3),write(' '),
   		 mark(A4),mark(A5),mark(A6),write(' '),
   		 mark(A7),mark(A8),mark(A9),write(' '),nl,
   write('    '),mark(K1),mark(K2),mark(K3),write(' '),
   		 mark(K4),mark(K5),mark(K6),write(' '),
   		 mark(K7),mark(K8),mark(K9),write(' ').
   
   
s :- showBoard.

mark(X) :- var(X), write('#').
mark(X) :- \+var(X),write(X).
