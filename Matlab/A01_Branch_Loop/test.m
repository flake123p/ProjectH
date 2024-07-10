clear;clc;

x = -299;
if x > 0
    disp('[if-else] x is positive');
elseif x == 0
    disp('[if-else] x is zero');
else
    disp('[if-else] x is negitive');
end

switch 'abcd'
case 'abc'
    disp('[switch-case] string is abc')
case '123'
    disp('[switch-case] string is 123')
otherwise
    disp('[switch-case] string is otherwise')
end

for forCtr = 1:3
    fprintf('forCtr = %d\n', forCtr);
end

whileCtr = 33;
while whileCtr > 0
    fprintf('whileCtr = %f\n', whileCtr);
    whileCtr = whileCtr - 11;
end
