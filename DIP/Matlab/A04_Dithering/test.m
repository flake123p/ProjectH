clear; clc;
%
% Display Image
%
myImage = '../../Images/wombats.tif';
w=imread(myImage);
%figure; % Create figure window. Flake: It seems like optional!?

D = [0 128; 192 64];
r = repmat(D, 128, 128);
x1 = w > r;
figure; % FIGURE 1
imshow(x1);


D = [0 128 32 160; 192 64 224 96; 48 176 16 144; 240 112 208 80];
r = repmat(D, 64, 64);
x2 = w > r;
figure; % FIGURE 2, better image
imshow(x2);

