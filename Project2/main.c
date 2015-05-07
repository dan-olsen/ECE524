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

#define SOURCE 0
#define MULT 1
#define ALU 2
#define SINK 3

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

} Node;

Node graph[100];
char token[80];
char tok_type;
char *prog;

/***************************************************************************************************************************
Function Declarations 
****************************************************************************************************************************/
void get_token(void);
int isdelim(char);
void TimeConstraints(FILE *resfile, int *c, int min_asap, int Mnode);
void MultOperations(FILE *resFile, int *c, int min_asap, int Mnode);
void AluOperations(FILE *resFile, int *c, int min_asap, int Mnode);
void MultipleTimeFrames(FILE *resFile, int min_asap, int Mnode);

/***************************************************************************************************************************
  Main Function
****************************************************************************************************************************/

int main(int argc,char **argv)
{
	FILE *infile, *resfile;

    int i = 0, counter, Mnode, Mnode_id = 0;
    int mult_count = 0, alu_count = 0, oper_count = 0;
    int Nasap, Fasap, C = 1, min_asap, flag, sameasap = 1;

    char resfileName[100], buf[100], LpCommand[100];

	infile = fopen(argv[1], "r");

    while(fgets(buf, MAX_LINE, infile) != '\0')
    {
        prog = buf;
        counter = 0;

        for(counter = 0; counter < 4; )
        {
            get_token();
            switch(tok_type) {
                case NUMBER:
                    switch(counter) {
                        case 0:
                            graph[i].nd_num = atoi(token);

                            if (Mnode_id < graph[i].nd_num)
                                Mnode_id = graph[i].nd_num;

                            break;
                        case 1:
                            graph[i].refe = atoi(token);

                            break;
                        case 2:
                            graph[i].asap = atoi(token);

                            if (i == 0)
                                min_asap = graph[i].asap;
                            else if(graph[i].asap < min_asap)
                                min_asap = graph[i].asap;

                            break;
                        case 3:
                            graph[i].alap = atoi(token);

                            break;
                        default:

                            break;
                    }

                    counter++;

                    break;
                case VARIABLE:
                    if(counter < 2)
                    {
                        if(!strcmp(token, "source"))
                            graph[i].oper = SOURCE;

                        if(!strcmp(token, "sink"))
                            graph[i].oper = SINK;
                    }

                    break;
                case DELIMITER:
                    if(counter > 0 && counter < 2)
                    {
                        if(strchr("*", *token))
                        {
                            graph[i].oper = MULT;
                            mult_count++;
                        }

                        if(strchr("+-/%^=()<>", *token))
                        {
                            graph[i].oper = ALU;
                            alu_count++;
                        }
                    }

                    break;
                default:
                    break;
            }
        }

        Mnode = i;
        i++;
    }

    sprintf(resfileName, "%s_equations.lp", argv[1]);

	resfile = fopen (resfileName, "w");

	// 1. Read the benchmark files(.asapalap) and store it in your own defined data structure 
	// 2. Form the linear equations using the information in the data structure and print in a file(ilpfile) 
	// 3. Use lp solver to solve the inequalities and store the output of lpsolver in file.
		  //lpsover command: lp_solve "ilpfile" 
	// 4. The two functions(get_token,isdelim) can be used for reading the benchmark files(.asapalap). 
          //Using these two functions in your implementation is optional.

    printf("min: a1 + a2;\n");
    fprintf(resfile, "min: a1 + a2;\n");

	TimeConstraints(resfile, &C, 2, Mnode);

    printf("\n");
    fprintf(resfile, "\n");

    MultOperations(resfile, &C, 2, Mnode);

    printf("\n");
    fprintf(resfile, "\n");

    AluOperations(resfile, &C, 2, Mnode);

    printf("\n");
    fprintf(resfile, "\n");

    MultipleTimeFrames(resfile, 2, Mnode);

	fclose(infile);
    fclose(resfile);

    sprintf(LpCommand, "lp_solve %s > %s.output", resfileName, resfileName);

    system(LpCommand);

    return 0;
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

    if(!*prog)
		return; /* at end of expression */

	while(isspace(*prog))
		++prog; /* skip over the white space */

    if(strchr("<+-*/%^=()", *prog))
    {
        tok_type = DELIMITER;
        /* advance to the next char */
        *temp++ = *prog++;
	
    } else if(isalpha(*prog)) {
        while((!isdelim(*prog)) && (!isspace(*prog)))
            *temp++ = *prog++;

        tok_type = VARIABLE;

    } else if(isdigit(*prog)) {
        while((!isdelim(*prog)) && (!isspace(*prog)))
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
    if(strchr("<+-/*%^=()", c) || c == 9 || c == 0 || c == '\r')
		return 1;
	else
		return 0;
}//end

void TimeConstraints(FILE *resfile, int *c, int min_asap, int Mnode)
{
    int i, j;

    for(i = 0; i < Mnode; i++, (*c)++)
    {
        if(graph[i].alap == graph[i].asap)
        {
            printf("c%d: X%d%d = 1;\n", *c, (int)graph[i].nd_num, graph[i].asap - min_asap);

            fprintf(resfile, "c%d: X%d%d = 1;\n", *c, (int)graph[i].nd_num, graph[i].asap - min_asap);
        } else {
			printf("c%d: ", *c);

			fprintf(resfile, "c%d: ", *c);

        	for(j = graph[i].asap - min_asap; j <= (graph[i].alap - min_asap); j++)
        	{
        		if(j == (graph[i].alap - min_asap))
        		{
        			printf("X%d%d", (int)graph[i].nd_num, j);

        			fprintf(resfile, "X%d%d", (int)graph[i].nd_num, j);
        		} else {
        			printf("X%d%d + ", (int)graph[i].nd_num, j);

        			fprintf(resfile, "X%d%d + ", (int)graph[i].nd_num, j);
        		}
        	}

        	printf(" = 1;\n");
        	fprintf(resfile, "= 1;\n");
        }
    }
}

void MultOperations(FILE *resFile, int *c, int min_asap, int Mnode)
{
	int i, j, Mmult, Mmult_at_time_frame = 0, k;

	for(i = 0; i < Mnode; i++)
	{
		if(graph[i].oper == MULT)
		{
			Mmult = graph[i].alap - min_asap;
		}
	}

	for(j = 1; j <= Mmult; j++, (*c)++, Mmult_at_time_frame = 0)
	{
		for(i = 0; i < Mnode; i++)
		{
			if(graph[i].oper == MULT && j >= (graph[i].asap - min_asap) && j <= (graph[i].alap - min_asap))
			{
				Mmult_at_time_frame++;
			}
		}

		if(Mmult_at_time_frame == 0)
			continue;

        printf("c%d: ", *c);

        fprintf(resFile, "c%d: ", *c);

		for(i = 0, k = 1; k <= Mmult_at_time_frame; i++)
		{
			if(graph[i].oper == MULT)
			{
		        if(j >= (graph[i].asap - min_asap) && j <= (graph[i].alap - min_asap))
		        {
		        	if(k == Mmult_at_time_frame)
		        	{
						printf("X%d%d ", (int)graph[i].nd_num, j);

						fprintf(resFile, "X%d%d ", (int)graph[i].nd_num, j);
		        	} else {
						printf("X%d%d + ", (int)graph[i].nd_num, j);

						fprintf(resFile, "X%d%d + ", (int)graph[i].nd_num, j);
		        	}

					k++;
		        }
			}
		}

		printf("- a1 <= 0;\n");

		fprintf(resFile, "- a1 <= 0;\n");
	}
}

void AluOperations(FILE *resFile, int *c, int min_asap, int Mnode)
{
	int i, j, Malu, Malu_at_time_frame = 0, k;

	for(i = 0; i < Mnode; i++)
	{
		if(graph[i].oper == ALU)
		{
			Malu = graph[i].alap - min_asap;
		}
	}

	for(j = 1; j <= Malu; j++, (*c)++, Malu_at_time_frame = 0)
	{
		for(i = 0; i < Mnode; i++)
		{
			if(graph[i].oper == ALU && j >= (graph[i].asap - min_asap) && j <= (graph[i].alap - min_asap))
			{
				Malu_at_time_frame++;
			}
		}

		if(Malu_at_time_frame == 0)
			continue;

        printf("c%d: ", *c);

        fprintf(resFile, "c%d: ", *c);

		for(i = 0, k = 1; k <= Malu_at_time_frame; i++)
		{
			if(graph[i].oper == ALU)
			{
		        if(j >= (graph[i].asap - min_asap) && j <= (graph[i].alap - min_asap))
		        {
		        	if(k == Malu_at_time_frame)
		        	{
						printf("X%d%d ", (int)graph[i].nd_num, j);

						fprintf(resFile, "X%d%d ", (int)graph[i].nd_num, j);
		        	} else {
						printf("X%d%d + ", (int)graph[i].nd_num, j);

						fprintf(resFile, "X%d%d + ", (int)graph[i].nd_num, j);
		        	}

		        	k++;
		        }
			}
		}

		printf("- a2 <= 0;\n");

		fprintf(resFile, "- a2 <= 0;\n");
	}
}

void MultipleTimeFrames(FILE *resFile, int min_asap, int Mnode)
{
    int i, j, Mmult_time_frames, k;

	printf("int a1, a2, ");

	fprintf(resFile, "int a1, a2, ");

    for(i = 0, Mmult_time_frames = 0; i < Mnode; i++)
    {
        if(graph[i].alap != graph[i].asap)
        {
        	Mmult_time_frames++;
        }
    }

    for(i = 0, k = 1; k <= Mmult_time_frames; i++)
    {
        if(graph[i].alap != graph[i].asap)
        {
        	for(j = graph[i].asap - min_asap; j <= (graph[i].alap - min_asap); j++)
        	{
        		if(k == Mmult_time_frames && j == (graph[i].alap - min_asap))
        		{
        			printf("X%d%d", (int)graph[i].nd_num, j);

        			fprintf(resFile, "X%d%d", (int)graph[i].nd_num, j);
        		} else {
        			printf("X%d%d, ", (int)graph[i].nd_num, j);

        			fprintf(resFile, "X%d%d, ", (int)graph[i].nd_num, j);
        		}
        	}

        	k++;
        }
    }

	printf(";\n");
	fprintf(resFile, ";\n");
}
/***************************************************************************************************************************/
