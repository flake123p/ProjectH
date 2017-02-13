
#include <stdio.h>
#include <string.h>
#include <stddef.h>

int isWinToLinux = 0;

int main(int argc, char *argv[])
{
	if (argc!=4) {
		goto PRINT_USAGE;
	}
	if (0 == strcmp(argv[1], "--linux")) {
		isWinToLinux = 1;
	} else if (0 == strcmp(argv[1], "--windows")) {
		isWinToLinux = 0;
	} else {
		goto PRINT_USAGE;
	}
	
	FILE *fpIn = fopen(argv[2], "rb");
	FILE *fpOut = fopen(argv[3], "wb");
	
	int ch;
	int ch2;
	
	if (isWinToLinux) {
		while (1) {
			ch = fgetc(fpIn);
			if (ch == EOF) {
				break;
			}
			if (ch == '\r') {
				ch2 = fgetc(fpIn);
				if (ch2 == EOF) {
					fputc(ch, fpOut);
					break;
				}
				if (ch2 == '\n') {
					fputc(ch2, fpOut);
				} else {
					fputc(ch, fpOut);
					fputc(ch2, fpOut);
				}
			} else {
				fputc(ch, fpOut);
			}
		}
	} else {
		while (1) {
			ch = fgetc(fpIn);
			if (ch == EOF) {
				break;
			}
			if (ch == '\n') {
				fputc('\r', fpOut);
				fputc('\n', fpOut);
			} else {
				fputc(ch, fpOut);
			}
		}
	}
	
	fclose(fpIn);
	fclose(fpOut);

	return 0;
PRINT_USAGE:
	printf("Usage:\n  lf --[linux|windows] [input file] [output file]\n");
	printf("Option:\n  --linux   : windows to linux (\\r\\n to \\n)\n  --windows : linux to windows (\\n   to \\r\\n)\n");
	return 1;
}
