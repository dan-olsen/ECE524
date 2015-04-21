#include "given.h"
#include "PatternSim.h"

const int robustSimOR [6][6]        =   {{S0, R1, F0, S1, X1, X0},
                                         {R1, X1, X1, S1, X1, X1},
                                         {F0, X1, F0, S1, X1, F0},
                                         {S1, S1, S1, S1, S1, S1},
                                         {X1, X1, X1, S1, X1, X1},
                                         {X0, X1, F0, S1, X1, X0}};

const int nonRobustSimOR [6][6]     =   {{S0, R1, F0, S1, X1, X0},
                                         {R1, X1, R1, S1, X1, R1},
                                         {F0, R1, F0, S1, X1, F0},
                                         {S1, S1, S1, S1, S1, S1},
                                         {X1, X1, X1, S1, X1, X1},
                                         {X0, R1, F0, S1, X1, X0}};

const int robustSimAND [6][6]       =   {{S0, S0, S0, S0, S0, S0},
                                         {S0, R1, X0, R1, R1, X0},
                                         {S0, X0, X0, F0, X0, X0},
                                         {S0, R1, F0, S1, X1, X0},
                                         {S0, R1, X0, X1, X1, X0},
                                         {S0, X0, X0, X0, X0, X0}};

const int nonRobustSimAND [6][6]    =   {{S0, S0, S0, S0, S0, S0},
                                         {S0, R1, F0, R1, R1, X0},
                                         {S0, F0, X0, F0, F0, X0},
                                         {S0, R1, F0, S1, X1, X0},
                                         {S0, R1, F0, X1, X1, X0},
                                         {S0, X0, X0, X0, X0, X0}};

const int simNOT [6]                =   {S1, F0, R1, S0, X0, X1};

int* getNextPattern(FILE **patFile, int Npi)
{
    int patIndex;
    char vector1 [Mpi];
    char vector2  [Mpi];
    int readCount1, readCount2;
    int *pattern;

    if ( NULL == (pattern =  (int*)malloc(Npi * sizeof(int)))) {
        printf("malloc failed\n");

        return NULL;
        //error
    }

    if(!feof(*patFile))
    {
        readCount1 = fscanf(*patFile, "%s", vector1);
        readCount2 = fscanf(*patFile, "%s", vector2);

        if((readCount2 != -1) && (readCount1 != -1) && (readCount1 == readCount2))
        {
            for(patIndex = 0; patIndex < Npi; patIndex++)
            {
                if((vector1[patIndex] == '0') && (vector2[patIndex] == '0'))
                {
                    pattern[patIndex] = S0;

                } else if((vector1[patIndex] == '0') && (vector2[patIndex] == '1')) {
                    pattern[patIndex] = R1;

                } else if((vector1[patIndex] == '1') && (vector2[patIndex] == '0')) {
                    pattern[patIndex] = F0;

                } else if((vector1[patIndex] == '1') && (vector2[patIndex] == '1')) {
                    pattern[patIndex] = S1;

                } else if(((vector1[patIndex] == 'x') || (vector1[patIndex] == 'X')) && (vector2[patIndex] == '1')) {
                    pattern[patIndex] = X1;

                } else if(((vector1[patIndex] == 'x') || (vector1[patIndex] == 'X')) && (vector2[patIndex] == '0')) {
                    pattern[patIndex] = X0;

                } else {
                    //error
                    continue;

                }
            }

            //printf("Resulting Pattern: ");
            //printPattern(pattern, Npi);
            //printf("\n");

        } else {
            printf("End of Pattern File\n");
            free(pattern);
            return NULL;
        }
    } else {
        printf("End of Pattern File\n");
        free(pattern);
        return NULL;
    }

    return pattern;
}

void applyPatternRobust(GATE *Node, int *pattern, int Tgat)
{
    LIST *tmpList = NULL;
    int tmpRVal = 0, tmpRVal2 = 0, i;
    int patIndex;

    for(i = 1, patIndex = 0; i <= Tgat; i++, tmpRVal = 0, tmpRVal2 = 0)
    {
        switch(Node[i].Type) {
            case INPT:
                Node[i].Val = pattern[patIndex];

                patIndex++;

                break;
            case AND:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;

                        continue;
                    }

                    tmpRVal = robustSimAND[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = tmpRVal;

                break;
            case NAND:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;

                        continue;
                    }

                    tmpRVal = robustSimAND[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = simNOT[tmpRVal];

                break;
            case OR:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;

                        continue;
                    }

                    tmpRVal = robustSimOR[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = tmpRVal;

                break;
            case NOR:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;

                        continue;
                    }

                    tmpRVal = robustSimOR[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = simNOT[tmpRVal];

                break;
            case XOR:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;
                        tmpRVal2 = simNOT[Node[tmpList->Id].Val];

                        continue;
                    }

                    tmpRVal2 = robustSimOR[tmpRVal2][simNOT[Node[tmpList->Id].Val]];
                    tmpRVal = robustSimOR[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = robustSimAND[tmpRVal][tmpRVal2];

                break;
            case XNOR:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;
                        tmpRVal2 = simNOT[Node[tmpList->Id].Val];

                        continue;
                    }

                    tmpRVal2 = robustSimAND[tmpRVal2][simNOT[Node[tmpList->Id].Val]];
                    tmpRVal = robustSimAND[tmpRVal][Node[tmpList->Id].Val];

                }

                Node[i].Val = robustSimOR[tmpRVal][tmpRVal2];

                break;
            case BUFF:
                tmpList = Node[i].Fin;

                Node[i].Val = Node[tmpList->Id].Val;

                break;
            case NOT:
                tmpList = Node[i].Fin;

                Node[i].Val = simNOT[Node[tmpList->Id].Val];

                break;
            case FROM:
                tmpList = Node[i].Fin;

                Node[i].Val = Node[tmpList->Id].Val;

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }

        //set mark
        if((Node[i].Val == R1) || (Node[i].Val == F0))
        {
            Node[i].Mark = 1;

        } else {
            Node[i].Mark = 0;

        }
    }
}

void applyPatternNonRobust(GATE *Node, int *pattern, int Tgat)
{
    LIST *tmpList = NULL;
    int tmpRVal = 0, tmpRVal2 = 0, i;
    int patIndex;

    for(i = 1, patIndex = 0; i <= Tgat; i++, tmpRVal = 0, tmpRVal2 = 0)
    {
        switch(Node[i].Type) {
            case INPT:
                Node[i].Val = pattern[patIndex];

                patIndex++;

                break;
            case AND:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;

                        continue;
                    }

                    tmpRVal = nonRobustSimAND[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = tmpRVal;

                break;
            case NAND:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;

                        continue;
                    }

                    tmpRVal = nonRobustSimAND[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = simNOT[tmpRVal];

                break;
            case OR:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;

                        continue;
                    }

                    tmpRVal = nonRobustSimOR[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = tmpRVal;

                break;
            case NOR:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;

                        continue;
                    }

                    tmpRVal = nonRobustSimOR[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = simNOT[tmpRVal];

                break;
            case XOR:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;
                        tmpRVal2 = simNOT[Node[tmpList->Id].Val];

                        continue;
                    }

                    tmpRVal2 = nonRobustSimOR[tmpRVal2][simNOT[Node[tmpList->Id].Val]];
                    tmpRVal = nonRobustSimOR[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = robustSimAND[tmpRVal][tmpRVal2];

                break;
            case XNOR:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpRVal = Node[tmpList->Id].Val;
                        tmpRVal2 = simNOT[Node[tmpList->Id].Val];

                        continue;
                    }

                    tmpRVal2 = nonRobustSimAND[tmpRVal2][simNOT[Node[tmpList->Id].Val]];
                    tmpRVal = nonRobustSimAND[tmpRVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = nonRobustSimOR[tmpRVal][tmpRVal2];

                break;
            case BUFF:
                tmpList = Node[i].Fin;

                Node[i].Val = Node[tmpList->Id].Val;

                break;
            case NOT:
                tmpList = Node[i].Fin;

                Node[i].Val = simNOT[Node[tmpList->Id].Val];

                break;
            case FROM:
                tmpList = Node[i].Fin;

                Node[i].Val = Node[tmpList->Id].Val;

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }

        //set mark
        if((Node[i].Val == R1) || (Node[i].Val == F0))
        {
            Node[i].Mark = 1;

        } else {
            Node[i].Mark = 0;

        }
    }
}

void printPattern(int *pattern, int Npi)
{
    int i;

    for(i = 0; i < Npi; i++)
    {
        printf("%d", pattern[i]);
    }

    printf("\n");
}
