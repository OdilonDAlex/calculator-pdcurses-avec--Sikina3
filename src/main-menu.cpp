#include <iostream>
#include <curses.h>
#include <main-menu.hh>

/**
 * Constructeur
 *
 * @param vector<string> menu
 * les differentes choix du menu
 *
 * @param int active
 * menu selectionner
 *
 *
*/
MainMenu::MainMenu(vector<string> menu, int active, int winWidth, int winHeight){
    this->_choices = menu;
    this->_active = active;
    this->_winWidth = winWidth;
    this->_winHeight = winHeight;
};

/**
 * Affiche la menu principale
 *
 * @params int active
 * menu selectionner
 *
*/
void MainMenu::display(PANEL* pan, int active) {

    this->_active = active;

    for( unsigned int i = 0; i < _choices.size() ; i++){
        if(i==this->_active){
            wattrset(panel_window(pan), A_REVERSE);
        }

        mvwaddstr(
            panel_window(pan),
            ( _winHeight / 2) - ( (int)(_choices.size() / 2) - i),
            ( _winWidth / 2 ) - ( this->menuMaxLength() / 2),
            stringToChar(_choices[i])
        );

        wattrset(panel_window(pan), A_NORMAL);
    }

    show_panel(pan);
    update_panels();
}

/**
 * @return int la menu la plus longue
*/
int MainMenu::menuMaxLength() const {

    unsigned int result = 0;
    unsigned int size_;
    for(unsigned int i = 0; i < _choices.size(); i++){
        size_ = _choices[i].length();
        if(_choices[i].length() > result){ result = size_; }
    }

    return result;
}

string MainMenu::operator[] (int index){
    return this->_choices[index % this->_choices.size()];
}

int MainMenu::getActive() const { return this->_active; }
int MainMenu::getSize() const { return this->_choices.size(); }

/**
 * Deplace le curseur ( en bas )
 *
 * @param PANEL* pan panel sur lequel la menu est affich�
*/
void MainMenu::moveDown(PANEL* pan){
    werase(panel_window(pan));
    this->display(pan, ++this->_active % this->getSize());
    wrefresh(panel_window(pan));
    update_panels();
    doupdate();
}

/**
 * Deplace le curseur ( en haut )
 *
 * @params PANEL* pan panel sur lequel la menu est affich�
*/
void MainMenu::moveUp(PANEL* pan){
     werase(panel_window(pan));
    this->display(
        pan,
        this->_active <= 0 ? this->_active + ( this->getSize() - 1 ) : --this->_active
        );
    wrefresh(panel_window(pan));
    update_panels();
    doupdate();
}
