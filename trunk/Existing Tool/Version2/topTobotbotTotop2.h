//Performs the columnar transposition encription or decryption.
//Read the file 340.txt from top to bottom and bottom  to top continously. 

#include <stdio.h>

void topTobotbotTotop() 
{
	char buffer[20][17] = {{0},{0}};  /* initialized to zeros */
	char tmp[17][20] = {{0}, {0}};
	int i, j;
	int n, x;
	char ch; 	
	char filename[BUFSIZ +1];
	FILE *ifp = fopen("340.txt", "r");
	FILE *ofp;
	
	
	if (ifp == NULL) {
        printf("Couldn't open the file!\n");
//        return 0;
    	}

    	for (i = 0; i < 20; i++) {
		for (j = 0; j < 18; j++) {
			ch = fgetc(ifp);
			if (ch != '\n' && j <= 17) {
	       			buffer[i][j] = ch;
	       		}
		}
	}
	printf("\n\t    Plain Text\t\tRead Top to Bottom\n");
	printf("\t\t\t\tand Bottom to Top\n");
	ofp = fopen("topTobotbotTotop.txt", "w");
	for(j=0; j<17;j++){
		x=j;
		x=x%2;
		if (x==0) { 
			for (i=0; i < 20; i++) {
				tmp[j][i]=buffer[i][j];
			}
		}//read top to bottom
		if (x==1) { 
			for (i=19, n=0; i >= 0 && n<20; n++, i--) {
				tmp[j][n]=buffer[i][j];
			}	
		}//read bottom to top
	}
			printf("\n");	
	for(i=0; i<20; i++) {
		printf("  \t");
			for(j=0; j<17; j++) {
				printf("%c", buffer[i][j]);
			}
			printf("\t");	
			for (n=0; n<20; n++) {
			if (i<17) {	
				printf("%c", tmp[i][n]);
				fputc(tmp[i][n], ofp);
			}
			if (i>17) {
				printf(" ");
				fputc(' ', ofp);
			}
			}
			printf("\n");
			fputc('\n', ofp);
	}
	
	if(ofp == NULL) {
	fprintf(stderr, "Can't open output file %s!\n", "topTobotbotTotop.txr");	
	exit(1);
	}	
		printf("\n\t\tSuccessfully Converted!\n\n");
	fclose(ifp);
	fclose(ofp);
	
}

