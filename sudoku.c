#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>


#define LIN 3 // nr de linii din patratel
#define COL 5 // nr de coloane din patratel
#define POZ(i, j) (10 * (i + 1) + j + 2)


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

        

    return 0;
}
