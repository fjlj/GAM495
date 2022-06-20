
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <thread>
#include <chrono>

//define names for key codes
#define KEY_W 0x57
#define KEY_S 0x53
#define KEY_A 0x41
#define KEY_D 0x44

//structure to pass from play to game, used for logging states and scores
struct GameVars
{
    WORD dir;
    bool running;
    bool drawn;
    int width = 10;
    double score = 0.0;
    bool demo = false;
};

//function to accept user input for directions or escape
void getDirKey(GameVars* iVars) {
    //the 4 direction indicies, followed by their opposite direction index
    WORD inv_dirs[] = { 1, 0, 3, 2, 0, 1, 2, 3};
    //keys W/A/S/D and arrows up/down/left/right
    int check_keys[] = {KEY_W,KEY_S,KEY_A,KEY_D,VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT};
    
    int itr = 0;
    do {
            //iterate over our keys, whose positions map to inv_dirs % 4
            while (itr < 8) {
                //check if key is being pressed or has been pressed since last check
                if (((GetAsyncKeyState(check_keys[itr]) & 0x01) || (GetAsyncKeyState(check_keys[itr]) < 0)) 
                    //check that the direction being pressed is not the current direction or its opposite
                    && ((iVars->dir != inv_dirs[itr % 4] && iVars->dir != inv_dirs[(itr % 4) + 4]) || iVars->demo)) {
                    //used when a game has ended before a new one begins or player quits
                    if (iVars->dir == 4 && !iVars->drawn) {
                        //clear the gameover message and prompt a new game
                        for (int i = 0; i < iVars->width; i++) {
                            printf(" ");
                        }
                        //reset running to true so a new game will begin
                        iVars->running = true;
                        break;
                    }
                    else {
                        //if in demo mode, exit demo...
                        if (iVars->demo) {
                            iVars->running = false;
                            break;
                        }
                        else {
                            //game is not over sooo update the direction to the iterators correct direction
                            iVars->dir = inv_dirs[(itr % 4) + 4];
                            iVars->drawn = false;
                            break;
                        }
                    }
                }
                //check next key
                itr++;
            }
            //reset iterator incase we are at the end of the game and this is looping
            itr = 0;
        //check if escape is pressed, if it is set the direction to something that
        //will not cause movement or the loop to continue
        if ((GetAsyncKeyState(VK_ESCAPE) & 0x01)) {
            iVars->running = false;
            iVars->demo = false;
            iVars->dir = 5;
        }
        //used to trap execution when gameover has been reached 
    } while(!iVars->running && iVars->dir == 4);
    return;
}

//calculates a random food position based on width,height, and border sizes
int rand_food_pos(int width, int height, int border_tb, int border_lr) {
    int f_x = (rand() % (width-(border_lr*2))) + border_lr;
    int f_y = (rand() % (height-(border_tb*2))) + border_tb;
    return width * f_y + f_x;
}

//get the map height via counting the number of newline characters
int getMapHeight(char* map) {
    int lCount = 0;
    while(*map != 0x00){
        if (*map == '\n') lCount++;
        map++;
    }
    return lCount;
}

void consoleSetup(HANDLE hStdout, int width=0, int height=0) {
    //the following is used to set the console font and size
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;

    //allow using ansi escape codes to draw colors
    DWORD dwMode;
    GetConsoleMode(hStdout, &dwMode);
    dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hStdout, dwMode);

    //set font to one that is monospaced
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 20;                   // Width of each character in the font
    cfi.dwFontSize.Y = 20;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_BOLD;
    wcscpy_s(cfi.FaceName, L"Cascadia Mono SemiBold"); // Choose font

    //set font
    SetCurrentConsoleFontEx(hStdout, true, &cfi);

    //more window sizing junk
    SMALL_RECT rect;
    COORD coord;
    coord.X = width; // Defining our X and
    coord.Y = height + 3;  // Y size for buffer.

    rect.Top = 0;
    rect.Left = 0;
    rect.Bottom = coord.Y; // height for window
    rect.Right = coord.X - 1;  // width for window


    SetConsoleScreenBufferSize(hStdout, coord);       // set buffer size
    SetConsoleWindowInfo(hStdout, true, &rect);       //set size of window
    ShowScrollBar(GetConsoleWindow(), SB_BOTH, 0);    //get rid of scrollbars
    SetConsoleCursorInfo(hStdout, &info);             //hide the cursor

}

//calculate direction toward specific point, from a specific point
int dirToPoint(int x1, int x2, int y1, int y2) {
    //cheap/easy distance between 2 points method
    int ud = (y2 - y1); 
    int lr = (x2 - x1); 
    unsigned int a_ud = abs(ud);
    unsigned int a_lr = abs(lr);

    if (ud > 0 && a_ud > a_lr)  return 1; //down is shortest path to point
    if (ud < 0 && a_ud >= a_lr) return 0; //up is shortest path to point
    if (lr > 0 && a_lr > a_ud)  return 3; //right is shortest path to point
    if (lr < 0 && a_lr >= a_ud) return 2; //left is shortest path to point

    return 1; //failsafe
}

int game(GameVars *iVars)
{
    //console variables to control cursor
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    //top left of console defined as a point to allow setcursorposition
    COORD org;
    org.X = 0;
    org.Y = 0;

    //ansi escape codes for various colors or defaulting
    char red[] = "\033[91m"; //12
    char color_off[] = "\033[00m"; //5
    char green[] = "\033[92m";
    char magenta[] = "\033[95m";
    char yellow[] = "\033[93m";
    char blue[] = "\033[94m";

    //design our playfield
    char map[] = " xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " x                                       x\n"
        " xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";

    //create a screen buffer to write to before writing to output
    //essentially double buffering, we allocate 4x map size to allow
    //for space needed for ansi color codes.
    char screen[(sizeof(map) / sizeof(char)) * 4]{ 0x00 };

    //create an int array to store a number reprisenting the segments
    //of the snake used to draw until the number is 0...
    int s_segments[sizeof(map) / sizeof(char)]{ 0x00 };

    //extract the height of the map via counting newline characters
    int height = getMapHeight(map);
    
    //define variables for map border sizes.
    uint8_t border_TB = 1;
    uint8_t border_LR = 2;

    //determine width of map based off overall size and height.
    int width = (sizeof(map) / sizeof(char)) / height;

    //calculate max_segments possible, used to check for win condition
    int max_segments = (height - (border_TB * 2)) * (width - (border_LR * 2));

    iVars->width = width;

    //size and configure console window and font.
    consoleSetup(hStdout,width, height);

    //since our map is a 1 dimensional array moving up or down is equivilent to
    //moving forward or backward by the map width. 0 used for paused/beginning of game
    int dirs[6] = { width * -1,width,-1,1,0,0};
    int segments = 1;

    //seed random using time...
    srand((unsigned int)time(NULL));

    //starting snake head position (middle)
    int s_h_pos = ((height / 2) * width) + (width / 2);

    //initial food position randomized
    int f_pos = rand_food_pos(width, height, border_TB, border_LR);

    //have we died/pressed escape or won?
    bool exit = false;
    bool win = false;

    //variables used to track how long the game has persisted,
    //hunger and the overall score, current speed. initialize to 0
    iVars->score = 0.0;
    unsigned long long life_timer = 0;
    int apple_timer = 100;
    long hunger = 500;
    int speed_calc = 0;
    

    //set initial head position and food position of the map respectively
    map[s_h_pos] = '*';
    map[f_pos] = '@';

    //game loop
    while (!exit) {
        //poll user input or run demo
        if (iVars->demo) {
            int opp_dirs[] = { 1,0,3,2,4 };
            //try to go toward food
            int rand_dir = dirToPoint(s_h_pos % width, f_pos % width, s_h_pos / width, f_pos / width);
            //if the resulting direction would result in death try 10 times to get a new direction
            int tries = 0;
            while(tries < 10 && (map[s_h_pos + dirs[rand_dir]] == 'x' || map[s_h_pos + dirs[rand_dir]] == '*' || rand_dir == opp_dirs[iVars->dir])) {
                rand_dir = rand() % 4;
                tries++;
            }
            iVars->dir = rand_dir;
        }
        getDirKey(iVars);

        //update head position
        s_h_pos += dirs[iVars->dir];
        //set head position in segments to number of segments
        s_segments[s_h_pos] = segments;

        //check for collisions or exit trigger
        for (int i = 0; i < sizeof(s_segments) / sizeof(int); i++) {
            //step through segments map, if the value is greater than 0,
            //the snake occupies this space on the map
            if (s_segments[i] > 0) {
                //if the character at the same position of the map contains
                //an x or a * while we are looking at the head position and
                //we are moving... OR the player has pressed escape, set the exit code
                if (map[i] == 'x' || (map[i] == '*' && s_h_pos == i && iVars->dir != 4 && !iVars->demo) || iVars->running == false) {
                    exit = true;
                }
                //if we are colliding with the food...
                if (map[i] == '@') {
                    //increase the segments count
                    segments += 1;
                    //increase score by remaining timer to reach apple * 2.5
                    iVars->score += iVars->demo ? 0 : (unsigned long long)apple_timer * 2.5;
                    
                    //increase score by 1/3 of remaining hunger
                    iVars->score += iVars->demo ? 0 : (unsigned long long)hunger * 0.33;

                    //reset timer to reach apple to the area of the map + 1
                    //(enough time to reach anywhere even when snake is very long
                    apple_timer = (int)(sqrt(((height*height)+(width * width))) + 1);
                    
                    //increase hunger by 25% more than % of winning the game... segments/max_segments
                    hunger += (long)((1.25f - ((float)segments/((float)max_segments*0.75f)))*100);

                    //grow the snake by increasing this positions seg count in seg map
                    s_segments[i]++;
                    
                    //check if player has won
                    if (segments >= max_segments) {
                        win = true;
                        exit = true;
                    }

                    //generate a new random food position
                    f_pos = rand_food_pos(width, height, border_TB, border_LR);
                    
                    //ensure the food position is not generated ontop of the snake,
                    //check that segments map is a 0, ensure it is generated from a 
                    //position that is a space character, ensure it is not in a space
                    //that is outside the bounds of the map.
                    while (s_segments[f_pos] > 0 || map[f_pos] != ' ' || map[f_pos-1] == '\n') {
                        f_pos = rand_food_pos(width, height, border_TB, border_LR);
                    }
                    //set that map position to a new food character
                    map[f_pos] = '@';
                }

                //since we are looking at segments... we need to set this position
                //to be a segment and decrease the count.
                map[i] = '*';
                s_segments[i]--;
            }
            //remove segments that are no longer valid, <= 0
            else if (map[i] == '*') {
                map[i] = ' ';
            }
        }

        //draw intro text if running the intro demo
        if (iVars->demo) {
            const char* intro_text[] = { "BHello! Welcome!N","BControls: WASD or ARROW keys.N","BPress ESC to exit!N" };
            int beginning_pos = (iVars->width * (height / 3)) + (iVars->width / 4) + 3;
            memcpy(map + beginning_pos, intro_text[0], strlen(intro_text[0]));
            memcpy(map + (iVars->width * ((height / 3)+3)) + (iVars->width / 4)-3, intro_text[1], strlen(intro_text[1]));
            memcpy(map + (iVars->width * ((height / 3)+9)) + (iVars->width / 4)+2, intro_text[2], strlen(intro_text[2]));
        }
        //prepare for drawing updated information by setting cursor position to top left
        SetConsoleCursorPosition(hStdout, org);

        //handle win/loss
        if (exit) {
            if (win) {
                printf(" %sYOU WIN!!%s - ESC = EXIT\n", green, color_off);
            }
            else if(!iVars->demo) {
                printf(" %sGAME OVER%s%s", red, (iVars->running == true ? " | ESC = EXIT | W/A/S/D = PLAY" : " - Thank You For Playing!"), color_off);
            }
        }

        //calculate the "speed" of the snake based off of % of game completion
        speed_calc = (((double)(segments*4)/(double)max_segments)*100.0);
        
        //clamp to a max of 100
        speed_calc = min(speed_calc, 100);

        //if moving (game is active) increase the score by a portion of lifetimer
        if (iVars->dir < 4 && !iVars->demo) iVars->score += ((life_timer % 3) / 2);
        
        //print UI bar for current segments/score/speed
        printf("\n SEGMENTS:%-4dSCORE:%-9lluSPEED:% 3d%%", segments, (long long)iVars->score, speed_calc);
        
        //track position of the screen separately from map data position
        //ANSI escape codes for color require 5 spaces in mose cases.
        //so we have to advance the screen position to account for this
        int screen_pos = 0;
        //set color for the initial border line and write to screen
        sprintf_s(screen,sizeof(screen),"\n%s", magenta);
        //advance screen position by length of magenta
        screen_pos += strlen(magenta)+1;
        
        //loop through each position of map data
        for (int o = 0; o < (sizeof(map) / sizeof(map[0])); o++) {
            switch ((map[o]))
            {
            case '@':
                //if food, write food with color code, then set back to magenta
                sprintf_s(screen + screen_pos, sizeof(screen) - screen_pos, "%s@%s", green, magenta);

                //advance screen position correctly
                screen_pos += (strlen(green) + strlen(magenta) + 1);
                break;
            case 'B':
                //if B set color to blue (used for demo)
                sprintf_s(screen + screen_pos, sizeof(screen) - screen_pos, "%s ", blue);

                //advance screen position correctly
                screen_pos += (strlen(green) + 1);
                break;
            case 'N':
                //used for demo, set back to magenta
                sprintf_s(screen + screen_pos, sizeof(screen) - screen_pos, "%s ", magenta);

                //advance screen position correctly
                screen_pos += (strlen(magenta) + 1);
                break;
            case '*':
                //if we come accross a snake segment and the previous map section was not a snake segment
                //set the color to yellow for the snake and advance position
                if (map[o] - 1 != '*')
                {
                    sprintf_s(screen + screen_pos, sizeof(screen) - screen_pos, "%s", yellow);
                    screen_pos += strlen(yellow);
                }

                //add the snake segment and advance position
                screen[screen_pos] = '*';
                screen_pos++;

                //if the next section of map data is not still a segment... set color back to magenta
                //and advance position correctly
                if (map[o] + 1 != '*')
                {
                    sprintf_s(screen + screen_pos, sizeof(screen) - screen_pos, "%s", magenta);
                    screen_pos += strlen(magenta);
                }
                break;
            case '\x00':  //end of map data reached we should just break this should only happen at the end of the data
                screen[screen_pos] = ' ';
                screen_pos++;
                break;
            default: //nothing special... jut write the map data into the screen and advance position
                screen[screen_pos] = map[o];
                screen_pos++;
                break;
            }
        }
        //finally turn the color back off and output the overall image to the console
        sprintf_s(screen + screen_pos, sizeof(screen) - screen_pos, "%s\x00", color_off);
        printf("%s", screen);

        //sleep in the game loop (determines speed of snake)
        //shorest possible sleep is 60ms, largest 160ms
        std::this_thread::sleep_for(std::chrono::milliseconds((100 - speed_calc) + 60));
        
        //if the time to reach apple is above 0 decrement it
        if (apple_timer > 0) {
            apple_timer--;
        }
        //if we have not reached the apple in time begin losing hunger
       else {
            hunger--;
            //if your hunger reaches 0.. game over (this is only possible if done intentionally at this time)
            if(hunger <= 0){
                exit = true;
            }
        }
        
        //increment lifetime count
        life_timer++;
    }
    
    //set direction to paused and running to false to capture execution in a input function
    if (iVars->running && !iVars->demo) {
        iVars->dir = 4;
    }
    iVars->running = false;
    
    // reset cursor position
    SetConsoleCursorPosition(hStdout, org);
    
    //call input function, execution will stay until either escape or a direction key is pressed
    getDirKey(iVars);

    if (iVars->demo)
        return 1;

    return 0;
}

void play() {
    GameVars* iVars = new GameVars;
    //demo setup
    iVars->running = true;
    iVars->dir = 1;
    iVars->demo = true;
    iVars->drawn = true;
    int exit_check = game(iVars);

    //set running only if the demo game ended by means other than pressing escape.
    iVars->running = exit_check > 0;
    //run game until user decides they are done...
    while (iVars->running) {
        iVars->drawn = true;
        iVars->demo = false;

        game(iVars);

    }

    delete iVars;
}

int main() {
    play();
    return 0;
}

