
#include <iostream>
#include "solver.h"
#include <sstream>
#include <windows.h> 
#include <stdlib.h>

using namespace std;


void clear_screen(char fill = ' ') {
    COORD tl = { 0,0 };
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
}

void playGame(const string &mapname){
    solver solver;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    filereader read;

    auto final_states = solver.solve(mapname);
    auto MAP = read.readmap(mapname);
    int SIZE = read.getsize();
    string solution = solver.findsolution(final_states,read.getsize());
    istringstream film(solution);
    int counter = 0;
    clear_screen();
    for (auto i : MAP) {
        switch (i) {
            case WALL:
              SetConsoleTextAttribute(hConsole, 31);
              cout << i << " ";
              break;
            case PLR:
              SetConsoleTextAttribute(hConsole, 30);
              cout << i << " ";
              break;
            case BOX: 
              SetConsoleTextAttribute(hConsole, 33);
              cout << i << " ";
              break;
            case BOX_ON_GOAL:
              SetConsoleTextAttribute(hConsole, 43);
              cout << i << " ";
              break;
            case PLR_ON_GOAL:
              SetConsoleTextAttribute(hConsole, 44);
              cout << i << " ";
              break;
            case GOAL:
              SetConsoleTextAttribute(hConsole, 91);
              cout << i << " ";
              break;
            case ' ':
              SetConsoleTextAttribute(hConsole, 40);
              cout << i << " ";
              break;
        }
        if ((counter + 1) % read.getsize() == 0) {
            SetConsoleTextAttribute(hConsole, 40);
            cout << endl;
        }
        counter++;
        SetConsoleTextAttribute(hConsole, 97);
       
    }
    Sleep(1000);
    for (auto move:solution) {
        clear_screen();
        int mover = 0;
        auto MAP_ = MAP;
        int PP , newPP;
        counter = 0;
        for (auto i : MAP) {
            if (i == PLR || i == PLR_ON_GOAL) PP = counter;
            counter++;
        }
        int modifier;
        switch (move){
        case 'U':
            modifier = -SIZE;
            break;
        case 'D':
            modifier = SIZE;
            break;
        case 'L':
            modifier = -1;
            break;
        case 'R':
            modifier = 1;
            break;
        }
        newPP = PP + modifier;
        if (MAP[newPP] == ' ') {
            MAP_[newPP] = PLR;
            if (MAP_[PP] == PLR_ON_GOAL)MAP_[PP] = GOAL;
            else MAP_[PP] = ' ';
        }
        else if (MAP[newPP] == BOX) {
            mover = 0;
            while (MAP[newPP + mover * modifier] == BOX || MAP[newPP + mover * modifier] == BOX_ON_GOAL) {
                mover++;
            } 
            if (MAP[newPP + mover * modifier] == GOAL)MAP_[newPP + mover * modifier] = BOX_ON_GOAL;
            else if (MAP[newPP + mover * modifier] == ' ')MAP_[newPP + mover * modifier] = BOX;
            MAP_[newPP] = PLR;
            if (MAP_[PP] == PLR_ON_GOAL)MAP_[PP] = GOAL;
            else MAP_[PP] = ' ';
        }
        else if (MAP[newPP] == BOX_ON_GOAL) {
            mover = 0;
            while (MAP[newPP + mover * modifier] == BOX || MAP[newPP + mover * modifier] == BOX_ON_GOAL) {
                mover++;
            }
            if (MAP[newPP + mover * modifier] == GOAL)MAP_[newPP + mover * modifier] = BOX_ON_GOAL;
            else if (MAP[newPP + mover * modifier] == ' ')MAP_[newPP + mover * modifier] = BOX;
            MAP_[newPP] = PLR_ON_GOAL;
            if (MAP_[PP] == PLR_ON_GOAL)MAP_[PP] = GOAL;
            else MAP_[PP] = ' ';
        }
        for (auto i : MAP_) {
            switch (i) {
            case WALL:
                SetConsoleTextAttribute(hConsole, 31);
                cout << i << " ";
                break;
            case PLR:
                SetConsoleTextAttribute(hConsole, 30);
                cout << i << " ";
                break;
            case BOX:
                SetConsoleTextAttribute(hConsole, 33);
                cout << i << " ";
                break;
            case BOX_ON_GOAL:
                SetConsoleTextAttribute(hConsole, 43);
                cout << i << " ";
                break;
            case PLR_ON_GOAL:
                SetConsoleTextAttribute(hConsole, 44);
                cout << i << " ";
                break;
            case GOAL:
                SetConsoleTextAttribute(hConsole, 91);
                cout << i << " ";
                break;
            case ' ':
                SetConsoleTextAttribute(hConsole, 40);
                cout << i << " ";
                break;
            }
            if ((counter + 1) % read.getsize() == 0) {
                SetConsoleTextAttribute(hConsole, 40);
                cout << endl;
            }
            counter++;
            SetConsoleTextAttribute(hConsole, 97);

        }
        MAP = MAP_;
        
        Sleep(1000);
   
    }
}



int main()
{
    cout << "Enter MAPNAME : ";
    string levelname;
    cin >> levelname;
    playGame(levelname);
}

