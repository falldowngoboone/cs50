/**
 * greedy.c
 *
 * Ryan Boone
 * falldowngoboone@gmail.com
 *
 * Calulates change in the fewest coins possible.
 */
 
#include <cs50.h>
#include <stdio.h>
#include <math.h>
 
int main(void)
{
    float change;
    
    // get the change owed
    printf("O hai! ");
    do
    {
        printf("How much change is owed?\n");
        change = GetFloat();
    }
    while (change < 0);
    
    // convert that pesky float to an int in cents
    int cents = round(change * 100);
    int coins = 0;
    
    // zero out the change, increment the coins
    while (cents - 25 >= 0)
    {
        cents -= 25;
        coins++;
    }
    while (cents - 10 >= 0)
    {
        cents -= 10;
        coins++;
    }
    while (cents - 5 >= 0)
    {
        cents -= 5;
        coins++;
    }
    while (cents - 1 >= 0)
    {
        cents -= 1;
        coins++;
    }
    
    printf("%d\n", coins);
}
