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
    // ...

    int starty, startx, j = 0;

    starty = 0;
    startx = 0;
    tabel[j] = newwin(2 * LIN, 10 * COL, starty, startx);  // Modify the window size
    mvwprintw(tabel[j], LIN / 2, (10 * COL - strlen("Sudoku")) / 2, "Sudoku");  // Center the header
    wrefresh(tabel[j]);

    j = 1;  // Start the index for the table elements

    // Loop to create the 9 by 9 table
    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            startx = col * COL;
            starty = (row + 1) * LIN;
            tabel[j] = newwin(LIN, COL, starty, startx);
            j++;
        }
    }

    // ...

    // Loop to draw the borders of each table element and add numbering
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

    // ...
    j = 101;  // Adjust the index for the footer
    starty += LIN;  // Adjust the starting position
    startx = 0;
    tabel[j] = newwin(2 * LIN, 10 * COL, starty, startx);  // Modify the window size
    // Optionally, add a footer message
    mvwprintw(tabel[j], LIN / 2, (10 * COL - strlen("Load table... Complete")) / 2, "Load table... Complete");  // Center the footer
    wrefresh(tabel[j]);
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

void Write(WINDOW *tabel[], char initial[][9]) {
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

/*alege un cuvant random
din dictionar*/
void Random(char initial[9][9], char raspuns[9][9]){
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
    random intre 0 si 27*/
    srand(time(NULL));
    int random = rand() % 10;
    
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++){
            initial[i][j] = dict[random].tab[i][j];
            raspuns[i][j] = dict[random+10].tab[i][j];
        }
    }
}



int main(){

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

        char initial[9][9];
        char raspuns[9][9];
        Random(initial, raspuns);
        Write(tabel, initial);

        

    return 0;
}
