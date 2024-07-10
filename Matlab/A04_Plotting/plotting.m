clear;clc;

%
% Create a plot with 10 points
%
x = 1 : 10;
y = x.*x;
plot(x,y);
xlabel('Here is a label for the x-axis');
ylabel('Here is a label for the y-axis');
title('Useless Plot')
axis([0 15  0 120])  % Set x-axis is 0~15, y-axis is 0~120
