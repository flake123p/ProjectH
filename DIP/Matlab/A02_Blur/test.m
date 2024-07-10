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

newW = uint8(zeros(size(w)));
% % a1 = [-1 -1 -1];
% % a2 = [-1 9 -1];
% % a3 = [-1 -1 -1];
% % Y = 1;
a1 = [1 1 1];
a2 = [1 1 1];
a3 = [1 1 1];
Y = 9;
sum = 0;

for rowCtr = 2:row-1
    for colCtr = 2:column-1
        sum = 0;
    	sum = sum + ( double(w(rowCtr-1, colCtr-1)) * a1(1) );
        sum = sum + ( double(w(rowCtr-0, colCtr-1)) * a1(2) );
        sum = sum + ( double(w(rowCtr+1, colCtr-1)) * a1(3) );
        sum = sum + ( double(w(rowCtr-1, colCtr-0)) * a2(1) );
        sum = sum + ( double(w(rowCtr-0, colCtr-0)) * a2(2) );
        sum = sum + ( double(w(rowCtr+1, colCtr-0)) * a2(3) );
        sum = sum + ( double(w(rowCtr-1, colCtr+1)) * a3(1) );
        sum = sum + ( double(w(rowCtr-0, colCtr+1)) * a3(2) );
        sum = sum + ( double(w(rowCtr+1, colCtr+1)) * a3(3) );
        sum = sum / Y;
        newW(rowCtr, colCtr) = uint8(sum);
    end
end

for rowCtr = 1:row
    for colCtr = 1:column
        w(rowCtr, colCtr) = newW(rowCtr, colCtr);
    end
end

imshow(w); % image show
