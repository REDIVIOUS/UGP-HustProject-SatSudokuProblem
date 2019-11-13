#include<stdio.h>

/*
	this function help us to translate sudoku question to SAT question so as to use DPLL to solve
	this function can also create the ".cnf" file 
	pay attention to change the path of ".cnf" file if you are not running the program in developer's computer 
*/


void SudokuToSat(int board[9][9], FILE *out, int count)
{
	char a[100]="Attention:";
	char b[100]="the first digital in the number means the colume in the sudoku.";
	char c[100]="the second digital in the number means the row in the sudoku.";
	char d[100]="the third digital in the number means the literal in the sudoku.";
	fprintf(out,"%s\n%s\n%s\n%s\n\n",a,b,c,d);
	fprintf(out,"p cnf 729 %d\n",11988+count);
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(board[i][j]!=0)
			{
				fprintf(out,"%d 0\n",100*j+10*i+board[i][j]+110);
			}
		}
	}

	/*the same place will only have one literal*/
	for(int i=1;i<=9;i++)
	{
		for(int j=1;j<=9;j++)
		{
			for(int w=1;w<=9;w++)
			{
				for(int u=w+1;u<=9;u++)
				{
					fprintf(out,"%d %d 0\n",-100*i-10*j-w,-100*i-10*j-u);
				}
			}
		}
	}
	/*the same colume must not be the same*/
	for(int i=1;i<=9;i++)
	{
		for(int j=1;j<=9;j++)
		{
			for(int w=1;w<=9;w++)
			{
				for(int u=w+1;u<=9;u++)
				{
					fprintf(out,"%d %d 0\n",-100*i-10*w-j,-200*i-10*u-j);
				}
			}
		}
	}
	/*the same row must not be the same*/
	for(int i=1;i<=9;i++)
	{
		for(int j=1;j<=9;j++)
		{
			for(int w=1;w<=9;w++)
			{
				for(int u=w+1;u<=9;u++)
				{
					fprintf(out,"%d %d 0\n",-100*w-10*i-j,-100*u-10*i-j);
				}
			}
		}
	}
	/*the same block must not be the same*/
	for(int p=0;p<3;p++) 
	{
		for(int q=0;q<3;q++)
		{
			for(int i=1;i<=9;i++)
			{
				for(int j=1+3*p;j<=3+3*p;j++)
				{
					for(int k=1+3*q;k<=3+3*q;k++)
					{
						for(int m=1+3*p;m<=3+3*p;m++)
						{
							for(int w=k+1;w<=3+3*q;w++)
							{
								fprintf(out,"%d %d 0\n",-i-100*j-10*k,-i-100*m-10*w);
							}
						}
					}
				}
			}
		}
	}
	/*each block must exsist 1 to 9*/
	for(int i=1;i<=9;i++)
	{
		for(int j=1;j<=9;j++)
		{
			for(int w=1;w<=9;w++)
			{
				fprintf(out,"%d ",100*i+10*j+w); 
			}
			fprintf(out,"%d\n",0);				 
		}
	}
	/*each col must exsist 1 to 9*/
	for(int i=1;i<=9;i++)
	{
		for(int j=1;j<=9;j++)
		{
			for(int w=1;w<=9;w++)
			{
				fprintf(out,"%d ",100*i+10*w+j); 
			}
			fprintf(out,"%d\n",0);				 
		}
	}
	/*each row must exsist 1 to 9*/
	for(int i=1;i<=9;i++)
	{
		for(int j=1;j<=9;j++)
		{
			for(int w=1;w<=9;w++)
			{
				fprintf(out,"%d ",100*w+10*i+j); 
			}
			fprintf(out,"%d\n",0);
		}
	}
	/*each block must exsist 1 to 9*/
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			for(int w=1;w<=9;w++)
			{
				for(int m=1+3*i;m<=3+3*i;m++)
				{
					for(int n=1+3*j;n<=3+3*j;n++)
					{
						fprintf(out,"%d ",m*100+n*10+w);
					}
				}
				fprintf(out,"%d\n",0);
			}
		}
	}	
}
