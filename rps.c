#include <stdio.h>
#include <string.h>

int main()
{
	int compScore=0, playerScore=0;
	char ans[100];
	//random number seed
	srand((unsigned int)time(NULL));
	printf("Welcome to Rock, Paper, Scissors!\n");
	printf("Would you like to play?\n");
	fgets(ans, sizeof(ans), stdin);
	//Check multiple variations on user input
	while((strcmp(ans,"yes\n")==0)||(strcmp(ans, "Yes\n")==0)||(strcmp(ans,"y\n")==0)||(strcmp(ans,"Y\n")==0))
	{
		//main game loop, executes until a score of 3 is reached for either side
		while(playerScore<3 && compScore<3)
		{
			printf("Rock, Paper, or Scissors?\n");
			char playerChoice[100];
			char*  compChoice;
			fgets(playerChoice, sizeof(playerChoice), stdin);
			//Random number generation for picking the computer's choice
			int pseudoRand = (rand() % (3 - 1 + 1) + 1);
			if(pseudoRand==1)
			{
				compChoice="scissors";
			}
			else if(pseudoRand==2)
			{
				compChoice = "rock";
			}
			else
			{
				compChoice = "paper";
			}
			//removing and replacing the "\n" characters from the playerChoice string after initial entry
			int length = strlen(playerChoice);
			if(length > 0 && playerChoice[length - 1] == '\n')
			{
					playerChoice[length - 1] = '\0';
			}
			//Accounting for when the computer and the player both pick the same thing
			if(strcmp(playerChoice, compChoice)==0)
			{
				printf("Same choice, try again\n");
			}
			//Multiple if statement block for comparing player to computer
			else if(((strcmp(playerChoice, "rock")==0)&&(strcmp(compChoice, "paper"))==0))
			{
				printf("You chose rock, the computer chose paper!\nPaper covers rock!\n");
				compScore++;
				printf("Score:\tYou:%d\tComputer:%d\n", playerScore, compScore);
			}
			else if(((strcmp(playerChoice, "rock")==0)&&(strcmp(compChoice, "scissors"))==0))
            {
				printf("You chose rock, the computer chose scissors\nRock crushes scissors!\n");
                playerScore++;
                printf("Score:\tYou:%d\tComputer:%d\n", playerScore, compScore);
            }
			else if(((strcmp(playerChoice, "scissors")==0)&&(strcmp(compChoice, "paper"))==0))
            {
				printf("You chose scissors, the computer chose paper\nScissors cuts paper!\n");
                playerScore++;
                printf("Score:\tYou:%d\tComputer:%d\n", playerScore, compScore);
            }
			else if(((strcmp(playerChoice, "scissors")==0)&&(strcmp(compChoice, "rock"))==0))
            {
				printf("You chose scissors, the computer chose rock\nRock crushes scissors!\n");
                compScore++;
                printf("Score:\tYou:%d\tComputer:%d\n", playerScore, compScore);
            }
			else if(((strcmp(playerChoice, "paper")==0)&&(strcmp(compChoice, "rock"))==0))
            {
				printf("You chose paper, the computer chose rock\nPaper covers rock!\n");
                playerScore++;
                printf("Score:\tYou:%d\tComputer:%d\n", playerScore, compScore);
            }
			else if(((strcmp(playerChoice, "paper")==0)&&(strcmp(compChoice, "scissors"))==0))
            {
				printf("You chose paper, the computer chose scissors\nScissors cuts paper!\n");
                compScore++;
                printf("Score:\tYou:%d\tComputer:%d\n", playerScore, compScore);
            }
			//handling all other inputs that are not vital to the game
			else
			{
				printf("Invalid entry try again\n");
			}
		}
		//final score printout
		printf("Final score\tYou:%d\tComputer:%d\n", playerScore, compScore);
		if(compScore==3)
		{
			printf("Try again next time!\n");
		}
		else if(playerScore==3)
		{
			printf("Congratulations!\n");
		}
		printf("Play again?\n");
		fgets(ans, sizeof(ans), stdin);
		playerScore=0, compScore=0;
	}
	if((strcmp(ans,"no\n")==0)||(strcmp(ans, "No\n")==0)||(strcmp(ans,"n\n")==0)||(strcmp(ans,"N\n")==0))
	{
		printf("Good bye!\n");
	}
	return 0;
}
