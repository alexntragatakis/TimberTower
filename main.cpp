/* Preprocessor directives */
#include "FEHLCD.h"
#include <FEHImages.h>
#include <math.h>
#include <stdio.h>
#include <FEHUtility.h>

/* Struct tag */
class Block {
    private:
        int shape;
        FEHImage image;
        // Location
        float x_pos;
        float y_pos;
        // Physical properties
        float height;
        float length;
        float mass;
        float centerOfMass;
    public:
        // Functions to access and change vars
        void SetXnY(float x, float y);
        float * GetXnLength();
        float * GetYnHeight();
        float GetMass();
        float GetCenterOfMass();
        FEHImage GetImage();
        int RandomizeBlock();
        void CalculatePhysicalProps();
};

/* Function prototypes */
bool CheckTowerFall(Block b[], int blockCount);
void PlayGame(int *, int *);
void DisplayResults(int, int);

int main()
{
    FEHImage menuBg, gameBg, woodLong, woodTall, woodSquare; // Images
    float x_pos, y_pos; // Coords clicked at
    float x_trash, y_trash; // Coords released at
    int total_games = 0, total_blocks = 0, tallest_tower = 0; // Tracked statistics across all games
    int game_blocks, game_height; // Statistics for the current game

    /* Opens images for menu and game backgrounds */
    menuBg.Open("MenuBG.png");
    menuBg.Draw(0, -10);
    gameBg.Open("GameBG.png");

    /* Opens images for building materials */
    woodLong.Open("WoodPlankLong.png");
    woodTall.Open("WoodPlankTall.png");
    woodSquare.Open("WoodPlankSquare.png");
    
    /* Draws buttons on menu */ 
    LCD.SetFontColor(LIGHTBLUE);
    LCD.FillRectangle(40, 115, 100, 40);
    LCD.FillRectangle(175, 115, 100, 40);
    LCD.FillRectangle(40, 175, 100, 40);
    LCD.FillRectangle(175, 175, 100, 40);
    LCD.SetFontColor(WHITESMOKE);
    LCD.DrawRectangle(40, 115, 100, 40);
    LCD.DrawRectangle(175, 115, 100, 40);
    LCD.DrawRectangle(40, 175, 100, 40);
    LCD.DrawRectangle(175, 175, 100, 40);
    LCD.SetFontColor(BLACK);
    LCD.SetFontScale(0.5);
    LCD.WriteAt("Play", 78, 128);
    LCD.WriteAt("How to Play", 192, 128);
    LCD.WriteAt("Statistics", 55, 188);
    LCD.WriteAt("Credits", 203, 188);
    // Title
    LCD.SetFontColor(LIGHTBLUE);
    LCD.FillRectangle(45, 30, 230, 40);
    LCD.SetFontColor(WHITESMOKE);
    LCD.DrawRectangle(45, 30, 230, 40);
    LCD.SetFontColor(BLACK);
    LCD.SetFontScale(1.5);
    LCD.WriteAt("Timber Tower", 50, 35);
    LCD.SetFontScale(0.5);

    while (1) {
        /* Waits for the user to click a button and remembers the location the click was at */
        while(!LCD.Touch(&x_pos, &y_pos)) {} // Wait for touch
        while(LCD.Touch(&x_trash, &y_trash)); // Wait for release

        /* If-else structure to determine which button was clicked by the player. If a location outside the four buttons is clicked, nothing will happen */
        if(x_pos > 40 && x_pos < 140 && y_pos > 115 && y_pos < 155) { // Play is clicked  
            gameBg.Draw(0, 0); // Draws game background
            LCD.Update();
            game_blocks = 0; // Number of blocks used in the current game (starts at 0)
            game_height = 0; // Height of the tower in the current game (starts at 0)

            // Calls PlayGame function to start the game
            PlayGame(&game_blocks, &game_height);

            total_games+=1; // Updates total number of games played, tracked for statistics screen
            total_blocks+=game_blocks; // Add blocks placed this game to the running total, tracked for statistics screen
            if(tallest_tower < game_height) { // If the tower from the current game is taller than the previous tallest tower, set the tallest tower equal to the current tower, tracked for statistics screen
                tallest_tower = game_height;
            }   

            // Calls the DisplayResults function to show the blocks placed and tower height from the current game
            DisplayResults(game_blocks, game_height);

            // Draws Quit button to return to main menu
            LCD.WriteAt("<Quit>", 280, 200);
        }
        else if(x_pos > 175 && x_pos < 275 && y_pos > 115 && y_pos < 155) { // How to Play is clicked
            // Draw background
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 500, 500);
            LCD.SetFontColor(WHITE);

            // Instructions are printed on screen 
            LCD.WriteAt("How to Play:", 120, 20);
            LCD.WriteAt("You will be given a random block and its mass", 20, 70);
            LCD.WriteAt("Stack the block on the platform or other blocks", 15, 90);
            LCD.WriteAt("Keep the center of gravity of the tower centered", 15, 110);
            LCD.WriteAt("If the tower becomes too unstable, it will fall", 20, 130);

            // Draws quit button to return to main menu
            LCD.WriteAt("<Quit>", 280, 200); 
        }
        else if(x_pos > 40 && x_pos < 140 && y_pos > 175 && y_pos < 215) { // Stats is clicked
            // Draw background
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 500, 500);
            LCD.SetFontColor(WHITE);

            // Display stats
            LCD.WriteAt("Statistics:", 120, 20);
            LCD.WriteAt("Total Games Played:", 60, 70);
            LCD.WriteAt("Tallest Tower:", 60, 100);
            LCD.WriteAt("meters", 220, 100);
            LCD.WriteAt("Total Blocks Stacked:", 60, 130);
            LCD.WriteAt(total_games, 200, 70);
            LCD.WriteAt(tallest_tower, 200, 100);
            LCD.WriteAt(total_blocks, 200, 130);

            // Draws Quit button to return to main menu
            LCD.WriteAt("<Quit>", 280, 200);
        }
        else if(x_pos > 175 && x_pos < 275 && y_pos > 175 && y_pos < 215 ) { // Credits is clicked
            // Draw background
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 500, 500);
            LCD.SetFontColor(WHITE);

            // Display credits
            LCD.WriteAt("Credits:", 130, 20);
            LCD.WriteAt("Developers:", 120, 70);
            LCD.WriteAt("Alexander Ntragatakis and Jennifer Lee", 42, 90);
            LCD.WriteAt("Menu Background Image:", 95, 120);
            LCD.WriteAt("freepik.com", 120, 140);

            // Draws Quit button to return to main menu
            LCD.WriteAt("<Quit>", 280, 200);
        }
        LCD.Update();
        
        // Returns to main menu once Quit is clicked 
        while(!LCD.Touch(&x_pos, &y_pos)) {} // Wait for touch
        while(LCD.Touch(&x_trash, &y_trash)); // Wait for release
        if (x_pos>0 && x_pos < 40 && y_pos>0 && y_pos < 20) { // Replay is clicked
            gameBg.Draw(0, 0); // Draws game background
            LCD.Update();
            game_blocks = 0; // Number of blocks used in the current game (starts at 0)
            game_height = 0; // Height of the tower in the current game (starts at 0)

            // Calls PlayGame function to start the game
            PlayGame(&game_blocks, &game_height);
        }
        if(x_pos > 280 && x_pos < 320 && y_pos > 200 && y_pos < 220) { // Quit is clicked
            /* Draws background image for menu screen */
            menuBg.Open("MenuBG.png");
            menuBg.Draw(0, -10);
            
            /* Draws buttons on menu */
            LCD.SetFontColor(LIGHTBLUE);
            LCD.FillRectangle(40, 115, 100, 40);
            LCD.FillRectangle(175, 115, 100, 40);
            LCD.FillRectangle(40, 175, 100, 40);
            LCD.FillRectangle(175, 175, 100, 40);
            LCD.SetFontColor(WHITESMOKE);
            LCD.DrawRectangle(40, 115, 100, 40);
            LCD.DrawRectangle(175, 115, 100, 40);
            LCD.DrawRectangle(40, 175, 100, 40);
            LCD.DrawRectangle(175, 175, 100, 40);
            LCD.SetFontColor(BLACK);
            LCD.SetFontScale(0.5);
            LCD.WriteAt("Play", 78, 128);
            LCD.WriteAt("How to Play", 192, 128);
            LCD.WriteAt("Statistics", 55, 188);
            LCD.WriteAt("Credits", 203, 188);
            // Title
            LCD.SetFontColor(LIGHTBLUE);
            LCD.FillRectangle(45, 30, 230, 40);
            LCD.SetFontColor(WHITESMOKE);
            LCD.DrawRectangle(45, 30, 230, 40);
            LCD.SetFontColor(BLACK);
            LCD.SetFontScale(1.5);
            LCD.WriteAt("Timber Tower", 50, 35);
            LCD.SetFontScale(0.5);
        }
        LCD.Update();
    }
    return 0;
}

/* void SetXnY - Allows private variables x_pos and y_pos to be modified outside of Block members */
void Block::SetXnY(float x, float y) {
    x_pos=x;
    y_pos=y;
}

/* float * GetXnLength - Allows private variables x_pos and length to be accessed outside of Block members 
Returns an array where x_pos is at index 0 and length is at index 1 
- Alex */
float * Block::GetXnLength() {
    float array[2];
    array[0] = x_pos;
    array[1] = length;
    return array;
}

/* float * GetYnHeight - Allows private variables y_pos and height to be accessed outside of Block members 
Returns an array where y_pos is at index 0 and width is at index 1 
- Alex */
float * Block::GetYnHeight() {
    float array[2];
    array[0] = y_pos;
    array[1] = height;
    return array;
}

/* float GetMass - Allows private variable mass to be accessed outside of Block members 
Returns mass as a float 
- Jennifer */
float Block::GetMass() {
    return mass;
}

/* float GetCenterOfMass - Allows private variable centerOfMass to be accessed outside of Block members 
Returns centerOfMass as a float 
- Jennifer */
float Block::GetCenterOfMass() {
    return centerOfMass;
}

/* FEHImage GetImage - Allows private variable image to be accessed outside of Block members 
Returns image as a FEHImage 
- Alex */
FEHImage Block::GetImage() {
    return image;
}

/* int RandomizeBlock - Each block will be assigned a random number (from 0-2) that will be used to decide its shape and a random mass (from 10-50kg)
The generated values are then stored in shape and mass, respectively, and the coresponding image is opened 
- Jennifer */
int Block::RandomizeBlock() {
    shape = rand() % 3; // Random number from 0-2 for the shape
    mass = rand() % 41 + 10; // Random mass from 10-50 for the mass
    if(shape == 0) { // Long wooden plank, height 12 pixels, length 36
        height = 12;
        length = 36;
        image.Open("WoodPlankLong.png");
    }
    else if(shape == 1) { // Tall wooden plank, height 36 pixels, length 12
        height = 36;
        length = 12;
        image.Open("WoodPlankTall.png");
    }
    else if(shape == 2) { // Square wooden plank, height 24 pixels, length 24
        height = 24;
        length = 24;
        image.Open("WoodPlankSquare.png");
    }
}

/* void CalculatePhysicalProps - Calculates the location of the center of mass of a block based on its position on-screen and its length 
- Alex */
void Block::CalculatePhysicalProps() {
    // Center of mass for both rectangles and box
    centerOfMass = x_pos + (0.5)*length;
}

/* bool CheckTowerFall - Returns true if the tower falls after a block is added and false if not 
The tower center of mass is calculated, and it is checked if this center of mass is on the platform.
Then, blocks on other blocks are checked to make sure that for each stack of blocks on other blocks, the
center of mass is over the length of the block.
- Alex */
bool CheckTowerFall(Block b[], int blockCount) {
    // Recalculate physical properties for each block
    for (int i=0; i<=blockCount; i++) { b[i].CalculatePhysicalProps(); }
    // Get the tower center of mass
    float towerCOM=0;
    float totalMass=0;
    for (int i=0; i<=blockCount; i++) {
        towerCOM+=(b[i].GetMass() * b[i].GetCenterOfMass());
        totalMass+=b[i].GetMass();
    }
    towerCOM/=totalMass;

    // Check if tower center of mass is outside the platform
    if (towerCOM < 130 || towerCOM > 190) {
        return true; // tower tips over
    }

    // Check if center of mass for blocks on another block are on the length of the block
    if (blockCount>0) {
        for (int i=0; i<=blockCount; i++) {
            int platformBlockIndex = i;
            float aboveCOM = 0;
            float aboveTotalMass=0;
            for (int j=i+1; j<=blockCount; j++) { // Loop through blocks above the platformBlock
                // Calculate aboveCOM
                aboveCOM+=(b[j].GetMass() * b[j].GetCenterOfMass());
                aboveTotalMass+=b[j].GetMass();
            }
            aboveCOM/=aboveTotalMass;

            if (aboveCOM < b[i].GetXnLength()[0] || aboveCOM > b[i].GetXnLength()[0]+b[i].GetXnLength()[1]) {
                return true;
            }
        }
    }
    return false; // Runs through the loop and nothing falls
}

/* void DisplayResults - Once the game has ended, the number of blocks placed and the final height of the tower are displayed to the player 
The player will be able to quit, returning to the main menu screen to play again or exit the game 
- Jennifer */
void DisplayResults(int game_blocks, int game_height) {
    LCD.SetFontColor(STEELBLUE);
    LCD.FillRectangle(0, 0, 320, 240);
    LCD.SetFontColor(LIGHTSTEELBLUE);
    LCD.FillRectangle(25, 15, 270, 210);
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(25, 15, 270, 210);
    LCD.WriteAt("Results", 140, 25);
    LCD.WriteAt("Tower Height:", 50, 50);
    LCD.WriteAt(game_height, 160, 50);
    LCD.WriteAt("meters", 180, 50);
    LCD.WriteAt("Number of Blocks:", 50, 70);
    LCD.WriteAt(game_blocks, 160, 70);
    // Draw replay button
    LCD.Write("<Replay>");
}

/*void moveNextBlock - This function takes in the address of a block and edits the X and Y positions of the box.
It then draws the block's image at the set X and Y positions.
This Y value is set so that the block is sitting visually on the floor.
The X position corresponds to the spot for which the next block (second in line) is sitting.
- Alex */
void moveNextBlock(class Block *block) {
    block->SetXnY(290,224-block->GetYnHeight()[1]); // todo: Set coords where "next" placeholder is
    block->GetImage().Draw(290,224-block->GetYnHeight()[1]);
}

/*void moveToPlayBlock - This function takes in the address of a block and edits the X and Y positions of the box.
It then draws the block's image at the set X and Y positions.
This Y value is set so that the block is sitting visually on the floor.
The X value corresponds to the spot for which the block next in line (the "to play" block) is sitting.
A statement that says that this block weights __ kg is shown on the screen.
- Alex */
void moveToPlayBlock(class Block *block) {
    block->SetXnY(250,224-block->GetYnHeight()[1]); // todo: Set coords where "play" placeholder is
    block->GetImage().Draw(250,224-block->GetYnHeight()[1]);
    int intWeight = block->GetMass();
    LCD.WriteAt(intWeight,210,175);
    LCD.WriteAt("This block weighs", 210, 165);
    LCD.WriteAt("Kilograms",230,175);
}

/*void generateNextBlock - This function uses the class block's functions to assign random values and physical
properties to the block according to those functions calculations (see their descriptions)
- Alex */
void generateNextBlock(class Block *block) {
    block->RandomizeBlock();
    block->CalculatePhysicalProps();
}

/*void PlayGame - This function defines how the game is played.
The first two blocks to play are randomized and placed on the screen, and the block counter increases accordingly.
A for loop defines the repetition of placing blocks on the screen includes the following process.
If the user's cursor is within the block's area, the user can drag this block to a place on the screen.
When it is released, the block is vertically placed at the top of the screen and drops by gravity to the platform.
Then, the tower is checked according to the previously defined functions, and values taken in (game blocks and game height)
are editted and and the function ends.
- Alex */
void PlayGame(int * game_blocks, int * game_height) {
    FEHImage gameBg;
    gameBg.Open("GameBG.png");
    LCD.SetFontColor(WHITESMOKE);

    bool playing = true;
    int blockCount = 0, blocksInPlay = 0, x_pos, y_pos;
    float towerHeight = 0;
    class Block blocks[50];
    while (playing) {
        // Generate and place first two blocks
        generateNextBlock(&blocks[blockCount]);
        moveToPlayBlock(&blocks[blockCount]);
        blockCount+=1;
        generateNextBlock(&blocks[blockCount]);
        moveNextBlock(&blocks[blockCount]);
        blockCount+=1;
        
        LCD.Update();
        for (int i=0;i<20;i++) {
            bool condition1 = (i==0), condition2 = (i>0);

            // Dragging for the toPlay block
            while (!(LCD.Touch(&x_pos, &y_pos) && x_pos>blocks[blocksInPlay].GetXnLength()[0]
            && x_pos<blocks[blocksInPlay].GetXnLength()[0]+blocks[blocksInPlay].GetXnLength()[1]
            && y_pos>blocks[blocksInPlay].GetYnHeight()[0]
            && y_pos<blocks[blocksInPlay].GetYnHeight()[0]+blocks[blocksInPlay].GetYnHeight()[1])){}; // todo: fix boundaries for clicking on blocks
            generateNextBlock(&blocks[blockCount]);
            moveToPlayBlock(&blocks[blockCount-1]);
            moveNextBlock(&blocks[blockCount]);
            while (LCD.Touch(&x_pos, &y_pos)) {
                gameBg.Draw(0,0);
                // Draw blocks in play
                for (int i=0;i<blocksInPlay;i++) {
                    blocks[i].GetImage().Draw(blocks[i].GetXnLength()[0],blocks[i].GetYnHeight()[0]);
                }
                // Draw blocks in line
                moveToPlayBlock(&blocks[blockCount-1]);
                moveNextBlock(&blocks[blockCount]);
                blocks[blocksInPlay].GetImage().Draw(x_pos, y_pos);
                
                LCD.Update();
            }
            blockCount+=1;
            blocks[blocksInPlay].SetXnY(x_pos, 0);
            LCD.Update();
            // While block isn't hitting the platform, make it fall
            while ((condition1 && blocks[blocksInPlay].GetYnHeight()[0]+blocks[blocksInPlay].GetYnHeight()[1] < 200) || 
            (condition2 && (blocks[blocksInPlay].GetYnHeight()[0]+blocks[blocksInPlay].GetYnHeight()[1] < blocks[blocksInPlay-1].GetYnHeight()[0]) )) {
                gameBg.Draw(0,0);
                // Draw blocks in play
                for (int i=0;i<blocksInPlay;i++) {
                    blocks[i].GetImage().Draw(blocks[i].GetXnLength()[0],blocks[i].GetYnHeight()[0]);
                }
                // Draw blocks in line
                moveToPlayBlock(&blocks[blocksInPlay+1]);
                moveNextBlock(&blocks[blocksInPlay+2]);
                // Have block fall
                blocks[blocksInPlay].SetXnY(blocks[blocksInPlay].GetXnLength()[0],blocks[blocksInPlay].GetYnHeight()[0]+0.8);
                blocks[blocksInPlay].GetImage().Draw(blocks[blocksInPlay].GetXnLength()[0],blocks[blocksInPlay].GetYnHeight()[0]);
                LCD.Update();
            }
            towerHeight+=blocks[blocksInPlay].GetYnHeight()[1];
            if(CheckTowerFall(&(blocks[0]), blocksInPlay)) { //If tower falls, end game and display results
                (*game_blocks) = blocksInPlay;
                for (int i=0; i<blocksInPlay; i++) {
                    (*game_height)+=blocks[i].GetYnHeight()[1];
                }
                LCD.WriteAt("You lose...", 160, 20);
                Sleep(5.0);
                return;
            } else if (towerHeight >= 160) {
                (*game_blocks) = blocksInPlay;
                for (int i=0; i<blocksInPlay; i++) {
                    (*game_height)+=blocks[i].GetYnHeight()[1];
                }
                LCD.WriteAt("You win!", 160, 20);
                Sleep(5.0);
                return;
            }
            else { blocksInPlay+=1; }
        }
        (*game_blocks) = blocksInPlay;
        for (int i=0; i<blocksInPlay; i++) {
            (*game_height)+=blocks[i].GetYnHeight()[1];
        }
        return;
    }
}