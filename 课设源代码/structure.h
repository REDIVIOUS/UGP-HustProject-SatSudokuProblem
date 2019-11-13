#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


/*
	this file amis to provide basic data structures and the basic operation to deal with the structure.
	the basic operation include create the structure, add the code, destroy the structure.
	this file also provide the functions to extract the information in the file and use it to create the list.
*/


 
/*
	the following four structures are built to slove DPLL
	Literlas is the structure of literal
	Clauses is the structure of clause
	Sign is the structure to record the number and sign of each literal
	Memory aims to remember the changes in one list when running DPLL, in order to regain it
*/

typedef struct literal
{
	int location;
	int num;
	bool state;
	struct literal *next;
}Literal;

typedef struct clause
{
	int location;
	int clauselen;
	bool state;
	struct literal *sub;
	struct clause *next;
}Clause;

typedef struct sign
{
	int sgn;
	int num;
	struct sign *next;
}Sign;

typedef struct memory
{
	int l_location;
	int c_location;
	struct memory *next;
}Memory;



/*the following four functions are to create the lists(structures) of each type*/ 
Clause *createClause()
{
	Clause *New=(Clause *)malloc(sizeof(Clause));
	New->clauselen=0;
	New->location=0;
	New->state=true;
	New->sub=NULL;
	New->next=NULL;
} 

Literal *createLiteral()
{
	Literal *New=(Literal *)malloc(sizeof(Literal));
	New->location=0;
	New->num=0;
	New->state=true;
	New->next=NULL;
	return New;
}

Sign *createSign()
{
	Sign *New=(Sign *)malloc(sizeof(Sign));
	New->num=0;
	New->sgn=-1;
	New->next=NULL;
	return New; 
}

Memory *createMemory()
{
	Memory *New=(Memory *)malloc(sizeof(Memory));
	New->c_location=0;
	New->l_location=0;
	New->next=NULL;
	return New;  
}




/*the following four functions are to add nodes to each structures*/
void addClause(Clause *Current, int location)
{
	Clause *New=(Clause *)malloc(sizeof(Clause));
	New->location=location;
	New->clauselen=0;
	New->state=true;
	New->next=NULL;
	New->sub=NULL;
	Current->next=New;
}

void addLiteral(Literal *Current, int num, int location)
{
	Literal *New=(Literal *)malloc(sizeof(Literal));
	New->num=num;
	New->location=location;
	New->state=true;
	New->next=NULL;
	Current->next=New; 
}

void addSign(Sign *Current, int num)
{
	Sign *New=(Sign *)malloc(sizeof(Sign));
	Sign *temps=Current->next;
	Sign *temph=Current;
	New->num=abs(num);
	if(num>0)
	{
		New->sgn=1; 
	}
	else
	{
		New->sgn=-1;
	}
	New->next=NULL;
	while(temps!=NULL)
	{
		if(temps->num==abs(num))
		{
			if(temps->num>0)
			{
				temps->sgn=1;
			}
			else
			{
				temps->sgn=-1;
			}
			free(New);
			break;
		}
		temph=temph->next;
		temps=temps->next;
	}
	if(temps==NULL)
	{
		temph->next=New;
	}
}

void addMemory(Memory *Current, int c_location, int l_location )
{
	Memory *New=(Memory *)malloc(sizeof(Memory));
	New->c_location=c_location;
	New->l_location=l_location;
	New->next=NULL;
	Current->next=New;
}




/*the following three functions are aimed to destroy the sturcture to free the space*/
void destroyClause(Clause *Current)
{
	Clause *tempc=Current;
	Clause *tempch;
	while(tempc!=NULL)
	{
		Literal *templ=tempc->sub,*templh;
		while(templ!=NULL)
		{
			templh=templ->next;
			free(templ);
			templ=templh;
		}
		tempch=tempc->next;
		free(tempc);
		tempc=tempch; 
	}
}

void destroySign(Sign *Current)
{
	Sign *temp=Current,*temph;
	while(temp!=NULL)
	{
		temph=temp->next;
		free(temp);
		temp=temph;
	}
}

void destroyMemory(Memory *Current)
{
	Memory *temp=Current,*temph;
	while(temp!=NULL)
	{
		temph=temp->next;
		free(temp);
		temp=temph;
	}
}



/*input the file to the program and create the outfile*/
void filehandle(char *in, char *out)
{
	getchar(); 
	char res[10]=".res";
	printf("what file do you what to deal with?\n");
	fgets(in,200,stdin);
	int len=strlen(in);
	in[len-1]='\0';
	for(int i=0;i<len;i++)
	{
		out[i]=in[i];
	}
	for(int i=len-5;i<len-1;i++)
	{
		out[i]=res[i-len+5];
	}
}

/*put out the information needed to the outfile*/
void fileout(FILE *out, Sign *Current, bool pass, clock_t time)
{
	if(pass==true)
	{
		Sign *temps=Current->next;
		fprintf(out,"s %d\nv ",pass);
		while(temps!=NULL)
		{
			fprintf(out,"%5d",temps->num*temps->sgn);
			temps=temps->next;
		}
		fprintf(out,"\nt %5ld ms\n",time);
	}
	else
	{
		fprintf(out,"s %d\nt %5ld ms\n",pass,time);
	}
}

/*to extract the digital information from the file*/
void filextract(FILE *in, int *clause, int *literal)
{
	char a[200];
	do
	{
		fgets(a,50,in);
	}while(a[0]!='p');
	int count=6;
	while(a[count]!=' ')
	{
		(*literal)=(a[count]-'0')+(*literal)*10;
		count++;
	}
	count++;
	while(a[count]!='\n')
	{
		(*clause)=(a[count]-'0')+(*clause)*10;
		count++;
	}
}


