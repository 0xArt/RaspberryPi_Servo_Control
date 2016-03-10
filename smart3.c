//Terminal Servo Control for Raspberry Pi2
//smart3.c
//by Artin Isagholian
//2/2/2016

#include<stdio.h>
#include<bcm2835.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char **argv)
{


if(!bcm2835_init()) return 1;

unsigned int hightime = 1;
int angle, prevangle, pulse, length, i, error;
char input[5];


bcm2835_gpio_fsel(18, BCM2835_GPIO_FSEL_OUTP); //set pin 18 as output


while(1)

{    
	error =0;
	printf("Enter an integer degree of rotation (0 to 180) or q to quit:\n");
	fgets(input,5,stdin);
    	length = strlen(input);
   	for(i=0; i<6; i++)
		{
			printf("Character %d: %c\n", i, input[i]);
		}
    printf("length: %d\n", length);
    //printf("input: %s\n", input);
	if (length==4)
	{
		input[5] = '\n';
	} 	   
	
	if(length>4 || input[length-1]!='\n' || (length!=2 && input[0]=='0') || input[0]=='\n')
	{
		error=1;
		//printf("error0\n");
	}
    
    
	if(length==2 && input[0]=='q' && input[1]=='\n' && error==0)
    	{
        	break;
	}

    	if(error==0)
   	{
    		for (i=0;i<(length-1); i++)
        	{
			if (!isdigit(input[i]) || isspace(input[i]))
			{
				error=1;
				//printf("error1\n");
				continue;
			}
    	 	}
	}
    


	if (error==0)
	{
		angle = (int)strtol(input,NULL,10);
	}
    
	//printf("angle: %d\n", angle);
		

		if ( (angle>180) || (angle<0) || (error==1)) //invalid input
		{
		printf("Invalid input, try again.\n");
		}

		if((angle<=180) && (angle>=0) && (error==0))	//valid input
		{
			
			if(hightime!=1)//number of pulses after first rotation
			{
			pulse = (int)abs(((angle - prevangle)*33)/180) +5;//it takes about 33 pulses to complete a turn from 0degrees to 180degrees
			}					          //therefore using ratio to calculate number of pulses required and adding 5 for good measure

				
			if(hightime==1)//default number of pulses for very first rotation
			{
				pulse =33;
				prevangle = angle;
			}
		


		hightime = (10*angle) + 546;

		prevangle = angle;
		

		for(i=0; i<pulse; i++)//exucting pulses
			{
 			bcm2835_gpio_set(18);//pin18 high
			bcm2835_delayMicroseconds(hightime);
			bcm2835_gpio_clr(18);//pin18 low
			bcm2835_delayMicroseconds(20000 - hightime);//each pulse is 20ms
			}
		}
	}		


int ch;
while ((ch = getchar()) != '\n' && ch != EOF);



return 0;
}



