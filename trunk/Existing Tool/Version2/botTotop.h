//Performs the columnar transposition encription or decryption.
//Read the file 340.txt from bottom to top and bottom  to top continously. 

#include <stdio.h>

void botTotop() 
{
	char buffer[20][17] = {{0},{0}};  /* initialized to zeros */
	char tmp[17][20] = {{0}, {0}};
	int i, j;
	char filename[BUFSIZ +1];
	FILE *ifp = fopen("340.txt", "r");
	FILE *ofp;
	char ch;	
	int x, n;
	
	if (ifp == NULL) {
        printf("Couldn't open the file!\n");
 //       return 0;
    	}

    	for (i = 0; i < 20; i++) {
		for (j = 0; j < 18; j++) {
			ch = fgetc(ifp);
			if (ch != '\n' && j <= 17) {
	       			buffer[i][j] = ch;
	       		}
		}
	}
    printf("\n\t    Plain Text\t\tRead Bottom to Top\n");
	printf("\t\t\t\t and Top to Bottom\n\n");
	ofp = fopen("botTotop.txt", "w");
	for (j=0; j < 17; j++) {
		for(i=19, n=0; i >= 0 && n<20; n++, i--) {
				tmp[j][n]=buffer[i][j];
		}	
	}
	
		for(i=0; i<20; i++) {
		printf("  \t");
			for(j=0; j<17; j++) {
				printf("%c", buffer[i][j]);
			}
			printf("\t");
			for (n=0; n<20; n++) {
				if(i<17) {
					printf("%c", tmp[i][n]);
					fputc(tmp[i][n], ofp);
				}
				if(i>17) {
					printf(" ");
					fputc(' ', ofp);
				}
			}
			printf("\n");
			fputc('\n',ofp);
	}
		
	
	if(ofp == NULL) {
	fprintf(stderr, "Can't open output file %s!\n", "result.txr");	
	exit(1);
	}	
		printf("\n\t\tSuccessfully converted!\n\n");
	fclose(ifp);
	fclose(ofp);
	
}



