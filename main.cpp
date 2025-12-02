#include "FEHLCD.h"
#include <FEHImages.h>

int main()
{
    FEHImage menuBg;
    float x_pos, y_pos; //coords clicked
    float x_trash, y_trash; //coords released at

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
    LCD.WriteAt("Leaderboard", 55, 188);
    LCD.WriteAt("Credits", 203, 188);

    /* Display next screen from menu */
    while(!LCD.Touch(&x_pos, &y_pos)) {} // Wait for touch
    while(LCD.Touch(&x_trash, &y_trash)); // Wait for release

    if(x_pos > 40 && x_pos < 140 && y_pos > 115 && y_pos < 155) { // Play
        LCD.Write("Play clicked");
    }
    else if(x_pos > 175 && x_pos < 275 && y_pos > 115 && y_pos < 155) { // How to Play
        LCD.Write("How to Play clicked");
    }
    else if(x_pos > 40 && x_pos < 140 && y_pos > 175 && y_pos < 215) { // Leaderboard
        LCD.Write("Leaderboard clicked");
    }
    else if(x_pos > 175 && x_pos < 275 && y_pos > 175 && y_pos < 215 ) { // Credits
        LCD.Write("Credits clicked");
    }

    while (1) {
        
        LCD.Update();
        // Never end
    }
    return 0;
}
