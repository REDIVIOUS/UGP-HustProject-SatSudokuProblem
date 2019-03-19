#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
 
/*
	this file gives functions to randomly creaate the sudoku.
	this file also gives the function to tell if the single input is right 
*/

int flag=1,total=0;

/*this function help us to control the location of our input*/		
void gotoxy(int x, int y)
{
	++x,++y; 
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/*this function help us to control the color of sudoku show*/
void color(int b)
{
    HANDLE hConsole = GetStdHandle((STD_OUTPUT_HANDLE)) ;
    SetConsoleTextAttribute(hConsole, b) ;
}
 
 
/*this function is aiming to create the checkboard of sudoku game*/
void Boundary()
{	
	gotoxy(3,1);
	color(7);
	printf("+---+---+---+---+---+---+---+---+---+");
	color(8);
	for(int i=1;i<10;++i)
	{
		color(8);
		gotoxy(3,i*2+1);
		if(i==3 || i==6)
		{
			color(7);
		}
		printf("+---+---+---+---+---+---+---+---+---+");
		for(int j=0;j<10;++j)
		{
			gotoxy(3+j*4,i*2);
			if(j==3 || j==6)
			{
				color(7);
			}
			if(j==0 || j==9)
			{
				color(7);
			}
			printf("|   ");
			color(7);
			gotoxy(3+j*4,i*2+1);
			if(i%3!=0 || j%3!=0)
			{
				color(8);
			}
			if(j==0 || j==9)
			{
				color(7);
			}
			if((i%3==0 || j%3==0) && j!=0 && j!=9)
			{
				color(7);
			}
			printf("+");
			color(8);
		}
	}
	color(7);
	gotoxy(3,19);
	printf("+---+---+---+---+---+---+---+---+---+");
	color(7);
	gotoxy(60,1);
	printf("Welcome to my Sudokugame");
	gotoxy(45,3);
	printf("please follow the instruction to play the game!");
	gotoxy(45,5);
	printf("if you input 123 which means that put 3 at col 1 & row 2");
	gotoxy(45,7);
	printf("if you input 1000, you will see the DPLL auto solving");
	for(int i=1;i<=9;++i)
	{
		gotoxy(4*i+1,0);
		printf("%d",i);
		gotoxy(1,2*i);
		printf("%d",i);
	}
}

/*this function is to judge if the current digital consist with the sudoku rule*/ 
bool Flag(int m,int n,int b,int board[9][9])
{
	int a[9];
	for(int i=0;i<9;++i)
	{
		if(board[i][m]==b)
		{
			return false;
		}
		if(board[n][i]==b)
		{
			return false;
		}
	}
	for(int i=0;i<9;++i)
	{
		a[i]=0;
	}
	a[0]=b;
	gotoxy(50,15);
	int q=(n/3)*3,p=(m/3)*3,r=1;
	for(int i=p;i<p+3;++i)
	{
		for(int j=q;j<q+3;++j)
		{
			if(board[j][i]>0 && board[j][i]<10)
			{
				a[r]=board[j][i];++r;
			}
		}
	} 
	for(int i=0;i<9;++i)
	{
		for(int j=i+1;j<9;++j)
		{
			if(a[i]==a[j] && i!=j && a[i]!=0)
			{
				return false;
			}
		}
	}
	return true;
}


/*help to create the complete sudoku randomly*/
void Sudoku(int board[9][9])
{
	time_t seed;
	srand(time(&seed));
	int number=15,p=0,q=0,i=0;
	while(i<number)
	{
		int m=rand()%9,n=rand()%9,b=rand()%9+1;
		if(Flag(n,m,b,board)==true)
		{
			board[m][n]=b;
			p=m,q=n;
			++i; 
		}
	}
	board[p][q]=0;
} 

/*this step is to dig out the digital from the complete sudoku*/
void digging(int input,int board[9][9],int sudoku[9][9][2])
{ 
	time_t seed;
	srand(time(&seed));
	int i=0;
	color(11);
	for(int i=0;i<9;++i)
	{
		for(int j=0;j<9;++j)
		{
			gotoxy(5+j*4,i*2+2);
			printf(" ");
			board[i][j]=0; 
		}
	}
	while(i<input)
	{
		int m=rand()%9, n=rand()%9;
		if(sudoku[m][n][1]==0)
		{
			gotoxy(5+n*4,m*2+2);
			printf("%d",sudoku[m][n][0]);
			sudoku[m][n][1]=1;
			board[m][n]=sudoku[m][n][0];
			++i;
		}
	}
} 
 

/*it aims to go thorugh the sudoku checkboard to judge if the input is right*/
int depth(int x,int y,int num,int board[9][9])
{
	int m,n,time=0;
	int dir[9][2]={{1,1},{1,0},{1,-1},{0,1},{0,-1},{-1,1},{-1,0},{-1,-1},{0,0}};
	for(int i=0;i<9;++i)
	{
		m=x+dir[i][0],n=y+dir[i][1];
		if(num==board[n][m])
		{
			++time;
		}
		if(time>1)
		{
			return 0;
		}
	}
	return 1;
}
 
 
/*to judge if the number input is right*/
int Judge(int x,int y,int n,int board[9][9])
{
	int t=0;
	for(int i=0;i<9;++i)
	{
		if(board[y][i]==n)
		{
			++t;
			if(t>1)
			{
				return 1;
			}
		}
	}
	t=0;
	for(int i=0;i<9;++i)
	{
		if(board[i][x]==n)
		{
			++t;
			if(t>1)
			{
				return 1;
			}
		}
	}
	if(depth(1+(x/3)*3,1+(y/3)*3,n,board)==0)
	{
		return 1;
	}
}
 
/*recursively go through the whole board to search if the digital input is legal*/
void search(int x,int y,int kind,int board[9][9],int sudoku[9][9][2],int &total)
{
	if(flag==0 && kind==0)
	{
		return;
	}
	if(y<9)
	{
		if(x<9)
		{
			if(board[y][x]<1 || board[y][x]>9)
			{
				for(int i=1;i<=9;++i)
				{
					board[y][x]=i;
					int k=Judge(x,y,i,board);
					if(k==0)
					{
						search(x+1,y,kind,board,sudoku,total);
					}
					board[y][x]=0;
				}
			}
			else
			{
				search(x+1,y,kind,board,sudoku,total);
			}
		}
		else
		{
			search(0,y+1,kind,board,sudoku,total);
		}
	}
	else
	{
		++total;
		flag=0;
		if(kind==0)
		{
			for(int i=0;i<9;++i)
			{
				for(int j=0;j<9;++j)
				{
					sudoku[i][j][0]=board[i][j];
				}
			}
		}
	}
}
