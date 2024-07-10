clear; clc;
%
% Display Image
%
myImage = '../../Images/wombats.tif';
w=imread(myImage);
figure; % Create figure window. Flake: It seems like optional!?
imshow(w); % image show

%
% Get the rows and columns of the image
%   - size() output is [row, column]
%
imgDimention = size(w);
row = imgDimention(1);
column = imgDimention(2);