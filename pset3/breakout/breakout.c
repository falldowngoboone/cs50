//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// height and width of game's paddle in pixels
#define PADDLE_HEIGHT 8
#define PADDLE_WIDTH  55

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// amount of gap between bricks
#define GAP 5

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);
    
    // number of points initially
    int points = 0;
    
    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    
    // instantiate scoreboard
    updateScoreboard(window, label, points);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;
    
    // set the X and Y velocity of the ball
    double x_velocity = 0.0, 
           y_velocity = 0.0, 
           velocity_factor = 4;
           
    while (x_velocity < 0.2 || x_velocity > 0.6)
    {
        x_velocity = drand48();
    }
    
    while (y_velocity < 0.6 || y_velocity > 0.9)
    {
        y_velocity = drand48();
    }
    
    // make sure the player's ready to play
    waitForClick();
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // paddle follows mouse x position
        GEvent mouse_event = getNextEvent(MOUSE_EVENT);
        
        if (mouse_event != NULL)
        {
            if (getEventType(mouse_event) == MOUSE_MOVED)
            {
                // cache half the width of the paddle
                double offset = getWidth(paddle) / 2, 
                       x = getX(mouse_event) - offset, 
                       y = getY(paddle);
                
                // keep the paddle in the window
                if (x < 0)
                {
                    x = 0;
                }
                else if (x > getWidth(window) - getWidth(paddle))
                {
                    x = getWidth(window) - getWidth(paddle);
                }
                
                // set the location of the paddle
                setLocation(paddle, x, y);
            }
        }
        
        // move the ball
        double delta_x = velocity_factor * x_velocity, 
               delta_y = velocity_factor * y_velocity;
               
        move(ball, delta_x, delta_y);
        
        // ball bounces off window bounds
        if (getX(ball) <= 0)
        {
            x_velocity = -x_velocity;
        }
        else if (getX(ball) >= getWidth(window) - getWidth(ball))
        {
            x_velocity = -x_velocity;
        }
        
        if (getY(ball) <= 0)
        {
            y_velocity = -y_velocity;
        }
        
        // if ball hits rock bottom
        else if (getY(ball) >= getHeight(window) - getWidth(ball))
        {
            // lose a life, break loop if it's the last life
            lives--;
            if (lives == 0)
            {
                break;
            }
            
            // wait for a click to unpause the game
            waitForClick();
            
            // reinitialize the ball in the center of the window
            double ball_x = getWidth(window) / 2 - getWidth(ball), 
                   ball_y = getHeight(window) / 2 - getHeight(ball);
            
            setLocation(ball, ball_x, ball_y);
        
        }
        
        // let's give the cpu a breather
        pause(10);
        
        
        GObject object = detectCollision(window, ball);
        
        if (object != NULL)
        {
            // ball bounces off the paddle
            if (object == paddle)
            {
                y_velocity = -y_velocity;
            }
            
            // ball bounces off bricks and removes them
            else if (strcmp(getType(object), "GRect") == 0)
            {
                y_velocity = -y_velocity;
                removeGWindow(window, object);
                
                // score!
                bricks--;
                points++;
                
                updateScoreboard(window, label, points);
                
                // game gets faster!!!
                velocity_factor += .1;
            }
        }
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // store the row colors in an array
    string colors[] = {"RED","BLACK","DARK_GRAY","GRAY","LIGHT_GRAY"};
    
    // figure out and declare brick width and height
    double brick_width = (WIDTH - (COLS + 1) * GAP) / COLS, 
           brick_height = 12, 
           offset = 50;
    
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            double brick_x = (brick_width + GAP) * j + GAP, 
                   brick_y = (brick_height + GAP) * i + offset;
                   
            GRect brick = newGRect(brick_x, brick_y, brick_width, brick_height);
            setFilled(brick, true);
            setColor(brick, colors[i]);
            add(window, brick);
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    double ball_x = getWidth(window) / 2 - RADIUS, 
           ball_y = getHeight(window) / 2 - RADIUS;
           
    // make the ball and place it in the center of the window
    GOval ball = newGOval(ball_x, ball_y, RADIUS * 2, RADIUS * 2);
    setFilled(ball, true);
    setColor(ball, "RED");
    add(window, ball);
    
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // set paddle x centered horizontally and y near the bottom of the window
    double paddle_x = (WIDTH - PADDLE_WIDTH) / 2, 
           paddle_y = 550;
    
    // create, style and add paddle object to window
    GRect paddle = newGRect(paddle_x, paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
    add(window, paddle);
    
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // scoreboard is set in GRAY SanSerif-36
    GLabel label = newGLabel("");
    setFont(label, "SanSerif-36");
    add(window, label);
    setColor(label, "GRAY");
    
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
