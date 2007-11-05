//Performs the columnar transposition encription or decryption.
//Read the file 340.txt from left to right and right to left 

#include <stdio.h>

void leToririTole() 
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
        printf("\n\t    Plain Text\t\tRead Left to Right\n");
        printf("\t\t\t\tand Right to Left\n\n");
	ofp = fopen("leToririTole.txt", "w");
	
	for(i=0; i<20;i++) {
		x=i;
		x=x%2;
		
		if(x==0) {
			printf("  \t");
			for (j=0; j <17 ; j++) {
				printf("%c", buffer[i][j]);
			}
			printf("\t");
			for (j=0; j <17 ; j++) {
				printf("%c", buffer[i][j]);
				fputc(buffer[i][j], ofp);
			}
				fputc('\n', ofp);
				printf("\n");
		}
		if(x==1) {
			printf("  \t");
			for (j=0; j <17 ; j++) {
				printf("%c", buffer[i][j]);
			}
			printf("\t");
			
			for (j=16; j >= 0 ; j--) {
				printf("%c", buffer[i][j]);
				fputc(buffer[i][j], ofp);
			}	
				fputc('\n', ofp);
				printf("\n");
		}
	}
	
	if(ofp == NULL) {
	fprintf(stderr, "Can't open output file %s!\n", "col_result.txr");	
	exit(1);
	}	
		printf("\n\t\tSuccessfully Converted!\n\n");
	fclose(ifp);
	fclose(ofp);
	
}

