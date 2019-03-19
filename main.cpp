#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>
#include "solveDPLL.h"
#include "sudokushow.h"
#include "sudokutosat.h"

/*it is the main function of sudoku game*/
void sudoku()
{
	int board[9][9],sudoku[9][9][2];
	int digital;
	
	/*input represent the initial numbers of input.*/
	/*after trying, input around 35 gives the most randomlism*/
	int input=35;
	
	for(int i=0;i<9;++i)
	{
		for(int j=0;j<9;++j)
		{
			board[i][j];
			sudoku[i][j][1]=0;
		}
	}
	
	/*create the checkboard and digging randomly*/
	Boundary();
	while(input>34 && input<56)
	{
		total=0,flag=1;
		while(total==0)
		{
			Sudoku(board);
			search(0,0,0,board,sudoku,total);
			gotoxy(50,22);
		}
		digging(input,board,sudoku); 
		search(0,0,1,board,sudoku,total);
		
		/*here comes the ouput of cnf file*/
		/*it is aimed to reduce the problem to DPLL and solve it*/ 
		/*pay attention to change the path of ".cnf" file if you are not running the program in developer's computer*/ 
		char filename[100]="C:\\Users\\18131\\Desktop\\ACM1701-U201714850¶¡ÎÄÂ¡\\²âÊÔÑùÀý\\sudoku.cnf";
		FILE *out=fopen(filename,"w");
		SudokuToSat(board,out,input);
		
		/*to tell if the single input is right*/
		gotoxy(45,5);
		color(7);
		gotoxy(54,9);
		int key=1;
		while(key==1)
		{
			int h=0; 
			gotoxy(45,9);
			printf("Please input the number:"); 
			scanf("%d",&digital);	
			int x,y,z;
			x=digital/100%10-1;
			y=digital/10%10-1;
			z=digital%10;			
			gotoxy(5+4*x,2+2*y);
			if(sudoku[y][x][1]==0 && z!=0 && digital<1000 && y!=9 && x!=9)
			{
				board[y][x]=0;
				if(Flag(x,y,z,board)==true)
				{
					board[y][x]=z;
					gotoxy(5+4*x,2+2*y);
					color(14);
					printf("%d",z);
					for(int i=0;i<9;++i)
					{
						for(int j=0;j<9;++j)
						{
							if(board[i][j]!=0)
							{
								++h;
							}
						}
					}
					if(h==81)
					{
						key=0;
						gotoxy(45,9);
						color(4);
						printf("Completed");
						Sleep(3000);
					}
				}
				else
				{
					color(12);
					gotoxy(45,11);
					color(4);
					printf("It is wrong");
					Sleep(500); 
				} 
			}
			else
			{
				if(digital<1000)
				{
					color(7);
					gotoxy(45,11);
					color(4);
					printf("place occupied!");
					Sleep(500);
				}
			}
			
			/*if your input is 1000, you will see the DPLL solving the whole game*/
			if(digital==1000)
			{
				key=0;
				gotoxy(45,9);
				printf("You will see the DPLL solving!");
				for(int i=0;i<9;++i)
				{
					for(int j=0;j<9;++j)
					{
						if(sudoku[i][j][1]==0)
						{
							Sleep(100);
							gotoxy(5+j*4,i*2+2);
							color(12);
							printf("%d",sudoku[i][j][0]);
						}
						board[i][j]=0;
						sudoku[i][j][0]=0;
						sudoku[i][j][1]=0; 
					}
				} 
			}
			
			
			color(7);
			gotoxy(45,9);
			printf("                                               ");
			gotoxy(45,11);
			printf("                                           ");
		}
		
		gotoxy(45,11);
		
		/*input 35 to restart*/
		/*in fact you can input 33-55 to set the initial digital number*/
		/*put in other number to quit*/ 
		printf("input 35 to reset the game, or else quit!");
		scanf("%d",&input);
		gotoxy(45,11);
		printf("                                                           ");
	} 
	gotoxy(0,20);
}


/*this is the main function to deal with the file with SAT*/ 
void SAT()
{
	/*to define the bool type pass, and apply for the space for in and out file*/
	bool pass;
	
	/*deal with the in and out file and extract the information*/
	char *in=(char *)malloc(300*sizeof(char));
	char *out=(char *)malloc(300*sizeof(char));
	filehandle(in,out);
	FILE *PutInFile=fopen(in,"r");
	if(in==NULL)
	{
		printf("Such file dose not exsist£¡");
		exit(0);
	}
	int C_num=0,L_num=0;
	filextract(PutInFile,&C_num,&L_num);
	
	/*create the list*/
	Clause *clauselist=createClause();
	Clause *tempc=clauselist;
	createStructure(PutInFile,C_num,tempc);
	fclose(PutInFile);
	free(in);
	
	/*print the list to the screen*/
	showSturcture(clauselist);
	
	/*run DPLL to solve the problem*/
	Sign *signlist=createSign();
	Sign *sortedsign;
	CreateVspace(clauselist);
	clock_t t1=clock();
	pass=DPLLsolving(clauselist,signlist);
	clock_t t2=clock();
	if(pass)
	{
		printf("this is a satisfiable example!\n");
	} 
	 else
	{
		printf("this is an unsatisfiable example!\n");
	}
	sortedsign=sortSign(signlist);
	destroyClause(clauselist);
	destroySign(signlist);
	
	/*set out the result to ".res"*/
	FILE *PutOutFile=fopen(out,"w");
	fileout(PutOutFile,sortedsign,pass,t2-t1);
	fclose(PutOutFile);
	free(out);
	destroySign(sortedsign);	
}

/*the main function, choose whether you want to do sudoku or the file handle by SAT*/
int main()
{
	gotoxy(50,0);
	printf("The Main Menu");
	gotoxy(0,2);
	printf("what function do you want to do?\n");
	int n;
	gotoxy(0,4);
	printf("1.if you input 1, deal with SAT file.\n");
	gotoxy(0,6);
	printf("2.if you input 2, play the sudokugame.\n");
	gotoxy(0,8);
	printf("input the control number:");
	scanf("%d",&n);
	system("CLS");
	if(n==1)
	{
		SAT();
	}
	if(n==2)
	{
		sudoku();
	}
	return 0;
}
