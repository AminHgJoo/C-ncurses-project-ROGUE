#include <ncurses.h>    //403110585 Mohammad Amin Haghjoo
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <math.h>

typedef struct point {

    char SYMBOL;

    char REAL_CONTENTS;

    int IS_DISCOVERED;

} POINT;

typedef struct settings {

    char HERO_COLOR;

    char MUSIC[50];

    char DIFFICULTY;

} SETTINGS;

typedef struct door {

    int x;

    int y;

    int SECTOR;

    char DIRECTION;

} DOOR;








SETTINGS GLOBAL_GUEST_SETTINGS;
int GLOBAL_LOAD_SAVE=FALSE;
int ROW,COLUMN;
int GLOBAL_IS_GUEST=0;
char GLOBAL_LOGGED_IN_USER[100];


void HANDLE_GUEST_USER();
char GAME_STARTUP_MENU();
void USER_REGISTERATION();
void HANDLE_NEXT_MENUS(char);
int USER_LOGIN();
void CLEAR_TEXT_FOR_LOGIN_ERRORS();
void RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();
void GENERATE_RANDOM_PASSWORD(char[]);
int VALIDATE_EMAIL(char[]);
void PRE_GAME_MENU();
void PRINT_PREGAME_MENU();
void OPEN_LEADERBOARDS();
void OPEN_SETTINGS();
void PRINT_SETTINGS_MENU();
void OPEN_MUSIC_MENU();
void OPEN_DIFFICULTY_MENU();
void OPEN_HERO_COLOR_MENU();
void PRINT_COLOR_GUI();




void LOAD_MAP(POINT [4][38][144]);

void GENERATE_MAP(POINT [4][38][144]);

void GENERATE_FLOOR_SKELETON(POINT [38][144]);

void CONNECT_ROOMS(POINT [38][144]);



int main() {

    setlocale(LC_ALL, "");

    srand(time(NULL));

    initscr();

    noecho();

    curs_set(0);

    keypad(stdscr,TRUE);

    getmaxyx(stdscr,ROW,COLUMN);

    if(has_colors()==TRUE) {

        start_color();

        init_pair(1,COLOR_YELLOW,COLOR_BLACK); init_pair(2,COLOR_WHITE,COLOR_YELLOW);

        init_pair(3,COLOR_RED,COLOR_BLACK); init_pair(8,COLOR_WHITE,COLOR_RED);

        init_pair(4,COLOR_GREEN,COLOR_BLACK); init_pair(9,COLOR_WHITE,COLOR_GREEN);

        init_pair(5,COLOR_BLUE,COLOR_BLACK); init_pair(10,COLOR_WHITE,COLOR_BLUE);

        init_pair(6,COLOR_WHITE,COLOR_BLACK); init_pair(11,COLOR_BLACK,COLOR_WHITE);

        //add colors later
    }

    char NEXT_MENU=GAME_STARTUP_MENU();

    HANDLE_NEXT_MENUS(NEXT_MENU);

    HANDLE_GUEST_USER();

    PRE_GAME_MENU();

    POINT MAP[4][38][144];    //4 floors, each floor has 38 rows and 144 columns . struct point is defined for easier tracking of exploration

    if(GLOBAL_LOAD_SAVE==TRUE) {//load from save onto MAP

        LOAD_MAP(MAP);

    }
    else {//create new map from scratch

        GENERATE_MAP(MAP);

    }

    while(TRUE) {//game loop









    }




    
    getch();

    endwin();

    return 0;
}

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

    FILE *SCOREBOARD=fopen("user_scores.txt","a+");

    fprintf(SCOREBOARD,"%s %d %d %d %d\n",USERNAME,0,0,0,0);

    fclose(SCOREBOARD);

    FILE *SETTINGS=fopen("user_settings.txt","a+");

    fprintf(SETTINGS,"%s w default n\n",USERNAME);

    fclose(SETTINGS);

    FILE *SAVES=fopen("user_saves_and_inventory.txt","a+");

    fprintf(SAVES,"%s f\n",USERNAME);

    fclose(SAVES);

    noecho();

    curs_set(0);

    CLEAR_TEXT_FOR_LOGIN_ERRORS();

    RESET_CURSOR_AND_COLORS_FOR_LOGIN_ERROR();

    mvprintw(ROW/3+ 3,COLUMN/2 - 25,"Registeration Successful. Press a Key to Continue.");

    refresh();

    getch();

    attroff(COLOR_PAIR(1));

    strcpy(GLOBAL_LOGGED_IN_USER,USERNAME);

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
void PRE_GAME_MENU() {

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

                break;

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
            else if(INPUT==KEY_DOWN) {

                attron(COLOR_PAIR(1));

                mvprintw(2*ROW/5,COLUMN/2 -7,"Load Last Save");

                attroff(COLOR_PAIR(1));

                attron(COLOR_PAIR(2) |A_BLINK);

                mvprintw(3* ROW/5, COLUMN/2 -6,"Leaderboards");

                attroff(COLOR_PAIR(2)); attroff(A_BLINK);

                refresh();

            }
            else if(INPUT=='e' || INPUT=='E') {

                GLOBAL_LOAD_SAVE=TRUE;

                break;

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
void OPEN_LEADERBOARDS() {//print and sort scores. if user is guest do not search for their ranking
    
    if(GLOBAL_IS_GUEST==TRUE) {


    }

    else {




    }    

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
void OPEN_MUSIC_MENU() {//
    if(GLOBAL_IS_GUEST==TRUE) {

    }

    else {
        
    }
}
void OPEN_DIFFICULTY_MENU() {//
    if(GLOBAL_IS_GUEST==TRUE) {

    }

    else {

    }
}
void OPEN_HERO_COLOR_MENU() {//
    
    PRINT_COLOR_GUI();

    if(GLOBAL_IS_GUEST==TRUE) {






    }

    else {
        




    }







}
void HANDLE_GUEST_USER() {

    if(GLOBAL_IS_GUEST==TRUE) {

        GLOBAL_GUEST_SETTINGS.DIFFICULTY='n';

        GLOBAL_GUEST_SETTINGS.HERO_COLOR='w';

        strcpy(GLOBAL_GUEST_SETTINGS.MUSIC,"DEFAULT");

    }

}
void PRINT_COLOR_GUI() {

    clear();

    attron(COLOR_PAIR(3));

    mvprintw(2*ROW/5, COLUMN/2 -2 ,"Red");

    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(4));

    mvprintw(3*ROW/5,COLUMN/2 -3,"Green");

    attroff(COLOR_PAIR(4));

    attron(COLOR_PAIR(5));

    mvprintw(4*ROW/5, COLUMN/2 -2,"Blue");

    attroff(COLOR_PAIR(5));

    attron(COLOR_PAIR(1));

    mvprintw(ROW-1,COLUMN/2 - 22,"Press e or E to Select and Arrow keys to Move");

    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(11) | A_BLINK);

    mvprintw(ROW/5,COLUMN/2 -3 ,"White");

    attroff(COLOR_PAIR(11)); attroff(A_BLINK);

    refresh();

}
void LOAD_MAP(POINT MAP[4][38][144]) {//

}
void GENERATE_MAP(POINT MAP[4][38][144]) {//

    


}

void GENERATE_FLOOR_SKELETON(POINT FLOOR[38][144]) {//

    for(int i=0 ; i<38 ; i++) {

        for(int j=0 ; j<144 ; j++) {

            FLOOR[i][j].IS_DISCOVERED=0;

            FLOOR[i][j].SYMBOL=' ';
        }
    }
    
    int X_START=rand()%27;
    int Y_START=rand()%9+1;
    int BIG_OR_SMALL=rand()%2;

    if(BIG_OR_SMALL==1) {
        int door1=rand()%14+X_START+1;

        int door2=rand()%5+Y_START+1;

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

    X_START=rand()%27+48;
    Y_START=rand()%9+1;
    BIG_OR_SMALL=rand()%2;

    if(BIG_OR_SMALL==1) {

        int door1=rand()%14+X_START+1;

        int door2=rand()%5+Y_START+1;

        int door3=rand()%5+Y_START+1;

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
    
    X_START=rand()%27+96;
    Y_START=rand()%9+1;
    BIG_OR_SMALL=rand()%2;

    if(BIG_OR_SMALL==1) {

        int door1=rand()%14+X_START+1;

        int door3=rand()%5+Y_START+1;

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

    X_START=rand()%27;
    Y_START=rand()%9+20;
    BIG_OR_SMALL=rand()%2;
    if(BIG_OR_SMALL==1) {

        int door1=rand()%14+X_START+1;

        int door2=rand()%5+Y_START+1;


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

    X_START=rand()%27+48;
    Y_START=rand()%9+20;
    BIG_OR_SMALL=rand()%2;

    if(BIG_OR_SMALL==1) {

        int door1=rand()%14+X_START+1;

        int door2=rand()%5+Y_START+1;

        int door3=rand()%5+Y_START+1;

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

    X_START=rand()%27+96;
    Y_START=rand()%9+20;
    BIG_OR_SMALL=rand()%2;

    if(BIG_OR_SMALL==1) {

        int door1=rand()%14+X_START+1;

        int door3=rand()%5+Y_START+1;

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

    CONNECT_ROOMS(FLOOR);

    //end of non-secret gen

    for(int i=0 ; i<38 ; i++) {
        for(int j=0 ; j<144 ; j++) {
            FLOOR[i][j].REAL_CONTENTS=FLOOR[i][j].SYMBOL;
        }
    }

    //secret objects generation. change real contents but not symbol





}

void CONNECT_ROOMS(POINT FLOOR[38][144]) {

    DOOR ALL_DOORS[14];

    int k=0;
    //search the board for doors, mark their locations and sectors
    for( int i=0 ; i<38 ; i++) {
        for( int j=0 ; j<144 ; j++) {
            if(FLOOR[i][j].SYMBOL=='+') {

                ALL_DOORS[k].x=j;

                ALL_DOORS[k].y=i;

                if(i>=0 && i<19) {//sectors 1,2,3

                    if(j>=0 && j<48) {

                        ALL_DOORS[k].SECTOR=1;

                    }

                    if(j>=48 && j<96) {

                        ALL_DOORS[k].SECTOR=2;

                    }

                    if(j>=96 && j<144) {

                        ALL_DOORS[k].SECTOR=3;

                    }

                }

                else {//sectors 4,5,6

                    if(j>=0 && j<48) {

                        ALL_DOORS[k].SECTOR=4;

                    }

                    if(j>=48 && j<96) {

                        ALL_DOORS[k].SECTOR=5;

                    }

                    if(j>=96 && j<144) {

                        ALL_DOORS[k].SECTOR=6;

                    }

                }

                k++;

            }
        }
    }

    //determine the direction of the doors. for easier reference, sort them first (in terms of sector).

    for(int i=0 ; i<14 ; i++) {
        for(int j=i+1 ; j<14 ; j++) {
            if(ALL_DOORS[i].SECTOR>ALL_DOORS[j].SECTOR) {
                DOOR temp=ALL_DOORS[i];
                ALL_DOORS[i]=ALL_DOORS[j];
                ALL_DOORS[j]=temp;

            }

        }
    }

    //sector 1 has 2 doors :

    if(ALL_DOORS[0].x>ALL_DOORS[1].x) {

        ALL_DOORS[0].DIRECTION='r';

        ALL_DOORS[1].DIRECTION='d';

    }
    else {
        ALL_DOORS[1].DIRECTION='r';

        ALL_DOORS[0].DIRECTION='d';
    }

    //sector 2 has 3 :

    if(ALL_DOORS[2].y>ALL_DOORS[3].y && ALL_DOORS[2].y>ALL_DOORS[4].y) {

        ALL_DOORS[2].DIRECTION='d';

        if(ALL_DOORS[3].x>ALL_DOORS[4].x) {

            ALL_DOORS[3].DIRECTION='r';

            ALL_DOORS[4].DIRECTION='l';
        }

        else {
            ALL_DOORS[4].DIRECTION='r';

            ALL_DOORS[3].DIRECTION='l';
        }


    }

    if(ALL_DOORS[3].y>ALL_DOORS[2].y && ALL_DOORS[3].y>ALL_DOORS[4].y) {

        ALL_DOORS[3].DIRECTION='d';

         if(ALL_DOORS[2].x>ALL_DOORS[4].x) {

            ALL_DOORS[2].DIRECTION='r';

            ALL_DOORS[4].DIRECTION='l';
        }

        else {
            ALL_DOORS[4].DIRECTION='r';

            ALL_DOORS[2].DIRECTION='l';
        }

    }

    if(ALL_DOORS[4].y>ALL_DOORS[2].y && ALL_DOORS[4].y>ALL_DOORS[3].y) {

        ALL_DOORS[4].DIRECTION='d';

         if(ALL_DOORS[3].x>ALL_DOORS[2].x) {

            ALL_DOORS[3].DIRECTION='r';

            ALL_DOORS[2].DIRECTION='l';
        }

        else {
            ALL_DOORS[2].DIRECTION='r';

            ALL_DOORS[3].DIRECTION='l';
        }

    }

    //sector 3 has 2:

    if(ALL_DOORS[5].y>ALL_DOORS[6].y) {

        ALL_DOORS[5].DIRECTION='d';

        ALL_DOORS[6].DIRECTION='l';

    }

    else {

        ALL_DOORS[6].DIRECTION='d';

        ALL_DOORS[5].DIRECTION='l';

    }

    //sector 4 has 2

    if(ALL_DOORS[7].x>ALL_DOORS[8].x) {

        ALL_DOORS[7].DIRECTION='r';

        ALL_DOORS[8].DIRECTION='u';

    }
    
    else {

        ALL_DOORS[8].DIRECTION='r';

        ALL_DOORS[7].DIRECTION='u';

    }

    //sector 5 has 3

    if(ALL_DOORS[9].y<ALL_DOORS[10].y && ALL_DOORS[9].y<ALL_DOORS[11].y) {

        ALL_DOORS[9].DIRECTION='u';

        if(ALL_DOORS[10].x>ALL_DOORS[11].x) {

            ALL_DOORS[10].DIRECTION='r';

            ALL_DOORS[11].DIRECTION='l';

        }

        else {

            ALL_DOORS[11].DIRECTION='r';

            ALL_DOORS[10].DIRECTION='l';

        }

    }

    else if(ALL_DOORS[10].y<ALL_DOORS[9].y && ALL_DOORS[10].y<ALL_DOORS[11].y) {

        ALL_DOORS[10].DIRECTION='u';

        if(ALL_DOORS[9].x>ALL_DOORS[11].x) {

            ALL_DOORS[9].DIRECTION='r';

            ALL_DOORS[11].DIRECTION='l';

        }

        else {

            ALL_DOORS[11].DIRECTION='r';

            ALL_DOORS[9].DIRECTION='l';

        }
    }

    else {

        ALL_DOORS[11].DIRECTION='u';

        if(ALL_DOORS[10].x>ALL_DOORS[9].x) {

            ALL_DOORS[10].DIRECTION='r';

            ALL_DOORS[9].DIRECTION='l';

        }

        else {

            ALL_DOORS[9].DIRECTION='r';

            ALL_DOORS[10].DIRECTION='l';

        }
    }

    //sector 6 has 2

    if(ALL_DOORS[12].y>ALL_DOORS[13].y) {

        ALL_DOORS[12].DIRECTION='l';

        ALL_DOORS[13].DIRECTION='u';

    }

    else {

        ALL_DOORS[13].DIRECTION='l';

        ALL_DOORS[12].DIRECTION='u';
    }

    //all doors now have been found and classified in terms of direction and sector. now to connect them.

    //test :

    /*  for(int k=0 ; k<14 ; k++) {

        int i=ALL_DOORS[k].y;

        int j=ALL_DOORS[k].x;

        FLOOR[i][j].SYMBOL=ALL_DOORS[k].DIRECTION;

        }   */

    //test end

    //1r -> 2l & 1d -> 4u | 2r ->3l & 2d -> 5u | 3d ->6u | 4r -> 5l | 5r -> 6l //

    for(int k=0 ; k<14 ; k++) {

        if(ALL_DOORS[k].SECTOR==1 && ALL_DOORS[k].DIRECTION=='r') {//1r

            int i=ALL_DOORS[k].y;

            int j=ALL_DOORS[k].x+1;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(ALL_DOORS[l].SECTOR==2 && ALL_DOORS[l].DIRECTION=='l') {

                    I_FINAL=ALL_DOORS[l].y;

                    J_FINAL=ALL_DOORS[l].x;

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

        if(ALL_DOORS[k].SECTOR==1 && ALL_DOORS[k].DIRECTION=='d') {//1d
            int i=ALL_DOORS[k].y+1;

            int j=ALL_DOORS[k].x;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(ALL_DOORS[l].SECTOR==4 && ALL_DOORS[l].DIRECTION=='u') {

                    I_FINAL=ALL_DOORS[l].y;

                    J_FINAL=ALL_DOORS[l].x;

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

        if(ALL_DOORS[k].SECTOR==2 && ALL_DOORS[k].DIRECTION=='r') {//2r
            int i=ALL_DOORS[k].y;

            int j=ALL_DOORS[k].x+1;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(ALL_DOORS[l].SECTOR==3 && ALL_DOORS[l].DIRECTION=='l') {

                    I_FINAL=ALL_DOORS[l].y;

                    J_FINAL=ALL_DOORS[l].x;

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

        if(ALL_DOORS[k].SECTOR==2 && ALL_DOORS[k].DIRECTION=='d') {//2d
            int i=ALL_DOORS[k].y+1;

            int j=ALL_DOORS[k].x;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(ALL_DOORS[l].SECTOR==5 && ALL_DOORS[l].DIRECTION=='u') {

                    I_FINAL=ALL_DOORS[l].y;

                    J_FINAL=ALL_DOORS[l].x;

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

        if(ALL_DOORS[k].SECTOR==3 && ALL_DOORS[k].DIRECTION=='d') {//3d
            int i=ALL_DOORS[k].y+1;

            int j=ALL_DOORS[k].x;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(ALL_DOORS[l].SECTOR==6 && ALL_DOORS[l].DIRECTION=='u') {

                    I_FINAL=ALL_DOORS[l].y;

                    J_FINAL=ALL_DOORS[l].x;

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

        if(ALL_DOORS[k].SECTOR==4 && ALL_DOORS[k].DIRECTION=='r') {//4r
            int i=ALL_DOORS[k].y;

            int j=ALL_DOORS[k].x+1;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(ALL_DOORS[l].SECTOR==5 && ALL_DOORS[l].DIRECTION=='l') {

                    I_FINAL=ALL_DOORS[l].y;

                    J_FINAL=ALL_DOORS[l].x;

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

        if(ALL_DOORS[k].SECTOR==5 && ALL_DOORS[k].DIRECTION=='r') {//5r
            int i=ALL_DOORS[k].y;

            int j=ALL_DOORS[k].x+1;

            int I_FINAL;

            int J_FINAL;

            for(int l=0 ; l<14 ; l++) {

                if(ALL_DOORS[l].SECTOR==6 && ALL_DOORS[l].DIRECTION=='l') {

                    I_FINAL=ALL_DOORS[l].y;

                    J_FINAL=ALL_DOORS[l].x;

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
