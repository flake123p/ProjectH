clear; clc;
%
% Display Image
%
myImage = '../../Images/wombats.tif';
w=imread(myImage);
%figure; % Create figure window. Flake: It seems like optional!?
%imshow(w); % image show

%
% Get the rows and columns of the image
%   - size() output is [row, column]
%
imgDimention = size(w);
row = imgDimention(1);
column = imgDimention(2);

for rowCtr = 1:row
    for colCtr = 1:column
        if w(rowCtr, colCtr) < 128
            w(rowCtr, colCtr) = 255 - w(rowCtr, colCtr);
        end
    end
end

subplot(2,2,1);
imshow(w); % image show
subplot(2,2,2);
imhist(w);
%
% Original Image
%
w=imread(myImage);
subplot(2,2,3);
imshow(w); % image show
subplot(2,2,4);
imhist(w);