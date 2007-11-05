#include "botTotop.h"
#include "botTotoptopTobot.h"
#include "leToririTole.h"
#include "riTole2.h"
#include "riToleleTori.h"
#include "topTobot.h"
#include "topTobotbotTotop2.h"
#include <stdio.h>

int main(void) {
	char c;
	
	printf("1. Bottom to Top and Bottom to top :\n");
	printf("2. Bottom to top and Top to Bottom :\n");
	printf("3. Left to Rigth and Right to Left :\n");
	printf("4. Right to Left and Right to Left :\n");
	printf("5. Right to Left and Left to Right :\n");
	printf("6. Top to Bottom and Top to Bottom :\n");
	printf("7. Top to Bottom and Bottom to Top :\n");
	printf("Please Choose the Options between 1~7: ");
	c=getchar();
	printf("\n");
	
	switch(c) {
	case '1' :
		botTotop();
		break;
	case '2' :
		botTotoptopTobot();
		break;
	case '3' :
		leToririTole();
		break;
	case '4' :
		riTole();
		break;
	case '5' :
		riToleleTori();
		break;
	case '6' :
		topTobot();
		break;
	case '7' :
		topTobotbotTotop();
		break;
	default :
		printf("You've Entered the Invalid Option Number\n");
	}

return 0;
}
