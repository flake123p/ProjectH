clear; clc;
%
% Display Image
%
myImage = '../../Images/wombats.tif';
img=imread(myImage);
subplot(4,3,1);
imshow(img); % image show
subplot(4,3,2);
imhist(img); axis tight; %axis tight = display complete data on axis

%
% histeq
%
newImg = histeq(img); % Histogram Equalization
subplot(4,3,4);
imshow(newImg); % image show
subplot(4,3,5);
imhist(newImg); axis tight; %axis tight = display complete data on axis
subplot(4,3,6);
plot(img, newImg, '.'),axis tight

%
% imdivide
%
newImg = imdivide(img, 4);
subplot(4,3,7);
imshow(newImg); % image show
subplot(4,3,8);
imhist(newImg); axis tight; %axis tight = display complete data on axis
subplot(4,3,9);
plot(img, newImg, '.'),axis tight

%
% histeq
%
newImg = histeq(newImg); % Histogram Equalization
subplot(4,3,10);
imshow(newImg); % image show
subplot(4,3,11);
imhist(newImg); axis tight; %axis tight = display complete data on axis
subplot(4,3,12);
plot(img, newImg, '.'),axis tight