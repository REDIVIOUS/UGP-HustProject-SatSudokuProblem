#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "extension.h"

/*
	this file is aiming at sloving DPLL
	it will provide the necessary functions to solve DPLL and it will also provide the main function of DPLL
*/ 

/*tell if the list is empty, it means the example can be sloved.(or to say it is satisfied)*/
bool IsEmpty(Clause *current)
{
	Clause *tempc=current->next;
	while(tempc!=NULL)
	{
		if(tempc->state)
		{
			return false;
		}
		tempc=tempc->next;
	}
	return true;
}

/*tell if there is a singel clause in the list*/
bool ExistOne(const Clause *current)
{
	bool existence=false;
	Clause *tempc=current->next;
	while(tempc!=NULL)
	{
		if(clauseSingle(tempc))
		{
			existence=true;
		}
		tempc=tempc->next;
	}
	return existence;
}

/*create the space and bulid the structure of v*/
void CreateVspace(Clause *current)
{
	Clause *tempc=current;
	Clause *tempch=current->next;
	addClause(tempc,0);
	tempc=tempc->next;
	tempc->state=false;
	Literal *head=createLiteral();
	addsub(tempc,head,0);
	tempc->next=tempch;
}

/*to delete the current place literal(pay attention that the pointer must move to the location you want to change)*/
void deliteral(Clause *current)
{
	Literal *templ=current->next->sub;
	free(templ->next);
	templ->next=NULL;
}

/*get a v(here it is optimized by chosing the mode in the list)*/
int choosev(Clause *current)
{
	int num;
	Clause *tempc=current->next->next;
	while(tempc!=NULL)
	{
		if(tempc->state)
		{
			Literal *templ=tempc->sub->next;
			while(templ!=NULL)
			{
				if(templ->state)
				{
					num=templ->num;
					break; 
				}
				templ=templ->next;
			}
		}
		if(num!=0)
		{
			break;
		}
		tempc=tempc->next;
	}
	return num;
}

/*change the literal v to a cluase and add it into the clause list*/
int vToClause(Clause *current, int chosenliteral)
{
	Clause *tempc=current->next;
	Literal *templ=tempc->sub;
	addLiteral(templ,chosenliteral,1);
	tempc->clauselen=1;
	tempc->state=true; 
}

/*here is the main function of DPLL, the idea of building it will be talked specificly in the report.*/
bool DPLLsolving(Clause *C_current,Sign *S_current)
{
	Memory *tempm=createMemory();
	Memory *tempmh=tempm;
	while(ExistOne(C_current))
	{
		int v=findv(C_current);
		addSign(S_current,v);
		Clause *tempc=C_current->next;
		while(tempc!=NULL)
		{
			if(tempc->state)
			{
				Literal *templ=tempc->sub->next;
				while(templ!=NULL)
				{
					if(templ->state)
					{
						if(templ->num==v)
						{
							notclause(tempc);
							templ=tempc->sub->next;
							while(templ!=NULL)
							{
								if(templ->state)
								{
									notliteral(templ);
									addMemory(tempmh,tempc->location,templ->location);
									tempmh=tempmh->next;
									tempc->clauselen=tempc->clauselen-1;
								}
								templ=templ->next;
							}
						}
						else if(templ->num==-v)
						{
							notliteral(templ);
							addMemory(tempmh,tempc->location,templ->location);
							tempmh=tempmh->next;
							tempc->clauselen=tempc->clauselen-1;
							templ=templ->next;
						}
						else
						{
							templ=templ->next;
						}
					}
					else
					{
						templ=templ->next;
					}
				}
			}
			tempc=tempc->next;
		}
		if(IsEmpty(C_current))
		{
			regain(C_current,tempm);
			destroyMemory(tempm);
			return true;
		}
		tempc=C_current->next;
		while(tempc!=NULL)
		{
			if(tempc->clauselen==0&&tempc->state==true)
			{
				regain(C_current,tempm);
				destroyMemory(tempm);
				return false;
			}
			tempc=tempc->next;
		}
	}
	int chosenv=choosev(C_current);
	if(C_current->next->sub->next!=NULL)
	{
		deliteral(C_current);
	}
	vToClause(C_current,chosenv);
	bool pass;
	pass=DPLLsolving(C_current,S_current);
	if(pass)
	{
		regain(C_current,tempm);
		destroyMemory(tempm);
		return true;
	}
	reverse(C_current,chosenv);
	pass=DPLLsolving(C_current,S_current);
	if(pass)
	{
		regain(C_current,tempm);
		destroyMemory(tempm);
		return true;
	}
	else
	{
		regain(C_current,tempm);
		destroyMemory(tempm);
		return false;	
	}
}
