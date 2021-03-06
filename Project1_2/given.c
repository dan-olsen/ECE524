#include "given.h"
/***************************************************************************************************
Insert an element "X" at end of LIST "Cur", if "X" is not already in "Cur".
If the "Cur" is Null, it creates a single element List using "X"
***************************************************************************************************/
void InsertEle(LIST **Cur, int X)
{
    LIST *tl = (LIST *) malloc(sizeof(LIST));
    LIST *nl = NULL;

    if (tl == NULL)
    {
        printf("LIST: Out of memory\n");
        exit(1);

    } else {
        tl->Id = X;  tl->Next = NULL;

        if(*Cur == NULL)
        {
            *Cur = tl;
            return;
        }

        nl = *Cur;

        while(nl != NULL)
        {
            if(nl->Id == X)
            {
                break;
            }

            if(nl->Next == NULL)
            {
                nl->Next = tl;
            }

            nl = nl->Next;
        }
    }

    return;
}//end of InsertEle
/***************************************************************************************************
Delete an element "X" from LIST "Cur",
***************************************************************************************************/
void DeleteEle(LIST **Cur, int X)
{
    LIST *fir = (*Cur);
    LIST *lst = NULL;

    if (fir == NULL)
        return;

    while((fir->Id != X) && (fir != NULL))
    {
        lst = fir;
        fir = fir->Next;
    }

    if(fir != NULL)
    {
        if(lst == NULL)
        {
            (*Cur) = (*Cur)->Next;

        } else {
            lst->Next = fir->Next;
        }

        free(fir);
    } else {
        return;
    }

    return;
}//end of DeleteEle
/***************************************************************************************************************************
Return 1 if the element "x" is present in LIST "Cur"; Otherwise return  0
*****************************************************************************************************************************/
int FindEle(LIST *Cur, int X)
{
    LIST *temp = NULL;

    temp = Cur;
    while(temp != NULL)
    {
        if(temp->Id == X)
            return 1;

        temp = temp->Next;
    }
    return 0;
}//end of FindEle
/***************************************************************************************************
Print the elements in LIST "Cur"
***************************************************************************************************/
void PrintList(LIST *Cur)
{
    LIST *tmp = Cur;

    while(tmp != NULL)
    {
        printf("%d  ", tmp->Id);
        tmp = tmp->Next;
    }
    return;
}//end of PrintList
/***************************************************************************************************
Count the total number  of elements in LIST "Cur"
***************************************************************************************************/
int CountList(LIST *Cur)
{
    LIST *tmp = Cur;
    int size = 0;

    while(tmp != NULL)
    {
        size++;
        tmp = tmp->Next;
    }

    return size;
}//end of PrintList
/***************************************************************************************************
Free all elements in  LIST "Cur"
***************************************************************************************************/
void FreeList(LIST **Cur)
{
    LIST *tmp = NULL;

    if(*Cur == NULL)
    {
        return;
    }

    tmp = (*Cur);

    while((*Cur) != NULL)
    {
        tmp = tmp->Next;
        free(*Cur);
        (*Cur) = tmp;
    }

    (*Cur) = NULL;

    return;
}//end of FreeList
/***************************************************************************************************
Initialize the paricular member of GATE structure
***************************************************************************************************/
void InitiGat(GATE *Node, int Num)
{
    //Node[Num].Name = (char *) malloc(Mnam *sizeof(char));           //Dynamic memory allocation for an array
    bzero(Node[Num].Name, Mnam);                                   //Clearing the string
    Node[Num].Type = Node[Num].Nfi = Node[Num].Nfo = Node[Num].Mark = 0;
    Node[Num].TempVal = 6;
    Node[Num].Fin = Node[Num].Fot = NULL;
    Node[Num].TempRpath = Node[Num].TempFpath = NULL;
    Node[Num].TempPath = NULL;
    Node[Num].Nnt = NULL;
    Node[Num].RnT = NULL;
    Node[Num].Snt = NULL;
    Node[Num].Pnt = NULL;
    Node[Num].Delay = 0;
    Node[Num].PathCount = NULL;
    Node[Num].CosensitizationMark = Node[Num].CheckValidateMark = Node[Num].ValidatedMark = 0;
    Node[Num].RobustVal = Node[Num].NonRobustVal =  Node[Num].FuncVal = 0;

    return;
}//end of InitiGat
/***************************************************************************************************
Print all contents(attribute) of all active member of GATE structure(DdNodes are not printed)
***************************************************************************************************/
void PrintGats(GATE *Node, int Tgat)
{
    int i;

    printf("\nId\tName\tType\t#In\t#Out\tMark\tValue\t\tFanin\t\tFanout");

    for(i = 1;i <= Tgat; i++)
    {
        if(Node[i].Type != 0)
        {
            printf("\n%d\t%s\t%d\t%d\t%d\t%d\t%d\t\t", i, Node[i].Name, Node[i].Type, Node[i].Nfi, Node[i].Nfo, Node[i].Mark, Node[i].TempVal);
            PrintList(Node[i].Fin);  printf("\t\t");
            PrintList(Node[i].Fot);
        }
    }

    return;
}//end of PrintGats
/***************************************************************************************************
Free the memory of all contents of all members of GATE structure(DdNodes are already cleared)
***************************************************************************************************/
void ClearGat(GATE *Node, int Tgat)
{
    int i;

    for(i = 1; i <= Tgat; i++)
    {
        //free(Node[i].Name);
        Node[i].Type = Node[i].Nfi = Node[i].Nfo = Node[i].Mark = Node[i].TempVal = 0;
        FreeList(&Node[i].Fin);
        FreeList(&Node[i].Fot);
        FreePathCounts(&Node[i].PathCount);
    }

    return;
}//end of ClearGat
/***************************************************************************************************
Count the Total Number of Primary inputs and outputs
***************************************************************************************************/
void CountPri(GATE *Node, int Tgat, int *Npi, int *Npo)
{
    int i, j, k;

    i = j = k = 0;

    for(i = 1; i <= Tgat; i++)
    {
        if(Node[i].Type != 0)
        {
            if(Node[i].Nfi == 0)
            {
                j++;
            }
            if(Node[i].Nfo == 0)
            {
                k++;
            }
        }
    }

    *Npi = j;
    *Npo = k;

    return;
}//end of CountPri
/***************************************************************************************************
Convert (char *) type read to (int)
***************************************************************************************************/
int AssignType(char *Gtyp)
{
    if((strcmp(Gtyp, "inpt") == 0) || (strcmp(Gtyp, "INPT") == 0))
        return INPT;
    else if ((strcmp(Gtyp, "from") == 0) || (strcmp(Gtyp, "FROM") == 0))
        return FROM;
    else if ((strcmp(Gtyp, "buff") == 0) || (strcmp(Gtyp, "BUFF") == 0))
        return BUFF;
    else if ((strcmp(Gtyp, "not") == 0)  || (strcmp(Gtyp, "NOT") == 0))
        return NOT;
    else if ((strcmp(Gtyp, "and") == 0)  || (strcmp(Gtyp, "AND") == 0))
        return AND;
    else if ((strcmp(Gtyp, "nand") == 0) || (strcmp(Gtyp, "NAND") == 0))
        return NAND;
    else if ((strcmp(Gtyp, "or") == 0)   || (strcmp(Gtyp, "OR") == 0))
        return OR;
    else if ((strcmp(Gtyp, "nor") == 0)  || (strcmp(Gtyp, "NOR") == 0))
        return NOR;
    else if ((strcmp(Gtyp, "xor") == 0)  || (strcmp(Gtyp, "XOR") == 0))
        return XOR;
    else if ((strcmp(Gtyp, "xnor") == 0) || (strcmp(Gtyp, "XNOR") == 0))
        return XNOR;
    else
        return 0;

}//end of AssignType
/***************************************************************************************************
 Function to read the Bench Mark(.isc files)
***************************************************************************************************/
int ReadIsc(FILE *Isc, GATE *Node)
{
    char *noty, *from, *str1, *str2, *name, *line = NULL;
    int  i, id, fid, fin, fot, tot = 0;
    //Dynamic memory allocation for temporary strings
    noty = (char *) malloc(Mlin * sizeof(char));
    from = (char *) malloc(Mlin * sizeof(char));
    str1 = (char *) malloc(Mlin * sizeof(char));
    str2 = (char *) malloc(Mlin * sizeof(char));
    name = (char *) malloc(Mlin * sizeof(char));
    line = (char *) malloc(Mlin * sizeof(char));
    //Intialize all Gates in Node structure
    for(i = 0; i < Mnod; i++)
    {
        InitiGat(Node, i);
    }

    //Skip the comment lines
    do
      fgets(line, Mlin, Isc);
    while(line[0] == '*');

    //Read line by line
    while(!feof(Isc)){

        //Initialize temporary strings
        bzero(noty, strlen(noty));    bzero(from, strlen(from));
        bzero(str1, strlen(str1));    bzero(str2, strlen(str2));
        bzero(name, strlen(name));

        //Break line into data
        sscanf(line, "%d %s %s %s %s", &id, name, noty, str1, str2);

        //Update the tot using id
        if(id > tot)
        {
            tot = id;
        }

        //Fill in the name and type of a Gate
        strcpy(Node[id].Name, name);
        Node[id].Type = AssignType(noty);

        //Fill in fanin and fanout numbersof a Gate
        if(Node[id].Type != FROM)
        {
            fot = atoi(str1);
            fin = atoi(str2);
        } else {
            fin = fot = 1;
            strcpy(from, str1);
        }

        Node[id].Nfo = fot;
        Node[id].Nfi = fin;

        //Create fanin and fanout lists
        switch(Node[id].Type){
            case INPT  : break;
            case FROM  :
                for(i = tot; i > 0; i--)
                {
                    if(Node[i].Type != 0)
                    {
                        if(strcmp(Node[i].Name, from) == 0)
                        {
                            fid = i;
                            break;
                        }
                    }
                }

                InsertEle(&Node[id].Fin, fid);
                InsertEle(&Node[fid].Fot, id);
                break;
            case BUFF  :
            case NOT   :
            case AND   :
            case NAND  :
            case OR    :
            case NOR   :
            case XOR   :
            case XNOR  :
                for(i = 1; i <= fin; i++)
                {
                    fscanf(Isc, "%d", &fid);
                    InsertEle(&Node[id].Fin, fid);
                    InsertEle(&Node[fid].Fot, id);
                }

                fscanf(Isc, "\n");
                break;
            default:
                printf("ReadIsc: Error in input file (Node %d)\n", id);
                exit(1);
                break;
        }//end case

        //Clear the contents in the string "line" and get the next line in the file
        bzero(line, strlen(line));
        fgets(line, Mlin, Isc);
    }//end while

    free(noty);
    free(from);
    free(str1);
    free(str2);
    free(name);
    free(line);

    //Return the Maximum node of the Isc file
    return tot;
}//end of ReadIsc

void FreePathCounts(PATH_COUNT **Cur)
{
    PATH_COUNT *tmp = NULL;

    if(*Cur == NULL)
    {
        return;
    }

    tmp = (*Cur);

    while((*Cur) != NULL)
    {
        tmp = tmp->Next;
        free(*Cur);
        (*Cur) = tmp;
    }

    (*Cur) = NULL;

    return;
}
/****************************************************************************************************************************/
