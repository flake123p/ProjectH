clear;clc;

% A = [row1 ; row2 ; row 3 ; ... ]
A = [1 2 3; 4 5 6]
sizePara = size(A)
row = sizePara(1)
column = sizePara(2)
% A(2) is 4. Matlab is column major, C is row major.