#include <ncurses.h>    //403110585 Mohammad Amin Haghjoo
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>

typedef struct point {
    int x;

    int y;

} POINT;








int GLOBAL_LOAD_SAVE=FALSE;
int ROW,COLUMN;
int GLOBAL_IS_GUEST=0;
char GLOBAL_LOGGED_IN_USER[100];
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

        init_pair(1,COLOR_YELLOW,COLOR_BLACK);

        init_pair(2,COLOR_WHITE,COLOR_YELLOW);

        init_pair(3,COLOR_RED,COLOR_BLACK);

        //add colors later
    }

    char NEXT_MENU=GAME_STARTUP_MENU();

    HANDLE_NEXT_MENUS(NEXT_MENU);

    PRE_GAME_MENU();

    if(GLOBAL_LOAD_SAVE==TRUE) {//load from save

        

    }
    else {//create new game



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

    fprintf(SCOREBOARD,"%s %d\n",USERNAME,0);

    fclose(SCOREBOARD);

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

    move(ROW/5,COLUMN/2 -4);

    attroff(COLOR_PAIR(1));

    refresh();
}
void OPEN_LEADERBOARDS() {

    

}
void OPEN_SETTINGS() {



}