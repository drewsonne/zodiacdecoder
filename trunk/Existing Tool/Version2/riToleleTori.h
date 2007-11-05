//Performs the columnar transposition encription or decryption.
//Read the file 340.txt from right to left and left to right. 

#include <stdio.h>

void riToleleTori() 
{
	char buffer[20][17] = {{0},{0}};  /* initialized to zeros */
	int i, j;
	char filename[BUFSIZ +1];
	FILE *ifp = fopen("340.txt", "r");
	FILE *ofp;
	char ch;	
	int x;
	
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
        printf("\n\t    Plain Text\t\tRead Right to Left\n");
        printf("\t\t\t\t and Left to Right\n\n");
	ofp = fopen("riToleleTori.txt", "w");
	for(i=0; i<20;i++) {
	printf("  \t");
		for (j=0; j <17 ; j++) {
			printf("%c", buffer[i][j]);
		}
		x=i;
		x=x%2;
	if(x==1) {
		printf("  \t");
		for (j=0; j <17 ; j++) {
			printf ("%c", buffer[i][j]);
			fputc(buffer[i][j], ofp);
		}
			printf("\n");
			fputc('\n', ofp);
	}
	if(x==0) {
		printf("  \t");
			for (j=16; j >= 0 ; j--) {
			printf ("%c", buffer[i][j]);
			fputc(buffer[i][j], ofp);
		}	
			printf("\n");
			fputc('\n', ofp);
			
	}
	}
	
	if(ofp == NULL) {
	fprintf(stderr, "Can't open output file %s!\n", "col_result.txr");	
	exit(1);
	}	
		printf("\n\t\tSuccessfully converted!\n\n");
	fclose(ifp);
	fclose(ofp);
	
}



