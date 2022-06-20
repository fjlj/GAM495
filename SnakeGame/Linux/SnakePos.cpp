// SnakePos.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string.h>
#include <future>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include "TinyMatrix.h"


struct InputVars
{
    char dir;
    bool running;
    bool drawn;
    bool show;
    int width = 10;
    double score = 0.0;
};

int rand_food_pos(int width, int height, int border_tb, int border_lr) {
    int f_x = (rand() % (width-(border_lr*2))) + border_lr;
    if(f_x % 2 == 0) f_x += (f_x == 2 ? 1 : -1);
    int f_y = (rand() % (height-(border_tb*2))) + border_tb;
    return width * f_y + f_x;
}

int getMapHeight(char* map) {
    int lCount = 0;
    while(*map != 0x00){
        if (*map == '\n') lCount++;
        map++;
    }
    return lCount;
}
int getMapWidth(char* map) {
    return (strstr(map, "\n") - map) + 1;
}

int mapToUniqInt(char* map) {
    int r = getMapHeight(map);
    int c = getMapWidth(map);
    TinyMatrix Hme(r, c, 0,(int16_t*)map,0xfe);
    Hme.dot(Hme.multiply(-2).Ints());
    Hme.multiply(((int)Hme.sum() % 10));
    return (int)Hme.sum()/5 % 2147483620;
}

char red[] = "\033[91m"; //12
char color_off[] = "\033[00m"; //5
char green[] = "\033[92m";
char blue[] = "\033[95m";
char yellow[] = "\033[93m";

int game(InputVars *iVars)
{

    char map[] =" xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " x                                                                               x\n"
                " xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n";

    /*char map[] = " xxxxxxx\n"
                 " x     x\n"
                 " x     x\n"
                 " x     x\n"
                 " x     x\n"
                 " x     x\n"
                 " xxxxxxx\n";*/

    char screen[(sizeof(map) / sizeof(char)) * 4]{ 0x00 };


    int s_segments[sizeof(map) / sizeof(char)]{ 0x00 };

    int height = getMapHeight(map);
    uint8_t border_TB = 1;
    uint8_t border_LR = 2;
    int width = (sizeof(map) / sizeof(char)) / height;

    iVars->width = width;
    int dirs[5] = { width * -1,width,-2,2,0 };
    int segments = 1;
    int speed_calc;
    std::thread::id m_id = std::this_thread::get_id();
    srand((unsigned int)time(NULL) + rand() + *(int*)&m_id);

    int s_h_pos = ((height / 2) * width) + (width / 2);
    int f_pos = rand_food_pos(width, height, border_TB, border_LR);

    bool exit = false;
    bool win = false;
    unsigned long long life_timer = 0, run_score = 0, score = 0;
    int apple_timer = 100;
    long hunger = 500;

    int speed = 100;

    map[s_h_pos] = '*';
    map[f_pos] = '@';

    while (!exit) {
        s_h_pos += dirs[iVars->dir];
        s_segments[s_h_pos] = segments;


        for (int i = 0; i < sizeof(s_segments) / sizeof(int); i++) {
            if (s_segments[i] > 0) {

                if (map[i] == 'x' || (map[i] == '*' && s_h_pos == i && iVars->dir != 4) || iVars->running == false) {
                    exit = true;
                }
                if (map[i] == '@') {
                    int max_seg = (height - (border_TB * 2)) * (width - (border_LR * 2));
                    segments += 1;
                    score += (unsigned long long)apple_timer * 2.5;
                    score += (unsigned long long)hunger * 0.33;
                    apple_timer = (int)(sqrt(((height*height)+(width * width))) + 1);
                    hunger += (long)((1.25f - ((float)segments/((float)max_seg*0.75f)))*100);
                    s_segments[i]++;
                    map[i] = '*';
                    
                    if (segments >= max_seg) {
                        win = true;
                        exit = true;
                    }
                    f_pos = rand_food_pos(width, height, border_TB, border_LR);
                    while (s_segments[f_pos] > 0 || map[f_pos] != ' ' || f_pos == 0 || map[f_pos-1] == '\n') {
                        f_pos = rand_food_pos(width, height, border_TB, border_LR);
                    }
                    map[f_pos] = '@';
                }
                map[i] = '*';
                s_segments[i]--;
            }
            else if (map[i] == '*') {
                map[i] = ' ';
            }
        }
        
        //set console cursor position to top left
        printf("\033[1;1H");


        speed_calc = (int)((1.0l - ((180.0l * (0.851l * (1.01l - ((double)segments / 165.01l)))) / 153.18l)) * 100);
        if (iVars->dir != 4) score += ((life_timer % 3) / 2); 
        run_score = score + (20 * ((double)segments - 1));


        printf("\n SEGMENTS:%-4dSCORE:%-9lluSPEED:% 3d%%\n UID:%d", segments, run_score, speed_calc, mapToUniqInt(map));
        
        int screen_pos = 0;
        sprintf(screen,"\n%s", blue);
        screen_pos += strlen(blue)+1;
        
        for (int o = 0; o < (sizeof(map) / sizeof(map[0])); o++) {
            switch ((map[o]))
            {
            case '@':
                sprintf(screen + screen_pos, "%s@%s", green, blue);
                screen_pos += (strlen(green) + strlen(blue) + 1);
                break;
            case '*':
                if (map[o] - 1 != '*')
                {
                    sprintf(screen + screen_pos, "%s", yellow);
                    screen_pos += strlen(yellow);
                }

                screen[screen_pos] = '*';
                screen_pos++;

                if (map[o] + 1 != '*')
                {
                    sprintf(screen + screen_pos, "%s", blue);
                    screen_pos += strlen(blue);
                }
                break;
            case '\x00':
                break;
            default:
                screen[screen_pos] = map[o];
                screen_pos++;
                break;
            }
        }
        sprintf(screen + screen_pos, "%s", color_off);
        printf("%s", screen);

        speed = (int)(180.0l * (0.851l * (1.01l - ((double)segments / 165.01l))));
        std::this_thread::sleep_for(std::chrono::milliseconds((int)roundf(speed*1.12f)));

        if (apple_timer > 0) {
            apple_timer--;
        }
       else {
            hunger--;
            if(hunger <= 0){
                exit = true;
            }
        }

        iVars->score = run_score;
        iVars->drawn = true;
        life_timer++;
    }
    iVars->drawn = true;
    
    //set console cursor position to top left
    printf("\033[1;1H");

    //q was pressed
    if(!iVars->running) return 5;
    //hit a wall or we have won            
    iVars->dir = 4;
    if(win) return 4;
    //hit a wall or ourselves
    return 1;
}

void play(InputVars* iVars) {
    int qflag = 0;
    bool notprinted = true;
    while (qflag != 5) {
        if(qflag > 0 && notprinted){
            notprinted = false;
            printf("\033c");
            printf("\033[1;1H");
            printf("%sGAME OVER - SCORE:%ld\n%s\n%s%s\n", red,(unsigned long)iVars->score, 
                   "PRESS Q to EXIT","PRESS W/A/S/D to PLAY AGAIN!", color_off);
        }
        if(qflag == 0){
            printf("\033c");
            printf("\033[1;1H");
            printf("%sPRESS W/A/S/D to PLAY - Q to QUIT!\n",green);
        }
        
        if(iVars->running || qflag == 0) {
           if(iVars->dir != 4 || qflag == 0){
               if(qflag != 0)printf("\033c");
               qflag = game(iVars);
               notprinted = true;
           }
        } else {
            qflag = 5;
        }

    }
    printf("\033c");
    printf("%sThank You For Playing!\n",yellow);
}

void get_keyboard_input(InputVars *iVars) {
  char input;
  while ((iVars->running || iVars->dir == 4) && read(STDIN_FILENO, &input, 1) > 0) {
    if (iVars->drawn || iVars->dir == 4) {
      if(input == 'w' && iVars->dir != 1 && iVars->dir != 0) {iVars->dir = 0; iVars->drawn = false;}
      if(input == 's' && iVars->dir != 0 && iVars->dir != 1) {iVars->dir = 1; iVars->drawn = false;}
      if(input == 'a' && iVars->dir != 3 && iVars->dir != 2) {iVars->dir = 2; iVars->drawn = false;}
      if(input == 'd' && iVars->dir != 2 && iVars->dir != 3) {iVars->dir = 3; iVars->drawn = false;}
      if(input == 'q'){
        iVars->drawn = false;
        iVars->running = false; 
        iVars->dir = 0;      
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}

struct termios orig_termios;
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();
    InputVars* iVars = new InputVars;

    iVars->running = true;
    iVars->dir = 4;

    auto f = std::async(std::launch::async, get_keyboard_input,iVars);
    while (f.wait_for(std::chrono::milliseconds(20)) != std::future_status::ready)
    {
        play(iVars);
    }
    disableRawMode();
    return 0;
}

