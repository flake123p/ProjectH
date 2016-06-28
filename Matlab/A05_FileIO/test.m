clear;clc;

% Read file
fid = fopen('file_for_read.txt', 'r');
x = fscanf(fid, '%d %d %d', [3,2]);
fclose(fid);

% Write file
a = int16([12, 33, 555; 8, 88, 888]);
fid = fopen('file_for_write.txt', 'w');
for i=1:3
    y = fprintf(fid, '%8d %8d\n', a(1,i), a(2,i));
end
fclose(fid);