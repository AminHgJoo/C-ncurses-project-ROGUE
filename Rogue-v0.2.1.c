#include <ncurses.h>    //403110585 Mohammad Amin Haghjoo
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


#define RED_COLORED_CELL COLOR_PAIR(3)
#define GREEN_COLORED_CELL COLOR_PAIR(4)
#define GOLD_COLORED_CELL COLOR_PAIR(1)
#define BLUE_COLORED_CELL COLOR_PAIR(5)
#define WHITE_COLORED_CELL COLOR_PAIR(6)
#define GOLD_HIGHLIGHT COLOR_PAIR(2)
#define RED_HIGHLIGHT COLOR_PAIR(8)
#define GREEN_HIGHLIGHT COLOR_PAIR(9)
#define BLUE_HIGHLIGHT COLOR_PAIR(10)
#define WHITE_HIGHLIGHT COLOR_PAIR(11)
#define ORANGE_COLORED_CELL COLOR_PAIR(7)
#define COLOR_ORANGE COLOR_CYAN// Cyan has been changed into Orange.





typedef struct point {

    char SYMBOL;

    char REAL_CONTENTS; 

    char IS_DISCOVERED;

    char PASSWORD_DOOR_TYPE;// '1' '2' 't' 'r' 'l' 1, 2, time, reverse, locked out!

    char COLOR; //y: yellow/golden | r :red | b: blue | w: white | gold : treasure room
                //red : nightmare | blue: spell room | white : regular | 'o'(orange) for password door warning system

    char IS_START; //1 means start, -1 means end

} POINT;

typedef struct settings {

    char HERO_COLOR;

    char MUSIC;

    char DIFFICULTY;

} SETTINGS;

typedef struct door {

    int x;

    int y;

    int SECTOR;

    char DIRECTION;

} DOOR;

typedef struct coords {

    int x;

    int y;

}   COORDS;

typedef struct inventory {//this struct must be saved

    int x;

    int y;

    int FLOOR;

    int KEYS;

    int BROKEN_KEYS;

    int HUNGER; int HP; int SCORE; int GOLD; int FOOD_ITEMS;//all 0 at start except HP/HUNGER

    int H_SPELL,S_SPELL,D_SPELL; //all 0 at start

    int MACE,DAGGER,WAND,ARROW,SWORD; //all 0 at start, except mace which is 1

    int TIME_PLAYED;



} INVENTORY_AND_STATS;




//----------------------------------------------Player Inventory/misc Handling--------------------------------------//

SETTINGS GLOBAL_SETTINGS;

INVENTORY_AND_STATS GLOBAL_INVENTORY_AND_STATS;//must be saved at exit

int GLOBAL_LOAD_SAVE=FALSE;

int ROW,COLUMN;

int GLOBAL_IS_GUEST=0;

char GLOBAL_LOGGED_IN_USER[100];

int GLOBAL_EXIT_CODE=0;        // 0 is no exiting, 1 is unsaved exit, 2 is end_of_game exit, 3 is saved and quit

int GLOBAL_SHOW_ALL=FALSE;

int GLOBAL_MAX_HP; //no need to save. determined based on settings.

unsigned long long int GLOBAL_LAST_HUNGER_TIME_QUERRY;

char GLOBAL_ALL_THEMES_COLOR[4][6]; //'w' is default. 'r' |red is nightmare. 'y' | yellow/gold is treasure. 'b'|blue is enchant room.

//----------------------------------------------Message Handling--------------------------------------------------------------------//

char GLOBAL_MOST_RECENT_MESSAGE[100];

char GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE;

//-----------------------------------------------Passkey Doors Handling-------------------------------------------------------------//

unsigned long long int GLOBAL_MOST_RECENT_PASSWORD_REQUEST_TIME=0;

int GLOBAL_DOOR_PASSWORD=-1; 

int GLOBAL_IS_PASSWORD_DYNAMIC=FALSE;

int GLOBAL_PASSWORD_DOOR_BACKGROUND_CHECK=FALSE;

//---------------------------------------------------Game Menus & startup/end---------------------------------------------------------//

void HANDLE_GUEST_USER();
char GAME_STARTUP_MENU();
void USER_REGISTERATION();
void HANDLE_NEXT_MENUS(char);
int USER_LOGIN();
void CLEAR_TEXT_FOR_LOGIN_ERRORS();
void RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();
void GENERATE_RANDOM_PASSWORD(char[]);
int VALIDATE_EMAIL(char[]);
int PRE_GAME_MENU();
void PRINT_PREGAME_MENU();
void OPEN_LEADERBOARDS();
void OPEN_SETTINGS();
void PRINT_SETTINGS_MENU();
void OPEN_MUSIC_MENU();
void OPEN_DIFFICULTY_MENU();
void OPEN_HERO_COLOR_MENU();
void PRINT_COLOR_GUI();
void LOAD_USER_SETTINGS();
void HANDLE_SETTINGS();
void PRINT_DIFFICULTY_MENU();
void PRINT_MUSIC_MENU();
void OPEN_PAUSE_MENU();
void PRINT_PAUSE_MENU();
void START_GAME();
void SHUTDOWN_GAME(Mix_Music *);
void SAVE_GAME(POINT [4][38][144],int );
void OPEN_FOOD_MENU();
void OPEN_WEAPONS_MENU();
void OPEN_SPELLS_MENU();

//-----------------------------------------------------In Game Functions(World Gen)----------------------------------------------------//

void PRINT_MESSAGE(char []);

void CLEAR_MESSAGE_BAR();

void TURN_OFF_COLOR_FOR_MAP_PRINT(char);

void TURN_ON_COLOR_FOR_MAP_PRINT(char);

int GIVE_SECTOR_NUMBER(int , int );

void LOAD_MAP_AND_INVENTORY(POINT [4][38][144]);

void GENERATE_MAP(POINT [4][38][144]);

int GENERATE_FLOOR_SKELETON(POINT [38][144], int ,int );

void CONNECT_ROOMS_AND_RETURN_DOORS(POINT [38][144], DOOR [14]);

void GENERATE_PILLARS(POINT [38][144]);

int GENERATE_ENTRY_EXIT( POINT [38][144], int );

COORDS GIVE_STAIRS_ADDRESS_ENTRY(POINT [38][144]);

COORDS GIVE_STAIRS_ADDRESS_EXIT(POINT [38][144]);

int FIND_SPECIFIED_DOOR(char ,  int , DOOR [14]);

void GENERATE_ANCIENT_KEY(POINT [38][144]);   

//-------------------------------------------In Game Functions(Movement & Exploration)------------------------------------------------//

void HANDLE_USER_INPUT_AND_MOVE_HERO(char *,POINT [4][38][144]);

int PROCESS_PLAYER_INPUT(int, POINT [4][38][144], char *);

void PRINT_MAP_ON_SCREEN(POINT [4][38][144]);

void UPDATE_EXPLORATION_AND_HANDLE_INTERACTIVE_OBJECTS(POINT [4][38][144]);

void REVEAL_ENTIRE_SECTOR(POINT [4][38][144],int );

void REVEAL_HALLWAYS(POINT [4][38][144], char,int ,int ,int );

void FIND_MAX_HP();

void PERFORM_DEATH_CHECK();

void OPEN_DEATH_MENU();

void HANDLE_TRAP_ACTIVATION();

//--------------------------------------------------Handling Passkey Doors-------------------------------------------------------------//

void PASSWORD_BACKGROUND_CHECK();

void HANDLE_DOOR_PASSKEY_GENERATION(POINT [4][38][144]);

void OPEN_PASSKEY_GUI(POINT [4][38][144]);

int HANDLE_ANCIENT_KEY_USAGE();

//-------------------------------------------------Item Pickups Handling------------------------------------------------------------------//

void HANDLE_ITEM_PICKUPS(POINT [4][38][144],char *);

void REDUCE_HUNGER();

//-------------------------------------------------------------main-----------------------------------------------------------------------//

int main() {

    START_GAME();

    char NEXT_MENU=GAME_STARTUP_MENU();

    HANDLE_NEXT_MENUS(NEXT_MENU);

    HANDLE_SETTINGS();

    PRE_GAME_MENU();

    POINT MAP[4][38][144];

    if(GLOBAL_LOAD_SAVE==TRUE) {

        LOAD_MAP_AND_INVENTORY(MAP);

    }
    else {

        GENERATE_MAP(MAP);   

    }

    FIND_MAX_HP();

    SDL_Init(SDL_INIT_AUDIO);   Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);  Mix_Music *music = NULL;

    switch(GLOBAL_SETTINGS.MUSIC) {

        case 'w' : 
            music=Mix_LoadMUS("the_weeknd_sacrifice.mp3");

            break;
        case 's' :
            music=Mix_LoadMUS("Sleep Away.mp3");
            break;
        case 'r' :
            music=Mix_LoadMUS("Rick-Astley-Never-Gonna-Give-You-Up.mp3");
            break;

    }

    Mix_PlayMusic(music, -1);

    unsigned long long int START_TIME=time(NULL); 

    GLOBAL_LAST_HUNGER_TIME_QUERRY=START_TIME;

    halfdelay(10); //1 sec delay

    char TRANSITION_STORAGE='.';    UPDATE_EXPLORATION_AND_HANDLE_INTERACTIVE_OBJECTS(MAP);    PRINT_MAP_ON_SCREEN(MAP);

    while(GLOBAL_EXIT_CODE==0) {//game loop//////////////////// more background checks needed

        HANDLE_USER_INPUT_AND_MOVE_HERO(&TRANSITION_STORAGE,MAP);

        REDUCE_HUNGER();

        CLEAR_MESSAGE_BAR();

        UPDATE_EXPLORATION_AND_HANDLE_INTERACTIVE_OBJECTS(MAP);

        PRINT_MAP_ON_SCREEN(MAP);

        if(GLOBAL_PASSWORD_DOOR_BACKGROUND_CHECK==TRUE) {

            PASSWORD_BACKGROUND_CHECK();

        }

        PERFORM_DEATH_CHECK();

    }   

    unsigned long long int FINISH_TIME=time(NULL);

    if(GLOBAL_EXIT_CODE==3) {

        SAVE_GAME(MAP,FINISH_TIME-START_TIME+GLOBAL_INVENTORY_AND_STATS.TIME_PLAYED);

    }

    if(GLOBAL_EXIT_CODE==2) {//update scoreboard. display victory message//////////////////////////////////////////////////////



    } 
    
    SHUTDOWN_GAME(music);

    return 0;
}

//----------------------------------------------------Function Definitions---------------------------------------------------------------//

char GAME_STARTUP_MENU() {

    attron(COLOR_PAIR(1));

    mvprintw(ROW-1,0,"v1.0.0 Developed By Mohammad Amin Haghjoo");

    mvprintw(ROW-1,COLUMN/2-22,"Press e or E and arrow keys to Navigate Menus");

    attron(A_ITALIC);

    mvprintw(ROW/4,COLUMN/2 -2,"Rogue");

    attroff(A_ITALIC);

    mvprintw(3*ROW/4,COLUMN/2-8,"Register New User");

    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2) | A_BLINK);

    mvprintw(ROW/2,COLUMN/2 -10 ,"Login or Play as Guest");

    attroff(COLOR_PAIR(2)); attroff(A_BLINK);

    move(ROW/2,COLUMN/2);

    int input;

    do {
        int x,y;

        getyx(stdscr,y,x);

        input=getch();

        if((input=='e' || input=='E' ) && y==ROW/2) {

            return 'p'; //play & login

        }
        else if((input=='e' || input=='E') && y==3*ROW/4) {
            
            return 'r'; //register new user

        }
        else if(input==KEY_DOWN && y==ROW/2) {

            attron(COLOR_PAIR(1));
            
            mvprintw(ROW/2,COLUMN/2 -10 ,"Login or Play as Guest");

            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(2) | A_BLINK);

            mvprintw(3*ROW/4,COLUMN/2-8,"Register New User");

            attroff(COLOR_PAIR(2)); attroff(A_BLINK);

            move(3*ROW/4,COLUMN/2);

        }
        else if(input==KEY_UP && y==3*ROW/4) {

            attron(COLOR_PAIR(1));

            mvprintw(3*ROW/4,COLUMN/2-8,"Register New User");

            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(2) | A_BLINK);

            mvprintw(ROW/2,COLUMN/2 -10 ,"Login or Play as Guest");

            attroff(COLOR_PAIR(2)); attroff(A_BLINK);

            move(ROW/2,COLUMN/2);

        }

        refresh();

    }

    while(TRUE);
}
void HANDLE_NEXT_MENUS(char NEXT_MENU) {

    if(NEXT_MENU=='r') {

        USER_REGISTERATION();

    }
    else if (NEXT_MENU=='p') {
    
        USER_LOGIN();

    }
}
void USER_REGISTERATION() {

    clear();

    echo();

    curs_set(1);

    FILE * USERS=fopen("passwords.txt","a+");

    attron(COLOR_PAIR(1));

    mvprintw(ROW/3,COLUMN/2 -11,"Enter Your Username :");

    attron(A_ITALIC);

    mvprintw(ROW/3 +1, COLUMN/2 -46 ,"Note : Username Must Not Contain Spaces, Be Already Taken, or Be Longer Than 15 Characters");

    attroff(A_ITALIC);

    mvprintw(2*ROW/3 +1 ,COLUMN/2 - 7 ,"###############");

    move(2*ROW/3 , COLUMN/2 -7);

    refresh();

    char USERNAME[100];

    while(TRUE) { //get valid username

        getstr(USERNAME);

        if(strlen(USERNAME)==0) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3 +2 , COLUMN/2 -9,"Username Is Empty");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            continue;

        }

        if(strlen(USERNAME)>15) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3+2 , COLUMN/2 -14 ,"Username Is Longer Than 15");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            continue;

        }

        if(strchr(USERNAME,' ')!=NULL) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3+2 , COLUMN/2 -13 ,"Username Contains Spaces");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            continue;

        }

        int IS_DUPLICATE=FALSE;

        while(TRUE) {

            char USER_RECORDS[16];

            if(fscanf(USERS," %s",USER_RECORDS)!=EOF) {

                if(strcmp(USER_RECORDS,USERNAME)==0) {

                    IS_DUPLICATE=TRUE;

                    CLEAR_TEXT_FOR_LOGIN_ERRORS();

                    mvprintw(ROW/3+2, COLUMN/2 -12 , "Username Already Taken");

                    RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

                    rewind(USERS);

                    break;

                }
                
                fscanf(USERS," %s",USER_RECORDS);

            }

            else {

                break;

            }
        }

        if(IS_DUPLICATE==FALSE)
            break;
    }

    char PASSWORD[100];

    clear();

    mvprintw(ROW/3,COLUMN/2 -24,"Choose a Password. Enter r For a Random Password");

    attron(A_ITALIC);

    mvprintw(ROW/3 +1, COLUMN/2 -54 ,"Note : Password Must Be Longer Than 6 and Shorter Than 16; Must Have a Number, Lowercase, and Capital Letter");

    attroff(A_ITALIC);

    mvprintw(2*ROW/3 +1 ,COLUMN/2 - 7 ,"###############");

    move(2*ROW/3 , COLUMN/2 -7);

    refresh();

    //get valid password, clear error bar and text box each time

    while(TRUE) {

        getstr(PASSWORD);

        if(strcmp(PASSWORD,"r")==0 || strcmp(PASSWORD,"R")==0) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3 +2 ,COLUMN/2 -26,"Random Password Generated. Press a Key to continue.");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            GENERATE_RANDOM_PASSWORD(PASSWORD);

            mvprintw(2*ROW/3,COLUMN/2 -strlen(PASSWORD)/2 ,"%s",PASSWORD);

            refresh();

            noecho();

            getch();

            echo();

            break;

        }

        if(strlen(PASSWORD)<7 || strlen(PASSWORD) >15) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3+2,COLUMN/2 -12 ,"Invalid Password Length");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            continue;
        }

        int HAS_DIGIT=FALSE;

        for(char i='0' ; i<='9' ; i++) { //check for digit

            if(strchr(PASSWORD,i)!=NULL) {

                HAS_DIGIT=TRUE;

                break;

            }
        }

        if(HAS_DIGIT==FALSE) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3 +2,COLUMN/2 -10 ,"Doesn't Have a Digit");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            continue;
        }       

        int HAS_LOWER=FALSE;

        for(char i='a' ; i<='z' ; i++) { //check for lowercase

            if(strchr(PASSWORD,i)!=NULL) {
                
                HAS_LOWER=TRUE;

                break;

            }
        }

        if(HAS_LOWER==FALSE) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3 + 2,COLUMN/2-11 ,"Doesn't Have Lowercase");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            continue;

        }

        int HAS_CAPITAL=FALSE;

        for(char i='A' ; i<='Z' ; i++) { //check for capital

            if(strchr(PASSWORD,i)!=NULL) {

                HAS_CAPITAL=TRUE;

                break;

            }

        }

        if(HAS_CAPITAL==FALSE) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3 +2,COLUMN/2 -10 ,"Doesn't Have Capital");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            continue;

        }

        break;
    }

    char EMAIL[100];

    clear();

    mvprintw(ROW/3,COLUMN/2-9,"Enter Your E-Mail");

    attron(A_ITALIC);

    mvprintw(ROW/3 +1,COLUMN/2 -27  ,"Entered E-Mail Must Be Valid and Not Be Longer than 40");

    attroff(A_ITALIC);

    mvprintw(2*ROW/3 +1, COLUMN/2 -20  ,"########################################");

    move(2*ROW/3, COLUMN/2 -20);

    refresh();
    //get valid email, clear error bar and text box each time
    
    while(TRUE) {

        getstr(EMAIL);

        if(strlen(EMAIL)>40) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3 +2 , COLUMN/2 -9 ,"Email Is Too Long");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            move(2*ROW/3, COLUMN/2 -20);

            refresh();

            continue;
        }

        if(strchr(EMAIL,' ')!=NULL || strchr(EMAIL,'@')==NULL || strchr(EMAIL,'.')==NULL) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3 +2 ,COLUMN/2 -8  ,"Email Is Invalid");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            move(2*ROW/3, COLUMN/2 -20);

            refresh();

            continue;
        }

        if(VALIDATE_EMAIL(EMAIL)==FALSE) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3 +2 ,COLUMN/2 -8  ,"Email Is Invalid");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            move(2*ROW/3, COLUMN/2 -20);

            refresh();

            continue;

        }
        break;
    }

    fprintf(USERS,"%s %s\n",USERNAME,PASSWORD);

    //store saved game and default settings later

    fclose(USERS);

    strcpy(GLOBAL_LOGGED_IN_USER,USERNAME);

    char FILE_NAME_FOR_USER[100]="saves_";

    strcat(FILE_NAME_FOR_USER,USERNAME);

    strcat(FILE_NAME_FOR_USER,".txt");

    FILE *SCOREBOARD=fopen("user_scores.txt","a+");

    fprintf(SCOREBOARD,"%s %d %d %d %d ",USERNAME,0,0,0,0);

    fclose(SCOREBOARD);

    FILE *SETTINGS=fopen("user_settings.txt","a+");

    fprintf(SETTINGS,"%s g r n ",USERNAME);

    fclose(SETTINGS);

    FILE *SAVES=fopen(FILE_NAME_FOR_USER,"a+"); //create that .txt file.

    fclose(SAVES);

    noecho();

    curs_set(0);

    CLEAR_TEXT_FOR_LOGIN_ERRORS();

    RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

    mvprintw(ROW/3+ 3,COLUMN/2 - 25,"Registeration Successful. Press a Key to Continue.");

    refresh();

    getch();

    attroff(COLOR_PAIR(1));

    

}
int USER_LOGIN() {

    clear();

    attron(COLOR_PAIR(1));

    echo();

    curs_set(1);

    FILE * USER_PASSWORD_LIST=fopen("passwords.txt","r+");

    mvprintw(ROW/3,COLUMN/2 -37  ,"Enter Your Username. First Time ? Press r to Register or g to Play as Guest");

    mvprintw(2*ROW/3+1, COLUMN/2 - 7,"###############");

    move(2*ROW/3, COLUMN/2 -7);

    refresh();

    char USERNAME[100];

    char PASSWORD[100];

    char CORRECT_PASSWORD[100];

    while (TRUE)  //check username
    {
        
        getstr(USERNAME);

        if(strcmp(USERNAME,"r")==0 || strcmp(USERNAME,"R")==0) {

            USER_REGISTERATION();

            return 1;

        }

        if(strcmp(USERNAME,"g")==0 || strcmp(USERNAME,"G")==0) {

            GLOBAL_IS_GUEST=TRUE;

            strcpy(GLOBAL_LOGGED_IN_USER,"Guest");

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            curs_set(0);

            mvprintw(ROW/3 +3 ,COLUMN/2 -26,"Temporary Login Successful. Press a Key to Continue.");

            refresh();

            getch();

            return 1;

        }

        int USER_EXISTS=FALSE;

        char USER_FROM_FILE[20];

        while(TRUE) {

            if(fscanf(USER_PASSWORD_LIST," %s",USER_FROM_FILE)!=EOF) {

                if(strcmp(USER_FROM_FILE,USERNAME)==0) {

                    USER_EXISTS=TRUE;

                    break;

                }

                fscanf(USER_PASSWORD_LIST," %s",USER_FROM_FILE);

                continue;

            }

            rewind(USER_PASSWORD_LIST);

            break;
        }

        if(USER_EXISTS==FALSE) {

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            mvprintw(ROW/3 +2,COLUMN/2 -11 ,"Username Doesn't Exist");

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            continue;
        }

        fscanf(USER_PASSWORD_LIST," %s",CORRECT_PASSWORD);

        break;
    }
    
    //password gui and forgot my password

    clear();

    mvprintw(ROW/3,COLUMN/2 -23,"Enter Your Password. Press f if You Forgot it.");

    mvprintw(2*ROW/3+1, COLUMN/2 - 7,"###############");

    move(2*ROW/3, COLUMN/2 -7);

    refresh();

    while(TRUE) {

        getstr(PASSWORD);

        if(strcmp(PASSWORD,CORRECT_PASSWORD)==0) {

            break;

        }

        if(strcmp("f",PASSWORD)==0 || strcmp("F",PASSWORD)==0) {

            mvprintw(ROW/3 +1,COLUMN/2 -18 ,"Hint : Your Password's Length is %lu.",strlen(CORRECT_PASSWORD));

            CLEAR_TEXT_FOR_LOGIN_ERRORS();

            RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

            continue;

        }

        CLEAR_TEXT_FOR_LOGIN_ERRORS();

        mvprintw(ROW/3 +2,COLUMN/2 -7,"Wrong Password");

        RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();
    }

    //display a welcome message and prompt to continue

    CLEAR_TEXT_FOR_LOGIN_ERRORS();

    RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

    curs_set(0);

    mvprintw(ROW/3 +3,COLUMN/2 -21 ,"Login Successful. Press a Key to Continue.");

    strcpy(GLOBAL_LOGGED_IN_USER,USERNAME);

    noecho();

    getch();

    attroff(COLOR_PAIR(1));

    curs_set(0);

    fclose(USER_PASSWORD_LIST);

}
void CLEAR_TEXT_FOR_LOGIN_ERRORS() {

    mvprintw(ROW/3+2 , COLUMN/2 -30 ,"                                                            ");

    mvprintw(2*ROW/3 , COLUMN/2-55 , "                                                                                                             ");

    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(3));
}
void RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR() {

    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(1));

    move(2*ROW/3 , COLUMN/2 -7);

    refresh();

}
void GENERATE_RANDOM_PASSWORD(char PASSWORD[]) {

    PASSWORD[0]=rand()%26 +'a';

    PASSWORD[1]=rand()%26 +'A';

    PASSWORD[2]=rand()%10 +'0';

    for(int i=3 ; i<7 ; i++) {

        PASSWORD[i]=rand()%94+'!';

    }

    PASSWORD[7]='\0';

}
int VALIDATE_EMAIL(char EMAIL[]) {
    int index=strlen(EMAIL)-1;

    while(EMAIL[index]!='.') {
        index--;
    }

    if(index==strlen(EMAIL)-1)
        return FALSE;

    if(EMAIL+index-strchr(EMAIL,'@')>=2)
        return TRUE;

    else 
        return FALSE;


}
int PRE_GAME_MENU() {

    PRINT_PREGAME_MENU();

    while(TRUE) {

        int INPUT=getch();

        int x,y;

        getyx(stdscr,y,x);

        if(y==ROW/5) {

            if(INPUT==KEY_DOWN) {

                attron(COLOR_PAIR(1));

                mvprintw(ROW/5,COLUMN/2 -4,"New Game");

                attroff(COLOR_PAIR(1));

                attron(COLOR_PAIR(2) | A_BLINK);

                mvprintw(2*ROW/5,COLUMN/2 -7,"Load Last Save");

                attroff(COLOR_PAIR(2)); attroff(A_BLINK);

                refresh();

            }

            else if(INPUT=='e' || INPUT=='E') {

                return 0;
            }
        }

        else if(y==2*ROW/5) {

            if(INPUT==KEY_UP) {

                attron(COLOR_PAIR(1));

                mvprintw(2*ROW/5,COLUMN/2 -7,"Load Last Save");

                attroff(COLOR_PAIR(1));

                attron(COLOR_PAIR(2) |A_BLINK);

                mvprintw(ROW/5,COLUMN/2 -4,"New Game");

                attroff(COLOR_PAIR(2)); attroff(A_BLINK);

                refresh();

            }
            if(INPUT==KEY_DOWN) {

                attron(COLOR_PAIR(1));

                mvprintw(2*ROW/5,COLUMN/2 -7,"Load Last Save");

                attroff(COLOR_PAIR(1));

                attron(COLOR_PAIR(2) |A_BLINK);

                mvprintw(3* ROW/5, COLUMN/2 -6,"Leaderboards");

                attroff(COLOR_PAIR(2)); attroff(A_BLINK);

                refresh();

            }
            if(INPUT=='e' || INPUT=='E') {

                GLOBAL_LOAD_SAVE=TRUE;

                return 0;

            }

        }

        else if(y==3* ROW/5) {
            
            if(INPUT==KEY_UP) {

                attron(COLOR_PAIR(1));

                mvprintw(3* ROW/5, COLUMN/2 -6,"Leaderboards");

                attroff(COLOR_PAIR(1));

                attron(COLOR_PAIR(2) |A_BLINK);

                mvprintw(2*ROW/5,COLUMN/2 -7,"Load Last Save");

                attroff(COLOR_PAIR(2)); attroff(A_BLINK);

                refresh();
            }

            else if(INPUT==KEY_DOWN) {

                attron(COLOR_PAIR(1));

                mvprintw(3* ROW/5, COLUMN/2 -6,"Leaderboards");

                attroff(COLOR_PAIR(1));

                attron(COLOR_PAIR(2) |A_BLINK);

                mvprintw(4*ROW/5 , COLUMN/2 -4,"Settings");

                attroff(COLOR_PAIR(2)); attroff(A_BLINK);

                refresh();
            }

            else if(INPUT=='e' || INPUT=='E') {//open leaderboards and then refresh the page once you are back

                OPEN_LEADERBOARDS();
                
                PRINT_PREGAME_MENU();

            }
        }

        else {//4/5 row

            if(INPUT==KEY_UP) {

                attron(COLOR_PAIR(1));

                mvprintw(4*ROW/5 , COLUMN/2 -4,"Settings");

                attroff(COLOR_PAIR(1));

                attron(COLOR_PAIR(2) | A_BLINK);

                mvprintw(3* ROW/5, COLUMN/2 -6,"Leaderboards");

                attroff(COLOR_PAIR(2)); attroff(A_BLINK);

                refresh();

            }

            else if(INPUT=='e' || INPUT=='E') {

                OPEN_SETTINGS();

                PRINT_PREGAME_MENU();

            }
        }

        

    }

}
void PRINT_PREGAME_MENU() {

    clear();

    noecho();

    curs_set(0);

    attron(COLOR_PAIR(2) | A_BLINK);

    mvprintw(ROW/5,COLUMN/2 -4,"New Game");

    attroff(COLOR_PAIR(2)); attroff(A_BLINK);

    attron(COLOR_PAIR(1));

    mvprintw(2*ROW/5,COLUMN/2 -7,"Load Last Save");

    mvprintw(3* ROW/5, COLUMN/2 -6,"Leaderboards");

    mvprintw(4*ROW/5 , COLUMN/2 -4,"Settings");

    mvprintw(ROW-1,COLUMN/2 - 22,"Press e or E to Select and Arrow keys to Move");

    mvprintw(0,0,"Welcome %s!",GLOBAL_LOGGED_IN_USER);

    move(ROW/5,COLUMN/2 -4);

    attroff(COLOR_PAIR(1));

    refresh();
}
void OPEN_LEADERBOARDS() {///////////////////////////////////////////////////////////////////////////////
    
    FILE* SCORES_FILE=fopen("user_scores.txt","r");









    if(GLOBAL_IS_GUEST==FALSE) {//show non-guest users' ranking



    }

    fclose(SCORES_FILE);

}
void OPEN_SETTINGS() {

    PRINT_SETTINGS_MENU();

    while(TRUE) {

    int INPUT=getch();

    int x,y;

    getyx(stdscr,y,x);

    if(y==ROW/5) {

        if(INPUT==KEY_DOWN) {

            attron(COLOR_PAIR(1));

            mvprintw(ROW/5,COLUMN/2 - 5 ,"Difficulty");

            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(2) | A_BLINK);

            mvprintw(2*ROW/5,COLUMN/2 -5  ,"Hero Color");

            attroff(COLOR_PAIR(2)); attroff(A_BLINK);

            refresh();

        }
        else if(INPUT=='e' || INPUT=='E') {
            
            OPEN_DIFFICULTY_MENU();

            PRINT_SETTINGS_MENU();

        }
    }
    else if(y==2*ROW/5) {

        if(INPUT==KEY_DOWN) {

            attron(COLOR_PAIR(1));

            mvprintw(2*ROW/5,COLUMN/2 -5  ,"Hero Color");

            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(2) | A_BLINK);

            mvprintw(3*ROW/5 , COLUMN/2 - 3 ,"Music");

            attroff(COLOR_PAIR(2)); attroff(A_BLINK);

            refresh();

        }
        else if(INPUT==KEY_UP) {

            attron(COLOR_PAIR(1));

            mvprintw(2*ROW/5,COLUMN/2 -5  ,"Hero Color");

            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(2) | A_BLINK);

            mvprintw(ROW/5,COLUMN/2 - 5 ,"Difficulty");

            attroff(COLOR_PAIR(2)); attroff(A_BLINK);

            refresh();

        }
        else if(INPUT=='e' || INPUT=='E') {

            OPEN_HERO_COLOR_MENU();

            PRINT_SETTINGS_MENU();
        }
    }

    else if(y==3*ROW/5) {

        if(INPUT==KEY_DOWN) {

            attron(COLOR_PAIR(1));

            mvprintw(3*ROW/5 , COLUMN/2 - 3 ,"Music");

            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(2) | A_BLINK);

            mvprintw(4*ROW/5 , COLUMN/2 - 2 ,"Back");

            attroff(COLOR_PAIR(2)); attroff(A_BLINK);

            refresh();
        }

        else if(INPUT==KEY_UP) {

            attron(COLOR_PAIR(1));

            mvprintw(3*ROW/5 , COLUMN/2 - 3 ,"Music");

            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(2) | A_BLINK);

            mvprintw(2*ROW/5,COLUMN/2 -5  ,"Hero Color");           

            attroff(COLOR_PAIR(2)); attroff(A_BLINK);

            refresh();
        }

        else if(INPUT=='e' || INPUT=='E') {

            OPEN_MUSIC_MENU();

            PRINT_SETTINGS_MENU();

        }
    }
    else {
        
        if(INPUT==KEY_UP) {

            attron(COLOR_PAIR(1));

            mvprintw(4*ROW/5 , COLUMN/2 - 2 ,"Back");

            attroff(COLOR_PAIR(1));

            attron(COLOR_PAIR(2) | A_BLINK);

            mvprintw(3*ROW/5 , COLUMN/2 - 3 ,"Music");

            attroff(COLOR_PAIR(2)); attroff(A_BLINK);

            refresh();

        }
        else if(INPUT=='e' || INPUT=='E') {
            
            break;

        }
    }

    }
}
void PRINT_SETTINGS_MENU() {

    clear();

    attron(COLOR_PAIR(1));

    mvprintw(0,0,"Welcome %s!",GLOBAL_LOGGED_IN_USER);

    mvprintw(2*ROW/5,COLUMN/2 -5  ,"Hero Color");

    mvprintw(3*ROW/5 , COLUMN/2 - 3 ,"Music");

    mvprintw(4*ROW/5 , COLUMN/2 - 2 ,"Back");

    mvprintw(ROW-1,COLUMN/2 - 22,"Press e or E to Select and Arrow keys to Move");

    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2) | A_BLINK);

    mvprintw(ROW/5,COLUMN/2 - 5 ,"Difficulty");

    attroff(COLOR_PAIR(2)); attroff(A_BLINK);

    refresh();

}
void OPEN_MUSIC_MENU() {

    PRINT_MUSIC_MENU();

    while(TRUE) {

        int x,y;

        getyx(stdscr,y,x);

        char LARGE_STRING[70]="Current Music is : ";

        char COLOR_SPELLED_OUT[100];

        if(GLOBAL_SETTINGS.MUSIC=='s') {
            strcpy(COLOR_SPELLED_OUT,"Sleep Away");
        }
        else if(GLOBAL_SETTINGS.MUSIC=='w') {
            strcpy(COLOR_SPELLED_OUT,"Sacrifice");
        }
        else if(GLOBAL_SETTINGS.MUSIC=='r'){
            strcpy(COLOR_SPELLED_OUT,"Never Gonna Give You Up");
        }

        strcat(LARGE_STRING,COLOR_SPELLED_OUT);

        attrset(GOLD_COLORED_CELL);

        mvprintw(1,COLUMN/2-36 ,"                                                                        ");

        mvprintw(1,COLUMN/2 -strlen(LARGE_STRING)/2,"%s",LARGE_STRING);

        attroff(GOLD_COLORED_CELL);

        int INPUT=getch();

        if(y==ROW/5) {

            if(INPUT==KEY_DOWN) {

                attron(GOLD_COLORED_CELL);

                mvprintw(ROW/5, COLUMN/2 -12 ,"Never Gonna Give You Up");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(2*ROW/5, COLUMN/2 -5 ,"Sleep Away");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);                

            }
            else if(INPUT=='e' || INPUT=='E') {

                GLOBAL_SETTINGS.MUSIC='r';

                move(ROW/5,0);

            }
            else {
                move(ROW/5,0);
            }

        }
        else if(y==2*ROW/5) {

            if(INPUT==KEY_UP) {

                attron(GOLD_COLORED_CELL);

                mvprintw(2*ROW/5, COLUMN/2 -5 ,"Sleep Away");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(ROW/5, COLUMN/2 -12 ,"Never Gonna Give You Up");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT==KEY_DOWN) {

                attron(GOLD_COLORED_CELL);

                mvprintw(2*ROW/5, COLUMN/2 -5 ,"Sleep Away");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(3*ROW/5, COLUMN/2 -5 ,"Sacrifice");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT=='e' || INPUT=='E') {
                GLOBAL_SETTINGS.MUSIC='s';

                move(2*ROW/5,0);
            }
            else {
                move(2*ROW/5,0);
            }
        }
        else if(y==3*ROW/5) {

            if(INPUT==KEY_UP) {

                attron(GOLD_COLORED_CELL);

                mvprintw(3*ROW/5, COLUMN/2 -5 ,"Sacrifice");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(2*ROW/5, COLUMN/2 -5 ,"Sleep Away");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT==KEY_DOWN) {

                attron(GOLD_COLORED_CELL);

                mvprintw(3*ROW/5, COLUMN/2 -5 ,"Sacrifice");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(4*ROW/5, COLUMN/2 -2 ,"Back");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT=='e' || INPUT=='E') {
                GLOBAL_SETTINGS.MUSIC='w';

                move(3*ROW/5,0);
            }
            else {
                move(3*ROW/5,0);
            }
        }
        else {

            if(INPUT==KEY_UP) {

                attron(GOLD_COLORED_CELL);

                mvprintw(4*ROW/5, COLUMN/2 -2 ,"Back");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(3*ROW/5, COLUMN/2 -5 ,"Sacrifice");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT=='e' || INPUT=='E') {
                
                break;

            }
            else {
                move(4*ROW/5,0);
            }
        }

    }

    if(GLOBAL_IS_GUEST==FALSE) {//write final result in file

        FILE* SETTINGS_FILE=fopen("user_settings.txt","r");

        char TEMP_STRING[100000];

        fgets(TEMP_STRING,100000,SETTINGS_FILE);

        char *PTR=strstr(TEMP_STRING,GLOBAL_LOGGED_IN_USER); 

        PTR+=strlen(GLOBAL_LOGGED_IN_USER)+1;//move PTR to first char location g r n    \green rickroll normal/

        PTR[2]=GLOBAL_SETTINGS.MUSIC;

        fclose(SETTINGS_FILE);

        FILE *WRITE_MODE_PTR=fopen("user_settings.txt","w");

        fprintf(WRITE_MODE_PTR,"%s",TEMP_STRING);

        fclose(WRITE_MODE_PTR); 

    }


}
void OPEN_DIFFICULTY_MENU() {

    PRINT_DIFFICULTY_MENU();

    while(TRUE) {

        int x,y;

        getyx(stdscr,y,x);

        char LARGE_STRING[70]="Current Difficulty is : ";

        char COLOR_SPELLED_OUT[10];

        if(GLOBAL_SETTINGS.DIFFICULTY=='h') {
            strcpy(COLOR_SPELLED_OUT,"Hard");
        }
        else if(GLOBAL_SETTINGS.DIFFICULTY=='e') {
            strcpy(COLOR_SPELLED_OUT,"Easy");
        }
        else if(GLOBAL_SETTINGS.DIFFICULTY=='n'){
            strcpy(COLOR_SPELLED_OUT,"Normal");
        }

        strcat(LARGE_STRING,COLOR_SPELLED_OUT);

        attrset(GOLD_COLORED_CELL);

        mvprintw(1,COLUMN/2-36 ,"                                                                        ");

        mvprintw(1,COLUMN/2 -strlen(LARGE_STRING)/2,"%s",LARGE_STRING);

        attroff(GOLD_COLORED_CELL);

        int INPUT=getch();

        if(y==ROW/5) {

            if(INPUT==KEY_DOWN) {

                attron(GOLD_COLORED_CELL);

                mvprintw(ROW/5, COLUMN/2 -2 ,"Easy");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(2*ROW/5, COLUMN/2 -3 ,"Normal");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);                

            }
            else if(INPUT=='e' || INPUT=='E') {

                GLOBAL_SETTINGS.DIFFICULTY='e';

                move(ROW/5,0);

            }
            else {
                move(ROW/5,0);
            }

        }
        else if(y==2*ROW/5) {

            if(INPUT==KEY_UP) {

                attron(GOLD_COLORED_CELL);

                mvprintw(2*ROW/5, COLUMN/2 -3 ,"Normal");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(ROW/5, COLUMN/2 -2 ,"Easy");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT==KEY_DOWN) {

                attron(GOLD_COLORED_CELL);

                mvprintw(2*ROW/5, COLUMN/2 -3 ,"Normal");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(3*ROW/5, COLUMN/2 -2 ,"Hard");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT=='e' || INPUT=='E') {
                GLOBAL_SETTINGS.DIFFICULTY='n';

                move(2*ROW/5,0);
            }
            else {
                move(2*ROW/5,0);
            }
        }
        else if(y==3*ROW/5) {

            if(INPUT==KEY_UP) {

                attron(GOLD_COLORED_CELL);

                mvprintw(3*ROW/5, COLUMN/2 -2 ,"Hard");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(2*ROW/5, COLUMN/2 -3 ,"Normal");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT==KEY_DOWN) {

                attron(GOLD_COLORED_CELL);

                mvprintw(3*ROW/5, COLUMN/2 -2 ,"Hard");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(4*ROW/5, COLUMN/2 -2 ,"Back");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT=='e' || INPUT=='E') {
                GLOBAL_SETTINGS.DIFFICULTY='h';

                move(3*ROW/5,0);
            }
            else {
                move(3*ROW/5,0);
            }
        }
        else {

            if(INPUT==KEY_UP) {

                attron(GOLD_COLORED_CELL);

                mvprintw(4*ROW/5, COLUMN/2 -2 ,"Back");

                attroff(GOLD_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(3*ROW/5, COLUMN/2 -2 ,"Hard");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT=='e' || INPUT=='E') {
                
                break;

            }
            else {
                move(4*ROW/5,0);
            }
        }

        

    }

    if(GLOBAL_IS_GUEST==FALSE) {//write final result in file

        FILE* SETTINGS_FILE=fopen("user_settings.txt","r");

        char TEMP_STRING[100000];

        fgets(TEMP_STRING,100000,SETTINGS_FILE);

        char *PTR=strstr(TEMP_STRING,GLOBAL_LOGGED_IN_USER); 

        PTR+=strlen(GLOBAL_LOGGED_IN_USER)+1;//move PTR to first char location w s n

        PTR[4]=GLOBAL_SETTINGS.DIFFICULTY;

        fclose(SETTINGS_FILE);

        FILE *WRITE_MODE_PTR=fopen("user_settings.txt","w");

        fprintf(WRITE_MODE_PTR,"%s",TEMP_STRING);

        fclose(WRITE_MODE_PTR); 

    }


}
void OPEN_HERO_COLOR_MENU() {
    
    PRINT_COLOR_GUI();

    while(TRUE) {

        int x,y;

        getyx(stdscr,y,x);

        char LARGE_STRING[70]="Current Color is : ";

        char COLOR_SPELLED_OUT[10];

        if(GLOBAL_SETTINGS.HERO_COLOR=='r') {
            strcpy(COLOR_SPELLED_OUT,"Red");
        }
        else if(GLOBAL_SETTINGS.HERO_COLOR=='b') {
            strcpy(COLOR_SPELLED_OUT,"Blue");
        }
        else if(GLOBAL_SETTINGS.HERO_COLOR=='g'){
            strcpy(COLOR_SPELLED_OUT,"Green");
        }

        strcat(LARGE_STRING,COLOR_SPELLED_OUT);

        attrset(GOLD_COLORED_CELL);

        mvprintw(1,COLUMN/2-36 ,"                                                                        ");

        mvprintw(1,COLUMN/2 -strlen(LARGE_STRING)/2,"%s",LARGE_STRING);

        attroff(GOLD_COLORED_CELL);

        int INPUT=getch();

        if(y==ROW/5) {

            if(INPUT==KEY_DOWN) {

                attron(RED_COLORED_CELL);

                mvprintw(ROW/5, COLUMN/2 -2 ,"Red");

                attroff(RED_COLORED_CELL);

                attrset(GREEN_HIGHLIGHT | A_BLINK);

                mvprintw(2*ROW/5, COLUMN/2 -3 ,"Green");

                attroff(GREEN_HIGHLIGHT); attrset(A_BLINK);                

            }
            else if(INPUT=='e' || INPUT=='E') {

                GLOBAL_SETTINGS.HERO_COLOR='r';

                move(ROW/5,0);

            }
            else {
                move(ROW/5,0);
            }

        }
        else if(y==2*ROW/5) {

            if(INPUT==KEY_UP) {

                attron(GREEN_COLORED_CELL);

                mvprintw(2*ROW/5, COLUMN/2 -3 ,"Green");

                attroff(GREEN_COLORED_CELL);

                attrset(RED_HIGHLIGHT | A_BLINK);

                mvprintw(ROW/5, COLUMN/2 -2 ,"Red");

                attroff(RED_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT==KEY_DOWN) {

                attron(GREEN_COLORED_CELL);

                mvprintw(2*ROW/5, COLUMN/2 -3 ,"Green");

                attroff(GREEN_COLORED_CELL);

                attrset(BLUE_HIGHLIGHT | A_BLINK);

                mvprintw(3*ROW/5, COLUMN/2 -2 ,"Blue");

                attroff(BLUE_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT=='e' || INPUT=='E') {
                GLOBAL_SETTINGS.HERO_COLOR='g';

                move(2*ROW/5,0);
            }
            else {
                move(2*ROW/5,0);
            }
        }
        else if(y==3*ROW/5) {

            if(INPUT==KEY_UP) {

                attron(BLUE_COLORED_CELL);

                mvprintw(3*ROW/5, COLUMN/2 -2 ,"Blue");

                attroff(BLUE_COLORED_CELL);

                attrset(GREEN_HIGHLIGHT | A_BLINK);

                mvprintw(2*ROW/5, COLUMN/2 -3 ,"Green");

                attroff(GREEN_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT==KEY_DOWN) {

                attron(BLUE_COLORED_CELL);

                mvprintw(3*ROW/5, COLUMN/2 -2 ,"Blue");

                attroff(BLUE_COLORED_CELL);

                attrset(GOLD_HIGHLIGHT | A_BLINK);

                mvprintw(4*ROW/5, COLUMN/2 -2 ,"Back");

                attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT=='e' || INPUT=='E') {
                GLOBAL_SETTINGS.HERO_COLOR='b';

                move(3*ROW/5,0);
            }
            else {
                move(3*ROW/5,0);
            }
        }
        else {

            if(INPUT==KEY_UP) {

                attron(GOLD_COLORED_CELL);

                mvprintw(4*ROW/5, COLUMN/2 -2 ,"Back");

                attroff(GOLD_COLORED_CELL);

                attrset(BLUE_HIGHLIGHT | A_BLINK);

                mvprintw(3*ROW/5, COLUMN/2 -2 ,"Blue");

                attroff(BLUE_HIGHLIGHT); attrset(A_BLINK);

                refresh();

            }
            else if(INPUT=='e' || INPUT=='E') {
                
                break;

            }
            else {
                move(4*ROW/5,0);
            }
        }

    }

    if(GLOBAL_IS_GUEST==FALSE) {//write final result in file

        FILE* SETTINGS_FILE=fopen("user_settings.txt","r");

        char TEMP_STRING[100000];

        fgets(TEMP_STRING,100000,SETTINGS_FILE);

        char *PTR=strstr(TEMP_STRING,GLOBAL_LOGGED_IN_USER); 

        PTR+=strlen(GLOBAL_LOGGED_IN_USER)+1;//move PTR to first char location g s n

        PTR[0]=GLOBAL_SETTINGS.HERO_COLOR;

        fclose(SETTINGS_FILE);

        FILE *WRITE_MODE_PTR=fopen("user_settings.txt","w");

        fprintf(WRITE_MODE_PTR,"%s",TEMP_STRING);

        fclose(WRITE_MODE_PTR); 

    }


}
void HANDLE_GUEST_USER() {

    GLOBAL_SETTINGS.DIFFICULTY='n';

    GLOBAL_SETTINGS.HERO_COLOR='g';

    GLOBAL_SETTINGS.MUSIC='s'; //sleep away, rick roll, sacrifice

}
void PRINT_COLOR_GUI() {

    clear();

    noecho();

    curs_set(0);

    attrset(GOLD_COLORED_CELL);

    char LARGE_STRING[70]="Current Color is : ";

    char COLOR_SPELLED_OUT[10];

    if(GLOBAL_SETTINGS.HERO_COLOR=='r') {
        strcpy(COLOR_SPELLED_OUT,"Red");
    }
    else if(GLOBAL_SETTINGS.HERO_COLOR=='g') {
        strcpy(COLOR_SPELLED_OUT,"Green");
    }
    else if(GLOBAL_SETTINGS.HERO_COLOR=='b'){
        strcpy(COLOR_SPELLED_OUT,"Blue");
    }

    strcat(LARGE_STRING,COLOR_SPELLED_OUT);

    mvprintw(1,COLUMN/2-36 ,"                                                                        ");

    mvprintw(1,COLUMN/2 -strlen(LARGE_STRING)/2,"%s",LARGE_STRING);

    attroff(GOLD_COLORED_CELL);

    attron(GREEN_COLORED_CELL);

    mvprintw(2*ROW/5,COLUMN/2 -3,"Green");

    attroff(GREEN_COLORED_CELL);

    attron(BLUE_COLORED_CELL);

    mvprintw(3*ROW/5, COLUMN/2 -2,"Blue");

    attroff(BLUE_COLORED_CELL);

    attron(GOLD_COLORED_CELL);

    mvprintw(4*ROW/5,COLUMN/2 -2 ,"Back");

    attroff(GOLD_COLORED_CELL); 

    attron(RED_HIGHLIGHT | A_BLINK);

    mvprintw(ROW/5, COLUMN/2 -2 ,"Red");

    attroff(RED_HIGHLIGHT); attroff(A_BLINK);

    refresh();

}

void LOAD_USER_SETTINGS() {

    FILE *SETTINGS_FILE=fopen("user_settings.txt","r");

    char USER_FROM_FILE[20]; char DUMMY_VAR;

    while(TRUE) {

        fscanf(SETTINGS_FILE," %s",USER_FROM_FILE);

        if(strcmp(USER_FROM_FILE,GLOBAL_LOGGED_IN_USER)==0) {

            fscanf(SETTINGS_FILE," %c",&GLOBAL_SETTINGS.HERO_COLOR);

            fscanf(SETTINGS_FILE," %c",&GLOBAL_SETTINGS.MUSIC);

            fscanf(SETTINGS_FILE," %c",&GLOBAL_SETTINGS.DIFFICULTY);

            break;

        }

        fscanf(SETTINGS_FILE," %c",&DUMMY_VAR);    fscanf(SETTINGS_FILE," %c",&DUMMY_VAR);    fscanf(SETTINGS_FILE," %c",&DUMMY_VAR);

    }

    fclose(SETTINGS_FILE);

}

void HANDLE_SETTINGS() {

    if(GLOBAL_IS_GUEST==TRUE) {

        HANDLE_GUEST_USER();

    }

    else {

        LOAD_USER_SETTINGS();

    }
}

void PRINT_DIFFICULTY_MENU() {

    clear();

    noecho();

    curs_set(0);

    char LARGE_STRING[70]="Current Difficulty is : ";

    char COLOR_SPELLED_OUT[10];

    if(GLOBAL_SETTINGS.DIFFICULTY=='h') {
        strcpy(COLOR_SPELLED_OUT,"Hard");
    }
    else if(GLOBAL_SETTINGS.DIFFICULTY=='e') {
        strcpy(COLOR_SPELLED_OUT,"Easy");
    }
    else if(GLOBAL_SETTINGS.DIFFICULTY=='n'){
        strcpy(COLOR_SPELLED_OUT,"Normal");
    }

    strcat(LARGE_STRING,COLOR_SPELLED_OUT);

    attrset(GOLD_COLORED_CELL);

    mvprintw(1,COLUMN/2 -strlen(LARGE_STRING)/2,"%s",LARGE_STRING);

    mvprintw(2*ROW/5,COLUMN/2 -3 ,"Normal");

    mvprintw(3*ROW/5,COLUMN/2 -2,"Hard");

    mvprintw(4*ROW/5, COLUMN/2 -2,"Back");

    attroff(GOLD_COLORED_CELL);

    attrset(GOLD_HIGHLIGHT | A_BLINK);

    mvprintw(ROW/5, COLUMN/2 -2 ,"Easy");

    attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

    refresh();

}

void PRINT_MUSIC_MENU() {

    clear();        

    noecho();

    curs_set(0);

    char LARGE_STRING[70]="Current Music is : ";

    char COLOR_SPELLED_OUT[100];

    if(GLOBAL_SETTINGS.MUSIC=='s') {
        strcpy(COLOR_SPELLED_OUT,"Sleep Away");
    }
    else if(GLOBAL_SETTINGS.MUSIC=='w') {
        strcpy(COLOR_SPELLED_OUT,"Sacrifice");
    }
    else if(GLOBAL_SETTINGS.MUSIC=='r'){
        strcpy(COLOR_SPELLED_OUT,"Never Gonna Give You Up");
    }

    strcat(LARGE_STRING,COLOR_SPELLED_OUT);

    attrset(GOLD_COLORED_CELL);

    mvprintw(1,COLUMN/2 -strlen(LARGE_STRING)/2,"%s",LARGE_STRING);

    mvprintw(2*ROW/5,COLUMN/2 -5 ,"Sleep Away");

    mvprintw(3*ROW/5,COLUMN/2 -5,"Sacrifice");

    mvprintw(4*ROW/5, COLUMN/2 -2,"Back");

    attroff(GOLD_COLORED_CELL);

    attrset(GOLD_HIGHLIGHT | A_BLINK);

    mvprintw(ROW/5, COLUMN/2 -12 ,"Never Gonna Give You Up");

    attroff(GOLD_HIGHLIGHT); attrset(A_BLINK);

    refresh();

}






void LOAD_MAP_AND_INVENTORY(POINT MAP[4][38][144]) {

    if(GLOBAL_IS_GUEST==TRUE) {

        clear(); 

        attron(RED_COLORED_CELL); 

        mvprintw(ROW/2, COLUMN/2 -31, "As a Guest You Have No Saves. Press a Key to Create New Game.");

        attroff(RED_COLORED_CELL); refresh();

        getch();

        GENERATE_MAP(MAP);

    }

    else {

        char FILE_NAME[100]="saves_";

        strcat(FILE_NAME,GLOBAL_LOGGED_IN_USER);

        strcat(FILE_NAME,".txt");

        FILE *SAVES=fopen(FILE_NAME,"r+");

        char DUMMY_DATA[20];

        fgets(DUMMY_DATA,19,SAVES);

        rewind(SAVES);

        int TEMPORARY;

        char *USER_POINTER=strstr(DUMMY_DATA,GLOBAL_LOGGED_IN_USER);

        if(USER_POINTER==NULL) {

            clear(); 

            attron(RED_COLORED_CELL); 

            mvprintw(ROW/2, COLUMN/2 -24, "No Saves Found. Press a Key to Create New Game.");

            attroff(RED_COLORED_CELL); refresh();

            getch();

            GENERATE_MAP(MAP);
        }

        else {

            fscanf(SAVES,"%s",DUMMY_DATA);//get dummy username first

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.H_SPELL);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.S_SPELL);
            
            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.D_SPELL);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.MACE);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.DAGGER);
            
            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.WAND);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.ARROW);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.SWORD);
            
            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.GOLD);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.SCORE);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.FOOD_ITEMS);
            
            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.HP);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.HUNGER);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.TIME_PLAYED);
            
            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.x);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.y);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.FLOOR);
            
            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.KEYS);

            fscanf(SAVES,"%d",&GLOBAL_INVENTORY_AND_STATS.BROKEN_KEYS);

            for(int i=0 ; i<4 ; i++) {
                for(int j=0 ; j<38 ; j++) {
                    for(int k=0 ; k<144 ; k++) {
                        fscanf(SAVES," %c%c%c%c%c%c",&MAP[i][j][k].COLOR,&MAP[i][j][k].IS_DISCOVERED,&MAP[i][j][k].IS_START
                        
                        ,&MAP[i][j][k].PASSWORD_DOOR_TYPE,&MAP[i][j][k].REAL_CONTENTS,&MAP[i][j][k].SYMBOL);
                        
                    }
                }
            }

        }

        fclose(SAVES);

    }

}

void GENERATE_MAP(POINT MAP[4][38][144]) {

    int NEXT_START=GENERATE_FLOOR_SKELETON(MAP[0],-1,1);

    NEXT_START=GENERATE_FLOOR_SKELETON(MAP[1],NEXT_START,2);

    NEXT_START=GENERATE_FLOOR_SKELETON(MAP[2],NEXT_START,3);

    NEXT_START=GENERATE_FLOOR_SKELETON(MAP[3],NEXT_START,4);

    for(int i=0 ; i<38 ; i++) {//find and store game exit and start in 1st and 4th floors. Specify player start coords

        for(int j=0 ; j<144 ; j++) {

            if(MAP[0][i][j].IS_START==1) {

                GLOBAL_INVENTORY_AND_STATS.x=j;

                GLOBAL_INVENTORY_AND_STATS.y=i;

                MAP[0][i][j].SYMBOL='H';

                MAP[0][i][j].REAL_CONTENTS='.';

                MAP[0][i][j].IS_START=0;

            }

            if(MAP[3][i][j].IS_START==-1) {

                MAP[3][i][j].SYMBOL='x';

                MAP[3][i][j].REAL_CONTENTS='x';

                MAP[3][i][j].IS_START=0;

            }

        }
    }

}

int GENERATE_FLOOR_SKELETON(POINT FLOOR[38][144],int START_SECTOR, int FLOOR_NUMBER) {///////////////////
    
    for(int i=0 ; i<38 ; i++) {//insert default values for IS_DISCOVERED, SYMBOL, COLOR, IS_START, and passdoor type.

        for(int j=0 ; j<144 ; j++) {

            FLOOR[i][j].IS_DISCOVERED=0;

            FLOOR[i][j].SYMBOL=' ';

            FLOOR[i][j].IS_START=0;

            FLOOR[i][j].COLOR='w';

            FLOOR[i][j].PASSWORD_DOOR_TYPE='a';

        }

    }
    
    int X_START=rand()%27;  int Y_START=rand()%9+1; int BIG_OR_SMALL=rand()%2;

    if(BIG_OR_SMALL==1) {
        int door1=rand()%14+X_START+1;

        int door2=rand()%5+Y_START+1;

        int WINDOW_QUERRY=rand()%100+1;

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+5 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+15 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+15].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START+6][i].SYMBOL='_';
        }

        if(WINDOW_QUERRY<=20) {
            if(door1<X_START+14) {
                FLOOR[Y_START+6][door1+1].SYMBOL='=';
            }
            else {
                FLOOR[Y_START+6][door1-1].SYMBOL='=';
            }
        }

        FLOOR[door2][X_START+15].SYMBOL='+';

        FLOOR[Y_START+6][door1].SYMBOL='+';
    }
    else {

        int door1=rand()%11+X_START+1;

        int door2=rand()%4+Y_START+1;


        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+4 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+12 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+12].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START+5][i].SYMBOL='_';
        }

        FLOOR[door2][X_START+12].SYMBOL='+';

        FLOOR[Y_START+5][door1].SYMBOL='+';
    }

    X_START=rand()%27+48;   Y_START=rand()%9+1; BIG_OR_SMALL=rand()%2;

    if(BIG_OR_SMALL==1) {

        int door1=rand()%14+X_START+1;

        int door2=rand()%5+Y_START+1;

        int door3=rand()%5+Y_START+1;

        int WINDOW_QUERRY=rand()%100+1;

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+5 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+15 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+15].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START+6][i].SYMBOL='_';
        }

        if(WINDOW_QUERRY<=20) {
            if(door1<X_START+14) {
                FLOOR[Y_START+6][door1+1].SYMBOL='=';
            }
            else {
                FLOOR[Y_START+6][door1-1].SYMBOL='=';
            }
        }

        FLOOR[door2][X_START+15].SYMBOL='+';

        FLOOR[Y_START+6][door1].SYMBOL='+';

        FLOOR[door3][X_START].SYMBOL='+';
    }
    else {

        int door1=rand()%11+X_START+1;

        int door2=rand()%4+Y_START+1;

        int door3=rand()%4+Y_START+1;

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+4 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+12 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+12].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START+5][i].SYMBOL='_';
        }

        FLOOR[door2][X_START+12].SYMBOL='+';

        FLOOR[Y_START+5][door1].SYMBOL='+';

        FLOOR[door3][X_START].SYMBOL='+';

    }
    
    X_START=rand()%27+96;   Y_START=rand()%9+1; BIG_OR_SMALL=rand()%2;

    if(BIG_OR_SMALL==1) {

        int door1=rand()%14+X_START+1;

        int door3=rand()%5+Y_START+1;

        int WINDOW_QUERRY=rand()%100+1;

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+5 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+15 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+15].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START+6][i].SYMBOL='_';
        }

        if(WINDOW_QUERRY<=20) {
            if(door1<X_START+14) {
                FLOOR[Y_START+6][door1+1].SYMBOL='=';
            }
            else {
                FLOOR[Y_START+6][door1-1].SYMBOL='=';
            }
        }

        FLOOR[Y_START+6][door1].SYMBOL='+';

        FLOOR[door3][X_START].SYMBOL='+';
    }
    else {

        int door1=rand()%11+X_START+1;

        int door3=rand()%4+Y_START+1;

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+4 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+12 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+12].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START+5][i].SYMBOL='_';
        }


        FLOOR[Y_START+5][door1].SYMBOL='+';

        FLOOR[door3][X_START].SYMBOL='+';


    }

    X_START=rand()%27;  Y_START=rand()%9+20;    BIG_OR_SMALL=rand()%2;
    if(BIG_OR_SMALL==1) {

        int door1=rand()%14+X_START+1;

        int door2=rand()%5+Y_START+1;

        int WINDOW_QUERRY=rand()%100+1;

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+5 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+15 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+15].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START+6][i].SYMBOL='_';
        }

        if(WINDOW_QUERRY<=20) {
            if(door1<X_START+14) {
                FLOOR[Y_START][door1+1].SYMBOL='=';
            }
            else {
                FLOOR[Y_START][door1-1].SYMBOL='=';
            }
        }

        FLOOR[door2][X_START+15].SYMBOL='+';

        FLOOR[Y_START][door1].SYMBOL='+';

    }
    else {

        int door1=rand()%11+X_START+1;

        int door2=rand()%4+Y_START+1;

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+4 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+12 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+12].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START+5][i].SYMBOL='_';
        }

        FLOOR[door2][X_START+12].SYMBOL='+';

        FLOOR[Y_START][door1].SYMBOL='+';

    }

    X_START=rand()%27+48;   Y_START=rand()%9+20;    BIG_OR_SMALL=rand()%2;

    if(BIG_OR_SMALL==1) {

        int door1=rand()%14+X_START+1;

        int door2=rand()%5+Y_START+1;

        int door3=rand()%5+Y_START+1;

        int WINDOW_QUERRY=rand()%100+1;

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+5 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+15 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+15].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START+6][i].SYMBOL='_';
        }

        if(WINDOW_QUERRY<=20) {
            if(door1<X_START+14) {
                FLOOR[Y_START][door1+1].SYMBOL='=';
            }
            else {
                FLOOR[Y_START][door1-1].SYMBOL='=';
            }
        }


        FLOOR[door2][X_START+15].SYMBOL='+';

        FLOOR[Y_START][door1].SYMBOL='+';

        FLOOR[door3][X_START].SYMBOL='+';
    }
    else {
        
        int door1=rand()%11+X_START+1;

        int door2=rand()%4+Y_START+1;

        int door3=rand()%4+Y_START+1;

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+4 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+12 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+12].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START+5][i].SYMBOL='_';
        }

        FLOOR[door2][X_START+12].SYMBOL='+';

        FLOOR[Y_START][door1].SYMBOL='+';

        FLOOR[door3][X_START].SYMBOL='+';
    }

    X_START=rand()%27+96;   Y_START=rand()%9+20;    BIG_OR_SMALL=rand()%2; 

    if(BIG_OR_SMALL==1) {

        int door1=rand()%14+X_START+1;

        int door3=rand()%5+Y_START+1;

        int WINDOW_QUERRY=rand()%100+1;

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+5 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+15 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+15].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+16 ; i++) {
            FLOOR[Y_START+6][i].SYMBOL='_';
        }

        if(WINDOW_QUERRY<=20) {
            if(door1<X_START+14) {
                FLOOR[Y_START][door1+1].SYMBOL='=';
            }
            else {
                FLOOR[Y_START][door1-1].SYMBOL='=';
            }
        }

        FLOOR[Y_START][door1].SYMBOL='+';

        FLOOR[door3][X_START].SYMBOL='+';

    }
    else {

        int door1=rand()%11+X_START+1;

        int door3=rand()%4+Y_START+1;

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START][i].SYMBOL='_';
        }

        for(int j=Y_START+1 ; j<=Y_START+4 ; j++) {
            FLOOR[j][X_START].SYMBOL='|';
            for(int i=X_START+1 ; i<X_START+12 ; i++) {

                FLOOR[j][i].SYMBOL='.';

            }
            FLOOR[j][X_START+12].SYMBOL='|';
        }

        for(int i=X_START ; i<X_START+13 ; i++) {
            FLOOR[Y_START+5][i].SYMBOL='_';
        }

        FLOOR[Y_START][door1].SYMBOL='+';

        FLOOR[door3][X_START].SYMBOL='+';

    }

    //generate non-secret random features

    DOOR RETURN_DOORS[14];

    CONNECT_ROOMS_AND_RETURN_DOORS(FLOOR,RETURN_DOORS);       

    int TEMP=GENERATE_ENTRY_EXIT(FLOOR,START_SECTOR);   

    GENERATE_PILLARS(FLOOR);    //fill pillars

    //password doors later | password door is either reverse ordered or normal or has changing password

    int ANOTHER_RANDOM_QUERRY=rand()%100+1;

    int PASSWORD_DOOR_EXISTS=FALSE;

    int PASSWORD_DOOR_SECTOR;

    if(ANOTHER_RANDOM_QUERRY<=40) {//generate password door 

        ANOTHER_RANDOM_QUERRY=rand()%3+1;

        if(ANOTHER_RANDOM_QUERRY==1 || FLOOR_NUMBER==4) {//normal pass door with 1 or 2 passwords

            if(FLOOR_NUMBER==4) {//2 passes

                int RAND_X,RAND_Y;

                while(TRUE) {

                    RAND_X=rand()%144; RAND_Y=rand()%38;

                    if(FLOOR[RAND_Y][RAND_X].SYMBOL=='.') {

                        if(FLOOR[RAND_Y][RAND_X-1].SYMBOL=='|' && FLOOR[RAND_Y-1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y+1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X+1].SYMBOL!='O' ) {
                            break;
                        }

                        if(FLOOR[RAND_Y][RAND_X+1].SYMBOL=='|' && FLOOR[RAND_Y-1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y+1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X-1].SYMBOL!='O' ) {
                            break;
                        }

                        if(FLOOR[RAND_Y][RAND_X-1].SYMBOL=='|' && FLOOR[RAND_Y+1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y-1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X+1].SYMBOL!='O' ) {
                            break;
                        }

                        if(FLOOR[RAND_Y][RAND_X+1].SYMBOL=='|' && FLOOR[RAND_Y+1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y-1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X-1].SYMBOL!='O' ) {
                            break;
                        }

                    }

                }

                FLOOR[RAND_Y][RAND_X].SYMBOL='&';

                FLOOR[RAND_Y][RAND_X].COLOR='w';

                PASSWORD_DOOR_SECTOR=GIVE_SECTOR_NUMBER(RAND_X,RAND_Y);

                while(TRUE) {

                    RAND_X=rand()%144; RAND_Y=rand()%38;

                    if(FLOOR[RAND_Y][RAND_X].SYMBOL=='.' && GIVE_SECTOR_NUMBER(RAND_X,RAND_Y)==PASSWORD_DOOR_SECTOR && FLOOR[RAND_Y][RAND_X].SYMBOL!='&') {

                        if(FLOOR[RAND_Y][RAND_X-1].SYMBOL=='|' && FLOOR[RAND_Y-1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y+1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X+1].SYMBOL!='O' ) {
                            break;
                        }

                        if(FLOOR[RAND_Y][RAND_X+1].SYMBOL=='|' && FLOOR[RAND_Y-1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y+1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X-1].SYMBOL!='O' ) {
                            break;
                        }

                        if(FLOOR[RAND_Y][RAND_X-1].SYMBOL=='|' && FLOOR[RAND_Y+1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y-1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X+1].SYMBOL!='O' ) {
                            break;
                        }

                        if(FLOOR[RAND_Y][RAND_X+1].SYMBOL=='|' && FLOOR[RAND_Y+1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y-1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X-1].SYMBOL!='O' ) {
                            break;
                        }

                    }

                }

                FLOOR[RAND_Y][RAND_X].SYMBOL='&';

                FLOOR[RAND_Y][RAND_X].COLOR='w';

                for(int k=0 ; k<14 ; k++) {

                    if(RETURN_DOORS[k].SECTOR==PASSWORD_DOOR_SECTOR) {

                        FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].SYMBOL='@';

                        FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].COLOR='r';

                        FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].PASSWORD_DOOR_TYPE='2';

                        break;

                    }

                }

            }

            else {// 1 pass

                int RAND_X,RAND_Y;

                while(TRUE) {

                    RAND_X=rand()%144; RAND_Y=rand()%38;

                    if(FLOOR[RAND_Y][RAND_X].SYMBOL=='.') {

                        if(FLOOR[RAND_Y][RAND_X-1].SYMBOL=='|' && FLOOR[RAND_Y-1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y+1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X+1].SYMBOL!='O' ) {
                            break;
                        }

                        if(FLOOR[RAND_Y][RAND_X+1].SYMBOL=='|' && FLOOR[RAND_Y-1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y+1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X-1].SYMBOL!='O' ) {
                            break;
                        }

                        if(FLOOR[RAND_Y][RAND_X-1].SYMBOL=='|' && FLOOR[RAND_Y+1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y-1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X+1].SYMBOL!='O' ) {
                            break;
                        }

                        if(FLOOR[RAND_Y][RAND_X+1].SYMBOL=='|' && FLOOR[RAND_Y+1][RAND_X].SYMBOL=='_' 
                        
                        && FLOOR[RAND_Y-1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X-1].SYMBOL!='O' ) {
                            break;
                        }

                    }

                }

                FLOOR[RAND_Y][RAND_X].SYMBOL='&';

                FLOOR[RAND_Y][RAND_X].COLOR='w';

                PASSWORD_DOOR_SECTOR=GIVE_SECTOR_NUMBER(RAND_X,RAND_Y);

                for(int k=0 ; k<14 ; k++) {

                    if(RETURN_DOORS[k].SECTOR==PASSWORD_DOOR_SECTOR) {

                        FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].SYMBOL='@';

                        FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].COLOR='r';

                        FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].PASSWORD_DOOR_TYPE='1';

                        break;

                    }

                }

            }

        }
        
        else if(ANOTHER_RANDOM_QUERRY==2) {//reverse ordered pass

            int RAND_X,RAND_Y;

            while(TRUE) {

                RAND_X=rand()%144; RAND_Y=rand()%38;

                if(FLOOR[RAND_Y][RAND_X].SYMBOL=='.') {

                    if(FLOOR[RAND_Y][RAND_X-1].SYMBOL=='|' && FLOOR[RAND_Y-1][RAND_X].SYMBOL=='_' 
                        
                    && FLOOR[RAND_Y+1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X+1].SYMBOL!='O' ) {
                        break;
                    }

                    if(FLOOR[RAND_Y][RAND_X+1].SYMBOL=='|' && FLOOR[RAND_Y-1][RAND_X].SYMBOL=='_' 
                        
                    && FLOOR[RAND_Y+1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X-1].SYMBOL!='O' ) {
                        break;
                    }

                    if(FLOOR[RAND_Y][RAND_X-1].SYMBOL=='|' && FLOOR[RAND_Y+1][RAND_X].SYMBOL=='_' 
                        
                    && FLOOR[RAND_Y-1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X+1].SYMBOL!='O' ) {
                        break;
                    }

                    if(FLOOR[RAND_Y][RAND_X+1].SYMBOL=='|' && FLOOR[RAND_Y+1][RAND_X].SYMBOL=='_' 
                        
                    && FLOOR[RAND_Y-1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X-1].SYMBOL!='O' ) {
                        break;
                    }

                }

            }

            FLOOR[RAND_Y][RAND_X].SYMBOL='&';

            FLOOR[RAND_Y][RAND_X].COLOR='w';

            PASSWORD_DOOR_SECTOR=GIVE_SECTOR_NUMBER(RAND_X,RAND_Y);

            for(int k=0 ; k<14 ; k++) {

                if(RETURN_DOORS[k].SECTOR==PASSWORD_DOOR_SECTOR) {

                    FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].SYMBOL='@';

                    FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].COLOR='r';

                    FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].PASSWORD_DOOR_TYPE='r';

                    break;

                }

            }


        }

        else {//changing pass

            int RAND_X,RAND_Y;

            while(TRUE) {

                RAND_X=rand()%144; RAND_Y=rand()%38;

                if(FLOOR[RAND_Y][RAND_X].SYMBOL=='.') {

                    if(FLOOR[RAND_Y][RAND_X-1].SYMBOL=='|' && FLOOR[RAND_Y-1][RAND_X].SYMBOL=='_' 
                        
                    && FLOOR[RAND_Y+1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X+1].SYMBOL!='O' ) {
                        break;
                    }

                    if(FLOOR[RAND_Y][RAND_X+1].SYMBOL=='|' && FLOOR[RAND_Y-1][RAND_X].SYMBOL=='_' 
                        
                    && FLOOR[RAND_Y+1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X-1].SYMBOL!='O' ) {
                        break;
                    }

                    if(FLOOR[RAND_Y][RAND_X-1].SYMBOL=='|' && FLOOR[RAND_Y+1][RAND_X].SYMBOL=='_' 
                        
                    && FLOOR[RAND_Y-1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X+1].SYMBOL!='O' ) {
                        break;
                    }

                    if(FLOOR[RAND_Y][RAND_X+1].SYMBOL=='|' && FLOOR[RAND_Y+1][RAND_X].SYMBOL=='_' 
                        
                    && FLOOR[RAND_Y-1][RAND_X].SYMBOL!='O' && FLOOR[RAND_Y][RAND_X-1].SYMBOL!='O' ) {
                        break;
                    }

                }

            }

            FLOOR[RAND_Y][RAND_X].SYMBOL='&';

            FLOOR[RAND_Y][RAND_X].COLOR='w';

            PASSWORD_DOOR_SECTOR=GIVE_SECTOR_NUMBER(RAND_X,RAND_Y);

            for(int k=0 ; k<14 ; k++) {

                if(RETURN_DOORS[k].SECTOR==PASSWORD_DOOR_SECTOR) {

                    FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].SYMBOL='@';

                    FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].COLOR='r';

                    FLOOR[RETURN_DOORS[k].y][RETURN_DOORS[k].x].PASSWORD_DOOR_TYPE='t';

                    break;

                }

            }            

        }

        PASSWORD_DOOR_EXISTS=TRUE;

    }

    //theme randomizer here.
    
    
    
    
    
    //generate pickups, items & themes!!!!!!

    GENERATE_ANCIENT_KEY(FLOOR); //one ancient key per floor






    //end of non-secret gen------------------------------------------------------------------------------------

    for(int i=0 ; i<38 ; i++) {
        for(int j=0 ; j<144 ; j++) { 
            FLOOR[i][j].REAL_CONTENTS=FLOOR[i][j].SYMBOL;
        }
    }   

    //secret door & trap generation. change real contents but not symbol!--------------------------------------------

    COORDS START_COORDS=GIVE_STAIRS_ADDRESS_ENTRY(FLOOR);   COORDS END_COORDS=GIVE_STAIRS_ADDRESS_EXIT(FLOOR);

    int START_SECTOR_FOR_RNG=GIVE_SECTOR_NUMBER(START_COORDS.x,START_COORDS.y); int END_SECTOR=GIVE_SECTOR_NUMBER(END_COORDS.x,END_COORDS.y);

    //make end and start sectors not have secrets. but end may be password protected.

    int RANDOM_QUERRY=rand()%100+1; int SECRET_ROOM_SECTOR;

    if(RANDOM_QUERRY<=40 && PASSWORD_DOOR_EXISTS==FALSE) {//attempt to generate secret room at edges

        while(TRUE) {

            SECRET_ROOM_SECTOR=rand()%6+1;

            if((SECRET_ROOM_SECTOR==1 || SECRET_ROOM_SECTOR==3 || SECRET_ROOM_SECTOR==4 || SECRET_ROOM_SECTOR==6 )

            &&(SECRET_ROOM_SECTOR!=START_SECTOR_FOR_RNG)

            &&(SECRET_ROOM_SECTOR!=END_SECTOR)) 

            {//valid generation sector selected

                break;

            }

        }

        //turn regular doors' chars into walls and real insides into ? . 1 2 3
                                                                    //   4 5 6
        if(SECRET_ROOM_SECTOR==1) {//connection : 1r <-> 2l, 1d <-> 4u

            int DOOR3L=FIND_SPECIFIED_DOOR('r',1,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR3L].y][RETURN_DOORS[DOOR3L].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR3L].y][RETURN_DOORS[DOOR3L].x].SYMBOL='|';

            int DOOR4R=FIND_SPECIFIED_DOOR('l',2,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR4R].y][RETURN_DOORS[DOOR4R].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR4R].y][RETURN_DOORS[DOOR4R].x].SYMBOL='|';

            int DOOR1D=FIND_SPECIFIED_DOOR('d',1,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR1D].y][RETURN_DOORS[DOOR1D].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR1D].y][RETURN_DOORS[DOOR1D].x].SYMBOL='_';

            int DOOR4U=FIND_SPECIFIED_DOOR('u',4,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR4U].y][RETURN_DOORS[DOOR4U].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR4U].y][RETURN_DOORS[DOOR4U].x].SYMBOL='_';

        }
    
        else if(SECRET_ROOM_SECTOR==3) {//connection : 3l <-> 2r, 3d <-> 6u

            int DOOR3L=FIND_SPECIFIED_DOOR('l',3,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR3L].y][RETURN_DOORS[DOOR3L].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR3L].y][RETURN_DOORS[DOOR3L].x].SYMBOL='|';

            int DOOR6L=FIND_SPECIFIED_DOOR('r',2,RETURN_DOORS);
            
            FLOOR[RETURN_DOORS[DOOR6L].y][RETURN_DOORS[DOOR6L].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR6L].y][RETURN_DOORS[DOOR6L].x].SYMBOL='|';

            int DOOR3D=FIND_SPECIFIED_DOOR('d',3,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR3D].y][RETURN_DOORS[DOOR3D].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR3D].y][RETURN_DOORS[DOOR3D].x].SYMBOL='_';

            int DOOR6U=FIND_SPECIFIED_DOOR('u',6,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR6U].y][RETURN_DOORS[DOOR6U].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR6U].y][RETURN_DOORS[DOOR6U].x].SYMBOL='_';

        }

        else if(SECRET_ROOM_SECTOR==4) {// connection : 4u <-> 1d, 4r <-> 5l

            int DOOR4U=FIND_SPECIFIED_DOOR('u',4,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR4U].y][RETURN_DOORS[DOOR4U].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR4U].y][RETURN_DOORS[DOOR4U].x].SYMBOL='_';

            int DOOR1D=FIND_SPECIFIED_DOOR('d',1,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR1D].y][RETURN_DOORS[DOOR1D].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR1D].y][RETURN_DOORS[DOOR1D].x].SYMBOL='_';

            int DOOR4R=FIND_SPECIFIED_DOOR('r',4,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR4R].y][RETURN_DOORS[DOOR4R].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR4R].y][RETURN_DOORS[DOOR4R].x].SYMBOL='|';

            int DOOR5L=FIND_SPECIFIED_DOOR('l',5,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR5L].y][RETURN_DOORS[DOOR5L].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR5L].y][RETURN_DOORS[DOOR5L].x].SYMBOL='|';

        }

        else {//6 connection : 6l <-> 5r, 6u <-> 3d

            int DOOR3D=FIND_SPECIFIED_DOOR('d',3,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR3D].y][RETURN_DOORS[DOOR3D].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR3D].y][RETURN_DOORS[DOOR3D].x].SYMBOL='_';

            int DOOR6U=FIND_SPECIFIED_DOOR('u',6,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR6U].y][RETURN_DOORS[DOOR6U].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR6U].y][RETURN_DOORS[DOOR6U].x].SYMBOL='_';

            int DOOR6L=FIND_SPECIFIED_DOOR('l',6,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR6L].y][RETURN_DOORS[DOOR6L].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR6L].y][RETURN_DOORS[DOOR6L].x].SYMBOL='|';

            int DOOR5R=FIND_SPECIFIED_DOOR('r',5,RETURN_DOORS);

            FLOOR[RETURN_DOORS[DOOR5R].y][RETURN_DOORS[DOOR5R].x].REAL_CONTENTS='?';

            FLOOR[RETURN_DOORS[DOOR5R].y][RETURN_DOORS[DOOR5R].x].SYMBOL='|';

        }

    }

    //add traps

    









    return TEMP;

}

void CONNECT_ROOMS_AND_RETURN_DOORS(POINT FLOOR[38][144], DOOR RETURN_DOORS[14]) {

    int k=0;

    //search the board for doors, mark their locations and sectors
    for( int i=0 ; i<38 ; i++) {
        for( int j=0 ; j<144 ; j++) {
            if(FLOOR[i][j].SYMBOL=='+') {

                RETURN_DOORS[k].x=j;

                RETURN_DOORS[k].y=i;

                if(i>=0 && i<19) {//sectors 1,2,3

                    if(j>=0 && j<48) {

                        RETURN_DOORS[k].SECTOR=1;

                    }

                    if(j>=48 && j<96) {

                        RETURN_DOORS[k].SECTOR=2;

                    }

                    if(j>=96 && j<144) {

                        RETURN_DOORS[k].SECTOR=3;

                    }

                }

                else {//sectors 4,5,6

                    if(j>=0 && j<48) {

                        RETURN_DOORS[k].SECTOR=4;

                    }

                    if(j>=48 && j<96) {

                        RETURN_DOORS[k].SECTOR=5;

                    }

                    if(j>=96 && j<144) {

                        RETURN_DOORS[k].SECTOR=6;

                    }

                }

                k++;

            }
        }
    }

    //determine the direction of the doors. for easier reference, sort them first (in terms of sector).

    for(int i=0 ; i<14 ; i++) {
        for(int j=i+1 ; j<14 ; j++) {
            if(RETURN_DOORS[i].SECTOR>RETURN_DOORS[j].SECTOR) {
                DOOR temp=RETURN_DOORS[i];
                RETURN_DOORS[i]=RETURN_DOORS[j];
                RETURN_DOORS[j]=temp;

            }

        }
    }

    //sector 1 has 2 doors :

    if(RETURN_DOORS[0].x>RETURN_DOORS[1].x) {

        RETURN_DOORS[0].DIRECTION='r';

        RETURN_DOORS[1].DIRECTION='d';

    }
    else {
        RETURN_DOORS[1].DIRECTION='r';

        RETURN_DOORS[0].DIRECTION='d';
    }

    //sector 2 has 3 :

    if(RETURN_DOORS[2].y>RETURN_DOORS[3].y && RETURN_DOORS[2].y>RETURN_DOORS[4].y) {

        RETURN_DOORS[2].DIRECTION='d';

        if(RETURN_DOORS[3].x>RETURN_DOORS[4].x) {

            RETURN_DOORS[3].DIRECTION='r';

            RETURN_DOORS[4].DIRECTION='l';
        }

        else {
            RETURN_DOORS[4].DIRECTION='r';

            RETURN_DOORS[3].DIRECTION='l';
        }


    }

    if(RETURN_DOORS[3].y>RETURN_DOORS[2].y && RETURN_DOORS[3].y>RETURN_DOORS[4].y) {

        RETURN_DOORS[3].DIRECTION='d';

        if(RETURN_DOORS[2].x>RETURN_DOORS[4].x) {

            RETURN_DOORS[2].DIRECTION='r';

            RETURN_DOORS[4].DIRECTION='l';
        }

        else {
            RETURN_DOORS[4].DIRECTION='r';

            RETURN_DOORS[2].DIRECTION='l';
        }

    }

    if(RETURN_DOORS[4].y>RETURN_DOORS[2].y && RETURN_DOORS[4].y>RETURN_DOORS[3].y) {

        RETURN_DOORS[4].DIRECTION='d';

        if(RETURN_DOORS[3].x>RETURN_DOORS[2].x) {

            RETURN_DOORS[3].DIRECTION='r';

            RETURN_DOORS[2].DIRECTION='l';
        }

        else {
            RETURN_DOORS[2].DIRECTION='r';

            RETURN_DOORS[3].DIRECTION='l';
        }

    }

    //sector 3 has 2:

    if(RETURN_DOORS[5].y>RETURN_DOORS[6].y) {

        RETURN_DOORS[5].DIRECTION='d';

        RETURN_DOORS[6].DIRECTION='l';

    }

    else {

        RETURN_DOORS[6].DIRECTION='d';

        RETURN_DOORS[5].DIRECTION='l';

    }

    //sector 4 has 2

    if(RETURN_DOORS[7].x>RETURN_DOORS[8].x) {

        RETURN_DOORS[7].DIRECTION='r';

        RETURN_DOORS[8].DIRECTION='u';

    }
    
    else {

        RETURN_DOORS[8].DIRECTION='r';

        RETURN_DOORS[7].DIRECTION='u';

    }

    //sector 5 has 3

    if(RETURN_DOORS[9].y<RETURN_DOORS[10].y && RETURN_DOORS[9].y<RETURN_DOORS[11].y) {

        RETURN_DOORS[9].DIRECTION='u';

        if(RETURN_DOORS[10].x>RETURN_DOORS[11].x) {

            RETURN_DOORS[10].DIRECTION='r';

            RETURN_DOORS[11].DIRECTION='l';

        }

        else {

            RETURN_DOORS[11].DIRECTION='r';

            RETURN_DOORS[10].DIRECTION='l';

        }

    }

    else if(RETURN_DOORS[10].y<RETURN_DOORS[9].y && RETURN_DOORS[10].y<RETURN_DOORS[11].y) {

        RETURN_DOORS[10].DIRECTION='u';

        if(RETURN_DOORS[9].x>RETURN_DOORS[11].x) {

            RETURN_DOORS[9].DIRECTION='r';

            RETURN_DOORS[11].DIRECTION='l';

        }

        else {

            RETURN_DOORS[11].DIRECTION='r';

            RETURN_DOORS[9].DIRECTION='l';

        }
    }

    else {

        RETURN_DOORS[11].DIRECTION='u';

        if(RETURN_DOORS[10].x>RETURN_DOORS[9].x) {

            RETURN_DOORS[10].DIRECTION='r';

            RETURN_DOORS[9].DIRECTION='l';

        }

        else {

            RETURN_DOORS[9].DIRECTION='r';

            RETURN_DOORS[10].DIRECTION='l';

        }
    }

    //sector 6 has 2

    if(RETURN_DOORS[12].y>RETURN_DOORS[13].y) {

        RETURN_DOORS[12].DIRECTION='l';

        RETURN_DOORS[13].DIRECTION='u';

    }

    else {

        RETURN_DOORS[13].DIRECTION='l';

        RETURN_DOORS[12].DIRECTION='u';
    }

    //all doors now have been found and classified in terms of direction and sector. now to connect them.

    // 1r -> 2l & 1d -> 4u | 2r ->3l & 2d -> 5u | 3d ->6u | 4r -> 5l | 5r -> 6l //

    for(int k=0 ; k<14 ; k++) {//hallway gen completed

        if(RETURN_DOORS[k].SECTOR==1 && RETURN_DOORS[k].DIRECTION=='r') {//1r

            int i=RETURN_DOORS[k].y;

            int j=RETURN_DOORS[k].x+1;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(RETURN_DOORS[l].SECTOR==2 && RETURN_DOORS[l].DIRECTION=='l') {

                    I_FINAL=RETURN_DOORS[l].y;

                    J_FINAL=RETURN_DOORS[l].x;

                    break;

                }

            }

            while (i!=I_FINAL) {

                FLOOR[i][j].SYMBOL='#';

                if(i<I_FINAL)
                    i++;
                else
                    i--;
            }

            for(int l=j ; l<J_FINAL ; l++) {
                FLOOR[i][l].SYMBOL='#';
            }
        }

        if(RETURN_DOORS[k].SECTOR==1 && RETURN_DOORS[k].DIRECTION=='d') {//1d
            int i=RETURN_DOORS[k].y+1;

            int j=RETURN_DOORS[k].x;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(RETURN_DOORS[l].SECTOR==4 && RETURN_DOORS[l].DIRECTION=='u') {

                    I_FINAL=RETURN_DOORS[l].y;

                    J_FINAL=RETURN_DOORS[l].x;

                    break;

                }

            }

            while (j!=J_FINAL) {

                FLOOR[i][j].SYMBOL='#';

                if(j<J_FINAL)
                    j++;
                else
                    j--;
            }

            for(int l=i ; l<I_FINAL ; l++) {
                FLOOR[l][J_FINAL].SYMBOL='#';
            }
        }

        if(RETURN_DOORS[k].SECTOR==2 && RETURN_DOORS[k].DIRECTION=='r') {//2r
            int i=RETURN_DOORS[k].y;

            int j=RETURN_DOORS[k].x+1;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(RETURN_DOORS[l].SECTOR==3 && RETURN_DOORS[l].DIRECTION=='l') {

                    I_FINAL=RETURN_DOORS[l].y;

                    J_FINAL=RETURN_DOORS[l].x;

                    break;

                }

            }

            while (i!=I_FINAL) {

                FLOOR[i][j].SYMBOL='#';

                if(i<I_FINAL)
                    i++;
                else
                    i--;
            }

            for(int l=j ; l<J_FINAL ; l++) {
                FLOOR[i][l].SYMBOL='#';
            }
        }

        if(RETURN_DOORS[k].SECTOR==2 && RETURN_DOORS[k].DIRECTION=='d') {//2d
            int i=RETURN_DOORS[k].y+1;

            int j=RETURN_DOORS[k].x;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(RETURN_DOORS[l].SECTOR==5 && RETURN_DOORS[l].DIRECTION=='u') {

                    I_FINAL=RETURN_DOORS[l].y;

                    J_FINAL=RETURN_DOORS[l].x;

                    break;

                }

            }

            while (j!=J_FINAL) {

                FLOOR[i][j].SYMBOL='#';

                if(j<J_FINAL)
                    j++;
                else
                    j--;
            }

            for(int l=i ; l<I_FINAL ; l++) {
                FLOOR[l][j].SYMBOL='#';
            }
        }

        if(RETURN_DOORS[k].SECTOR==3 && RETURN_DOORS[k].DIRECTION=='d') {//3d
            int i=RETURN_DOORS[k].y+1;

            int j=RETURN_DOORS[k].x;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(RETURN_DOORS[l].SECTOR==6 && RETURN_DOORS[l].DIRECTION=='u') {

                    I_FINAL=RETURN_DOORS[l].y;

                    J_FINAL=RETURN_DOORS[l].x;

                    break;

                }

            }

            while (j!=J_FINAL) {

                FLOOR[i][j].SYMBOL='#';

                if(j<J_FINAL)
                    j++;
                else
                    j--;
            }

            for(int l=i ; l<I_FINAL ; l++) {
                FLOOR[l][j].SYMBOL='#';
            }
        }

        if(RETURN_DOORS[k].SECTOR==4 && RETURN_DOORS[k].DIRECTION=='r') {//4r
            int i=RETURN_DOORS[k].y;

            int j=RETURN_DOORS[k].x+1;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(RETURN_DOORS[l].SECTOR==5 && RETURN_DOORS[l].DIRECTION=='l') {

                    I_FINAL=RETURN_DOORS[l].y;

                    J_FINAL=RETURN_DOORS[l].x;

                    break;

                }

            }

            while (i!=I_FINAL) {

                FLOOR[i][j].SYMBOL='#';

                if(i<I_FINAL)
                    i++;
                else
                    i--;
            }

            for(int l=j ; l<J_FINAL ; l++) {
                FLOOR[i][l].SYMBOL='#';
            }
        }

        if(RETURN_DOORS[k].SECTOR==5 && RETURN_DOORS[k].DIRECTION=='r') {//5r
            int i=RETURN_DOORS[k].y;

            int j=RETURN_DOORS[k].x+1;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(RETURN_DOORS[l].SECTOR==6 && RETURN_DOORS[l].DIRECTION=='l') {

                    I_FINAL=RETURN_DOORS[l].y;

                    J_FINAL=RETURN_DOORS[l].x;

                    break;

                }

            }

            while (i!=I_FINAL) {

                FLOOR[i][j].SYMBOL='#';

                if(i<I_FINAL)
                    i++;
                else
                    i--;
            }

            for(int l=j ; l<J_FINAL ; l++) {
                FLOOR[i][l].SYMBOL='#';
            }
        }

    }

}

void GENERATE_PILLARS(POINT FLOOR[38][144]) {

    for(int k=0 ; k<6 ; k++) {//only 2 pillars for each sector

        int NUMBER=2;

        for(int i=(k/3)*19 ; i<19+(k/3)*19 ; i++ ) {

            for(int j=(k%3)*48; j<48+(k%3)*48 ;j++) {

                if(FLOOR[i][j].SYMBOL=='.') {

                    int CAN_BE_PUT=TRUE;

                    if(NUMBER) {

                        if(FLOOR[i][j-1].SYMBOL=='+' || FLOOR[i][j-1].SYMBOL=='=' || FLOOR[i][j-1].SYMBOL=='<')
                            CAN_BE_PUT=FALSE;
                        if(FLOOR[i][j+1].SYMBOL=='+' || FLOOR[i][j+1].SYMBOL=='=' || FLOOR[i][j+1].SYMBOL=='<')
                            CAN_BE_PUT=FALSE;
                        if(FLOOR[i-1][j].SYMBOL=='+' || FLOOR[i-1][j].SYMBOL=='=' || FLOOR[i-1][j].SYMBOL=='<')
                            CAN_BE_PUT=FALSE;
                        if(FLOOR[i+1][j].SYMBOL=='+' || FLOOR[i+1][j].SYMBOL=='=' || FLOOR[i+1][j].SYMBOL=='<')
                            CAN_BE_PUT=FALSE;
                        if(FLOOR[i-1][j-1].SYMBOL=='+' || FLOOR[i-1][j-1].SYMBOL=='=' || FLOOR[i-1][j-1].SYMBOL=='<')
                            CAN_BE_PUT=FALSE;
                        if(FLOOR[i-1][j+1].SYMBOL=='+' || FLOOR[i-1][j+1].SYMBOL=='=' || FLOOR[i-1][j+1].SYMBOL=='<')
                            CAN_BE_PUT=FALSE;
                        if(FLOOR[i+1][j-1].SYMBOL=='+' || FLOOR[i+1][j-1].SYMBOL=='=' || FLOOR[i+1][j-1].SYMBOL=='<')
                            CAN_BE_PUT=FALSE;
                        if(FLOOR[i+1][j+1].SYMBOL=='+' || FLOOR[i+1][j+1].SYMBOL=='=' || FLOOR[i+1][j+1].SYMBOL=='<')
                            CAN_BE_PUT=FALSE;

                    }

                    int QUERRY=rand()%100+1;

                    if(QUERRY<=3 && CAN_BE_PUT==TRUE && NUMBER>0)  {
                        FLOOR[i][j].SYMBOL='O';
                        NUMBER--;
                    }
                        

                }
                
            


            }

        }
    }

}

int GENERATE_ENTRY_EXIT(POINT FLOOR[38][144], int PREDETERMINED_START_SECTOR ) {

    //if passed on sector is -1, generate random start

    int X_START;

    int Y_START;

    if(PREDETERMINED_START_SECTOR==-1)  {//random start

        while(TRUE) {

            X_START=rand()%144;

            Y_START=rand()%38;

            int IS_VALID=FALSE;

            if(FLOOR[Y_START][X_START].SYMBOL=='.') {

                IS_VALID=TRUE;

                if(FLOOR[Y_START+1][X_START].SYMBOL=='=' || FLOOR[Y_START+1][X_START].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                if(FLOOR[Y_START-1][X_START].SYMBOL=='=' || FLOOR[Y_START-1][X_START].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                if(FLOOR[Y_START][X_START+1].SYMBOL=='=' || FLOOR[Y_START][X_START+1].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                if(FLOOR[Y_START][X_START-1].SYMBOL=='=' || FLOOR[Y_START][X_START-1].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                
            }

            if(IS_VALID==TRUE) {
                break;
            }

        }

    }

    else {

        while(TRUE) {

            X_START=rand()%144;

            Y_START=rand()%38;

            int IS_VALID=FALSE;

            if(FLOOR[Y_START][X_START].SYMBOL=='.') {

                IS_VALID=TRUE;

                if(FLOOR[Y_START+1][X_START].SYMBOL=='=' || FLOOR[Y_START+1][X_START].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                if(FLOOR[Y_START-1][X_START].SYMBOL=='=' || FLOOR[Y_START-1][X_START].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                if(FLOOR[Y_START][X_START+1].SYMBOL=='=' || FLOOR[Y_START][X_START+1].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                if(FLOOR[Y_START][X_START-1].SYMBOL=='=' || FLOOR[Y_START][X_START-1].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                
            }

            if(IS_VALID==TRUE && GIVE_SECTOR_NUMBER(X_START,Y_START)==PREDETERMINED_START_SECTOR) {
                break;
            }

        }

    }

    int PROHIBITED_EXIT_SECTOR=GIVE_SECTOR_NUMBER(X_START,Y_START);

    int X_END;

    int Y_END;

    while(TRUE) {

            X_END=rand()%144;

            Y_END=rand()%38;

            int IS_VALID=FALSE;

            if(FLOOR[Y_END][X_END].SYMBOL=='.') {

                IS_VALID=TRUE;

                if(FLOOR[Y_END+1][X_END].SYMBOL=='=' || FLOOR[Y_END+1][X_END].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                if(FLOOR[Y_END-1][X_END].SYMBOL=='=' || FLOOR[Y_END-1][X_END].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                if(FLOOR[Y_END][X_END+1].SYMBOL=='=' || FLOOR[Y_END][X_END+1].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                if(FLOOR[Y_END][X_END-1].SYMBOL=='=' || FLOOR[Y_END][X_END-1].SYMBOL=='+') {
                    IS_VALID=FALSE;
                }
                
            }

            if(IS_VALID==TRUE && GIVE_SECTOR_NUMBER(X_END,Y_END)!=PROHIBITED_EXIT_SECTOR) {
                break;
            }

    }

    FLOOR[Y_START][X_START].IS_START=1;

    FLOOR[Y_START][X_START].SYMBOL='<';

    FLOOR[Y_END][X_END].IS_START=-1;

    FLOOR[Y_END][X_END].SYMBOL='<';

    return GIVE_SECTOR_NUMBER(X_END,Y_END);

}

int GIVE_SECTOR_NUMBER(int x, int y) {

    if(y<19) {

        if(x<48) {

            return 1;
        }
        else if(x>=48 && x<96) {

            return 2;

        }
        else {
            return 3;
        }
    }

    else {

        if(x<48) {

            return 4;

        }
        else if(x>=48 && x<96) {

            return 5;
        }
        else {
            return 6;
        }

    }
}

COORDS GIVE_STAIRS_ADDRESS_ENTRY(POINT FLOOR[38][144]) {

    for(int i=0 ; i<38 ; i++) {

        for(int j=0 ; j<144 ; j++) {

            if(FLOOR[i][j].SYMBOL=='<' && FLOOR[i][j].IS_START==1) {

                COORDS ANS;

                ANS.x=j; ANS.y=i;

                return ANS;

            }
        }
    }
}

COORDS GIVE_STAIRS_ADDRESS_EXIT(POINT FLOOR[38][144]) {

    for(int i=0 ; i<38 ; i++) {
        
        for(int j=0 ; j<144 ; j++) {

            if(FLOOR[i][j].SYMBOL=='<' && FLOOR[i][j].IS_START==-1) {

                COORDS ANS;

                ANS.x=j; ANS.y=i;

                return ANS;

            }
        }
    }
}

int FIND_SPECIFIED_DOOR(char WANTED_DIRECTION,  int WANTED_SECTOR, DOOR ALL_DOORS[14]) {

    for(int k=0 ; k<14 ; k++) {

        if(ALL_DOORS[k].DIRECTION==WANTED_DIRECTION && ALL_DOORS[k].SECTOR==WANTED_SECTOR) {

            return k;

        }
    }

}

void GENERATE_ANCIENT_KEY(POINT FLOOR[38][144]) {

    int RAND_X;

    int RAND_Y;

    while(TRUE) {

        RAND_X=rand()%144;

        RAND_Y=rand()%38;

        if(FLOOR[RAND_Y][RAND_X].SYMBOL=='.') {

            FLOOR[RAND_Y][RAND_X].SYMBOL=-1; // Placeholder Ancient Key Convention

            FLOOR[RAND_Y][RAND_X].COLOR='y';

            break;

        }
    }


} 

void HANDLE_USER_INPUT_AND_MOVE_HERO(char *TEMP_PTR, POINT MAP[4][38][144]) {

    int USER_INPUT=getch();

    int VALID;

    if(USER_INPUT!=ERR && USER_INPUT!=KEY_BACKSPACE && USER_INPUT!='f' && USER_INPUT!='F' && USER_INPUT!='g' && USER_INPUT!='G') {

        int CURRENT_X=GLOBAL_INVENTORY_AND_STATS.x,CURRENT_Y=GLOBAL_INVENTORY_AND_STATS.y;

        PROCESS_PLAYER_INPUT(USER_INPUT,MAP, TEMP_PTR);

        if(CURRENT_X!=GLOBAL_INVENTORY_AND_STATS.x || CURRENT_Y!=GLOBAL_INVENTORY_AND_STATS.y) {

            HANDLE_ITEM_PICKUPS(MAP,TEMP_PTR);

        }

    }  

    else if(USER_INPUT==KEY_BACKSPACE) {//open pause menu

        cbreak(); //if user chooses to quit, get appropriate exit code from pause menu function
        //////////////for now just closes the program
        //load pause menu
        OPEN_PAUSE_MENU();


        halfdelay(10);

    }

    else if(USER_INPUT=='f' || USER_INPUT=='F') {//movement Macro

        int MACRO_MOVEMENT=getch();

        if(MACRO_MOVEMENT=='1' || MACRO_MOVEMENT=='2'||MACRO_MOVEMENT=='3' || MACRO_MOVEMENT=='4'
        
        || MACRO_MOVEMENT=='6' || MACRO_MOVEMENT=='7'||MACRO_MOVEMENT=='8' || MACRO_MOVEMENT=='9'
        
        ) {//initiate macro

            int CURRENT_X,CURRENT_Y;

            do {

                CURRENT_X=GLOBAL_INVENTORY_AND_STATS.x; CURRENT_Y=GLOBAL_INVENTORY_AND_STATS.y;

                PROCESS_PLAYER_INPUT(MACRO_MOVEMENT,MAP,TEMP_PTR);

                UPDATE_EXPLORATION_AND_HANDLE_INTERACTIVE_OBJECTS(MAP);

                HANDLE_ITEM_PICKUPS(MAP,TEMP_PTR);

                PRINT_MAP_ON_SCREEN(MAP);

                //add anything else as added in main

            } while(CURRENT_X!=GLOBAL_INVENTORY_AND_STATS.x || CURRENT_Y!= GLOBAL_INVENTORY_AND_STATS.y);

        }

    }

    else if(USER_INPUT=='g' || USER_INPUT=='G') {

        USER_INPUT=getch();

        PROCESS_PLAYER_INPUT(USER_INPUT,MAP,TEMP_PTR);

    }
    
    //no input received. loop in main will go on

}

int PROCESS_PLAYER_INPUT(int USER_INPUT,POINT MAP[4][38][144], char * TEMP_PTR) {//////////////////////////2 keys 4 weapon/spell menu

    switch(USER_INPUT) //inventory and other stuff will be implemented later.
    //movement with numpad added
    {
        case '8': {//up

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='|'

            ) {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL=*TEMP_PTR;

                GLOBAL_INVENTORY_AND_STATS.y--;

                *TEMP_PTR=MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL;

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='H';

            }

            else if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL=='='
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='#'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='+') 
            {

                REVEAL_ENTIRE_SECTOR(MAP,GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS.x,GLOBAL_INVENTORY_AND_STATS.y)-3);

            }

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='?';

            }

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='@'
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].PASSWORD_DOOR_TYPE!='l'

            ) {

                OPEN_PASSKEY_GUI(MAP);

            }


            break;
        }

        case '6': {//right

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='|'

            ) {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL=*TEMP_PTR;

                GLOBAL_INVENTORY_AND_STATS.x++;

                *TEMP_PTR=MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL;

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='H';

            }

            else if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL=='='
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='#'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='+')
            {

                REVEAL_ENTIRE_SECTOR(MAP,GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS.x,GLOBAL_INVENTORY_AND_STATS.y)+1);

            }

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='?';

            }

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='@'
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].PASSWORD_DOOR_TYPE!='l'

            ) {

                OPEN_PASSKEY_GUI(MAP);

            }

            break;
        }

        case '4': {//left

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='|'

            ) {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL=*TEMP_PTR;

                GLOBAL_INVENTORY_AND_STATS.x--;

                *TEMP_PTR=MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL;

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='H';

            }

            else if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL=='='
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='#'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='+'
            
            )
            {

                REVEAL_ENTIRE_SECTOR(MAP,GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS.x,GLOBAL_INVENTORY_AND_STATS.y)-1);

            }

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='?';

            }

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='@'
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].PASSWORD_DOOR_TYPE!='l'

            ) {

                OPEN_PASSKEY_GUI(MAP);

            }

            break;
        }

        case '2': {//down

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='|'

            ) {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL=*TEMP_PTR;

                GLOBAL_INVENTORY_AND_STATS.y++;

                *TEMP_PTR=MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL;

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='H';

            }

            else if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL=='='
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='#'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='+' 
            ) {

                REVEAL_ENTIRE_SECTOR(MAP,GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS.x,GLOBAL_INVENTORY_AND_STATS.y)+3);

            }

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='?';

            }

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='@'
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].PASSWORD_DOOR_TYPE!='l'

            ) {

                OPEN_PASSKEY_GUI(MAP);

            }

            break;
        }

        case '9' : {// up right

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='|'
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='|'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='|'

            ) {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL=*TEMP_PTR;

                GLOBAL_INVENTORY_AND_STATS.x++;    GLOBAL_INVENTORY_AND_STATS.y--;

                *TEMP_PTR=MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL;

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='H';

            }

            break;
        }

        case '7' : {//up left

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='|'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='|'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='|'

            ) {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL=*TEMP_PTR;

                GLOBAL_INVENTORY_AND_STATS.x--;    GLOBAL_INVENTORY_AND_STATS.y--;

                *TEMP_PTR=MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL;

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='H';

            }
        
            break;
        }

        case '3' : {//down right

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='|'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='|'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL!='|'

            ) {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL=*TEMP_PTR;

                GLOBAL_INVENTORY_AND_STATS.x++;    GLOBAL_INVENTORY_AND_STATS.y++;

                *TEMP_PTR=MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL;

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='H';

            }

            break;
        }

        case '1' : {//down left

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL!='|'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='|'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='=' 
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS!='@' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!=' '
            
            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='O'

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='_' 

            && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL!='|'

            ) {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL=*TEMP_PTR;

                GLOBAL_INVENTORY_AND_STATS.y++; GLOBAL_INVENTORY_AND_STATS.x--;

                *TEMP_PTR=MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL;

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='H';

            }

            break;
        }

        case KEY_RIGHT : {//deeper into a a dungeon

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].IS_START==-1) {

                GLOBAL_INVENTORY_AND_STATS.FLOOR++;

                int BREAK=0;

                GLOBAL_DOOR_PASSWORD=-1;

                GLOBAL_IS_PASSWORD_DYNAMIC=FALSE;

                GLOBAL_PASSWORD_DOOR_BACKGROUND_CHECK=FALSE;

                for(int i=0 ; i<38 ; i++) {

                    for(int j=0 ; j<144 ; j++) {

                        if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].IS_START==1) {

                            GLOBAL_INVENTORY_AND_STATS.y=i;

                            GLOBAL_INVENTORY_AND_STATS.x=j;

                            BREAK++;

                            break;
                        }

                    }

                    if(BREAK==1) {

                        break;
                    }

                }

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='H';

            }

            break; 
        }

        case KEY_LEFT : {//back a floor

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].IS_START==1) {

                GLOBAL_INVENTORY_AND_STATS.FLOOR--;

                GLOBAL_DOOR_PASSWORD=-1;

                GLOBAL_IS_PASSWORD_DYNAMIC=FALSE;

                GLOBAL_PASSWORD_DOOR_BACKGROUND_CHECK=FALSE;

                int BREAK=0;

                for(int i=0 ; i<38 ; i++) {

                    for(int j=0 ; j<144 ; j++) {

                        if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].IS_START==-1) {

                            GLOBAL_INVENTORY_AND_STATS.y=i;

                            GLOBAL_INVENTORY_AND_STATS.x=j;

                            BREAK++;

                            break;
                        }

                    }

                    if(BREAK==1) {

                        break;
                    }

                }

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='H';

            }


            break;
        }

        case 'm' : {

            if(GLOBAL_SHOW_ALL==FALSE) {
                GLOBAL_SHOW_ALL=TRUE;
            }
            else {
                GLOBAL_SHOW_ALL=FALSE;
            }

            break;
        }

        case 'M' : {

            if(GLOBAL_SHOW_ALL==FALSE) {
                GLOBAL_SHOW_ALL=TRUE;
            }
            else {
                GLOBAL_SHOW_ALL=FALSE;
            }

            break;
        }

        case 's' : {

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='^';

            }

            break;
        }

        case 'S' : {

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='?') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS='+';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='?';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y-1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x-1].SYMBOL='^';

            }
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].REAL_CONTENTS=='^') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y+1][GLOBAL_INVENTORY_AND_STATS.x+1].SYMBOL='^';

            }

            break;

        }

        case 'e' : {

            OPEN_FOOD_MENU();

            break;
        }

        case 'E' : {

            OPEN_FOOD_MENU();

            break;
        }

        case 'i' : {

            OPEN_WEAPONS_MENU();

            break;
        }

        case 'I' : {

            OPEN_WEAPONS_MENU();

            break;
        }

        case 'p' : {

            OPEN_SPELLS_MENU();

            break;
        }

        case 'P' : {

            OPEN_SPELLS_MENU();

            break;
        }

    }

}

void PRINT_MAP_ON_SCREEN(POINT MAP[4][38][144]) {

    clear();
    
    TURN_ON_COLOR_FOR_MAP_PRINT(GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE);

    mvprintw(0,0,"%s",GLOBAL_MOST_RECENT_MESSAGE);

    TURN_OFF_COLOR_FOR_MAP_PRINT(GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE);

    move(0,0);

    attrset(WHITE_COLORED_CELL);

    for(int i=1 ; i<37 ; i++) {

        for(int j=0 ; j<144 ; j++) {

            if(GLOBAL_SHOW_ALL==FALSE) {

                if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].IS_DISCOVERED==1) {

                    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL=='H') {//print hero color

                        TURN_ON_COLOR_FOR_MAP_PRINT(GLOBAL_SETTINGS.HERO_COLOR);

                        mvprintw(i,j,"%c",MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL);

                        TURN_OFF_COLOR_FOR_MAP_PRINT(GLOBAL_SETTINGS.HERO_COLOR);

                    }
                    else {

                        TURN_ON_COLOR_FOR_MAP_PRINT(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR);

                        if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL>0) {

                            mvprintw(i,j,"%c",MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL);

                        }

                        else {// special symbols////////more added later

                            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL==-1) {

                                mvprintw(i,j,"Δ");

                            }


                        }

                        TURN_OFF_COLOR_FOR_MAP_PRINT(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR);

                    }
                }
                else
                    mvprintw(i,j," ");
            }

            else {

                if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL=='H') {//print hero color

                    TURN_ON_COLOR_FOR_MAP_PRINT(GLOBAL_SETTINGS.HERO_COLOR);

                    mvprintw(i,j,"%c",MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL);

                    TURN_OFF_COLOR_FOR_MAP_PRINT(GLOBAL_SETTINGS.HERO_COLOR);

                }
                else {

                    TURN_ON_COLOR_FOR_MAP_PRINT(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR);

                    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL>0) {

                        mvprintw(i,j,"%c",MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL);

                    }

                    else {// special symbols////////more added later

                        if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL==-1) {

                            mvprintw(i,j,"Δ");

                        }



                    }

                    TURN_OFF_COLOR_FOR_MAP_PRINT(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR);
                }

            }

        }

    }

    attroff(WHITE_COLORED_CELL);

    mvprintw(ROW-2,0,"Keys: %d | Broken Keys: %d",GLOBAL_INVENTORY_AND_STATS.KEYS,GLOBAL_INVENTORY_AND_STATS.BROKEN_KEYS);

    mvprintw(ROW-1,0,"Floor : %d |",GLOBAL_INVENTORY_AND_STATS.FLOOR+1);

    mvprintw(ROW-1,13,"HP : %d/%d | Saturation : %d/100 | Gold : %d | Score : %d"
    
    ,GLOBAL_INVENTORY_AND_STATS.HP,GLOBAL_MAX_HP,GLOBAL_INVENTORY_AND_STATS.HUNGER
    
    ,GLOBAL_INVENTORY_AND_STATS.GOLD,GLOBAL_INVENTORY_AND_STATS.SCORE);

    move(0,0);

    refresh();

    strcpy(GLOBAL_MOST_RECENT_MESSAGE,"\0");

}

void UPDATE_EXPLORATION_AND_HANDLE_INTERACTIVE_OBJECTS(POINT MAP[4][38][144]) {

    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='+' 
    
    || MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='<'
    
    || MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='.'

    ) {//reveal entire room

        REVEAL_ENTIRE_SECTOR(MAP,GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS.x,GLOBAL_INVENTORY_AND_STATS.y));

    }

    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='+' 

    ||  MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='#')

    {//start revealing corridors

        REVEAL_HALLWAYS(MAP,0,6,GLOBAL_INVENTORY_AND_STATS.x,GLOBAL_INVENTORY_AND_STATS.y);
    
    }

    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='&' 
    
    && (MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR=='y'

    || MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR=='o'
    
    || MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR=='w'))

    {

        HANDLE_DOOR_PASSKEY_GENERATION(MAP);

    }

    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS=='^') {

        strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Stepped On a Trap!");

        GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='r';

        PRINT_MESSAGE("You Stepped On a Trap!");

        HANDLE_TRAP_ACTIVATION();

    }

}

void REVEAL_ENTIRE_SECTOR(POINT MAP[4][38][144],int SECTOR) {

    SECTOR--;

    for(int i=(SECTOR/3)*19; i<19+(SECTOR/3)*19 ; i++) {

        for(int j=(SECTOR%3)*48 ; j<(SECTOR%3)*48+48 ; j++) {

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL!='#') {

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].IS_DISCOVERED=1;

            }
        }
    }
}

void REVEAL_HALLWAYS(POINT MAP[4][38][144], char DONT_CHECK_DIRECTION,int MOVES_LEFT , int CURSOR_X , int CURSOR_Y) {//

    if(MOVES_LEFT!=0) {

        if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y][CURSOR_X].REAL_CONTENTS=='+' 
    
        || MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y][CURSOR_X].REAL_CONTENTS=='#') 
    
        {

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y][CURSOR_X].IS_DISCOVERED=1;

        }



        if((MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y][CURSOR_X+1].REAL_CONTENTS=='#'
    
        || MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y][CURSOR_X+1].REAL_CONTENTS=='+'
    
        )
    
        && DONT_CHECK_DIRECTION!='r'
    
        ) {

            REVEAL_HALLWAYS(MAP,'l',MOVES_LEFT-1,CURSOR_X+1,CURSOR_Y);

        }

        if((MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y][CURSOR_X-1].REAL_CONTENTS=='#'
    
        || MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y][CURSOR_X-1].REAL_CONTENTS=='+'
    
        )
    
        && DONT_CHECK_DIRECTION!='l'
    
        ) {

            REVEAL_HALLWAYS(MAP,'r',MOVES_LEFT-1,CURSOR_X-1,CURSOR_Y);

        }

        if((MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y+1][CURSOR_X].REAL_CONTENTS=='#'
    
        || MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y+1][CURSOR_X].REAL_CONTENTS=='+'
    
        )
    
        && DONT_CHECK_DIRECTION!='d'
    
        ) {

            REVEAL_HALLWAYS(MAP,'u',MOVES_LEFT-1,CURSOR_X,CURSOR_Y+1);

        }

        if((MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y-1][CURSOR_X].REAL_CONTENTS=='#'
    
        || MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][CURSOR_Y-1][CURSOR_X].REAL_CONTENTS=='+'
    
        )
    
        && DONT_CHECK_DIRECTION!='u'
    
        ) {

            REVEAL_HALLWAYS(MAP,'d',MOVES_LEFT-1,CURSOR_X,CURSOR_Y-1);

        }


    }

}

void TURN_ON_COLOR_FOR_MAP_PRINT(char COLOR_TO_BE_PRINTED) {

    switch(COLOR_TO_BE_PRINTED) {

        case 'r' :

            attron(RED_COLORED_CELL);

            break;
        case 'b' :

            attron(BLUE_COLORED_CELL);

            break;
        case 'y' : 

            attron(GOLD_COLORED_CELL);

            break;
        case 'w' :

            attron(WHITE_COLORED_CELL);

            break;
        case 'g' :

            attron(GREEN_COLORED_CELL);

            break;
        case 'o':

            attron(ORANGE_COLORED_CELL);

            break;
    }

}

void TURN_OFF_COLOR_FOR_MAP_PRINT(char COLOR_TO_BE_OFFED) {

    switch(COLOR_TO_BE_OFFED) {

        case 'r' :

            attroff(RED_COLORED_CELL);

            break;
        case 'b' :

            attroff(BLUE_COLORED_CELL);

            break;
        case 'y' : 

            attroff(GOLD_COLORED_CELL);

            break;
        case 'w' :

            attroff(WHITE_COLORED_CELL);

            break;
        case 'g' :

            attroff(GREEN_COLORED_CELL);

            break;
        case 'o':

            attroff(ORANGE_COLORED_CELL);

            break;
    }

}

void PRINT_MESSAGE(char MESSAGE[]) {

    mvprintw(0,0,"%s",MESSAGE);

}

void CLEAR_MESSAGE_BAR() {

    mvprintw(0,0,"                                                                                       ");

}

void PASSWORD_BACKGROUND_CHECK() {

    unsigned long long int CURRENT_TIME_QUERRY=time(NULL);

    if(CURRENT_TIME_QUERRY-GLOBAL_MOST_RECENT_PASSWORD_REQUEST_TIME>=30) {

        GLOBAL_PASSWORD_DOOR_BACKGROUND_CHECK=FALSE;

        if(GLOBAL_IS_PASSWORD_DYNAMIC==TRUE) {

            GLOBAL_DOOR_PASSWORD=-1;

            GLOBAL_IS_PASSWORD_DYNAMIC=FALSE;

        }

    }

    else {

        attron(GOLD_COLORED_CELL | A_BLINK);

        mvprintw(1,COLUMN/2 - 11,"Door Passkey is : %.4d",GLOBAL_DOOR_PASSWORD);

        attroff(GOLD_COLORED_CELL); attroff(A_BLINK);

        refresh();

    }

}

void HANDLE_DOOR_PASSKEY_GENERATION(POINT MAP[4][38][144]) {

    GLOBAL_MOST_RECENT_PASSWORD_REQUEST_TIME=time(NULL);

    int IS_DYNAMIC=FALSE;

    int BREAK=0;

    for(int i=0 ; i<38 ; i++) {

        for(int j=0 ; j<144 ; j++) {

            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL=='@') {

                if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].PASSWORD_DOOR_TYPE=='t') {

                    GLOBAL_IS_PASSWORD_DYNAMIC=TRUE;

                }

                BREAK++;

                break;

            }

        }

        if(BREAK==1) {
            break;
        }

    }

    GLOBAL_PASSWORD_DOOR_BACKGROUND_CHECK=TRUE;

    GLOBAL_DOOR_PASSWORD=rand()%10000;

}

void OPEN_PASSKEY_GUI(POINT MAP[4][38][144]) {

    clear();

    attron(GOLD_COLORED_CELL);

    cbreak();

    mvprintw(0,COLUMN/2 -30,"Input 4 Digit Passkey. Enter 0 to Use a Key If You Have One");

    mvprintw(ROW/2 , COLUMN/2 -2,"####");

    echo(); curs_set(1); move(ROW/2 -1,COLUMN/2 -2); refresh();

    attroff(GOLD_COLORED_CELL);

    int INPUTTED_PASSKEY; 

    int CORRECT_PASSWORD; int DOOR_X,DOOR_Y;

    for(int i=0 ; i<38 ; i++) {//reverse entering passkey and finding door x,y
        int BREAK=0;
        for(int j=0 ; j<144 ; j++) {
            if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL=='@') {

                DOOR_X=j; DOOR_Y=i;

                if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].PASSWORD_DOOR_TYPE=='r') {

                    int X,Y,Z,T;

                    T=GLOBAL_DOOR_PASSWORD%10; Z=(GLOBAL_DOOR_PASSWORD%100-T)/10 ; Y=(GLOBAL_DOOR_PASSWORD%1000-Z*10-T)/100;

                    X=(GLOBAL_DOOR_PASSWORD%10000-100*Y-10*Z-T)/1000;

                    CORRECT_PASSWORD=1000*T+100*Z+10*Y+X;

                }

                BREAK++;
                break;
            }
        }
        if(BREAK==1) {
            break;
        }
    }

    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].PASSWORD_DOOR_TYPE!='r') {

        CORRECT_PASSWORD=GLOBAL_DOOR_PASSWORD;

    }

    scanw("%d",&INPUTTED_PASSKEY);

    noecho(); curs_set(0);

    if(INPUTTED_PASSKEY==0) {//ancient key usage

        int ANCIENT_KEY_USAGE_QUERRY=HANDLE_ANCIENT_KEY_USAGE(); //returns 1 if used successfuly. 0 if none in player inventory.
                                                                 // -1 if key broke in the proccess. 
        if(ANCIENT_KEY_USAGE_QUERRY==1) {

            clear(); attron(GREEN_COLORED_CELL);

            mvprintw(ROW/2,COLUMN/2 -22,"Key Used Successfully! Press a Key to Exit.");

            attroff(GREEN_COLORED_CELL); refresh(); getch();

            GLOBAL_INVENTORY_AND_STATS.KEYS--;

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].COLOR='g';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].REAL_CONTENTS='+';

            for(int i=0 ; i<38 ; i++) {

                for(int j=0 ; j<144 ; j++) {

                    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL=='&') {

                        MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR='g';

                    }
                }
            }

        }

        else if(ANCIENT_KEY_USAGE_QUERRY==-1) {

            GLOBAL_INVENTORY_AND_STATS.KEYS--; GLOBAL_INVENTORY_AND_STATS.BROKEN_KEYS++;

            clear(); attron(GOLD_COLORED_CELL);

            mvprintw(ROW/2,COLUMN/2 -21,"Oops. The Key Broke! Press a Key to Exit.");

            if(GLOBAL_INVENTORY_AND_STATS.BROKEN_KEYS==2) {

                mvprintw(ROW/2+1, COLUMN/2 -23,"You Combined 2 Broken Keys to Make a New Key!");

                GLOBAL_INVENTORY_AND_STATS.BROKEN_KEYS-=2;

                GLOBAL_INVENTORY_AND_STATS.KEYS++;

            }

            attroff(GREEN_COLORED_CELL); refresh(); getch();

        }

        else {

            clear(); attron(ORANGE_COLORED_CELL);

            mvprintw(ROW/2 ,COLUMN/2 -23,"You Don't Have Any Keys! Press a key to Exit.");

            attroff(ORANGE_COLORED_CELL); refresh(); getch();

        }

    }

    else if(INPUTTED_PASSKEY==CORRECT_PASSWORD) {

        GLOBAL_PASSWORD_DOOR_BACKGROUND_CHECK=FALSE;

        GLOBAL_DOOR_PASSWORD=-1;

        clear(); attron(GREEN_COLORED_CELL);

        mvprintw(ROW/2 ,COLUMN/2 -20,"Correct Passkey! Press a Key to Continue");

        refresh(); getch();

        attroff(GREEN_COLORED_CELL);

        for(int i=0 ; i<38 ; i++) {
            for(int j=0 ; j<144 ; j++) {
                if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL=='&' && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR!='g') {
                    MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR='g';
                    break;
                }
            }
        }

        if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].PASSWORD_DOOR_TYPE=='2') {

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].PASSWORD_DOOR_TYPE='1';

        }

        else {

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].COLOR='g';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].REAL_CONTENTS='+';

        }
                    
    
    }

    else {

        GLOBAL_PASSWORD_DOOR_BACKGROUND_CHECK=FALSE;

        if(GLOBAL_IS_PASSWORD_DYNAMIC==TRUE) {
            GLOBAL_DOOR_PASSWORD=-1;
        }

        GLOBAL_IS_PASSWORD_DYNAMIC=FALSE;
        
        //determine warning level
        if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].COLOR=='r') {

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].COLOR='y';

            for(int i=0 ; i<38 ; i++) {
                for (int j=0 ; j<144 ; j++) {
                    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL=='&' && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR!='g') {

                        MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR='y';

                    }
                }
            }

            attron(GOLD_COLORED_CELL); clear();
            mvprintw(ROW/2, COLUMN/2 - 29,"Wrong Passkey! 1st Warning Issued. Press a Key to Continue"); 
            attroff(GOLD_COLORED_CELL); refresh(); getch();

        }
        else if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].COLOR=='y') {

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].COLOR='o';

            for(int i=0 ; i<38 ; i++) {
                for (int j=0 ; j<144 ; j++) {
                    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL=='&' && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR!='g') {

                        MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR='o';

                    }
                }
            }
            clear(); attron(ORANGE_COLORED_CELL);
            mvprintw(ROW/2 , COLUMN/2 -27,"Wrong Passkey! Last Warning! Press a Key to Continue."); 
            refresh(); getch();
            attroff(ORANGE_COLORED_CELL);
        }
        else if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].COLOR=='o') {

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].COLOR='r';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][DOOR_Y][DOOR_X].PASSWORD_DOOR_TYPE='l';

            for(int i=0 ; i<38 ; i++) {
                for (int j=0 ; j<144 ; j++) {
                    if(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].SYMBOL=='&' && MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR!='g') {

                        MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][i][j].COLOR='r';

                    }
                }
            }
            clear(); attron(RED_COLORED_CELL);
            mvprintw(ROW/2 , COLUMN/2 -30,"Wrong Passkey! Security Activated! Press a Key to Continue."); 
            refresh(); getch();
            attroff(RED_COLORED_CELL);
        }

    }

    halfdelay(10);

    PRINT_MAP_ON_SCREEN(MAP);

}

int HANDLE_ANCIENT_KEY_USAGE() {

    if(GLOBAL_INVENTORY_AND_STATS.KEYS>0) {

        int RANDOM_TRIAL=rand()%100+1;

        if(RANDOM_TRIAL<=10) {

            return -1;

        }

        else {

            return 1;

        }

    }

    else {

        return 0;

    }

}

void HANDLE_ITEM_PICKUPS(POINT MAP[4][38][144],char *TEMP_PTR) {//---------------------------------------seems OK-----------//
    //change picked up items' color to theme default
    switch(MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS) {

        case -1 :{//key

            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            GLOBAL_INVENTORY_AND_STATS.KEYS++;

            attron(GOLD_COLORED_CELL);

            PRINT_MESSAGE("Picked Up an Ancient Key!");

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,"Picked Up an Ancient Key!");

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            attroff(GOLD_COLORED_CELL);

            refresh();

            break;
        }
        case '$' : {//normal gold

            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            int RANDOM_TRIAL_1=rand()%5; //1st and 2nd digit of gold amount

            int RANDOM_TRIAL_2=rand()%9+1;

            GLOBAL_INVENTORY_AND_STATS.GOLD+=RANDOM_TRIAL_1*10+RANDOM_TRIAL_2;

            GLOBAL_INVENTORY_AND_STATS.SCORE+=10*(RANDOM_TRIAL_1*10+RANDOM_TRIAL_2);

            char STRING[100]="You Picked Up ";

            char TEMP[3]; TEMP[0]=48+RANDOM_TRIAL_1; TEMP[1]=48+RANDOM_TRIAL_2; TEMP[2]='\0';

            strcat(STRING,TEMP);

            strcat(STRING," Gold");

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,STRING);

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            PRINT_MESSAGE(STRING);

            break;
        }
        case '~' : {//food

            if(GLOBAL_INVENTORY_AND_STATS.FOOD_ITEMS<5) {
                
                *TEMP_PTR='.';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

                MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
                =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
                .x,GLOBAL_INVENTORY_AND_STATS.y)-1];
                
                GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

                strcpy(GLOBAL_MOST_RECENT_MESSAGE,"Picked Up 1 Ration.");

                PRINT_MESSAGE("Picked Up 1 Ration.");

                GLOBAL_INVENTORY_AND_STATS.FOOD_ITEMS++;

            }

            else {

                GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='r';

                strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Can't Hold More Than 5 Rations!");

                PRINT_MESSAGE("You Can't Hold More Than 5 Rations!");

            }



            break;
        }
        case '*' : {//black gold

            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            int RANDOM_1=1; int RANDOM_2=rand()%10; int RANDOM_3=rand()%10;

            GLOBAL_INVENTORY_AND_STATS.GOLD+=100*RANDOM_1+10*RANDOM_2+RANDOM_3;

            GLOBAL_INVENTORY_AND_STATS.SCORE+=10*(100*RANDOM_1+10*RANDOM_2+RANDOM_3);

            char STRING[100]="You Picked Up Black Gold : ";

            char TEMPORARY[4]; TEMPORARY[0]=48+RANDOM_1; TEMPORARY[1]=48+RANDOM_2; TEMPORARY[2]=48+RANDOM_3; TEMPORARY[3]='\0';

            strcat(STRING,TEMPORARY);

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,STRING);

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            break;
        }
        case 's' : {//speed spell

            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            GLOBAL_INVENTORY_AND_STATS.S_SPELL++;

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Picked Up a Speed Spell");

            PRINT_MESSAGE("You Picked Up a Speed Spell");

            break;
        }
        case 'h' : {//health spell

            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            GLOBAL_INVENTORY_AND_STATS.H_SPELL++;

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Picked Up a Health Spell");

            PRINT_MESSAGE("You Picked Up a Health Spell");

            break;
        }
        case 'd' : {//damage spell

            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            GLOBAL_INVENTORY_AND_STATS.D_SPELL++;

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Picked Up a Damage Spell");

            PRINT_MESSAGE("You Picked Up a Damage Spell");

            break;
        }
        
        case 'S' : {//sword
            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            GLOBAL_INVENTORY_AND_STATS.SWORD++;

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Picked Up a Sword");

            PRINT_MESSAGE("You Picked Up a Sword");

            break;
        }
        case 'M' : {//mace
            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            GLOBAL_INVENTORY_AND_STATS.MACE++;

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Picked Up a Mace");

            PRINT_MESSAGE("You Picked Up a Mace");

            break;
        }
        case 'W' : {//magic wand
            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            GLOBAL_INVENTORY_AND_STATS.WAND++;

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Picked Up a Magic Wand");

            PRINT_MESSAGE("You Picked Up a Magic Wand");

            break;
        }
        case 'A' : {//arrow
            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            GLOBAL_INVENTORY_AND_STATS.ARROW++;

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Picked Up a Normal Arrow");

            PRINT_MESSAGE("You Picked Up a Normal Arrow");

            break;
        }
        case 'D' : {//dagger
            *TEMP_PTR='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].REAL_CONTENTS='.';

            MAP[GLOBAL_INVENTORY_AND_STATS.FLOOR][GLOBAL_INVENTORY_AND_STATS.y][GLOBAL_INVENTORY_AND_STATS.x].COLOR
            
            =GLOBAL_ALL_THEMES_COLOR[GLOBAL_INVENTORY_AND_STATS.FLOOR][GIVE_SECTOR_NUMBER(GLOBAL_INVENTORY_AND_STATS
            
            .x,GLOBAL_INVENTORY_AND_STATS.y)-1];

            GLOBAL_INVENTORY_AND_STATS.DAGGER++;

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='y';

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Picked Up a Dagger");

            PRINT_MESSAGE("You Picked Up a Dagger");


            break;
        }
    }

}

void OPEN_PAUSE_MENU() {

    PRINT_PAUSE_MENU();

    while(TRUE) {

        int x,y,INPUT; getyx(stdscr,y,x); INPUT=getch();

        if(y==2*ROW/5) {

            if(INPUT==KEY_DOWN) {

                attron(GOLD_COLORED_CELL); mvprintw(2*ROW/5 , COLUMN/2 -2,"Quit"); attroff(GOLD_COLORED_CELL);

                attron(GOLD_HIGHLIGHT |A_BLINK); mvprintw(3*ROW/5 ,COLUMN/2 -7,"Save and Quit"); attroff(GOLD_HIGHLIGHT); attroff(A_BLINK);

            }
            else if(INPUT=='e' || INPUT=='E') {//unsaved

                GLOBAL_EXIT_CODE=1;

                break;
            }

        }
        else if(y==3*ROW/5) {

            if(INPUT==KEY_UP) {

                attron(GOLD_COLORED_CELL); mvprintw(3*ROW/5 , COLUMN/2 -7,"Save and Quit"); attroff(GOLD_COLORED_CELL);

                attron(GOLD_HIGHLIGHT |A_BLINK); mvprintw(2*ROW/5 ,COLUMN/2 -2,"Quit"); attroff(GOLD_HIGHLIGHT); attroff(A_BLINK);

            }
            else if(INPUT==KEY_DOWN) {

                attron(GOLD_COLORED_CELL); mvprintw(3*ROW/5 , COLUMN/2 -7,"Save and Quit"); attroff(GOLD_COLORED_CELL);

                attron(GOLD_HIGHLIGHT |A_BLINK); mvprintw(4*ROW/5 ,COLUMN/2 -2,"Back"); attroff(GOLD_HIGHLIGHT); attroff(A_BLINK);

            }
            else if(INPUT=='e' ||INPUT=='E') {//save and quit

                GLOBAL_EXIT_CODE=3;

                break;

            }

        }
        else {

            if(INPUT==KEY_UP) {

                attron(GOLD_COLORED_CELL); mvprintw(4*ROW/5 , COLUMN/2 -2,"Back"); attroff(GOLD_COLORED_CELL);

                attron(GOLD_HIGHLIGHT |A_BLINK); mvprintw(3*ROW/5 ,COLUMN/2 -7,"Save and Quit"); attroff(GOLD_HIGHLIGHT); attroff(A_BLINK);  

            }

            else if(INPUT=='e' ||INPUT=='E') {//back

                break;
                
            }

        }

        refresh();

    }

}

void PRINT_PAUSE_MENU() {

    clear();

    attron(GOLD_COLORED_CELL | A_ITALIC);

    mvprintw(ROW/5,COLUMN/2 -5,"Pause Menu");

    attroff(A_ITALIC);

    mvprintw(2*ROW/5 , COLUMN/2 -2, "Quit");

    mvprintw(3*ROW/5 , COLUMN/2 -7 ,"Save and Quit");

    attroff(GOLD_COLORED_CELL);

    attron(GOLD_HIGHLIGHT | A_BLINK);

    mvprintw(4*ROW/5,COLUMN/2 -2,"Back");

    attroff(GOLD_HIGHLIGHT); attroff(A_BLINK);

    refresh();

}

void START_GAME() {

    GLOBAL_INVENTORY_AND_STATS.FLOOR=0; GLOBAL_INVENTORY_AND_STATS.KEYS=0; GLOBAL_INVENTORY_AND_STATS.BROKEN_KEYS=0;

    GLOBAL_INVENTORY_AND_STATS.H_SPELL=0; GLOBAL_INVENTORY_AND_STATS.D_SPELL=0 ; GLOBAL_INVENTORY_AND_STATS.S_SPELL=0;

    GLOBAL_INVENTORY_AND_STATS.SCORE=0; GLOBAL_INVENTORY_AND_STATS.GOLD=0; GLOBAL_INVENTORY_AND_STATS.FOOD_ITEMS=0;

    GLOBAL_INVENTORY_AND_STATS.HUNGER=100; GLOBAL_INVENTORY_AND_STATS.HP=0;

    GLOBAL_INVENTORY_AND_STATS.ARROW=0; GLOBAL_INVENTORY_AND_STATS.DAGGER=0; GLOBAL_INVENTORY_AND_STATS.MACE=1; 

    GLOBAL_INVENTORY_AND_STATS.SWORD=0; GLOBAL_INVENTORY_AND_STATS.WAND=0; GLOBAL_INVENTORY_AND_STATS.TIME_PLAYED=0;

    for(int i=0 ; i<4 ; i++) {
        for(int j=0 ; j<6 ; j++) {
            GLOBAL_ALL_THEMES_COLOR[i][j]=0;
        }
    }

    setlocale(LC_ALL, "");  srand(time(NULL));

    initscr();  noecho();   curs_set(0);    keypad(stdscr,TRUE);

    getmaxyx(stdscr,ROW,COLUMN);

    if(has_colors()==TRUE) {

        start_color();

        init_pair(1,COLOR_YELLOW,COLOR_BLACK); init_pair(2,COLOR_WHITE,COLOR_YELLOW);

        init_pair(3,COLOR_RED,COLOR_BLACK); init_pair(8,COLOR_WHITE,COLOR_RED);

        init_pair(4,COLOR_GREEN,COLOR_BLACK); init_pair(9,COLOR_WHITE,COLOR_GREEN);

        init_pair(5,COLOR_BLUE,COLOR_BLACK); init_pair(10,COLOR_WHITE,COLOR_BLUE);

        init_pair(6,COLOR_WHITE,COLOR_BLACK); init_pair(11,COLOR_BLACK,COLOR_WHITE);

        init_color(COLOR_ORANGE,690,310,0);

        init_pair(7,COLOR_ORANGE,COLOR_BLACK);

        //add colors later
    }

}

void SHUTDOWN_GAME(Mix_Music *music) {

    cbreak();   clear();    attrset(GOLD_COLORED_CELL);  curs_set(0);

    mvprintw(ROW/2,COLUMN/2 -24,"Hope You Had Fun! Press a Key to Close The Game.");

    refresh();  getch();    endwin();

    Mix_CloseAudio();   Mix_FreeMusic(music);   SDL_Quit();

}

void FIND_MAX_HP() {

    if(GLOBAL_SETTINGS.DIFFICULTY=='h') {
        GLOBAL_MAX_HP=50;
    }
    else if(GLOBAL_SETTINGS.DIFFICULTY=='n') {
        GLOBAL_MAX_HP=100;
    }
    else {
        GLOBAL_MAX_HP=150;
    }

    if(GLOBAL_INVENTORY_AND_STATS.HP==0) {
        GLOBAL_INVENTORY_AND_STATS.HP=GLOBAL_MAX_HP;
    }
    if(GLOBAL_INVENTORY_AND_STATS.HP>GLOBAL_MAX_HP) {
        GLOBAL_INVENTORY_AND_STATS.HP=GLOBAL_MAX_HP;
    }


}

void SAVE_GAME(POINT MAP[4][38][144],int DELTA_T_PLAYED) {

    if(GLOBAL_IS_GUEST==FALSE) {

        char FREE_SPACE[100];

        clear(); cbreak(); attron(GOLD_COLORED_CELL); echo(); curs_set(1);

        mvprintw(0,COLUMN/2 -8,"Name Your Save.");

        mvprintw(ROW/2, COLUMN/2 -8 , "###############");

        move(ROW/2 -1 , COLUMN/2 -8); refresh(); attroff(GOLD_COLORED_CELL);

        getstr(FREE_SPACE); curs_set(0); noecho();

        char STRING[100]="saves_";

        strcat(STRING,GLOBAL_LOGGED_IN_USER);

        strcat(STRING,".txt");

        FILE *SAVE_LOCATION=fopen(STRING,"w+");

        fprintf(SAVE_LOCATION,"%s ",GLOBAL_LOGGED_IN_USER);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.H_SPELL);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.S_SPELL);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.D_SPELL);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.MACE);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.DAGGER);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.WAND);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.ARROW);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.SWORD);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.GOLD);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.SCORE);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.FOOD_ITEMS);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.HP);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.HUNGER);

        fprintf(SAVE_LOCATION,"%d ",DELTA_T_PLAYED);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.x);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.y);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.FLOOR);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.KEYS);

        fprintf(SAVE_LOCATION,"%d ",GLOBAL_INVENTORY_AND_STATS.BROKEN_KEYS);

        for(int i=0 ; i<4 ; i++) {
            for(int j=0 ; j<38 ; j++) {
                for(int k=0 ; k<144 ; k++) {

                    fprintf(SAVE_LOCATION,"%c",MAP[i][j][k].COLOR);

                    fprintf(SAVE_LOCATION,"%c",MAP[i][j][k].IS_DISCOVERED);

                    fprintf(SAVE_LOCATION,"%c",MAP[i][j][k].IS_START);

                    fprintf(SAVE_LOCATION,"%c",MAP[i][j][k].PASSWORD_DOOR_TYPE);

                    fprintf(SAVE_LOCATION,"%c",MAP[i][j][k].REAL_CONTENTS);

                    fprintf(SAVE_LOCATION,"%c",MAP[i][j][k].SYMBOL);
                }
            }
        }

        fclose(SAVE_LOCATION);

    }

    else {

        clear(); attron(RED_COLORED_CELL); cbreak();

        mvprintw(ROW/2 ,COLUMN/2 -24 ,"As a Guest, You Can't Save! Press a Key to Exit.");

        attroff(RED_COLORED_CELL); refresh(); getch();

    }
}

void PERFORM_DEATH_CHECK() {

    if(GLOBAL_INVENTORY_AND_STATS.HP<=0) {

        OPEN_DEATH_MENU();

    }

}

void OPEN_DEATH_MENU() {

    GLOBAL_EXIT_CODE=1;

    clear(); attron(RED_COLORED_CELL); cbreak();

    mvprintw(ROW/2, COLUMN/2 -17 ,"You Died! Press a Key to Continue.");

    refresh();  attroff(RED_COLORED_CELL); getch();

}

void REDUCE_HUNGER() {

    unsigned long long int NEW_QUERRY=time(NULL);

    if(GLOBAL_INVENTORY_AND_STATS.HUNGER==0) {

        if(NEW_QUERRY-GLOBAL_LAST_HUNGER_TIME_QUERRY>=10) {

            GLOBAL_INVENTORY_AND_STATS.HP--; attron(RED_COLORED_CELL);

            PRINT_MESSAGE("You Are Starving!"); attroff(RED_COLORED_CELL); refresh();

            strcpy(GLOBAL_MOST_RECENT_MESSAGE,"You Are Starving!");

            GLOBAL_MOST_RECENT_COLOR_FOR_MESSAGE='r';

            GLOBAL_LAST_HUNGER_TIME_QUERRY=NEW_QUERRY;

        }

    }

    else {

        if(NEW_QUERRY-GLOBAL_LAST_HUNGER_TIME_QUERRY>=5) {

            GLOBAL_INVENTORY_AND_STATS.HUNGER--;

            GLOBAL_LAST_HUNGER_TIME_QUERRY=NEW_QUERRY;

        }

    }

}

void OPEN_FOOD_MENU() {

    cbreak(); 

    while(TRUE) {

        clear(); attron(GOLD_COLORED_CELL);

        move(0,0);

        printw("Saturation Bar : |");

        for(int i=1 ; i<=GLOBAL_INVENTORY_AND_STATS.HUNGER/10 ; i++) {
            printw("#");
        }

        mvprintw(0,28,"| ");

        move(2,0); printw("Health Bar : |"); 

        for(int i=1 ; i<=10 *GLOBAL_INVENTORY_AND_STATS.HP/GLOBAL_MAX_HP ; i++) {
            printw("#");
        }

        mvprintw(2,24,"| ");

        mvprintw(4,COLUMN/2 -26 ,"Select a Food Item to Use. Press Backspace to Exit."); attroff(GOLD_COLORED_CELL);

        attron(GOLD_HIGHLIGHT | A_BLINK);    mvprintw(ROW/2,COLUMN/2 - 9,"Regular Rations :%d",GLOBAL_INVENTORY_AND_STATS.FOOD_ITEMS); 
        
        attroff(GOLD_HIGHLIGHT); attroff(A_BLINK);

        int INPUT=getch();

        if(INPUT==KEY_BACKSPACE) {

            break;
        }

        if(INPUT=='e' || INPUT=='E') {

            if(GLOBAL_INVENTORY_AND_STATS.FOOD_ITEMS>0) {

                GLOBAL_INVENTORY_AND_STATS.FOOD_ITEMS--;

                GLOBAL_INVENTORY_AND_STATS.HP+=20;

                if(GLOBAL_INVENTORY_AND_STATS.HP>GLOBAL_MAX_HP) {

                    GLOBAL_INVENTORY_AND_STATS.HP=GLOBAL_MAX_HP;

                }

                GLOBAL_INVENTORY_AND_STATS.HUNGER+=30;

                if(GLOBAL_INVENTORY_AND_STATS.HUNGER>100) {

                    GLOBAL_INVENTORY_AND_STATS.HUNGER=100;

                }

            }

            else {

                clear(); attron(RED_COLORED_CELL); mvprintw(ROW/2,COLUMN/2 -23,"You Have Nothing to Eat! Press a Key to Exit.");

                attroff(RED_COLORED_CELL); getch();

            }
        }


    }

    halfdelay(10); 

}

void HANDLE_TRAP_ACTIVATION() {//will change in phase 2

    GLOBAL_INVENTORY_AND_STATS.HP-=5;

}


void OPEN_WEAPONS_MENU() {

    clear(); cbreak(); attron(GOLD_COLORED_CELL);

}

void OPEN_SPELLS_MENU() {




}


//------------------------------------------------------------End----------------------------------------------------------------------//