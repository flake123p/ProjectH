%
% Purpose: filter2() example
%   figure 1: original
%   figure 2: small filter
%   figure 3: big filter
%   figure 4: big filter + mirroring
%
clear; clc;
%
%   figure 1: original
%
myImage = '../../Images/wombats.tif';
img=imread(myImage);
subplot(2,2,1);
imshow(img); % image show
subplot(2,2,2);
imhist(img); axis tight; %axis tight = display complete data on axis

%
%   figure 2: small filter
%
smallFilter = ones(9,9)/81;
newImg = uint8(filter2(smallFilter, img));
subplot(2,2,3);
imshow(newImg); % image show
subplot(2,2,4);
imhist(newImg); axis tight; %axis tight = display complete data on axis

%
%   figure 3: big filter
%
bigFilter = ones(25,25)/625;
newImg = uint8(filter2(bigFilter, img));
figure
subplot(2,2,1);
imshow(newImg); % image show
subplot(2,2,2);
imhist(newImg); axis tight; %axis tight = display complete data on axis

%
%   figure 4: big filter + mirroring
%
wr = 12; % 25/2, integer part = 12
wc = 12; % 25/2, integer part = 12
mirrorImg = [img(wr:-1:1,:); img; img(end:-1:end-(wr-1), :)];
mirrorImg = [mirrorImg(:,wc:-1:1), mirrorImg, mirrorImg(:, end:-1:end-(wc-1))];
bigFilter = ones(25,25)/625;
newImg = uint8(filter2(bigFilter, mirrorImg, 'valid'));
subplot(2,2,3);
imshow(newImg); % image show
subplot(2,2,4);
imhist(newImg); axis tight; %axis tight = display complete data on axis