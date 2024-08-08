#ifndef __MAIN_MENU__
#define __MAIN_MENU__
#include <curses.h>
#include <vector>
#include <tools.hh>
#include <panel.h>

using std::string; using std::vector;

class MainMenu{
    private:
        vector<string> _choices;
        int _winHeight, _winWidth;
        unsigned int _active;

    public:
        MainMenu(vector<string>, int, int, int);
        void display(PANEL* , int);
        int menuMaxLength() const;
        int getActive() const;
        int getSize() const;
        string operator[] (int);
        void moveDown(PANEL*);
        void moveUp(PANEL*);
};

#endif
