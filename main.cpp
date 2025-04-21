#include <iostream>
#include <limits>
#include <string>
#include <thread>
#include <chrono>
#include <cmath>
#define fast_io ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
using namespace std;

namespace func_utility{
    void czysc_bufor(){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    void wyswietl_menu(){
        cout << "\033[1;33m" << "  ____  ____     ____        _ _ _ " << endl << " |___ \\|  _ \\   | __ )  __ _| | | |" << endl << "   __) | | | |  |  _ \\ / _` | | | |" << endl << "  / __/| |_| |  | |_) | (_| | | | |" << endl << " |____||____/   |____/ \\__,_|_|_|_|" << "\033[0m" << endl;
    }
    void wprowadz_dane(double* a, double* vp, double* dt, int& plansza, int& iteracje){
        string input;
        cout << "\033[1;33m»\033[0m Przyszpieszenie a (domyslnie 9.81 m/s2): ";
        getline(cin, input);
        if (input.empty()){
            *a = 9.81;
            cout << "\033[A\033[999C\r\033[1;33m»\033[0m Przyszpieszenie a (domyslnie 9.81 m/s2): 9.81m/s2\n";
        }
        else *a = stod(input);
        
        cout << "\033[1;33m»\033[0m Predkosc vp (domyslnie 5 m/s): ";
        getline(cin, input);
        if (input.empty()){
            *vp = 5;
            cout << "\033[A\033[999C\r\033[1;33m»\033[0m Predkosc vp (domyslnie 5 m/s): 5m/s\n";
        }
        else *vp = stod(input);
        cout << "\033[1;33m»\033[0m Odstepy czasowe dt (domyslnie 0.5) ";
        getline(cin, input);
        if (input.empty()){
            *dt = 0.5;
            cout << "\033[A\033[999C\r\033[1;33m»\033[0m Odstepy czasowe dt (domyslnie 0.5): 0.5\n";
        }
        else *dt = stod(input);
        cout << "\033[1;33m»\033[0m Wielkosc planszy x (domyslnie 10): ";
        getline(cin, input);
        if (input.empty()){
            plansza = 10;
            cout << "\033[A\033[999C\r\033[1;33m»\033[0m Wielkosc planszy x (domyslnie 10): 10\n";
        }
        else plansza = stod(input);

        cout << "\033[1;33m»\033[0m Liczba iteracji (domyslnie 100): ";
        getline(cin, input);
        if (input.empty()){
            iteracje = 100;
            cout << "\033[A\033[999C\r\033[1;33m»\033[0m Liczba iteracji (domyslnie 100): 100\n";
        }
        else iteracje = stod(input);
    }


    void wyswietl(double *a, double* vp, double* dt, int plansza, int iteracje){
        cout << "\033[2J\033[H";
        
        cout << "\033[1;33m   .-\"\"\"-.\033[0m" << " a = " << *a << "m/s2" << endl;
        cout << "\033[1;33m  /       \\\033[0m" << " vp = " << *vp << "m/s" << endl;
        cout << "\033[1;33m |         |\033[0m" << " dt = " << *dt << "s" << endl;
        cout << "\033[1;33m  \\       /\033[0m" << " plansza = " << plansza << "x" << plansza << endl;
        cout << "\033[1;33m   '-._.-'\033[0m" << " iteracje = " << iteracje << endl;
        cout << "\033[1;33mKliknij ENTER aby zaczac symulacje... ";
        cin.get();
    }
}

namespace silnik{
    void alokuj_tablice(int** tab, int plansza, int pos_x, int pos_y){
        for(int i = 0; i < plansza; ++i){
            tab[i] = new int[plansza];
            for(int j = 0; j < plansza; ++j){
                tab[i][j] = 0;
            }
        }
        tab[pos_x][pos_y] = 1;
    }
    void wyswietl_tablice(int** tab, int plansza, double*v, double*a, double*dt, int iteracja, int iteracje){
        cout << "\033[1;33mIteracja " << iteracja << "/" << iteracje << ":\033[0m" << endl;
        for(int i = 0; i < plansza; ++i){
            cout << i << (i<10 ? "m  |" : "m |");
            for(int j = 0; j < plansza; ++j){
                int data = tab[i][j];
                switch(data){
                    case 0:
                        cout << "\033[37m-\033[0m";
                        break;
                    case 1:
                        cout << "\033[1;33mO\033[0m";
                        
                        break;
                    default:
                        cout << "E";
                        break;
                }
                cout << " ";
            }
            cout << "|\n";
        }
        
        cout << "v = " << *v << "m/s  a = " << *a << "   dt = " << *dt << "s" << endl << "\033[1;33mCTRL+C aby przerwac symulacje\033[0m";
        cout << "\n";
    }

    void aktualizuj_pozycje(int** tab, int plansza, int &pos_x, int &pos_y, double a, double* v, double* dt, double max_v, bool& kierunek, double vp, double dt_p){
        if (pos_x+1 >= plansza){
            kierunek = false;
            *v = vp;
            *dt = dt_p;
        }
        else if (pos_x-1 < 0){
            kierunek = true;
            *v = vp;
            *dt = dt_p;
        }
        if (kierunek == true){
            tab[pos_x+1][pos_y] = 1;
            tab[pos_x][pos_y] = 0;
            pos_x+=1;
        }
        else{
            tab[pos_x-1][pos_y] = 1;
            tab[pos_x][pos_y] = 0;
            pos_x-=1;
        }
        *v+=a;
        *dt = (max_v/1000) - ((*v)/1000);  
    }
}
int main(){
    fast_io;
    //definicja zmiennych
    double* a = new double; double* v = new double; double* dt = new double;
    int plansza = 0, iteracje = 0;
    //wczytanie zmiennych i wyswietlenie wczytanych oraz ustawienie poczatkowych zmiennych na symulacje kolizji
    
    func_utility::wyswietl_menu();
    func_utility::wprowadz_dane(a, v, dt, plansza, iteracje);
    func_utility::wyswietl(a, v, dt, plansza, iteracje);
    double v_poczatkowe {*v};
    double dt_poczatkowe {*dt};

    //obliczenie max. predkosci do prawidlowej symulacji przyspieszenia
    double max_v = (plansza-1) * (*a) + (*v);
    cout << "MAX_v = " << max_v << endl;
    //zinicjalizowanie potrzebnych zmiennych do obliczania pozycji x i y
    int pos_x = 0, pos_y = static_cast<int>(round(plansza/2)) - 1;
    bool kierunek = true;
    //alokowanie tablicy do symulacji oraz wskaznika do tablicy wskaznikow (TAB 2D)
    int** macierz = new int*[plansza];
    silnik::alokuj_tablice(macierz, plansza, pos_x, pos_y);
    //petla glowna symulacji
    for(int i = 1; i <= iteracje; ++i){
        silnik::wyswietl_tablice(macierz,plansza, v, a, dt, i, iteracje);
        silnik::aktualizuj_pozycje(macierz, plansza, pos_x, pos_y, *a, v, dt, max_v, kierunek, v_poczatkowe, dt_poczatkowe);
        this_thread::sleep_for(chrono::duration<double>(*dt));
        cout << "\033[2J\033[H";
    }

    //komunikat konczacy program
    cout << "\033[1;32mPomyslnie zakonczono symulacje!\033[0m" << endl;
    cout << "\033[1;33mKliknij ENTER aby zakonczyc program...\033[0m" << endl;
    //usuwanie dynamicznie zaalokwanych zmiennych
    delete a;
    delete v;
    delete dt;
    for(int i = 0; i < plansza; ++i){
        delete[] macierz[i];
    }
    delete[] macierz;
    return 0;
}
