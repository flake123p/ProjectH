myImage = '../../Images/wombats.tif';
w=imread(myImage);
%figure
imshow(w);

% size() output is [row, column]
imgSize = size(w);
row = imgSize(1);
column = imgSize(2);