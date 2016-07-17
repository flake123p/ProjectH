clear; clc;
%
% Display Image
%
myImage = '../../Images/lena256.bmp';
w=imread(myImage);
%figure; % Create figure window. Flake: It seems like optional!?
ResizeFactor = 8;
w = imresize(imresize(w, 1/ResizeFactor), ResizeFactor);
imshow(w); % image show
