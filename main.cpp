#include "FEHLCD.h"
#include <FEHImages.h>
#include <math.h>

class Block { // 3x1 (long), 1x3 (tall), 2x2
    private:
        int shape;
        // Dimensions
        float x_pos;
        float y_pos;
        float height;
        float length;
        float mass;
        // Physical properties
        float momentOfInertia;
        float centerOfMass;
    public:
        int RandomizeBlock();
        void CalculatePhysicalProps();
};

void BlockFall();
bool CheckTowerFall();
void DisplayResults();

int main()
{
    FEHImage menuBg;
    float x_pos, y_pos; //coords clicked
    float x_trash, y_trash; //coords released at
    int counter;
    Block blocks[100];
    int total_games = 0, total_blocks = 0, tallest_tower = 0; //for DisplayResults()

    /* Background image for menu screen */
    menuBg.Open("MenuBG.png");
    menuBg.Draw(0, -10);
    
    /* Buttons on menu */ 
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

    while (1) {
        /* Display next screen from menu */
        while(!LCD.Touch(&x_pos, &y_pos)) {} // Wait for touch
        while(LCD.Touch(&x_trash, &y_trash)); // Wait for release

        if(x_pos > 40 && x_pos < 140 && y_pos > 115 && y_pos < 155) { // Play        
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 500, 500);
            LCD.SetFontColor(WHITE);
            LCD.Write("Play game here");
            LCD.WriteAt("< Quit >", 280, 200);
        }
        else if(x_pos > 175 && x_pos < 275 && y_pos > 115 && y_pos < 155) { // How to Play
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 500, 500);
            LCD.SetFontColor(WHITE);
            LCD.WriteAt("How to Play:", 120, 20);
            LCD.WriteAt("You will be given a random block and its mass", 20, 70);
            LCD.WriteAt("Stack the block on the platform or other blocks", 15, 90);
            LCD.WriteAt("Keep the center of gravity of the tower centered", 15, 110);
            LCD.WriteAt("If the tower becomes too unstable, it will fall", 20, 130);
            LCD.WriteAt("< Quit >", 280, 200);
        }
        else if(x_pos > 40 && x_pos < 140 && y_pos > 175 && y_pos < 215) { // Stats/Leaderboard
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 500, 500);
            LCD.SetFontColor(WHITE);
            LCD.WriteAt("Statistics:", 120, 20);
            LCD.WriteAt("Total Games Played:", 60, 70);
            LCD.WriteAt("Tallest Tower:", 60, 100);
            LCD.WriteAt("Total Blocks Stacked:", 60, 130);
            /* Example stats */
            LCD.WriteAt("4", 200, 70);
            LCD.WriteAt("20 m", 200, 100);
            LCD.WriteAt("52", 200, 130);
            LCD.WriteAt("< Quit >", 280, 200);
        }
        else if(x_pos > 175 && x_pos < 275 && y_pos > 175 && y_pos < 215 ) { // Credits
            LCD.SetFontColor(BLACK);
            LCD.FillRectangle(0, 0, 500, 500);
            LCD.SetFontColor(WHITE);
            LCD.WriteAt("Credits:", 130, 20);
            LCD.WriteAt("Developers:", 120, 70);
            LCD.WriteAt("Alexander Ntragatakis and Jennifer Lee", 42, 90);
            LCD.WriteAt("Menu Background Image:", 95, 120);
            LCD.WriteAt("freepik.com", 120, 140);
            LCD.WriteAt("< Quit >", 280, 200);
        }
        LCD.Update();
        
        while(!LCD.Touch(&x_pos, &y_pos)) {} // Wait for touch
        while(LCD.Touch(&x_trash, &y_trash)); // Wait for release
        if(x_pos > 280 && x_pos < 300 && y_pos > 200 && y_pos < 220) { // Quit
            /* Background image for menu screen */
            menuBg.Open("MenuBG.png");
            menuBg.Draw(0, -10);
            
            /* Buttons on menu */
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
        }
        LCD.Update();
    }
    return 0;
}

int Block::RandomizeBlock() {
    shape = rand() % 3;
    mass = rand() % 41 + 10;
    if(shape == 0) {
        height = 1;
        length = 3;
    }
    else if(shape == 1) {
        height = 3;
        length = 1;
    }
    else if(shape == 2) {
        height = 2;
        length = 2;
    }
}

void Block::CalculatePhysicalProps() {
    // TODO: Create moment of inertia calculations for each shape
    // Rectangle
    momentOfInertia = (1/12)*mass*(pow(height,2)+pow(length,2));

    // TODO: Create center of gravity calculations for each shape
    // Rectangle
    centerOfMass = x_pos + (1/2)*length;
}
