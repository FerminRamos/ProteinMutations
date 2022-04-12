/******************************************************************************
 *Fermin Ramos
 *protein.c grabs both a protein sequence from a file and command line 
 * arguments and outputs a modified protein sequence based on the command 
 * line arguments. Command line arguments consist of either delete commands or
 * replace commands. Delete commands are written in "d##" format, where ## is
 * the position of the protein to be deleted. Replace commands are written in
 * "o##n" format, where o is old protein letter, ## is the position of the
 * protein to be deleted, and n is new protein letter. Protein.c does NOT have
 * error handling. 
 * 
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*List constantly shifts since we're deleting. This keeps track of that*/
static int numsDeleted = 0;

/******************************************************************************
 *Defines a Node for linked list. Contains data (protein) and next (pointer to
 * next empty space)
 *****************************************************************************/
struct ListNode
{
  char data;
  struct ListNode* next;
};

/******************************************************************************
 *Creates linked list by creating a new node. Creates a node by allocating
 * memory and then assigning protein (data) to that pointer. Then, moves next
 * pointer to NULL value.
 *Returns created node
 *****************************************************************************/
struct ListNode* createNode(char data)
{
  struct ListNode* node = malloc(sizeof(struct ListNode));
  node->data = data;
  node->next = NULL;
  return node;
};

/******************************************************************************
 *Prints Node List. Iterates through linked list by using head node. Prints
 * all values that are not NULL. Printing is formatted by groups of 10 as well
 * as padding field widths.
 *Returns nothing
 *****************************************************************************/
void printList(struct ListNode* head, int listSize)
{ 
  /*Groups of 10 to print*/
  int currentGroup[5] = {10, 20, 30, 40, 50};
  int totalGroups;
  totalGroups = listSize % 10;

  /*Prints Proteins + Groups*/
  struct ListNode* current = head;
  int protein;
  protein = 0;
  while(current != NULL)
  {
    /*Prints Labels (at start or after 50 proteins groups)*/
    if(protein % 50 == 0)
    {
      int i;
      for(i = 0; i < 5; i++)
      {
	/*If label is over listSize, changes prints to empty.*/
	if(currentGroup[i] > listSize)
	  printf("%10c", 32);
	else
	  printf("%10d", currentGroup[i]);
	
	/*Adds space between group labels, except last group*/
	if(i != 4)
	  printf(" ");
	
      }
      
      /*newline to print next group of proteins*/
      printf("\n");
      
      /*Adds +50 to each group, for next round of printing labels*/
      for(i = 0; i < 5; i++)
	currentGroup[i] = currentGroup[i] + 50;
      
    }
    
    /*Prints Proteins*/
    printf("%c", current->data);
    current = current->next;
    protein++;

    /*Add Space If:group of 10 proteins reached or not end of row/list*/
    if(protein % 10 == 0 && protein % 50 != 0 && protein < listSize-numsDeleted+1)
      printf(" ");
    else if(protein % 50 == 0)
    {
      /*newline to print group labels*/
      printf("\n");
    }
  }
  printf("\n");
};

/******************************************************************************
 *Deletes a Node in a certain position. DeleteNode() requires head Node and 
 * position to delete. DeleteNode() does NOT care about char value, only 
 * position.
 *Returns nothing
 *****************************************************************************/
struct ListNode* deleteNode(struct ListNode* head, int position)
{
  int i;
  i = 1 + numsDeleted;

  struct ListNode* prevPosition = NULL;
  struct ListNode* current = head;

  /*Iterates through linked list until position-1 is found*/
  while(i < position)
  {
    prevPosition = current;
    current = current->next;
    i++;
  }
  
  /*Skips over protein to delete*/
  prevPosition->next = current->next;
  numsDeleted++;

  /*Deletes skipped protein from inventory*/
  free(current);

};

/******************************************************************************
 *Replaces a node in a given position (position), and replaces the amino acid 
 * with a given protein (c). Iterates through linked list using head Node
 * until given position is reached.
 *Returns nothing
 *****************************************************************************/
struct ListNode* replaceNode(struct ListNode* head, int position, char c)
{
  struct ListNode* current = head;
  int i;
  i = 1;

  /*Iterates through linked list until protein position is found*/
  while(i != position)
  {
    current = current->next;
    i++;
  }

  /*Changes Protein*/
  current->data = c;
};

/******************************************************************************
 *Main Function.
 * (see project description for details)
 *****************************************************************************/
int main(int argc, char *argv[])
{
  int listSize;
  struct ListNode* head = NULL;
  struct ListNode* newNode = NULL;
  
  /*Grabs Head Node from file & Sets head to last known node "currentNode"*/
  int c;
  c = getchar();
  head = createNode(c);
  struct ListNode* currentNode = head;
  
  /*Grabs rest of input*/
  listSize = 0;
  while((c = getchar()) != EOF)
  {
    /*Only grabs protein, NOT newline chars*/
    if(c != '\n')
    {
      /*Insert next nodes*/
      newNode = createNode(c);
      newNode->next = currentNode->next;
      currentNode->next = newNode;
      
      /*Switch "newNode" into last known Node "currentNode"*/
      currentNode = newNode;
      listSize++;
    }
  }


  /*Creates Pointer Arrays for delete & replace cmds*/
  int i, deleteSize, replaceSize;
  deleteSize = replaceSize = 1;
  char **deleteCmds = malloc(argc * sizeof(*deleteCmds));
  char **replaceCmds = malloc(argc * sizeof(*replaceCmds));

  
  /*Below: Reading in command line args, skipping first 2 args*/
  for(i = 2; i < argc; i++)
  {
    /*Grabs 1st char from command line arg*/
    char initialChar;
    initialChar = argv[i][0];
    
    /*Checks if initialChar = d because that's a delete command. Else, it's a replace command.*/
    if(initialChar == 'd')
    {
      /*Allocates memory & adds command to delete (254)*/
      deleteCmds[deleteSize-1] = malloc(argc * sizeof(char*));
      strcpy(deleteCmds[deleteSize-1], argv[i]);
      deleteSize++;
      
    }
    else
    {
      /*Allocates memory & adds command to replace*/
      replaceCmds[replaceSize-1] = malloc(argc * sizeof(char*));
      strcpy(replaceCmds[replaceSize-1], argv[i]);
      replaceSize++;
      
    }
  }
  
  long position;
  char *tempother;

  /*Run Replace Commands*/
  for(i = 0; i < replaceSize; i++)
  {
    if(replaceCmds[i] != NULL)
    {
      char initial = replaceCmds[i][0];

      /*Shift deleteCmd left*/
      int j;
      j = 0;
      while(replaceCmds[i][j] != 0)
      {
	replaceCmds[i][j] = replaceCmds[i][j+1];
	j++;
      }

      /*Grab Position of replaceCmd*/
      position = strtol(replaceCmds[i], &tempother, 10);
      
      /*Send to replaceNode() to replace*/
      replaceNode(head, position, tempother[0]);
    }
  }
  
  /*Run Delete Commands*/
  for(i = 0; i < deleteSize; i++)
  {
    if(deleteCmds[i] != NULL)
    {
      char initial = deleteCmds[i][0];

      /*Shift deleteCmd left*/
      int j;
      j = 0;
      while(deleteCmds[i][j] != 0)
      {
	deleteCmds[i][j] = deleteCmds[i][j+1];
	j++;
      }

      /*Grab Position of deleteCmd*/
      position = strtol(deleteCmds[i], &tempother, 10);
      
      /*Send to deleteNode() to delete*/
      deleteNode(head, position);
    }
  }
  
  /*Print Final List*/
  printf("Spike protein sequence for %s:\n", argv[1]);
  printList(head, listSize);


  /*Free Items*/
  free(deleteCmds);
  free(replaceCmds);
  
  /*Frees the Entire list*/
  struct ListNode* temp;
  
  while(head != NULL)
  {
    temp = head;
    head = head->next;
    free(temp);
  }
  /*Frees Commands*/
  for(i = 0; i < deleteSize; i++)
    free(deleteCmds[i]);
  for(i = 0; i < replaceSize; i++)
    free(replaceCmds[i]);
  
  return 0;
}
