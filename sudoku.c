#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// This program will solve any solvable sudoku puzzle.
// Suharsh Sivakumar
// December 28, 2011

// This program will now store puzzles in another file and have the ability to write and read puzzles from there.
// January 5, 2012

// Define storage file location
#define PUZZLES "C:\\Users\\Suharsh\\Documents\\projects\\sudoku\\puzzles.txt"


// Prototypes
int menu(void);
void readPuzzle(char *);
void show(char *);
int indexfinder(char *, int *);
void solve(int, int, char *, int *, int);
int chartoint(char);
char inttochar(int);
int isnumeric(char  *);
int getNumPuz(void);
void puzzleWriter(char *);
void puzzleReader(char * , int);


// Main Function is below
void main(void)
{
  char puzzle[82]; // holds the puzzle
  int indexes[82]; // holds the location of each empty cell
  int maxIndex; // total count of empty cells

  while(1)
  {
    readPuzzle(puzzle);
    show(puzzle);
    maxIndex = indexfinder(puzzle, indexes); 
    solve(0,1, puzzle, indexes, maxIndex);
    show(puzzle);
  }
}


// Function Definitions

// User menu
int menu(void)
{
  int choice = 5;
  printf("\n****MENU****\n\nEnter (1) to enter your own puzzle.\nEnter (2) to run the program using a random preset puzzle.\nEnter (any other number) to exit the program.\n\n");
  printf("Your choice: ");
  scanf("%i", &choice);
  return choice;
}

// Chooses which puzzle to use based on user input.
void readPuzzle(char * puzzle)
{
  int choice;
  choice = menu();

  // The following portion is to use user input
  if(choice == 1)
  {
    printf("\nInput puzzle in one line of 81 numbers (empty squares should be input as 0. \nYour puzzle: ");
    scanf("%s", puzzle); 
    int numeric = isnumeric(puzzle);
    while (strlen(puzzle) != 81 || numeric == 0)
    {
      printf("Please enter a string of 81 numeric characters: \n");
      scanf("%s", puzzle);
      numeric = isnumeric(puzzle);
    }
    
    puzzleWriter(puzzle);
  }

  // The following portion runs a random preset puzzle.
  else if (choice == 2)
  {
    int numpuz;
    int randpuz = 0;
    numpuz = getNumPuz();
    srand(time(NULL));
    randpuz = rand() % numpuz;

    puzzleReader(puzzle, randpuz);

  }  

  // Exits the program.
  else
    exit(1); 
}
  

// Shows the puzzle.
void show(char * puzzle)
{

int p = 0;

for (int i = 0; i < 9; i++)
{
  for (int j = 0; j < 9; j++, p++)
  {
    if ((j % 3 == 2)  && (j % 9 != 8))
      {
        printf(" %c |", puzzle[p]);
      }
    else
      {
        printf(" %c ", puzzle[p]);
      }
  }
  if (i % 3 == 2 && (i % 9 != 8))
  {
    printf("\n-----------------------------\n");
  }
  else
  {
    printf("\n");
  }
}
printf("\n");
}


// This function stores the indexes of the changeable function in the array 'indexes'.
int indexfinder(char * puzzle, int * indexes)
{
  int maxIndex; // the total count of empty cells
  maxIndex = -1;
  char choice[10];
  printf("Is this the puzzle you want to solve? (y/n)\n");
  scanf("%s", choice);
  printf("\n");
  if (choice[0] == 'n' || choice[0] == 'N' )
  {
    printf("Below is the puzzle that was input.\n\nMake a new choice.\n\n");
    return maxIndex;
  }
  for(int i = 0; i < 81; i++)
  {
    if (puzzle[i] == '0')
    {
      indexes[++maxIndex] = i;
    }
  }
  return maxIndex;
}




// This function actually solves the puzzle.
void solve(int cell, int trial, char * puzzle, int * indexes, int maxIndex)
{

// The following can be viewed if compiled with option -DDEBUG
#ifdef DEBUG
printf("Blah-1  cell=%i  trial=%i   indexes[cell]=%i  a=%i\n", cell, trial, indexes[cell], maxIndex);
#endif

if (maxIndex < 0)
{
  return;
}

// Base case (This means we have reaches the last cell)
if (cell > maxIndex)
{
  printf("\nThat was easy!\n\n");
  return;
}

// No possible number for the cell (means we have to go and try different numbers for the previous cells)
if (trial > 9 && cell > 0)
{
  puzzle[indexes[cell]] = '0';
  return solve(cell - 1, chartoint(puzzle[indexes[cell-1]]) + 1, puzzle, indexes, maxIndex);
}

// If no numbers are possible for the first cell then the puzzle is not solvable!
else if (trial > 9 && cell == 0)
{
  printf("Puzzle not solvable\n");
  return;
}

  
// flag keeps track of whether or not the trial is possible for the current cell
int flag = 1;

    
// Rule for vertical columns
for (int f = 0; f < 81; f++)
{
  if ((f % 9) == (indexes[cell] % 9))
  {
    if (puzzle[f] == inttochar(trial))
    {
      flag = 0;         }
    }
}
        
// Rule for horizontal rows
int b = indexes[cell] - (indexes[cell] % 9);
for (int d = 0; d < 9; ++d, ++b)
{
  if (puzzle[b] == inttochar(trial))
  {
    flag = 0;
  }
}

// Rule for sqaures
int c = (indexes[cell]-(indexes[cell] % 27)+(indexes[cell] % 9)-(indexes[cell] % 3));
for (int z = 0; z < 3; z++)
{
  for (int y = 0; y < 3; y++)
  {
    if (puzzle[9*z + y + c] == inttochar(trial))
    {
      flag = 0;
    }
  }
}


// If the trial works go to the next cell
if (flag == 1)
{
  puzzle[indexes[cell]] = inttochar(trial);
  return solve(cell + 1, 1, puzzle, indexes, maxIndex);
}

// Else try the next number
else
{
  return solve(cell, trial + 1, puzzle, indexes, maxIndex);
}
  
}


// Convert a character to an integer
int chartoint(char input) 
{
  switch (input){
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    default : return -1;
  }
}


// Converts an integer to a character
char inttochar(int input)
{
  switch (input){
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    default: return -1;
  }
}

int isnumeric(char * string)
{
  for (int i = 0; string[i] != '\0'; i++)
  { 
    if (string[i] > '9' || string[i] < '0')
    { 
      return 0;
    }
  }
  return 1;
}

// The next functions writes and reads from the file "puzzles.txt"
int getNumPuz(void)
{
  FILE *fptr;
  fptr = fopen(PUZZLES,"r");
  int ones, tens, hunds;
  ones = chartoint((char)fgetc(fptr));
  tens = chartoint((char)fgetc(fptr));
  hunds = chartoint((char)fgetc(fptr));
  fclose(fptr);

  int numpuz;
  numpuz = 100*hunds + 10*tens + ones;
  return numpuz;
}
  
void puzzleWriter(char * puzzle)
{
  FILE *fptr;

  fptr = fopen(PUZZLES, "r+"); /*Opens the file for reading and writing*/

  int numpuz;
  char curpuz[82];
  int ones = 0, tens = 0, hunds = 0;
  numpuz = getNumPuz();
  // The next part makes sure that you are not writing the same puzzle in the file again.
  for (int i = 0; i < numpuz; ++i)
  {
    fseek(fptr, (4 + (82*i)), 0);
    fgets(curpuz, 82, fptr);
    if (!strcmp(puzzle, curpuz))
    {
      fclose(fptr);
      return;
    }
  }

  ++numpuz;
  hunds = numpuz / 100;
  numpuz = numpuz % 100;
  tens = numpuz / 10;
  numpuz = numpuz % 10;
  ones = numpuz;
  
  
  fprintf(fptr, "%i%i%i", ones, tens, hunds);
  fseek(fptr, 0, 2);  /* Goes to the end of the file */
  fputs(puzzle, fptr); /* Puts puzzle and "/n" to end */
  fputc('\n', fptr);
  fclose(fptr);
}

void puzzleReader(char * puzzle, int puzzleNumber)
{
  FILE *fptr;
  fptr = fopen(PUZZLES, "r");
  fseek(fptr, (4 + (82*puzzleNumber)), 0);
  fgets(puzzle, 82, fptr);
  fclose(fptr);
}











  






