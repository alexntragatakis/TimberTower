#include "FEHLCD.h"
#include <FEHImages.h>
#include <math.h>

class Block {
    private:
        int shape;
        FEHImage image;
        // Dimensions
        float x_pos;
        float y_pos;
        float height;
        float length;
        float mass;
        // Physical properties
        float centerOfMass;
    public:
        void SetXnY(float x, float y);

        float * GetXnLength();
        float * GetYnHeight();
        float GetMass();
        float GetCenterOfMass();
        FEHImage GetImage();

        int RandomizeBlock();
        void CalculatePhysicalProps();
};

class Tower {
    private:
        class Block b[200];
        int blockCount;
        float towerCOM;
    public:
        void AddBlockCount();

        bool CheckTowerFall();
        bool CheckBlockFall();
};

void PlayGame();
void DisplayResults();

int main()
{
    FEHImage menuBg, woodLong, woodTall, woodSquare;
    float x_pos, y_pos; //coords clicked
    float x_trash, y_trash; //coords released at
    int total_games = 0, total_blocks = 0, tallest_tower = 0;

    /* Background image for menu screen */
    menuBg.Open("MenuBG.png");
    menuBg.Draw(0, -10);

    /* Images for building materials */
    woodLong.Open("WoodPlankLong.png");
    woodTall.Open("WoodPlankTall.png");
    woodSquare.Open("WoodPlankSquare.png");
    
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
            PlayGame();     
            //LCD.SetFontColor(BLACK);
            //LCD.FillRectangle(0, 0, 500, 500);
            //LCD.SetFontColor(WHITE);
            DisplayResults();
            LCD.Write("Play game here");
            LCD.WriteAt("<Quit>", 280, 200);
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
            LCD.WriteAt("<Quit>", 280, 200);
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
            LCD.WriteAt("<Quit>", 280, 200);
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
            LCD.WriteAt("<Quit>", 280, 200);
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

void Block::SetXnY(float x, float y) {
    x_pos=x;
    y_pos=y;
}

float * Block::GetXnLength() {
    float array[2];
    array[0] = x_pos;
    array[1] = length;
    return array;
}

float * Block::GetYnHeight() {
    float array[2];
    array[0] = y_pos;
    array[1] = height;
    return array;
}

float Block::GetMass() {
    return mass;
}

float Block::GetCenterOfMass() {
    return centerOfMass;
}

FEHImage Block::GetImage() {
    return image;
}

int Block::RandomizeBlock() {
    shape = rand() % 3;
    mass = rand() % 41 + 10;
    if(shape == 0) {
        height = 12;
        length = 36;
        image.Open("WoodPlankLong.png");
    }
    else if(shape == 1) {
        height = 36;
        length = 12;
        image.Open("WoodPlankTall.png");
    }
    else if(shape == 2) {
        height = 24;
        length = 24;
        image.Open("WoodPlankSquare.png");
    }
}

void Block::CalculatePhysicalProps() {
    // Center of mass for both rectangles and box
    centerOfMass = x_pos + (0.5)*length;
}

void Tower::AddBlockCount() { blockCount+=1; }

bool Tower::CheckBlockFall() {
    // While block isn't hitting a previous block's y, make it fall
    while (b[blockCount-1].GetYnHeight()[0]+b[blockCount-1].GetYnHeight()[1] < b[blockCount-2].GetYnHeight()[0]) {
        // Draw over previous shape
        // Draw new shape over previous shape
    }

    // If the center of mass is outside the block's length, it will tip and fall
    if (b[blockCount-1].GetCenterOfMass() < b[blockCount-2].GetXnLength()[0]
    && b[blockCount-1].GetCenterOfMass() > b[blockCount-2].GetXnLength()[0]+b[blockCount-2].GetXnLength()[1]) {
        return true; // Block will tip over and fall
    }
    else {
        return false;
    }
}

bool Tower::CheckTowerFall() {
    // Get the tower center of mass
    float towerCOM=0;
    float totalMass=0;
    for (int i=0; i<blockCount; i++) {
        towerCOM+=(b[i].GetMass() * b[i].GetCenterOfMass());
        totalMass+=b[i].GetMass();
    }
    towerCOM/=totalMass;

    // Check if tower center of mass is outside the platform
    // TODO: make actual platform dimensions (this assumes 100 < x < 200)
    if (towerCOM < 100 || towerCOM > 200) {
        return true; // tower tips over
    }
    else {
        return false;
    }

    // Check if center of mass for blocks on another block are on the length of the block
    for (int i=0; i<blockCount; i++) {
        int platformBlockIndex = i;
        float aboveCOM = 0;
        float aboveTotalMass=0;
        for (int j=i+1; j<blockCount; j++) { // Loop through blocks above the platformBlock
            // Calculate aboveCOM
            aboveCOM+=(b[j].GetMass() * b[j].GetCenterOfMass());
            aboveTotalMass+=b[j].GetMass();
        }
        aboveCOM/=aboveTotalMass;

        if (aboveCOM < b[i].GetXnLength()[0] || aboveCOM > b[i].GetXnLength()[0]+b[i].GetXnLength()[1]) {
            return true;
        }
    }
    return false; // Runs through the loop and nothing falls
}

void DisplayResults() {
    LCD.SetFontColor(STEELBLUE);
    LCD.FillRectangle(0, 0, 320, 240);
    LCD.SetFontColor(LIGHTSTEELBLUE);
    LCD.FillRectangle(25, 15, 270, 210);
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(25, 15, 270, 210);
    LCD.WriteAt("Results", 140, 25);
    LCD.WriteAt("Tower Height:", 50, 50);
    LCD.WriteAt("Number of Blocks:", 50, 70);
}




void addNextBlock(class Block *block) {
    block->RandomizeBlock();
    block->CalculatePhysicalProps();
    block->SetXnY(250,250); // todo: Set coords where "next" placeholder is
    block->GetImage().Draw(200,120);
    LCD.Update();
}

void addToPlayBlock(class Block *block) {
    block->SetXnY(100,100); // todo: Set coords where "play" placeholder is
    block->GetImage().Draw(120,120);
    LCD.Update();
}

void dragBlock(class Block *block, int count) { // Index of dragged block is count-1
    float x_pos, y_pos;
    while (LCD.Touch(&x_pos, &y_pos)) {
    // Print background
    
    // Reprint previous blocks
        for (int i=0; i<count-1; i++) {
            block[i].GetImage().Draw(block[i].GetXnLength()[0], block[i].GetYnHeight()[0]);
        }
    // Print next block
        block[count].GetImage().Draw(block[count].GetXnLength()[0], block[count].GetYnHeight()[0]);
    // Draw dragged block
        block[count-1].GetImage().Draw(x_pos,y_pos);
        LCD.Update();
    }
}

void PlayGame() {
    bool playing = true;
    class Block blocks[100];
    while (playing) {
        // Print Background Image

        // Create the first block
        blocks[0].RandomizeBlock();
        blocks[0].CalculatePhysicalProps();
        addToPlayBlock(&blocks[0]);
        // Create next block
        addNextBlock(&blocks[1]);
    }
}