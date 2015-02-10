#include "given.h"
/***************************************************************************************************************************
Insert an element "x" at end of LIST "l", if it is not already there. 
****************************************************************************************************************************/
void InsertList(LIST **l, int x)
{
	LIST *p, *tl;

	if ((p = (LIST *) malloc(sizeof(LIST))) == NULL)
	{    
		printf("LIST: Out of memory\n");
		exit(1);

	} else {
		p->id = x;  
		p->nxt = NULL;
	  	if(*l == NULL) 
		{
			*l = p;
			return;

		}
	
		tl=NULL; 
		tl = *l;
	
		while(tl != NULL)
		{
			if(tl->id == x)
				break;
			if(tl->nxt == NULL)
				tl->nxt = p;

			tl = tl->nxt;
		}
	}

	return;
}//end of InsertList
/****************************************************************************************************************************
Form the LIST "l" from an array "ele". Make "l" equal to null before calling this function
*****************************************************************************************************************************/
void FormList(int *ele, int size, LIST **l)
{
	int a = 0;
	for(a = 0; a < size; a++)
	{  
		//printf("%d:%d  ",a,ele[a]);
		InsertList(l, ele[a]);
	}

	return;
}// end of FormList
/***************************************************************************************************************************
Print the elements in LIST "l"          
***************************************************************************************************************************/
void PrintList(LIST *l)
{
	LIST *temp=NULL;

	temp=l;
	while(temp!=NULL){   
	  printf("%d ", temp->id);
	  temp = temp->nxt; } 
	return;
}//end of PrintList
/***************************************************************************************************************************
Count total number of elements in LIST "l"
***************************************************************************************************************************/
int ListSize(LIST *l)
{
	LIST *temp=NULL;
	int count=0;

	temp=l;
	while(temp!=NULL){  
	  count++;
	  temp=temp->nxt; }
	return count;
}//end of ListSize
/***************************************************************************************************************************
Return 1 if the element "x" is present in LIST "l"; Otherwise return  0  
*****************************************************************************************************************************/
int InList(LIST *l,int x)
{
	LIST *temp=NULL;
	
	temp=l;
	while(temp!=NULL){
	  if(temp->id==x) return 1;
	  temp=temp->nxt; }
	return 0;
}//end of InList	   
/***************************************************************************************************************************
Delete an element  "x" from LIST 'l'  
***************************************************************************************************************************/
void DeleteNoList(LIST **l,int x)
{
	LIST *curr,*prv;

	curr=prv=NULL;
	curr=(*l);
	if(curr == NULL){ return; } 

	while((curr->id!=x)&&(curr!=NULL)){   prv = curr;  curr = curr->nxt; }   
	if(curr!=NULL){
	  if(prv==NULL){  (*l)=(*l)->nxt;      }
	  else         {  prv->nxt=curr->nxt;  }
	  free(curr);                }
	else              { return;  } //nd is not contained in l
	return;
}//end of DeleteNoList
/***************************************************************************************************************************
Copy the contents of LIST "l1" to LIST "l2". Make "l2" equal to null before calling this function
***************************************************************************************************************************/
void CopyList(LIST *l1,LIST **l2)
{
	LIST *t1=NULL; 

	t1=l1;
	while(t1!=NULL){   
	  InsertList(l2,t1->id);
	  t1=t1->nxt; }  
	return;
}//end of CopyLIST
/***************************************************************************************************************************
Free all elements in  LIST "l"  
****************************************************************************************************************************/
void FreeList(LIST **l)
{
	LIST *temp=NULL;

	if(*l==NULL){   return;  }
	temp=(*l);
	while((*l) != NULL){
	  temp=temp->nxt;
	  free(*l); (*l)=temp; }   
	(*l)=NULL;
	return;
}//end of FreeList 	
/*****************************************************************************************************************************
 Routine to read the Bench Mark(.isc files)
*****************************************************************************************************************************/
int ReadIsc(FILE *fisc, NODE *graph)
{
	char c, noty[Mlin], from[Mlin], str1[Mlin], str2[Mlin], name[Mlin], line[Mlin];
	int  i,id,fid,fin,fout,mid=0;

	//dynamic memory allocation of temporary strings
	//noty=(char *) malloc(Mlin * sizeof(char));    from=(char *) malloc(Mlin * sizeof(char));  
	//str1=(char *) malloc(Mlin * sizeof(char));    str2=(char *) malloc(Mlin * sizeof(char));  
	//name=(char *) malloc(Mlin * sizeof(char));    line=(char *) malloc(Mlin * sizeof(char));
	 
	// intialize all nodes in graph structure
	for(i=0;i<Mnod;i++){ InitializeCircuit(graph,i); } 

	//skip the comment lines 
	do
	fgets(line,Mlin,fisc);
	while(line[0] == '*');

	// read line by line 
	while(!feof(fisc)){

	  //initialize temporary strings 
	  bzero(noty,strlen(noty));    bzero(from,strlen(from));
	  bzero(str1,strlen(str1));    bzero(str2,strlen(str2));   
	  bzero(name,strlen(name));

	  //break line into data 
	  sscanf(line, "%d %s %s %s %s",&id,name,noty,str1,str2); 

	  //fill in the type
	  strcpy(graph[id].nam,name);
	  graph[id].typ=AssignType(noty);

	  //fill in fanin and fanout numbers
	  if(graph[id].typ!=FROM) {   fout= atoi(str1);  fin=atoi(str2); }    
	  else{                        fin=fout= 1; strcpy(from,str1);    } 
	  
	  if(id > mid){ mid=id;  }

	  graph[id].nfo=fout;  graph[id].nfi=fin;
	  if(fout==0){  graph[id].po=1; }

	  //create fanin and fanout lists   		  
	  switch (graph[id].typ)  {
	    case 0     : printf("ReadIsc: Error in input file (Node %d)\n",id); exit(1);
	    case INPT  : break;
	    case AND   :
	    case NAND  :
	    case OR    :
	    case NOR   :
	    case XOR   :
	    case XNOR  :
	    case BUFF  :
	    case NOT   : for(i=1;i<=fin;i++) {
			   fscanf(fisc, "%d", &fid);
		           InsertList(&graph[id].fin,fid);  
		           InsertList(&graph[fid].fot,id); }  
			   fscanf(fisc,"\n");  break; 		   	        	   
	    case FROM  : for(i=mid;i>0;i--){
	 	   	   if(graph[i].typ!=0){
		             if(strcmp(graph[i].nam,from)==0){  fid=i; break; } } }
		          InsertList(&graph[id].fin,fid);
		          InsertList(&graph[fid].fot,id);   break; 
	    } //end case 
	  bzero(line,strlen(line)); 
	  fgets(line,Mlin,fisc);  
	} // end while 
	//free(noty);  free(from);  free(str1);  free(str2);  free(name);  free(line);
	return mid;
}//end of ReadIsc 
/****************************************************************************************************************************
Initialize the paricular memeber of graph structure
****************************************************************************************************************************/           
void InitializeCircuit(NODE *graph,int num)
{
	//graph[num].nam=(char *) malloc(Mnam *sizeof(char)); // dynamic memory allocation for an array
	bzero(graph[num].nam,Mnam);
	graph[num].typ=graph[num].nfi=graph[num].nfo=graph[num].mar=graph[num].po=0; 
	graph[num].fin=graph[num].fot=NULL;   
	graph[num].fun=NULL;
	graph[num].bpb=graph[num].npb=0.0;
	return;
}//end of InitializeCircuit 
/****************************************************************************************************************************
Convert (char *) type read to (int)     
****************************************************************************************************************************/
int AssignType(char *typ)
{
	if      ((strcmp(typ,"inpt")==0) || (strcmp(typ,"INPT")==0))       return 1;
	else if ((strcmp(typ,"and")==0)  || (strcmp(typ,"AND")==0))        return 2;
	else if ((strcmp(typ,"nand")==0) || (strcmp(typ,"NAND")==0))       return 3;
	else if ((strcmp(typ,"or")==0)   || (strcmp(typ,"OR")==0))         return 4;
	else if ((strcmp(typ,"nor")==0)  || (strcmp(typ,"NOR")==0))        return 5;
	else if ((strcmp(typ,"xor")==0)  || (strcmp(typ,"XOR")==0))        return 6;
	else if ((strcmp(typ,"xnor")==0) || (strcmp(typ,"XNOR")==0))       return 7;
	else if ((strcmp(typ,"buff")==0) || (strcmp(typ,"BUFF")==0))       return 8;
	else if ((strcmp(typ,"not")==0)  || (strcmp(typ,"NOT")==0))        return 9;
	else if ((strcmp(typ,"from")==0) || (strcmp(typ,"FROM")==0))       return 10;
	else                          			                   return 0;
}//end of AssignType
/****************************************************************************************************************************
Print all members of graph structure(except typ=0) after reading the bench file(bdd, fbdd and xrbdd  element are not formed)
*****************************************************************************************************************************/
void PrintCircuit(NODE *graph,int Max)
{
	LIST *temp;
	int  i;
	printf("\nID\tNAME\tTYPE\tPO\tIN#\tOUT#\tBPB\t\tNBP\t\tFANIN\t\tFANOUT");
	for(i=0;i<=Max;i++){
	  if(graph[i].typ!=0){
	    printf("\n%d\t%s\t%d\t%d\t%d\t%d\t%f\t%f\t",i,graph[i].nam,graph[i].typ,graph[i].po,graph[i].nfi,graph[i].nfo,graph[i].bpb,graph[i].npb);
	    temp=NULL;  temp=graph[i].fin;   if(temp!=NULL){  PrintList(temp); } printf("\t\t");
	    temp=NULL;  temp=graph[i].fot;   if(temp!=NULL){  PrintList(temp); }
	    //printf("\nBDD\n"); Cudd_PrintMinterm(manager,graph[i].fun); 
	}}
	printf("\n");
	return;
}//end of PrintCircuit
/****************************************************************************************************************************
Free the memory of all member of graph structure(bdd,fbdd and xrbdd should be dereferenced earlier)
*****************************************************************************************************************************/
void ClearCircuit(NODE *graph)
{
	int num=0;
	for(num=0;num<Mnod;num++){
	  //free(graph[num].nam);
	  if(graph[num].typ!=0){     
	    graph[num].typ=graph[num].nfi=graph[num].nfo=graph[num].mar=graph[num].po=0;    
	    if(graph[num].fin!=NULL){   FreeList(&graph[num].fin);  graph[num].fin = NULL;     } 
	    if(graph[num].fot!=NULL){   FreeList(&graph[num].fot);  graph[num].fot = NULL;     } 
	    graph[num].fun=NULL; graph[num].bpb=graph[num].npb=0.0;} }
	return;
}//end of ClearCircuit
/*****************************************************************************************************************************
Demo Function which uses all the LIST structure and File Operation Functions(This function has no relation to Project)
*****************************************************************************************************************************/
void DemoList(char *filename)
{
	int a,b,c,d;      //random variables 
	int ele[Mlin];         //int array  
	char str[Mlin];        //char array
	FILE *fp;         //file pointer  
	LIST *tmp,*tnp;   //random LISTs   

	//ele=(int *) malloc(Mlin * sizeof(int));             //dynamic memory allocation 
	//str=(char *) malloc(Mlin * sizeof(char));           //dynamic memory allocation 
	 
	a=b=c=d=0;                   //intializing the temporary variables 

	//open  the demo.txt file and read line by line and store in element array
	fp=fopen(filename,"r");     //file pointer to open demo.txt file 
	while(!feof(fp)){    
	  bzero(str,Mlin);             //initialing the string   
	  fgets(str,Mlin,fp);          //reading a single line
	  if(*str!='\0'){ 
	    ele[a]= atoi(str);       //convert the srting into integer and store it in ele array
	    a++;  } }
	fclose(fp);                  //close file pointer for demo.txt file

	//print the elements in ele array
	printf("\nContents in ele Array"); 
	for(b=0;b<a;b++){ printf("\n%d: %d",b,ele[b]); }

	//form the LIST tmp using the array ele 
	tmp=NULL; FormList(ele,a,&tmp);

	//print the elements in the LIST
	printf("\nContents in LIST tmp\n"); 
	PrintList(tmp);

	//insert a new element 8 to tmp
	InsertList(&tmp,8);
	printf("\nContents in LIST tmp after adding 8\n"); 
	PrintList(tmp);

	//count the number of Contents in tmp
	c=0; c=ListSize(tmp);
	printf("\nTotal No of Contents in LIST tmp: %d",c);

	//check a "2" is present in tmp or not
	d=-1; d=InList(tmp,2);
	if(d==1){ printf("\n2 is present in tmp");     }
	else    { printf("\n2 is not present in tmp"); }

	//delete "3" from tmp
	DeleteNoList(&tmp,3);
	printf("\nContents in LIST tmp after deleting 3\n"); 
	PrintList(tmp);

	//check now "3" is present in tmp or not
	d=-1; d=InList(tmp,3);
	if(d==1){ printf("\n3 is present in tmp");     }
	else    { printf("\n3 is not present in tmp"); }

	//copy the conts of tmp to tnp
	tnp=NULL;  CopyList(tmp,&tnp);
	printf("\nContents in LIST tnp\n"); 
	PrintList(tnp);
	printf("\n");

	//clear the contents and free the memory of tmp and tnp
	FreeList(&tmp); FreeList(&tnp);

	//memory deallocation for dynamic arrays
	//free(str); free(ele);
	return;
}//end of DemoList
/*****************************************************************************************************************************
Demo function to show how to Create a BDD for a Function
*****************************************************************************************************************************/
void DemoBdd(DdManager *manager)
{
	DdNode *a,*b,*c,*d;              //input variable bdds
	DdNode *tmp1,*tmp2,*fun1;        //function bdds

	printf("BDD Demo Start\n"); 
	manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0); //intializing CUDD package manger

	//create input variable bdds
	a=Cudd_bddIthVar(manager,0);  Cudd_Ref(a); printf("BDD a \n"); Cudd_PrintMinterm(manager,a);
	b=Cudd_bddIthVar(manager,1);  Cudd_Ref(b); printf("BDD b \n"); Cudd_PrintMinterm(manager,b);
	c=Cudd_bddIthVar(manager,2);  Cudd_Ref(c); printf("BDD c \n"); Cudd_PrintMinterm(manager,c);
	d=Cudd_bddIthVar(manager,3);  Cudd_Ref(d); printf("BDD d \n"); Cudd_PrintMinterm(manager,d);

	//create bdd fun1= abc+ac(dbar)  tmp1,tmp2 are used for temporary storage
	fun1=Cudd_ReadLogicZero(manager);              Cudd_Ref(fun1);                                        //fun1=logic 0;
	//cerate tmp1=abc
	tmp1=Cudd_ReadOne(manager);                    Cudd_Ref(tmp1);                                       //tmp1=constant 1
	tmp2=Cudd_bddAnd(manager,tmp1,a);              Cudd_Ref(tmp2);   Cudd_RecursiveDeref(manager,tmp1);  //tmp2=a
	tmp1=tmp2;                                     Cudd_Ref(tmp1);   Cudd_RecursiveDeref(manager,tmp2);  //tmp1=a 
	tmp2=Cudd_bddAnd(manager,tmp1,b);              Cudd_Ref(tmp2);   Cudd_RecursiveDeref(manager,tmp1);  //tmp2=ab
	tmp1=tmp2;                                     Cudd_Ref(tmp1);   Cudd_RecursiveDeref(manager,tmp2);  //tmp1=ab
	tmp2=Cudd_bddAnd(manager,tmp1,c);              Cudd_Ref(tmp2);   Cudd_RecursiveDeref(manager,tmp1);  //tmp2=abc
	tmp1=tmp2;                                     Cudd_Ref(tmp1);   Cudd_RecursiveDeref(manager,tmp2);  //tmp1=abc 

	printf("BDD tmp1=abc \n"); Cudd_PrintMinterm(manager,tmp1);
	printf("1.BDD tmp1 %.2f minterms\n", Cudd_CountMinterm(manager,tmp1, Cudd_SupportSize(manager,tmp1)));
	printf("2.BDD tmp1 %.2f minterms\n", Cudd_CountMinterm(manager,tmp1, 4));

	tmp2=Cudd_bddOr(manager,tmp1,fun1);            Cudd_Ref(tmp2);  Cudd_RecursiveDeref(manager,tmp1);  //tmp2=abc
	Cudd_RecursiveDeref(manager,fun1);  
	fun1=tmp2;                                     Cudd_Ref(fun1);  Cudd_RecursiveDeref(manager,tmp2);  //fun1=abc 

	//cerate tmp1=ac(dbar)
	tmp1=Cudd_ReadOne(manager);                    Cudd_Ref(tmp1);                                       //tmp1=constant 1
	tmp2=Cudd_bddAnd(manager,tmp1,a);              Cudd_Ref(tmp2);   Cudd_RecursiveDeref(manager,tmp1);  //tmp2=a
	tmp1=tmp2;                                     Cudd_Ref(tmp1);   Cudd_RecursiveDeref(manager,tmp2);  //tmp1=a 
	tmp2=Cudd_bddAnd(manager,tmp1,c);              Cudd_Ref(tmp2);   Cudd_RecursiveDeref(manager,tmp1);  //tmp2=ac
	tmp1=tmp2;                                     Cudd_Ref(tmp1);   Cudd_RecursiveDeref(manager,tmp2);  //tmp1=ac
	tmp2=Cudd_bddAnd(manager,tmp1,Cudd_Not(d));    Cudd_Ref(tmp2);   Cudd_RecursiveDeref(manager,tmp1);  //tmp2=ac(dbar)
	tmp1=tmp2;                           Cudd_Ref(tmp1);   Cudd_RecursiveDeref(manager,tmp2);            //tmp1=ac(dbar) 

	printf("BDD tmp1=ac(dbar)\n"); Cudd_PrintMinterm(manager,tmp1);
	printf("1.BDD tmp1 %.2f minterms\n", Cudd_CountMinterm(manager,tmp1, Cudd_SupportSize(manager,tmp1)));
	printf("2.BDD tmp1 %.2f minterms\n", Cudd_CountMinterm(manager,tmp1, 4));

	tmp2=Cudd_bddOr(manager,tmp1,fun1);  Cudd_Ref(tmp2);  Cudd_RecursiveDeref(manager,tmp1);  //tmp2=abc+ac(dbar)
	Cudd_RecursiveDeref(manager,fun1);  
	fun1=tmp2;                           Cudd_Ref(fun1);  Cudd_RecursiveDeref(manager,tmp2);  //fun1=abc+ac(dbar)

	printf("BDD fun1= abc+ac(dbar)\n");  Cudd_PrintMinterm(manager,fun1);
	printf("1.BDD fun1 %.2f minterms\n", Cudd_CountMinterm(manager,fun1, Cudd_SupportSize(manager,tmp1)));
	printf("2.BDD fun1 %.2f minterms\n", Cudd_CountMinterm(manager,fun1, 4));

	//dereferencing all the created bdds
	Cudd_RecursiveDeref(manager,fun1);
	Cudd_RecursiveDeref(manager,a);  Cudd_RecursiveDeref(manager,b);
	Cudd_RecursiveDeref(manager,c);  Cudd_RecursiveDeref(manager,d);

	printf("No of Unreferenced Bdds %d\n", Cudd_CheckZeroRef(manager));

	Cudd_Quit(manager);   //closing the cudd package manager
	printf("BDD Demo End\n");  
	return;	
}// end of DemoBdd
/****************************************************************************************************************************/
