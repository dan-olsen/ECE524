/***************************************************************************************************************************
Header Files
****************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <assert.h>

/***************************************************************************************************************************
Constant Declarations 
****************************************************************************************************************************/
#define DELIMITER 	1
#define VARIABLE 	2
#define NUMBER 		3
#define MAX_LINE	100

char token[80];
char tok_type;
char *prog;
/***************************************************************************************************************************
Structure Declarations 
****************************************************************************************************************************/
typedef struct struct_node {
	char nd_num;
	int refe;
	int oper;
	int asap;
	int Nasap;
	int alap;
	int Fasap;
	
} node;

/***************************************************************************************************************************
Function Declarations 
****************************************************************************************************************************/
void get_token(void);
int isdelim(char);

/***************************************************************************************************************************
  Main Function
****************************************************************************************************************************/

void main(int argc,char **argv) 
{
	FILE *infile, *resfile;
	node graph[100];

	int i=0, counter, Mnode, Mnode_id=0;
	int mult_count=0, alu_count=0, oper_count=0;
	int Nasap, Fasap, C=1, min_asap, flag, sameasap =1;

	char line[100], buf[100];

	infile = fopen(argv[1], "r");

	while(fgets(buf, MAX_LINE, infile)!='\0')
  	{
		prog=buf;
		counter=0;
		do
		{
			get_token();
			switch(tok_type)
			{
				case NUMBER:
					if (counter==0)
					{
						graph[i].nd_num= atoi(token);

						if (Mnode_id < graph[i].nd_num)
							Mnode_id = graph[i].nd_num;
					}

					if (counter ==1)
					{
						graph[i].refe = atoi(token);
					}

					if (counter == 2)
					{
						graph[i].asap = atoi(token);
						if (i==0)
							min_asap = graph[i].asap;
						else if(graph[i].asap < min_asap)
							min_asap = graph[i].asap;
					}

					if (counter==3)
					{
						graph[i].alap = atoi(token);
					}
					counter++;
				
					break;
				case VARIABLE:
					if (counter < 2)
					{
						if(! strcmp(token, "source"))
						{
							graph[i].oper=0;
						}

						if(! strcmp (token, "sink"))
						{
							graph[i].oper=3;
						}
					}
				
					break;
				case DELIMITER:
					if (counter > 0 && counter < 2)	
					{	
						if (strchr("*", *token))
						{
							graph[i].oper=1;
							mult_count++;
						}

						if (strchr("+-/%^=()<>",*token))
						{
							graph[i].oper=2;
							alu_count++;
						}
					}
				
					break;
				default:
					break;
			}
		}

		while (counter < 4);
		Mnode=i;
		i++;
	
	} 

	resfile = fopen ("equations.lp", "w");

	// 1. Read the benchmark files(.asapalap) and store it in your own defined data structure 
	// 2. Form the linear equations using the information in the data structure and print in a file(ilpfile) 
	// 3. Use lp solver to solve the inequalities and store the output of lpsolver in file.
		  //lpsover command: lp_solve "ilpfile" 
	// 4. The two functions(get_token,isdelim) can be used for reading the benchmark files(.asapalap). 
		  //Using these two functions in your implementation is optional. 

	fclose(infile);
	fclose(resfile);
}//end of main
/***************************************************************************************************************************
 Read a group of characters together return as a single string refered as token
(This piece code is taken form the Text Book: C: The complte reference by Herbert Schildt(4th Edition) page no:592) 
****************************************************************************************************************************/
void get_token(void)
{
	register char *temp;

	tok_type = 0;
	temp = token;
	*temp = '\0';
	//printf("here");
	if(!*prog)
		return; /* at end of expression */

	while(isspace(*prog))
		++prog; /* skip over the white space */

	if(strchr("<+-*/%^=()", *prog) ){
	  tok_type = DELIMITER;
	  /* advance to the next char */
	  *temp++ = *prog++;  
	
	} else if( isalpha(*prog) ){
	  while((!isdelim(*prog))&&(!isspace(*prog))  )
		*temp++ = *prog++;
		tok_type = VARIABLE;

	} else if( isdigit(*prog) ) {
	  while( (!isdelim(*prog))&&(!isspace(*prog)) )
		*temp++ = *prog++;
		tok_type = NUMBER;

	}

	*temp = '\0';
}//end
/***************************************************************************************************************************
 Return true if c is a delimiter
(This piece code is taken form the Text Book: C: The complte reference by Herbert Schildt(4th Edition) page no:593) 
****************************************************************************************************************************/
int isdelim(char c)
{
	if( strchr("<+-/*%^=()", c) || c==9 || c==0 || c=='\r' )
		return 1;
	else
		return 0;
}//end
/***************************************************************************************************************************/
