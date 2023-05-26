#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include <time.h>


#define LIN 3 // nr de linii din patratel
#define COL 5 // nr de coloane din patratel
#define POZ(i, j) (10 * (i + 1) + j + 2)


typedef struct Sudoku{
    char tab[9][9];
} Sudoku;


/*Initializeaza ferestrele si
deseneaza tabelul*/
void Desen_tabel(WINDOW *tabel[]) {
    int starty, startx, j = 0;

    starty = 0;
    startx = 0;
    tabel[j] = newwin(2 * LIN, 10 * COL, starty, startx);
    mvwprintw(tabel[j], LIN / 2, (10 * COL - strlen("Sudoku")) / 2, "Sudoku");
    wrefresh(tabel[j]);

    /*indexul de la care incepe tabla propriu-zisa*/
    j = 1;

    /*se creaza tabla de 10 pe 10, pentru a acomoda linia si coloana
    care numeroteaza tabla de joc*/
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            startx = col * COL;
            starty = (row + 1) * LIN;
            tabel[j] = newwin(LIN, COL, starty, startx);
            j++;
        }
    }

    /*adaugare numere pe linii si coloane, plus chenar*/
    int i;
    int number = 1;
    for (i = 1; i <= 100; i++) {
        box(tabel[i], 0, 0);

        // Add numbering to the borders
        if (i % 10 == 1) {
            mvwprintw(tabel[i], LIN / 2, COL/2, "%d", (i - 1) / 10);
        }
        if (i <= 10) {
            mvwprintw(tabel[i], LIN / 2, COL / 2, "%d", i - 1);
        }

        wrefresh(tabel[i]);
        number++;
    }

    /*adaugare mesaj de control*/
    j = 101;
    starty += LIN;
    startx = 0;
    tabel[j] = newwin(2 * LIN, 10 * COL, starty, startx);
    /*adaugare mesaj de control*/
    mvwprintw(tabel[j], LIN / 2, (10 * COL - strlen("Load table... Complete")) / 2, "Load table... Complete");
    wrefresh(tabel[j]);
}

/*scrie in ferestrele tabelului matricea initiala sau matricea raspuns*/
void Write(WINDOW *tabel[], char **initial) {
    int i, j;
    int pozitie;
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            pozitie = POZ(i, j);
            if (initial[i][j] != '*') {
                mvwprintw(tabel[pozitie], LIN / 2, COL / 2, "%c", initial[i][j]);
                wrefresh(tabel[pozitie]);
            }
        }
    }
}


/*Face citirea pe tabla, tine cont de asezarea pe tabla si de optiunea utilizatorului*/
void Citire(WINDOW *tabel[], int *lin, int *col, char **curent, char **initial, int *optiune){
    /*retine pe rand caracterele
    introduse de utilizator*/
    char ch = '\0';

    int i = (*lin);
    int j = (*col);
    int pozitie = POZ(i, j);

    while(i >= 0 && i < 9 && j >= 0 && j < 9){
        /*citeste doar cifre sau '\b' sau '\n' sau ':'*/
        ch = wgetch(tabel[0]);

        while(!(ch >= '0' && ch <= '9') && !(ch == 8 || ch == 127)
        && ch != '\n' && ch != ':'){
            /*deplasarea se face cu wasd (ca la jocuri)*/
            if (ch == 'a'){
                if(j > 0){
                    j--;
                    werase(tabel[101]);
                    mvwprintw(tabel[101], 0, 0, "pozitia %d %d", i+1, j+1);
                    wrefresh(tabel[101]);
                }
            }
            else if (ch =='d'){
                if(j < 8){
                    j++;
                    werase(tabel[101]);
                    mvwprintw(tabel[101], 0, 0, "pozitia %d %d", i+1, j+1);
                    wrefresh(tabel[101]);
                }
            }
            else if (ch == 'w'){
                if(i > 0){
                    i--;
                    werase(tabel[101]);
                    mvwprintw(tabel[101], 0, 0, "pozitia %d %d", i+1, j+1);
                    wrefresh(tabel[101]);
                }
            }
            else if (ch == 's'){
                if(i < 8){
                    i++;
                    werase(tabel[101]);
                    mvwprintw(tabel[101], 0, 0, "pozitia %d %d", i+1, j+1);
                    wrefresh(tabel[101]);
                }
            }
            werase(tabel[101]);
            mvwprintw(tabel[101], 0, 0, "pozitia %d %d", i+1, j+1);
            wrefresh(tabel[101]);
            ch = wgetch(tabel[0]);
        }
        pozitie = POZ(i, j);

        /*accesare meniu de control*/
        if(ch == ':'){
           (*optiune) = 3;
            goto end;
        }

        if(ch == 8 || ch == 127){
            /*daca am citit \b, sterg ce am scris anterior*/
            if(i > 0){
                pozitie = POZ(i, j);
                mvwprintw(tabel[pozitie], LIN/2, COL/2, " ");
                wrefresh(tabel[pozitie]);
            }
        }

        if (ch == '\n') {
            /*daca am citit enter, atunci ies din functie,
            deoarece se cere verificarea castigului*/
            werase(tabel[101]);
            wprintw(tabel[101], "Ai apasat Enter! Vrei sa verifici castigul?\n");
            wprintw(tabel[101], "Apasa 1 pt intoarcere joc si 2 pt verificare\n");
            wrefresh(tabel[101]);
            wscanw(tabel[0], "%d", &(*optiune));
            if ((*optiune) != 2) {
                (*optiune) = 4;
                goto end;
            }
            
            goto end;
        }

        if((ch >= '0' && ch <= '9') || (ch == 8 || ch == 127)) {
            /*altfel, afisez caracterul citit*/
            if (initial[i][j] != '*') {
                /*daca caracterul este initial, nu il pot modifica*/
                continue;
            } else {
                if(ch == 8 || ch == 127)
                    ch = ' ';
                pozitie = POZ(i, j);
                mvwprintw(tabel[pozitie], LIN/2, COL/2, "%c", ch);
                wrefresh(tabel[pozitie]);
                curent[i][j] = ch;
            }
        }
    }
end:
    (*lin) = i;
    (*col) = j;
    werase(tabel[101]);
    wrefresh(tabel[101]);
}

/*alege un cuvant random
din dictionar*/
void Random(char **initial, char **raspuns){
    Sudoku dict[20];
    char linie_1[10][82] = {"73*5*1****69*3****5*1**2***3*46*32***12**4****5****628**5*98*6*24*9*57****8*613549",
                            "5**2*9*6**6273**984*96**1329*****4*7*8734**1*2**1****334**268****6*51*****1***2*6",
                            "45*********2*7*63********28***95*****86***2***2*6**75*******476*7**45*****8**9***",
                            "*75*9***6*23*8**4*8****3**15**7*2****4*8*6*2****9*1**39**4****7*6**7*58*7***1*39*",
                            "*3**8***1**74*1*5*9***5*2****2**5*1*3**21*5**59**6***2**65*2*****96***27*****8*65",
                            "8*6**3*9**4**1**682**87***51*8**5*2**3*1***5*7*5*3*9***21**7*4*6***2*8***876*4**3",
                            "5*****8***2**7**4*87*5***1**8*1**6**3**2*9**1**1**6*9**1***5*28*5**6**3***2*****6",
                            "*4***1**3****5**7956***28*41**27**8**82***96**3**18**73*61***9847**8****8**5***4*",
                            "39***58*4*6*9***7*1***4**3*7*5*3*1*2*8***24****3*58**7**8*7**1*5**2*6**8*2***1*6*",
                            "**6*****1*7**6**5*8**1*32****5*4*8***4*7*2*9***8*1*7****12*5**3*6**7**8*2*****4**"
                        };

    char linie_2[10][82] = {"734561928692378415815429763946832157128745396573196284351984672469257831287613549",
                            "538219764162734598479685132913568427687342915254197683345926871726851349891473256",
                            "453826197892571634167493528714952863586137249329684751935218476671345982248769315",
                            "175294836623187945894563271519732468347856129286941753938425617461379582752618394",
                            "235986741687421953914357286472835619368219574591764832146572398859643127723198465",
                            "876543192543219768219876435198765324432198657765432981321987546654321879987654213",
                            "594321867123678549876594213985143672367259481241786395619435728758962134432817956",
                            "947861253218453679563792814194276385782345961635918427356124798471689532829537146",
                            "397165824864923571152847936745639182986712453213458697638574219571296348429381765",
                            "536827941172964358894153267715349826643782195928516734481295673369471582257638419"
                        };

    int i, j, k;
    for(i = 0; i < 10; i++){
        for(j = 0; j < 9; j++){
            for(k = 0; k < 9; k++){
                dict[i].tab[j][k] = linie_1[i][j*9 + k];
                dict[i+10].tab[j][k] = linie_2[i][j*9 + k];
            }
        }
    }

    /*raspunsul este un numar
    random intre 0 si 9*/
    srand(time(NULL));
    int random = rand() % 10;
    
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++){
            initial[i][j] = dict[random].tab[i][j];
            raspuns[i][j] = dict[random+10].tab[i][j];
        }
    }
}

/*strege ecranul, pentru un viitor joc*/
void Stergere_tabel(WINDOW *tabel[]){
    int i;

    /*sterge toate patratelele si
    sterge toate ferestrele*/
    for (i = 0; i <= 31; i++) {
        wborder(tabel[i], ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        wrefresh(tabel[i]);
        werase(tabel[i]);
        wrefresh(tabel[i]);

        delwin(tabel[i]);
    }
}

void Verif_raspuns(char **curent, char **raspuns, char **codif) {
    int i, j, x;
    for ( i = 0; i < 9; i++) {
        for ( j = 0; j < 9; j++) {
            x = curent[i][j] ^ raspuns[i][j];
            if(x == 0)
                codif[i][j] = '1';
            else
                codif[i][j] = '0';
        }
    }
}

int Castig(char **codif) {
    int ok = 1, i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (codif[i][j] == '0')
                ok = 0;
        }
    }
    return ok;
}

int main(){
    /*optiunea utilizatorului de
    a juca, initaial 1, adica
    doreste sa joace, urmand sa
    devina 2, daca nu se mai joaca,
    sau 3 daca se doreste verificarea intermediara
    si continuarea jocului cu ajutorul meniului*/

    /*optiune = 1, continua jocul
    optiune = 2, termina jocul
    optiune = 3, verifica solutie partiala
    optiune = 4, continua de unde ai ramas*/
    int optiune = 1;
    int castig = 0;

    char **initial;
    char **raspuns;
    char **curent;
    char **codif;
    initial = calloc(9, sizeof(char*));
    raspuns = calloc(9, sizeof(char*));
    curent = calloc(9, sizeof(char*));
    codif = calloc(9, sizeof(char*));

    for (int i = 0; i < 9; i++) {
        initial[i] = calloc(9, sizeof(char));
        raspuns[i] = calloc(9, sizeof(char));
        curent[i] = calloc(9, sizeof(char));
        codif[i] = calloc(9, sizeof(char));
    }

    while((optiune == 1) && castig == 0){

        WINDOW *tabel[102];

        /* initializeaza ecranul*/
        initscr();
        noecho();
        cbreak();

        start_color();
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
        wbkgd(stdscr, COLOR_PAIR(3));
        wrefresh(stdscr);

        /*deseneaza tabelul*/
        Desen_tabel(tabel);

        /*alege un tabel random
        din dictionar*/
        int i, j;
        Random(initial, raspuns);

        for(i = 0; i < 9; i++){
            for(j = 0; j < 9; j++){
                curent[i][j] = initial[i][j];
            }
        }
        Write(tabel, initial);

        start_color();
        init_pair(1, COLOR_BLACK, COLOR_GREEN);
        init_pair(2, COLOR_BLACK, COLOR_YELLOW);
        init_pair(3, COLOR_WHITE, COLOR_BLACK);
        init_pair(4, COLOR_BLACK, COLOR_RED);
        init_pair(5, COLOR_BLACK, COLOR_BLUE);

        /*color board*/
        for(i = 0; i < 9; i++){
            for(j = 0; j < 9; j++){
                if (initial[i][j] != '*') {
                    int pozitie = POZ(i, j);
                    wbkgd(tabel[pozitie], COLOR_PAIR(2));
                    wrefresh(tabel[pozitie]);
                }
            }
        }

        for (i = 1; i <= 10; i++) {
            wbkgd(tabel[i], COLOR_PAIR(5));
            wrefresh(tabel[i]);
        }

        for (i = 1; i < 100; i = i + 10) {
            wbkgd(tabel[i], COLOR_PAIR(5));
            wrefresh(tabel[i]);
        }

        /*pozitie initiala pe tabla*/
        int lin = 0, col = 0;

play:
        /*citeste pana la noi instructiuni*/
        Citire(tabel, &lin, &col, curent, initial, &optiune);

        if (optiune == 4) {
            goto play;
        }

        Verif_raspuns(curent, raspuns, codif);
        int castig = Castig(codif);

        if(castig == 1){
            werase(tabel[101]);
            wprintw(tabel[101], "Ai castigat!");
            wrefresh(tabel[101]);
            wprintw(tabel[101],
            " Mai joci?\nApasa 1 pt da sau 2 pt nu");
            wrefresh(tabel[101]);
            wscanw(tabel[0], "%d", &optiune);
            goto end;
        }

        if(castig == 0){
            if (optiune != 3) {

                /*afisare raspuns*/
                werase(tabel[101]);
                wprintw(tabel[101], "Ai pierdut...\n");
                wrefresh(tabel[101]);
                //werase(tabel[0]);
                mvwprintw(tabel[j], LIN / 2, (10 * COL - strlen("Sudoku")) / 2, "Sudoku");
                wprintw(tabel[0], "RASPUNSUL ERA:");
                wrefresh(tabel[0]);
                Write(tabel, raspuns);

                /*color board*/
                for(i = 0; i < 9; i++){
                    for(j = 0; j < 9; j++){
                        int pozitie = POZ(i, j);
                        wbkgd(tabel[pozitie], COLOR_PAIR(1));
                        wrefresh(tabel[pozitie]);
                    }
                }

                for (i = 1; i <= 10; i++) {
                    wbkgd(tabel[i], COLOR_PAIR(3));
                    wrefresh(tabel[i]);
                }

                for (i = 1; i < 100; i = i + 10) {
                    wbkgd(tabel[i], COLOR_PAIR(3));
                    wrefresh(tabel[i]);
                }

                /*meniu*/
                wprintw(tabel[101],
                "Mai joci?\nApasa 1 pt da sau 2 pt nu");
                wrefresh(tabel[101]);
                wscanw(tabel[0], "%d", &optiune);
            } else if (optiune == 3) {

                for(i = 0; i < 9; i++){
                    for(j = 0; j < 9; j++){
                        if (codif[i][j] != '1') {
                            int pozitie = POZ(i, j);
                            wbkgd(tabel[pozitie], COLOR_PAIR(4));
                            wrefresh(tabel[pozitie]);
                        } else {
                            int pozitie = (i+1)*10 + j + 2;
                            wbkgd(tabel[pozitie], COLOR_PAIR(1));
                            wrefresh(tabel[pozitie]);
                        }
                    }
                }

                /*color board*/
                for(i = 0; i < 9; i++){
                    for(j = 0; j < 9; j++){
                        if (initial[i][j] != '*') {
                            int pozitie = POZ(i, j);
                            wbkgd(tabel[pozitie], COLOR_PAIR(2));
                            wrefresh(tabel[pozitie]);
                        }
                    }
                }

                for (i = 1; i <= 10; i++) {
                    wbkgd(tabel[i], COLOR_PAIR(5));
                    wrefresh(tabel[i]);
                }

                for (i = 1; i < 100; i = i + 10) {
                    wbkgd(tabel[i], COLOR_PAIR(5));
                    wrefresh(tabel[i]);
                }

                werase(tabel[101]);
                wprintw(tabel[101], "Cu verde e corect!");
                wrefresh(tabel[101]);
                wprintw(tabel[101],
                " Mai joci?\nApasa 1 pt da sau 2 pt nu");
                wrefresh(tabel[101]);
                wscanw(tabel[0], "%d", &optiune);
                if (optiune == 1) {

                    for(i = 0; i < 9; i++){
                        for(j = 0; j < 9; j++){
                            if (initial[i][j] != '*') {
                                int pozitie = POZ(i, j);
                                wbkgd(tabel[pozitie], COLOR_PAIR(2));
                                wrefresh(tabel[pozitie]);
                            } else {
                                int pozitie = POZ(i, j);
                                wbkgd(tabel[pozitie], COLOR_PAIR(3));
                                wrefresh(tabel[pozitie]);
                            }
                        }
                    }

                    for(i = 0; i < 9; i++){
                        for(j = 0; j < 9; j++){
                            if (codif[i][j] != '1') {
                                int pozitie = POZ(i, j);
                                mvwprintw(tabel[pozitie], LIN/2, COL/2, " ");
                                wrefresh(tabel[pozitie]);
                            }
                        }
                    }
                    werase(tabel[101]);
                    wprintw(tabel[101], "Joaca in continuare!");
                    wrefresh(tabel[101]);

                    goto play;

                } else {
                    goto end;
                }

            }
            
        }

end:
        Stergere_tabel(tabel);
        endwin();
        if(optiune == 3)
            optiune = 1;

    }

    for (int i = 0; i < 9; i++)
    {
        free(initial[i]);
        free(raspuns[i]);
        free(curent[i]);
        free(codif[i]);
    }
    free(initial);
    free(raspuns);
    free(curent);
    free(codif);

    return 0;
}
