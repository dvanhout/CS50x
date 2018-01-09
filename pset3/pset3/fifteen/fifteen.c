/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();
        
        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw! You are amazing and you rock and are sooooo smart!!!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // c, r is column and row
    // x is counter
    // swap is for swapping values in special case of odd tiles on board

    int x = 1, c, r, swap;

    // populate the board with values of (d*d-x) 
    for (r = 0; r < d; r++)
    {
        for (c = 0; c < d; c++)
        {
            board[r][c] = d * d - x;
            x++;
        }
    }
    // check for odd number of pieces on board and swaps last two non-0 nums
    // game not solvable if below step not taken
    if (board[0][0] % 2 != 0)
    {
        swap = board[r-1][c-2];
        board[r-1][c-2] = board[r-1][c-3];
        board[r-1][c-3] = swap;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // c, r is column, row
    int c, r;
    
    for (r = 0; r < d; r++)
    {
        for (c = 0; c < d; c++)
        {
            // if array space is not the blank space (0), then print it
            if (board[r][c] != 0)
            {
                // add extra space if number is less than two digits
                if (board[r][c] < 10)
                {
                    printf(" %d ", board[r][c]);
                    
                }
                else
                {
                    printf("%d ", board[r][c]);
                }
            }
            // print a blank space for 0
            else
            {
                printf("   ");
            }
            
        }
        // new line at end of row
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // c, r is column and row
    int c, r;
    
    for (r = 0; r < d; r++)
    {
        for (c = 0; c < d; c++)
        {
            // find the user input tile
            if (board[r][c] == tile)
            {

                // check for adjacent blank space and edge of board
                if ((board[r+1][c] == 0) && (r+1 <= d-1))
                {
                    board[r+1][c] = board[r][c];
                    board[r][c] = 0;
                    return true;
                }
                else if ((board[r-1][c] == 0) && (r-1 >= 0))
                {
                    board[r-1][c] = board[r][c];
                    board[r][c] = 0;
                    return true;
                }
                else if ((board[r][c+1] == 0) && (c+1 <= d-1))
                {
                    board[r][c+1] = board[r][c];
                    board[r][c] = 0;
                    return true;
                }
                else if ((board[r][c-1] == 0) && (c-1 >= 0))
                {
                    board[r][c-1] = board[r][c];
                    board[r][c] = 0;
                    return true;
                }
            }
        
        }
    }
    // move is illegal, no adjacent blank space
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // x is a counter
    // r, c is row and column
    int x = 1, r = 0, c = 0;

    // if blank space is at the end of array and 1 is first number, check position of other numbers
    if ((board[d-1][d-1] == 0) && (board[r][c] == 1))
    {
        for (r = 0; r < d; r++)
        {
            for (c = 0; c < d; c++)
            {
                // if a number is out of place before last blank space encountered
                if ((board[r][c] != x) && (board[r][c] != 0))
                {
                    return false;
                }
            x++;
            }
        }
        //if all conditions met, and end of array is met, board must be in proper order
        exit(0);
    }
    return false;
}
