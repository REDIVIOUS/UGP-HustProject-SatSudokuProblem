#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "structure.h"

/*
	this file will provide some necessary functions based on the structure
	And it will also provide the basic structure and functions to solve DPLL  
*/

/*to add the subchain to the clause chain*/
void addsub(Clause *c_current, Literal *l_current, int clauselen)
{
	c_current->sub=l_current;
	c_current->clauselen=clauselen;
}

/*this function is to deal with the number in a sorted condition*/
Sign *sortSign(Sign *Current)
{
	int len=0,k=0,w=0,time=0;
	Sign *Temp1=Current->next;
	while(Temp1!=NULL)
	{
		len++;
		Temp1=Temp1->next;
	}
	int num[len],sgn[len]; 
	Temp1=Current->next;
	while(Temp1!=NULL)
	{
		num[time]=Temp1->num;
		sgn[time]=Temp1->sgn;
		Temp1=Temp1->next;
		time++;
	}
	for(int i=0;i<len-1;i++)
	{
		for(int j=i+1;j<len;j++)
		{
			if(num[i]>num[j])
			{
				k=num[i];
				num[i]=num[j];
				num[j]=k;
				w=sgn[i];
				sgn[i]=sgn[j];
				sgn[j]=w;
				
			}
		}
	}
	Sign *Sorted = createSign();
	for(int i=0;i<len;i++)
	{
		addSign(Sorted,num[i]*sgn[i]);
	}
	return Sorted;	
}

/*this function aims to tell if a clause only have single literal*/
bool clauseSingle(Clause *current)
{
	if(current->clauselen==1&&current->state==true)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*find the literal v in the list(pay attention that the pointer must move to the location you want to change)*/
int findv(Clause *current)
{
	int num=0;
	Clause *tempc=current->next;
	while(tempc!=NULL)
	{
		if(clauseSingle(tempc))
		{
			Literal *templ=tempc->sub->next;
			while(templ!=NULL)
			{
				if(templ->state)
				{
					num=templ->num;	
				}
				templ=templ->next;
			}
			break;
		}
		tempc=tempc->next;		
	}
	return num;
}

/*the following four functions aims to change the state of each node(either clause or literal)*/ 
void notclause(Clause *current)
{
	current->state=false;
}
void notliteral(Literal *current)
{
	current->state=false;
}
void yesclause(Clause *current)
{
	current->state=true;
}
void yesliteral(Literal *current)
{
	current->state=true;
}

/*to change the sign of the literal.(pay attention that the pointer must move to the location you want to change)*/
void reverse(Clause *current,int chosenliteral)
{
	Clause *tempc=current->next;
	Literal *templ=tempc->sub->next;
	templ->num=-chosenliteral;
	tempc->state=true;
}

/*to change the sign of the literal.(pay attention that the pointer must move to the location you want to change)*/
void regain(Clause *C_current, Memory *M_current)
{
	Memory *tempm=M_current->next;
	while(tempm!=NULL)
	{
		Clause *tempc=C_current->next;
		while(tempc->location!=tempm->c_location)
		{
			tempc=tempc->next;
		}
		yesclause(tempc);
		Literal *templ=tempc->sub->next;
		while(templ->location!=tempm->l_location)
		{
			templ=templ->next;
		}
		yesliteral(templ);
		if(tempc==C_current->next)
		{
			tempc->clauselen=1;
		}
		else
		{
			tempc->clauselen=tempc->clauselen+1;
		}
		tempm=tempm->next;
	}
} 
 
/*print the list to the screen*/ 
void showSturcture(const Clause *head)
{
	Clause *tempc=head->next;
	while(tempc!=NULL)
	{
		Literal *templ=tempc->sub->next;
		while(templ!=NULL)
		{
			printf("%5d",templ->num);
			templ=templ->next;
		}
		tempc=tempc->next;
		printf("\n");
	} 
}

/*using the information in the file to create the list*/
void createStructure(FILE *in, int clause, Clause *Current)
{
	int i=0,count=0;
	int c_location=0,l_location=0;
	for(int j=0;j<clause;j++)
	{
		addClause(Current,c_location+1);
		Current=Current->next;
		Literal *head=createLiteral(),*templ=head;
		fscanf(in,"%d",&i);
		while(i!=0)
		{
			addLiteral(templ,i,l_location+1);
			templ=templ->next;
			l_location++;
			count++;
			fscanf(in,"%d",&i);
		}
		c_location++;
		l_location=0;
		addsub(Current,head,count);
		count=0;
	}
}

