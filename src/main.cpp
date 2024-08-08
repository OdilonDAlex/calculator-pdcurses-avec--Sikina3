#include <curses.h>
#include <iostream>
#include <string>
#include <main-menu.hh>
#include <view-matrix.hh>
#include <tools.hh>
#include <panel.h>
#include <main.hh>
#include <matrix.hh>
#include <test.hh>
#include <test-item.hh>


using namespace std;
int supportColor;

PANEL *body;
WINDOW* headerWin;
WINDOW* bodyWin;
WINDOW* footerWin;

int main(){

    initscr();

    supportColor = has_colors() ? 1 : 0;

    if(supportColor == 1) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_RED, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_RED, COLOR_BLACK);
    }

    keypad(stdscr, TRUE);
    string helperValues[] = {
        "Addition matricielle",
        "Soustraction matricielle",
        "Multiplication matricielle",
        "Comparaison matricielle",
        "Quitter le programme :'("
    };
    vector<string> helpers(helperValues, helperValues + 5);


    /**  Debut de creation du layout  */

    /**
     * @var int footerHeight en nombre de lignes
    */
    int footerHeight = 3, headerHeight = 3;
    int bodyWidth = COLS, bodyHeight = LINES - footerHeight;

    bodyWin = newwin(bodyHeight, bodyWidth, 0, 0);
    headerWin = subwin(bodyWin, headerHeight, COLS - 10, 1, 5);
    footerWin  = newwin(footerHeight, COLS, bodyHeight, 0);

    body = new_panel(bodyWin);
    PANEL *footer = new_panel(footerWin);

    box(headerWin, 0, 0);
    box(panel_window(footer), 0, 0);

    string choiceValues[] = {"1. Addition", "2. Soustraction", "3. Multiplication", "4. Comparaison", "5. Quitter"};
    vector<string> choices(choiceValues, choiceValues + 5);

    MainMenu menu = MainMenu(choices, 0, bodyWidth, bodyHeight);

    displayHelp(footer, footerHeight);


    modifyHeader(headerWin, "Calcul Matricielle avec Cpp et Pdcurses");
    menu.display(body, 0);

    doupdate();
    int quit = 0;
    while(!quit){

        noecho();
        int key = getch();
        menu.getActive();
        switch (key)
        {
            case 456:
            case KEY_DOWN:
                menu.moveDown(body);
                modifyHeader(headerWin, helpers[menu.getActive()]);
                break;

            case 450:
            case KEY_UP:
                menu.moveUp(body);
                modifyHeader(headerWin, helpers[menu.getActive()]);
                wrefresh(panel_window(body));
                break;

            case 'Q':
            case 'q':
                erase();
                endwin();
                std::cout << "Veloma Tompoko!" << std::endl;
                return 0;
                break;

            case KEY_ENTER:
            case 10:
                if(menu.getActive() == menu.getSize()-1){ endwin();std::cout<<"Veloma Tompoko!" << std::endl; return 0;}
                wclear(bodyWin);
                wrefresh(bodyWin);
                modifyHeader(headerWin, "Entrer la taille des matrices");
                echo();
                compute(menu.getActive(), panel_window(body));
                modifyHeader(headerWin, "Faire une autre operation ?");
                menu.display(body, 0);
                doupdate();
                break;

            case 'T':
            case 't':
                {
                    wclear(bodyWin);
                    vector<TestItem> tests = runTest();
                    int subWinWidth = string("Test multiplication dimension incompatible: Reussi").size() + 2;
                    int subWinHeight = tests.size();
                    modifyHeader(headerWin, "Resultat des tests unitaires.");
                    WINDOW* testWin = derwin(
                            bodyWin,
                            subWinHeight,
                            subWinWidth,
                            (LINES - subWinHeight) / 2,
                            (COLS - subWinWidth) / 2
                        );
                    for(unsigned int i = 0; i < subWinHeight; i++){
                        mvwprintw(testWin, i, 1, "%s", stringToChar(tests[i].test));
                        wattron(testWin, COLOR_PAIR(3));
                        mvwprintw(testWin, i, 1 + tests[i].test.size(), "%s", stringToChar(tests[i].result));
                        wattroff(testWin, COLOR_PAIR(3));
                    }
                    wrefresh(testWin);
                    string continue_ = "Appuyer sur n'importe quel touche pour continuer...";
                    wattron(bodyWin, COLOR_PAIR(4));
                    mvwprintw(bodyWin, LINES - 4, (COLS / 2) - (continue_.size() / 2), "%s", stringToChar(continue_));
                    wrefresh(bodyWin);
                    doupdate();
                    wattroff(bodyWin, COLOR_PAIR(4));
                    getch();
                    wclear(bodyWin);
                    menu.display(body, menu.getActive());
                    modifyHeader(headerWin, helpers[menu.getActive()]);
                    wrefresh(panel_window(body));

                }
                break;
            default:
                break;
        }
    }

    endwin();

    return 0;
}

void displayHelp(PANEL* pan, int height){
    WINDOW *win = panel_window(pan);

    /**
     * @var int posY position du help
     *
     * Hauteur du footer: h
     * -> (int)(h / 2) => posY: milieu du hauteur
    */

    int posY = (int)(height / 2);

    /**
     * @var int startX ( en nombre de colonne )
     * une colonne = une caractere
    */
    int startX = 3;

    /**
     * Affichage pour l'aide de navigation
    */
    mvwaddstr(win, posY, startX, "Navigation:");
    wattrset(win, A_REVERSE);
    mvwaddch(win, posY, startX + string("Navigation: ").size(), ACS_UARROW);
    mvwaddch(win, posY, startX + string("Navigation: ").size()+ 4, ACS_DARROW);
    wattrset(win, A_NORMAL);

    /**
     * Aide quitter
    */
    mvwaddstr(win, posY, COLS - startX - string("Quitter: Q  ").size(), "Quitter:");
    wattrset(win, A_REVERSE);
    mvwaddstr(win, posY, COLS - startX - 2, " Q ") ;
    wattrset(win, A_NORMAL);

    /**
     * Aide pour la confirmation
    */
    mvwaddstr(win, posY, (COLS / 2) - (string("Valider: ENTRER").size() / 2), "Valider: ");
    wattrset(win, A_REVERSE);
    mvwaddstr(win, posY, (COLS / 2) - (string("Valider: ENTRER ").size() / 2) + string("Valider:  ").size(), " ENTRER ");
    wattrset(win, A_NORMAL);

    show_panel(pan);
    update_panels();
}

void modifyHeader(WINDOW* header, string value) {
    wclear(header);
    mvwaddstr(
        header,
        1,
        ((COLS - 10) / 2) -  (value.length() / 2),
        stringToChar(value)
        );
    box(header, 0, 0);
    wrefresh(header);
    doupdate();
}

Matrix readMatrix(WINDOW *parentWindow){
    int linesNumber, colsNumber;

    WINDOW* win = subwin(parentWindow, 3, (COLS - 10), 5, 5);

    box(win, 0, 0);

    mvwprintw(win, 1, 1, "Nombre de ligne: ");

    mvwscanw(win, 1, 1 + string("Nombre de ligne: ").size(), "%d", &linesNumber);

    while(linesNumber <= 0){
        wclear(win);

        box(win, 0, 0);

        wattron(win, COLOR_PAIR(5));
        mvwprintw(win, 1, 1, "Nombre de ligne(minimum 1): ");

        wattroff(win, COLOR_PAIR(5));
        mvwscanw(win, 1, 1 + string("Nombre de ligne(minimum 1): ").size(), "%d", &linesNumber);
        wrefresh(win);
    }

    mvwprintw(win, 1, 1 + ((COLS - 10) / 2), "Nombre de colonne: ");
    mvwscanw(win, 1, ((COLS - 10) / 2) + string("Nombre de colonne: ").size(), "%d", &colsNumber);

    while(colsNumber <= 0){
        wclear(win);

        box(win, 0, 0);

        mvwprintw(win, 1, 1, "Nombre de ligne: %d", linesNumber);

        wattron(win, COLOR_PAIR(5));
        mvwprintw(win, 1, 1 + ((COLS - 10) / 2), "Nombre de colonne(minimum 1): ");
        mvwscanw(win, 1, ((COLS - 10) / 2) + string("Nombre de Colonne(minimum 1): ").size(), "%d", &colsNumber);

        wattroff(win, COLOR_PAIR(5));
    }

    /**
     * Remplissage et creation des deux matrice matrices
    */

    ViewMatrix A = ViewMatrix(linesNumber, colsNumber);
    A.display(parentWindow, COLS, LINES, "Entrer les valeurs de la matrice");


    wclear(parentWindow);

    return A.getMatrix();
}

// void makeAddition(WINDOW *parentWindow){
//     modifyHeader(header, "Entrer la premiere matrice");


// }

int compute(int index, WINDOW* parentWindow){

    unsigned int matNumber;

    WINDOW* promptWin = derwin(parentWindow, 3, COLS - 10, 6, 5);
    WINDOW* werror = derwin(parentWindow, 3, COLS - 10, LINES - 10, 5);

    modifyHeader(headerWin, "Nombre de matrice");
    if(index != 3){
        mvwprintw(promptWin, 1, 1, "Entrer le nombre de matrice(minimum 2): ");

        wrefresh(parentWindow);
        wrefresh(promptWin);


        mvwscanw(promptWin, 1, string("Entrer le nombre de matrice(minimum 2): ").size(), "%d", &matNumber);

        while(matNumber < 2){
            wclear(promptWin);
            wattron(promptWin, COLOR_PAIR(5));
            mvwprintw(promptWin, 1, 1, "Entrer une valeur superieur ou egale a 2: ");
            wattroff(promptWin, COLOR_PAIR(5));
            wrefresh(parentWindow);
            wrefresh(promptWin);
            mvwscanw(promptWin, 1, string("Entrer une valeur superieur ou egale a 2: ").size(), "%d", &matNumber);
        }

    }
    else{
        matNumber = 2;
    }

    vector<Matrix> matrix;


    for(unsigned int i=0; i < matNumber; i++){

        wclear(headerWin);

        mvwprintw(
            headerWin,
            1,
            ((COLS - 10) / 2) - (string("Entrer les dimensions de la matrice _-ieme matrice").size() / 2),
            ( i == 0 ? "Entrer les dimensions de la premiere matrice" : "Entrer les dimensions de la %d-ieme matrice")
            ,
            i + 1
        );

        box(headerWin, 0, 0);
        wrefresh(headerWin);

        matrix.push_back(readMatrix(parentWindow));

        wclear(parentWindow);
        wrefresh(parentWindow);
    }

    switch(index){

        case 0: {
                try {
                    Matrix result = matrix[0] + matrix[1];

                    for(unsigned int i=2; i < matrix.size(); i++) { result = result + matrix[i]; }

                    wclear(parentWindow);

                    matrix.push_back(result);

                    displayResult(parentWindow, matrix, '+');
                    wrefresh(parentWindow);
                }
                catch(const string &error_){
                    printError(werror, error_);
                }
        }
        break;
        case 1: {
                try {
                    Matrix result = matrix[0] - matrix[1];

                    for(unsigned int i=2; i < matrix.size(); i++) { result = result - matrix[i]; }

                    wclear(parentWindow);

                    matrix.push_back(result);

                    displayResult(parentWindow, matrix, '-');
                    wrefresh(parentWindow);

                }
                catch(const string &error_){
                    printError(werror, error_);
                }
        }
        break;
        case 2: {
                try {
                    Matrix result = matrix[0] * matrix[1];

                    for(unsigned int i=2; i < matrix.size(); i++) { result = result * matrix[i]; }

                    wclear(parentWindow);

                    matrix.push_back(result);

                    displayResult(parentWindow, matrix, '*');
                    wrefresh(parentWindow);
                }
                catch(const string &error_){
                    printError(werror, error_);
                }
        }
        break;

        case 3: {
            string subMenuValues[] = {
                "1. premiere matrice > deuxieme matrice",
                "2. premiere matrice < deuxieme matrice",
                "3. premiere matrice >= deuxieme matrice",
                "4. premiere matrice <= deuxieme matrice",
                "5. premiere matrice = deuxieme matrice",
                "6. premiere matrice != deuxieme matrice"
            };

            vector<string> subMenu(subMenuValues, subMenuValues + 6);

            MainMenu subMainMenu = MainMenu(subMenu, 0, COLS, LINES);

            subMainMenu.display(body, 0);

            wrefresh(panel_window(body));

            noecho();
            int exit_ = 0;
            int userChoice;
            while(! (exit_ == 1)){
                modifyHeader(headerWin, "Quel comparaison souhaiteriez-vous faire ?");
                userChoice = getch();
                switch (userChoice)
                {
                    case 456:
                    case KEY_DOWN:
                        subMainMenu.moveDown(body);
                        wrefresh(panel_window(body));
                        break;

                    case 450:
                    case KEY_UP:
                        subMainMenu.moveUp(body);
                        wrefresh(panel_window(body));
                        break;

                    case 'q':
                    case 'Q':
                        exit_ = 1;
                        wclear(bodyWin);
                        wrefresh(bodyWin);
                        wattron(bodyWin, COLOR_PAIR(3));
                        mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("Operation de comparaison annuler").size() / 2), "Operation de comparaison annuler");
                        wrefresh(bodyWin);
                        wattroff(bodyWin, COLOR_PAIR(3));
                        break;

                    case KEY_ENTER:
                    case 10:
                        wclear(bodyWin);
                        wrefresh(bodyWin);
                        modifyHeader(headerWin, "Resultat du comparaison");
                        {
                            int choosenComp = subMainMenu.getActive();
                            switch(choosenComp){
                                case 0:
                                    {
                                        try {
                                            if(matrix[0] > matrix[1]){
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("OUI, la premiere matrice est strictement superieur au deuxieme").size() / 2), "OUI, la premiere matrice est strictement superieur au deuxieme");
                                            }
                                            else {
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("NON, la premiere matrice n'est pas strictement superieur au deuxieme").size() / 2), "NON, la premiere matrice n'est pas strictement superieur au deuxieme");
                                            }

                                            wrefresh(bodyWin);
                                            exit_ = 1;
                                        }
                                        catch(const string &error_){
                                            printError(werror, error_);
                                        }
                                    }
                                break;
                                case 1:
                                    {
                                        try {
                                            if(matrix[0] < matrix[1]){
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("OUI, la premiere matrice est strictement inferieur au deuxieme").size() / 2), "OUI, la premiere matrice est strictement inferieur au deuxieme");
                                            }
                                            else {
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("NON, la premiere matrice n'est pas strictement inferieur au deuxieme").size() / 2), "NON, la premiere matrice n'est pas strictement inferieur au deuxieme");
                                            }

                                            wrefresh(bodyWin);
                                            exit_ = 1;
                                        }
                                        catch(const string &error_){
                                            printError(werror, error_);
                                        }
                                    }
                                break;
                                case 2:
                                    {
                                        try {
                                            if(matrix[0] >= matrix[1]){
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("OUI, la premiere matrice est superieur au deuxieme").size() / 2), "OUI, la premiere matrice est superieur au deuxieme");
                                            }
                                            else {
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("NON, la premiere matrice n'est pas superieur au deuxieme").size() / 2), "NON, la premiere matrice n'est pas superieur au deuxieme");
                                            }

                                            wrefresh(bodyWin);
                                            exit_ = 1;
                                        }
                                        catch(const string &error_){
                                            printError(werror, error_);
                                        }
                                    }
                                break;
                                case 3:
                                    {
                                        try {
                                            if(matrix[0] <= matrix[1]){
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("OUI, la premiere matrice est inferieur au deuxieme").size() / 2), "OUI, la premiere matrice est inferieur au deuxieme");
                                            }
                                            else {
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("NON, la premiere matrice n'est pas inferieur au deuxieme").size() / 2), "NON, la premiere matrice n'est pas inferieur au deuxieme");
                                            }

                                            wrefresh(bodyWin);
                                            exit_ = 1;
                                        }
                                        catch(const string &error_){
                                            printError(werror, error_);
                                        }
                                    }
                                break;
                                case 4:
                                    {
                                        try {
                                            if(matrix[0] == matrix[1]){
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("OUI, les deux matrices sont egaux").size() / 2), "OUI, les deux matrices sont egaux");
                                            }
                                            else {
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("NON, les deux matrices sont differentes").size() / 2), "NON, les deux matrices sont differentes");
                                            }

                                            wrefresh(bodyWin);
                                            exit_ = 1;
                                        }
                                        catch(const string &error_){
                                            printError(werror, error_);
                                        }
                                    }
                                break;
                                case 5:
                                    {
                                        try {
                                            if(matrix[0] != matrix[1]){
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("OUI, les deux matrices sont differentes").size() / 2), "OUI, les deux matrices sont differentes");
                                            }
                                            else {
                                                mvwprintw(bodyWin, LINES / 2, (COLS / 2) - (string("NON, les deux matrices sont egaux").size() / 2), "NON, les deux matrices sont egaux");
                                            }

                                            wrefresh(bodyWin);
                                            exit_ = 1;
                                        }
                                        catch(const string &error_){
                                            printError(werror, error_);
                                        }
                                    }
                                break;
                            }
                        }

                }
            }
        }

    }

    string continue_ = "Appuyer sur n'importe quel touche pour continuer...";
    wattron(bodyWin, COLOR_PAIR(4));
    mvwprintw(bodyWin, LINES - 4, (COLS / 2) - (continue_.size() / 2), "%s", stringToChar(continue_));
    wrefresh(bodyWin);
    doupdate();
    wattroff(bodyWin, COLOR_PAIR(4));
    getch();
    wclear(parentWindow);
    wrefresh(parentWindow);
    return 0;
}

void displayResult(WINDOW* parentWindow, vector<Matrix> matrix, char operator_){

    vector<ViewMatrix> vMatrix;
    int totalWidth = 0;
    int rowSpacing = 2, colSpacing = 5;
    int maxHeight = -1, maxWidth=-1;
    int size_ = matrix.size();
    int width = 0, height = 0;
    for(unsigned int i = 0; i < size_; i++ ){
        vMatrix.push_back(ViewMatrix(matrix[i]));
        width = vMatrix[i].getTotalWidth(colSpacing);
        height = vMatrix[i].getTotalHeight(rowSpacing);
        totalWidth += (width + 2*colSpacing);
        if((height + 2*rowSpacing) > maxHeight){ maxHeight = height + 2*rowSpacing; }
        if((width + 2*colSpacing) > maxWidth){ maxWidth = width + 2*colSpacing; }
    }

    totalWidth += 3*(size_);
    int borderLeft = (COLS - totalWidth) / 2;
    int borderTop = (LINES - maxHeight ) / 2;

    modifyHeader(headerWin, "Resultat du calcul.");

    /**
     * Si la taille totale necessaire pour l'affichage complet est plus grand que la taille de la fenetre
     * Vaut mieux afficher que le resultat du calcul
     */
    if(totalWidth > COLS || maxHeight > LINES || ( matrix[size_ - 1].colonne() == 1 || matrix[size_ - 1].ligne() == 1)){
        vMatrix[size_ - 1].display(parentWindow, COLS, LINES, "Resultat du calcul.");
        wrefresh(parentWindow);
    }

    else {

        for(unsigned int i = 0; i < size_; i++){
            WINDOW* win = derwin(
                parentWindow,
                maxHeight,
                maxWidth,
                borderTop,
                borderLeft + 3*i + maxWidth*i
            );
            vMatrix[i].display(win, maxWidth, maxHeight, "");
            wrefresh(win);
        }

        for(unsigned int i = 1; i < size_ - 1; i++) {
           mvwaddch(parentWindow, borderTop + maxHeight / 2, borderLeft + i*maxWidth + 1, operator_);
           wrefresh(parentWindow);
        }

        mvwaddch(parentWindow, borderTop + maxHeight / 2, borderLeft + maxWidth*(size_-1) + 3*(size_-2) + 1, '=');
        wrefresh(parentWindow);
    }
    wrefresh(parentWindow);
}

void printError(WINDOW* werror, const string &error_){
    wattron(werror, COLOR_PAIR(2));
    attrset(A_REVERSE);
    mvwprintw(werror, 1, (COLS - 10 )/ 2 - ( error_.size() / 2 ), "%s", stringToChar(error_));
    box(werror, 0, 0);
    wrefresh(werror);
    wattroff(werror, COLOR_PAIR(2));
}
