clear;clc;

%
% Create a plot with 1000 points, radian = 2pi = 6.28... = 360 degree
%
radian = 0 : 0.01 : 10;
y = sin(radian);
%y = y+sin(2*radian);
plot(radian,y);
xlabel('Here is a label for the x-axis');
ylabel('Here is a label for the y-axis');
title('Sinosoid Plot')
axis([0 15  -2 2])  % Set x-axis is 0~15, y-axis is -2~2
