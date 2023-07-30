#include <exception>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
using namespace std;

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

#define szerokosc 512
#define wysokosc 340


const int dlugoscKompresja = 43520;
SDL_Color paleta5N[32];
SDL_Color paleta5D[32];
int ileKolorow = 0;
SDL_Color paleta[256 * 170];
SDL_Color paleta5d[32];
SDL_Color paleta5n[32];
SDL_Color paleta5bw[32];
int ileKolorow5=0;
SDL_Color obrazek[(szerokosc/2)*(wysokosc/2)];
char* piksele= new char[szerokosc/2*wysokosc/2];
Uint8 z24RGBna5RGB(SDL_Color);
SDL_Color z5RGBna24RGB(Uint8 kolor5bit);

Uint8 z24RGBna5WB(SDL_Color);
SDL_Color z5BWna24RGB(Uint8 BW5bit); //przesyłamy BW8bit, dostajemy SDL_Color;

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y);

void czyscEkran(Uint8 R, Uint8 G, Uint8 B);

/* Część I projektu GKiM*/
Uint8 z24RGBna5RGB(SDL_Color kolor)
{
    int R, G, B;
    int nowyR, nowyG, nowyB;
    int kolor5bit;

    R = kolor.r;
    G = kolor.g;
    B = kolor.b;
    nowyR = round(R * 3.0 / 255.0);
    nowyG = round(G * 3.0 / 255.0);
    nowyB = round(B * 1.0 / 255.0);



    kolor5bit = (nowyR << 3) + (nowyG << 1) + (nowyB);


    return kolor5bit;
}

SDL_Color z5RGBna24RGB(Uint8 kolor5bit)
{

    SDL_Color kolor;
    kolor.r = (((kolor5bit & 0b00011000) >> 3) * 255.0/3.0);
    kolor.g = (((kolor5bit & 0b00000110) >> 1) * 255.0/3.0);
    kolor.b = (((kolor5bit & 0b00000001) * 255.0/1.0));

    return kolor;
}

Uint8 z24RGBna5BW(SDL_Color kolor)
{
    int R, G, B;
    int nowyR, nowyG, nowyB;
    int BW;
    int noweBW;
    R = kolor.r;
    G = kolor.g;
    B = kolor.b;

    nowyR = round(R * 3.0 / 255.0);
    nowyG = round(G * 3.0 / 255.0);
    nowyB = round(B * 1.0 / 255.0);

    R = nowyR * 255.0 / 3.0;
    G = nowyG * 255.0 / 3.0;
    B = nowyB * 255.0 / 1.0;

    BW = 0.299 * R + 0.587 * G + 0.114 * B;
    noweBW = round(BW * 31.0 / 255.0);


    return noweBW;
}

SDL_Color z5BWna24RGB(Uint8 BW5bit)
{
    Uint8 BW;
    SDL_Color kolor;
    BW = BW5bit*255.0/31.0;
    kolor.b = (BW);
    kolor.g = (BW);
    kolor.r = (BW);

    return kolor;
}

/* Część II projektu GKiM*/

int dodajKolor(SDL_Color kolor) {
    int aktualnyKolor = ileKolorow;
    paleta[aktualnyKolor] = kolor;
    //cout << aktualnyKolor << ": [" << (int)kolor.r << ", " << (int)kolor.g << ", " << (int)kolor.b << "]" << endl;
    ileKolorow++;
    return aktualnyKolor;
}

bool porownajKolory(SDL_Color kolor1, SDL_Color kolor2) {
    if (kolor1.r != kolor2.r)
        return false;
    if (kolor1.g != kolor2.g)
        return false;
    if (kolor1.b != kolor2.b)
        return false;

    return true;
}

int sprawdzKolor(SDL_Color kolor) {
    if (ileKolorow > 0)
    {
        for (int k = 0; k < ileKolorow; k++)
        {
            if (porownajKolory(kolor, paleta[k]))
                return k;
        }
    }

    return dodajKolor(kolor);
}

void narysujPalete(int px, int py, SDL_Color paleta5[]) //paleta5bw
{
    SDL_Color kolor;
    int nrKolor;
    ileKolorow = 0;
    for (int y = 0; y < wysokosc / 2; y++)
    {
        for (int x = 0; x < szerokosc / 2; x++)
        {
            kolor = getPixel(x, y);
            nrKolor = sprawdzKolor(kolor);
        }
    }
    for (int k = 0; k < 32; k++)
    {
        if (paleta5 == paleta5n)
            paleta5[k] = z5RGBna24RGB(k);
        else if (paleta5 == paleta5bw)
            paleta5[k] = z5BWna24RGB(k);
    }

    if (ileKolorow < 32)
    {
        for (int k = 0; k < 32; k++)
        {
            if (paleta5 == paleta5d )
                paleta5[k] = paleta[k];
        }
    }

    int tmp = -1;
    int hight = 0;
    for (int g = 0; g < 4; g++)
    {
        tmp++;
        if (g != 0)
            hight += 43;
        for (int k = 0; k < szerokosc/2; k++) //szerokosc
        {
            if (k % 32 == 0 && k != 0){
                tmp++;
            }

            for (int y = 0; y < 43; y++) //wysokosc
            {


                setPixel(k + px, y + py + hight, paleta5[tmp].r, paleta5[tmp].g, paleta5[tmp].b);

            }
        }
    }


    SDL_UpdateWindowSurface(window);
}

/* Część III projektu GKiM*/

void narysujPalete5(int px,int py,SDL_Color paleta5[]){
for(int p=0;p<256;p++){
    for(int y=0;y<85;y++){
        setPixel(p+px,y+py,paleta5[p/8].r,paleta5[p/8].g,paleta5[p/8].b);

        }
    }
}

void sortujKubelekBW(int start,int koniec)
{
    int maksimum=start;
    for(int p=start;p<koniec;p++){
        maksimum=p;
        for(int i=p;i<koniec;i++){
            if(obrazek[i].r>obrazek[maksimum].r)
                maksimum=i;
        }
        swap(obrazek[p],obrazek[maksimum]);
    }

}
void medianCutBW(int start,int koniec,int iteracja){

    if(iteracja>0){
        //najpierw sortujemy kubełek, apóźniej dzielimy go na 2 mnniejsze
        //konieczne jest ustalić względem której składowej prosortować kubełek
        //Uint8 ktoraSkladowa = najwekszaRoznica(start,koniec);
        sortujKubelekBW(start,koniec);//sortujKubelek(start,koniec,ktoraSkladowa);



    cout<<"Dzielimy kubelek na poziomie"<<iteracja<<endl;
    int srodek=(start+koniec)/2;//wyznaczamy sredek podziału zakładamy  ze zawsze uda nam się podzielic na równo
    medianCutBW(start,srodek,iteracja-1);
    medianCutBW(srodek+1,koniec,iteracja-1);
    cout<<"Zakonczono podzial na poziomie"<<iteracja<<endl;
    }
    else{
        //budujemy palete usredniajac kolory z okreslonego kubelka
        int sumaBW=0;
        for(int p=start;p<koniec;p++){
            sumaBW+=obrazek[p].r;

        }
        Uint8 BW = sumaBW/(koniec-start);
        SDL_Color nowyKolor = {BW,BW,BW};
        paleta5D[ileKolorow5]=nowyKolor;
        cout<<ileKolorow5<<": ["<<(int)nowyKolor.r<<", "<<(int)nowyKolor.g<<", "<<(int)nowyKolor.b<<"]"<<endl;
        ileKolorow5++;

    }
}

int znajdzSasiadaBW(SDL_Color kolor, SDL_Color paleta5[]){
    int minimum=999;
    int indexMinimum=0;

    SDL_Color kolorPaleta;
    int odleglosc;


    for(int i=0;i<32;i++){
        kolorPaleta=paleta5[i];
        odleglosc=abs(kolor.r-kolorPaleta.r);

        if(odleglosc<minimum){
            indexMinimum = i;
            minimum = odleglosc;
        }
    }
    return indexMinimum;
}

SDL_Color obrazekRGB[(szerokosc/2)*(wysokosc/2)];
Uint8 najwiekszaRoznica(int start,int koniec){

    SDL_Color kolor;

    Uint8 lowerR=obrazekRGB[0].r,lowerG=obrazekRGB[0].g,lowerB=obrazekRGB[0].b;
    Uint8 upperR=0,upperG=0,upperB=0;
    Uint8 R,G,B;
    Uint8 maxDifference;
for(int i=start;i<koniec;i++){
            //kolor=getPixel(start,koniec);
                kolor=obrazekRGB[i];
                lowerR=min(lowerR,kolor.r);
                lowerG=min(lowerG,kolor.g);
                lowerB=min(lowerB,kolor.b);

                upperR=max(upperR,kolor.r);
                upperG=max(upperG,kolor.g);
                upperB=max(upperB,kolor.b);

                R=upperR-lowerR;
                G=upperG-lowerG;
                B=upperB-lowerB;
}

 maxDifference=max(max(R,G),B);

 if(max(max(R,G),B)==R){
    return 1;
 }
    else if(max(max(R,G),B)==G){
    return 2;
    }
    else if(max(max(R,G),B)==B){
    return 3;
    }



}

void sortujKubelek(int start, int koniec, Uint8 ktoraSkladowa){


if(ktoraSkladowa==1){
    int maksimum=start;
    for(int p=start;p<koniec;p++){
        maksimum=p;
        for(int i=p;i<koniec;i++){
            if(obrazekRGB[i].r>obrazekRGB[maksimum].r)
                maksimum=i;
        }
        swap(obrazekRGB[p].r,obrazekRGB[maksimum].r);
    }
}
else if(ktoraSkladowa==2){
    int maksimum=start;
    for(int p=start;p<koniec;p++){
        maksimum=p;
        for(int i=p;i<koniec;i++){
            if(obrazekRGB[i].g>obrazekRGB[maksimum].g)
                maksimum=i;
        }
        swap(obrazekRGB[p].g,obrazekRGB[maksimum].g);
    }
}
else if(ktoraSkladowa==3){
 int maksimum=start;
    for(int p=start;p<koniec;p++){
        maksimum=p;
        for(int i=p;i<koniec;i++){
            if(obrazekRGB[i].b>obrazekRGB[maksimum].b)
                maksimum=i;
        }
        swap(obrazekRGB[p].b,obrazekRGB[maksimum].b);
    }
}

}

void medianCut(int start, int koniec, int iteracja){

    if(iteracja>0){

    Uint8 ktoraSkladowa = najwiekszaRoznica(start,koniec);
    sortujKubelek(start,koniec,ktoraSkladowa);
    //cout<<"Dzielimy kubelek na poziomie"<<iteracja<<endl;
    int srodek=(start+koniec)/2;//wyznaczamy sredek podziału zakładamy  ze zawsze uda nam się podzielic na równo
    medianCut(start,srodek,iteracja-1);
    medianCut(srodek+1,koniec,iteracja-1);
    //cout<<"Zakonczono podzial na poziomie"<<iteracja<<endl;
    }

 else{
        //budujemy palete usredniajac kolory z okreslonego kubelka
        int sumaR=0;
        int sumaG=0;
        int sumaB=0;

        for(int p=start;p<koniec;p++){
            sumaR+=obrazekRGB[p].r;
            sumaG+=obrazekRGB[p].g;
            sumaB+=obrazekRGB[p].b;

        }
        Uint8 R = sumaR/(koniec-start);
        Uint8 G = sumaG/(koniec-start);
        Uint8 B = sumaB/(koniec-start);
        SDL_Color nowyKolor = {R,G,B};
        paleta5D[ileKolorow5]=nowyKolor;
       // cout<<ileKolorow5<<": ["<<(int)nowyKolor.r<<", "<<(int)nowyKolor.g<<", "<<(int)nowyKolor.b<<"]"<<endl;
        ileKolorow5++;

    }
}

void narysujPaleteRGB(int px, int py, SDL_Color paleta5[]){

    int tmp = -1;
    int hight = 0;
    for (int g = 0; g < 4; g++)
    {
        tmp++;
        if (g != 0)
            hight += 43;
        for (int k = 0; k < szerokosc/2; k++) //szerokosc
        {
            if (k % 32 == 0 && k != 0){
                tmp++;
            }

            for (int y = 0; y < 43; y++) //wysokosc
            {


                setPixel(k + px, y + py + hight, paleta5[tmp].r, paleta5[tmp].g, paleta5[tmp].b);

            }
        }
    }
}
int znajdzSasiada(SDL_Color kolor, SDL_Color paleta5[]){
    int minimum=999;
    int indexMinimum=0;

    SDL_Color kolorPaleta;
    int odlegloscR;
    int odlegloscG;
    int odlegloscB;
    int suma;

    for(int i=0;i<32;i++){
        kolorPaleta=paleta5[i];
        odlegloscR=abs(kolor.r-kolorPaleta.r);
        odlegloscG=abs(kolor.g-kolorPaleta.g);
        odlegloscB=abs(kolor.b-kolorPaleta.b);
        suma=odlegloscR+odlegloscG+odlegloscB;


        if(suma<minimum){
            indexMinimum = i;
            minimum = suma;
        }
    }
    return indexMinimum;
}
/////////////////////////////////////////////////////////////////////////
void wypelnijPalete(SDL_Color paleta5[]) //paleta5bw
{
    SDL_Color kolor;
    int nrKolor;
    ileKolorow = 0;
    for (int y = 0; y < wysokosc / 2; y++)
    {
        for (int x = 0; x < szerokosc / 2; x++)
        {
            kolor = getPixel(x, y);
            nrKolor = sprawdzKolor(kolor);
        }
    }
    for (int k = 0; k < 32; k++)
    {
        if (paleta5 == paleta5n)
            paleta5[k] = z5RGBna24RGB(k);
        else if (paleta5 == paleta5bw)
            paleta5[k] = z5BWna24RGB(k);
    }

    if (ileKolorow <= 32)
    {
        for (int k = 0; k < 32; k++)
        {
            if (paleta5 == paleta5d )
                paleta5[k] = paleta[k];
        }
    }

}
////////////////////////////////////////////////////////////////////////////////////////////
void zapiszObraz5BW(){

    SDL_Color kolor;

    Uint16 szerokoscObrazka=szerokosc/2;
    Uint16 wysokoscObrazka=wysokosc/2;
    char identyfikator[]="BB";
    Uint8 wartosc;
     cout<<"Zapisujemy plik 'obraz5.bin' uzywajac metody write()"<<endl;
    ofstream wyjscie("obraz5.bin",ios::binary);

    wyjscie.write((char*)&identyfikator, sizeof(char)*2);
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16));



    for(int y=0;y<wysokoscObrazka;y++){
        for(int x=0;x<szerokoscObrazka;x++){
                    kolor=getPixel(x,y);
                    wartosc=z24RGBna5BW(kolor);

                    wyjscie.write((char*)&wartosc,sizeof(Uint8));
        }
    }

    wyjscie.close();
}
void zapiszObraz5N(){

SDL_Color kolor;

    Uint16 szerokoscObrazka=szerokosc/2;
    Uint16 wysokoscObrazka=wysokosc/2;
    char identyfikator[]="NN";
    Uint8 wartosc;
     cout<<"Zapisujemy plik 'obraz5.bin' uzywajac <<"<<endl;
    ofstream wyjscie("obraz5.bin",ios::binary);

    wyjscie.write((char*)&identyfikator, sizeof(char)*2);
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16));



    for(int y=0;y<wysokoscObrazka;y++){
        for(int x=0;x<szerokoscObrazka;x++){
                    kolor=getPixel(x,y);
                    wartosc=z24RGBna5RGB(kolor);

                    wyjscie.write((char*)&wartosc,sizeof(Uint8));


        }
    }

    wyjscie.close();

}
void zapiszObraz5D(){

SDL_Color kolorPaleta;
SDL_Color kolor;
Uint8 R,G,B;
Uint8 kolor5bit;
int ilePikseli=0;

 for(int y=0;y<wysokosc/2;y++){
        for(int x=0;x<szerokosc/2;x++){
                kolorPaleta=getPixel(x,y);
                kolor5bit=z24RGBna5RGB(kolorPaleta);
                R = (((kolor5bit & 0b00011000) >> 3) * 255.0/3.0);
                G = (((kolor5bit & 0b00000110) >> 1) * 255.0/3.0);
                B = (((kolor5bit & 0b00000001) *  255.0/1.0));
                obrazekRGB[ilePikseli]={R,G,B};
                ilePikseli++;
        }
     }


    ileKolorow5=0;
    medianCut(0,ilePikseli,5);
    narysujPaleteRGB(0,0,paleta5D);



    Uint16 szerokoscObrazka=szerokosc/2;
    Uint16 wysokoscObrazka=wysokosc/2;
    char identyfikator[]="DD";
    Uint8 wartosc;
     cout<<"Zapisujemy plik 'obraz5.bin' uzywajac <<"<<endl;
    ofstream wyjscie("obraz5.bin",ios::binary);

    wyjscie.write((char*)&identyfikator, sizeof(char)*2);
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16));


    for(int y=0;y<wysokoscObrazka;y++){
        for(int x=0;x<szerokoscObrazka;x++){
                    kolor=getPixel(x,y);

                    wyjscie.write((char*)&kolor.r,sizeof(Uint8));
                    wyjscie.write((char*)&kolor.g,sizeof(Uint8));
                    wyjscie.write((char*)&kolor.b,sizeof(Uint8));

        }
    }

    wyjscie.close();
}

vector<char> kompresjaByteRun(char wejscie[], int dlugosc)
{
    int i = 0;
    vector<char> kompresowane;
    //dopoki wszystkie bajty nie są skompresowane
    while(i<dlugosc)
    {
        //sekwencja powtarzajacych sie min. 2 bajtow
           if((i<dlugosc-1) && (wejscie[i] == wejscie[i+1]))
           {
               //mierzymy dlugosc sekwencji
               int licznik = 0;
               while((i+licznik<dlugosc-1) && (wejscie[i+licznik] == wejscie[i+licznik+1])&& licznik<127)
               {
                   licznik++;
               }
               //wypisujemy skompresowana sekwencje
               //cout<<-licznik<<", "<<wejscie[i+licznik]<<", ";
               kompresowane.push_back(-licznik);
               kompresowane.push_back(wejscie[i+licznik]);

               //przesuwamy wskaznik o dlugosci sekwencji
               i+= (licznik + 1);
           }
           //sekwencja roznych bajtow
           else
           {
             //mierzymy dlugosc sekwencji
             int licznik = 0;
             while((i+licznik < dlugosc-1) &&(wejscie[i+licznik] != wejscie[i+licznik+1])&& licznik<127)
             {
                licznik++;
             }
             // dodajemy jeszcze końcówke
             if((i+licznik == dlugosc-1)&& (licznik<127))
             {
                 licznik++;
             }
             // wypisujemy "skompresowana" sekwencje
             //cout<<(licznik-1)<<", ";
             kompresowane.push_back(licznik-1);
             for(int k =0 ; k<licznik; k++)
             {
                 //cout<<wejscie[i+k]<<", ";
                 kompresowane.push_back(wejscie[i+k]);
             }
             //przesuwamy wskaznik o dlugosc sekwencji
             i += licznik;
           }

    }
    return kompresowane;
}

vector<char> dekompresjaByteRun(char wejscie[], int dlugosc)
{
    int i = 0;
    vector<char> dekompresowane;
    //dopoki wszystkie bajty nie zostana zdekompresowane
    while(i < dlugosc)
    {
        //kod pusty
        if(wejscie[i] == -128)
        {
            i++;
        }
        // sekwencja powtarzajacych sie bajtow
        else if(wejscie[i]<0)
        {
            int licznik = -(wejscie[i]-1);
            for(int k=0; k<licznik; k++)
            {
               //cout<<(int)wejscie[i+1]<<", ";
               dekompresowane.push_back(wejscie[i+1]);
               //cout<<"N "<<dekompresowane[i];
            }
            //przesuwamy wskaznik o dlugosc sekwencji
            i+=2;
        }
        else{
            int licznik = wejscie[i]+1;
            for(int k = 0; k<licznik; k++)
            {
                //cout<<wejscie[i+k+1]<<", ";
                dekompresowane.push_back(wejscie[i+1+k]);
                //cout<<"N "<<dekompresowane[i];
            }
            //przesuwamy wskaznik o dlugosci sekwencji
            i += wejscie[i]+2;
        }
    }
    return dekompresowane;
}

vector<char> dekompresjaByteRun(vector<char> wejscie,int dlugosc)
{
    int i = 0;
    vector<char> dekompresowane;
    //dopoki wszystkie bajty nie zostana zdekompresowane
    while(i < dlugosc)
    {
        //cout<<"dekompresja iteracja: "<<i<<endl;
        //kod pusty
        if(wejscie[i] == -128)
        {
            i++;
        }
        // sekwencja powtarzajacych sie bajtow
        else if(wejscie[i]<0)
        {
            int licznik = -(wejscie[i]-1);
            for(int k=0; k<licznik; k++)
            {
               //cout<<(int)wejscie[i+1]<<", ";
               dekompresowane.push_back(wejscie[i+1]);
               //cout<<"N "<<dekompresowane[i];
            }
            //przesuwamy wskaznik o dlugosc sekwencji
            i+=2;
        }
        else{
            int licznik = wejscie[i]+1;
            for(int k = 0; k<licznik; k++)
            {
                //cout<<wejscie[i+k+1]<<", ";
                dekompresowane.push_back(wejscie[i+1+k]);
                //cout<<"N "<<dekompresowane[i];
            }
            //przesuwamy wskaznik o dlugosci sekwencji
            i += wejscie[i]+2;
        }
    }
    return dekompresowane;
}

void odczytajObraz5(){

    SDL_Color kolor;
    Uint16 szerokoscObrazka=0;
    Uint16 wysokoscObrazka=0;
    int tablicaDekompresja[dlugoscKompresja];
    char identyfikator[]=" ";

    Uint8 wartosc=0;

    cout<<"Odczytujemy plik 'obraz5.bin' uzywajac metody read()"<<endl;
    ifstream wejscie("obraz5.bin",ios::binary);


    wejscie.read((char*)&identyfikator, sizeof(char)*2);
    wejscie.read((char*)&szerokoscObrazka, sizeof(Uint16));
    wejscie.read((char*)&wysokoscObrazka, sizeof(Uint16));



    cout<<"id: "<<identyfikator<<endl;
    cout<<"szerokosc: "<<szerokoscObrazka<<endl;
    cout<<"wysokosc: "<<wysokoscObrazka<<endl;


    for(int y=0;y<wysokoscObrazka;y++){

        for(int x=0;x<szerokoscObrazka;x++){
            int i=0;

            if(identyfikator[0]=='B' &&identyfikator[1]=='B' ) {

            wejscie.read((char*)&wartosc,sizeof(Uint8));
            kolor=z5BWna24RGB(wartosc);

            setPixel(x,y+wysokoscObrazka,kolor.r,kolor.g,kolor.b);

            }

            else if(identyfikator[0] == 'N'&&identyfikator[1]=='N') {

            wejscie.read((char*)&wartosc,sizeof(Uint8));
            kolor=z5RGBna24RGB(wartosc);

            setPixel(x,y+wysokoscObrazka,kolor.r,kolor.g,kolor.b);
            }


            else if(identyfikator[0] == 'D'&&identyfikator[1]=='D') {

            wejscie.read((char*)&kolor.r,sizeof(Uint8));
            wejscie.read((char*)&kolor.g,sizeof(Uint8));
            wejscie.read((char*)&kolor.b,sizeof(Uint8));


            setPixel(x,y+wysokoscObrazka,kolor.r,kolor.g,kolor.b);
            }

            else {
                cout<<"ERROR";
            }
        }
    }

wejscie.close();

}

void ditheringBW() {
    //binaryzacja + dithering
    int R, G, B;
    int BW;

    float bledy[(szerokosc/2)+2][(wysokosc/2)+1];
    memset(bledy,0,sizeof(bledy));
    float blad = 0;
    int przesuniecie = 1;

    for(int y=0; y<wysokosc/2; y++)
    {
        for(int x=0; x<szerokosc/2; x++)
        {
            R=getPixel(x,y).r;
            G=getPixel(x,y).g;
            B=getPixel(x,y).b;

            BW = 0.299*R + 0.587*G + 0.114*B;
            BW+= bledy[x+przesuniecie][y];

            if (BW < 42)
                blad = BW;
            else if (BW < 85)
                blad = BW-85;
            else if (BW < 128)
                blad = BW-128;
            else if (BW < 170)
                blad = BW-170;
            else if (BW < 212)
                blad = BW-212;
            else
                blad = BW - 255;

            if (BW > 255) BW = 255;
            if (BW < 0) BW = 0;

            setPixel(x+szerokosc/2,y,BW,BW,BW);

            bledy[x+przesuniecie+1][y  ] += blad*7.0/16.0;
            bledy[x+przesuniecie+1][y+1] += blad*1.0/16.0;
            bledy[x+przesuniecie  ][y+1] += blad*5.0/16.0;
            bledy[x+przesuniecie-1][y+1] += blad*3.0/16.0;
        }

    }

}

void ditheringColor(){
    float bledyR[szerokosc/2 + 2][wysokosc/2 + 1]{0};
    float bledyG[szerokosc/2 + 2][wysokosc/2 + 1]{0};
    float bledyB[szerokosc/2 + 2][wysokosc/2 + 1]{0};

    float bladR = 0;
    float bladG = 0;
    float bladB = 0;

    int R, G, B;
    int przesuniecie = 1;

    for(int y = 0 ; y < wysokosc/2 ; ++y)
    {
        for(int x = 0 ; x < szerokosc/2 ; ++x)
        {
            R = getPixel(x ,y).r;
            G = getPixel(x,y).g;
            B = getPixel(x,y).b;

            R += bledyR[x+przesuniecie][y];
            G += bledyG[x+przesuniecie][y];
            B += bledyB[x+przesuniecie][y];

            int nowyR = 0;
            int nowyG = 0;
            int nowyB = 0;

            if(R < 128)
            {
                nowyR = 0;
                bladR = R;
            }
            else
            {
                nowyR = 255;
                bladR = R - 255;
            }

            if (G < 128)
            {
                nowyG = 0;
                bladG = G;
            }
            else
            {
                nowyG = 255;
                bladG = G - 255;
            }

            if(B < 128)
            {
                nowyB = 0;
                bladB = B;
            }
            else
            {
                nowyB = 255;
                bladB = B - 255;
            }

            setPixel(x + szerokosc/2,y, nowyR,nowyG,nowyB);

            //Aktualizacja tablicy bledow
            bledyR[x+przesuniecie+1][y] += bladR*7.0/16.0;
            bledyR[x+przesuniecie+1][y + 1] += bladR*1.0/16.0;
            bledyR[x+przesuniecie][y + 1] += bladR*5.0/16.0;
            bledyR[x+przesuniecie - 1][y + 1] += bladR*3.0/16.0;

            bledyG[x+przesuniecie+1][y] += bladG*7.0/16.0;
            bledyG[x+przesuniecie+1][y + 1] += bladG*1.0/16.0;
            bledyG[x+przesuniecie][y + 1] += bladG*5.0/16.0;
            bledyG[x+przesuniecie - 1][y + 1] += bladG*3.0/16.0;

            bledyB[x+przesuniecie+1][y] += bladB*7.0/16.0;
            bledyB[x+przesuniecie+1][y + 1] += bladB*1.0/16.0;
            bledyB[x+przesuniecie][y + 1] += bladB*5.0/16.0;
            bledyB[x+przesuniecie - 1][y + 1] += bladB*3.0/16.0;
        }
    }


}

void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void Funkcja7();
void Funkcja8();
void Funkcja9();
void Funkcja10();
void Funkcja11();
void Funkcja12();
void Funkcja13();
void Funkcja14();
void Funkcja15();
void Funkcja16();
void Funkcja17();
void Funkcja18();

//////////////////////////////////////////////
void Funkcja1() {


      int R, G, B;
    int kolor5bit;
    int nowyR, nowyG, nowyB;
    SDL_Color kolor;

    for(int y=0; y<wysokosc/2; y++)
    {
        for(int x=0; x<szerokosc/2; x++)
        {
            kolor = getPixel(x,y);

            R = kolor.r;
            G = kolor.g;
            B = kolor.b;

            //wersja 1
            nowyR = R >> 6;
            nowyG = G >> 6;
            nowyB = B >> 7;

            //kolor5bit = (nowyR<<3) + (nowyG<<1) + (nowyB);

            //cout<<"nowyR: "<<nowyR<<"nowyG: "<<nowyG<<"nowyB: "<<nowyB<<endl;
            //cout<<"kolor8bit: "<kolor8bit<<endl;

             R = nowyR<<6;
             G = nowyG<<6;
             B = nowyB<<7;
             setPixel(x + szerokosc/2, y,R,G,B);

              //wersja 2
              R = nowyR*255.0/3.0;
              G =nowyG*255.0/3.0;
              B =nowyB*255.0/1.0;
              setPixel(x,y+wysokosc/2,R,G,B);

             //wersja 3
             R = kolor.r;
             G = kolor.g;
             B = kolor.b;


            nowyR=round(R*3.0/255.0);
            nowyG=round(G*3.0/255.0);
            nowyB=round(B*1.0/255.0);

            kolor5bit=(nowyR<<3)+(nowyG<<1)+(nowyB);

              R = nowyR*255.0/3.0;
              G =nowyG*255.0/3.0;
              B =nowyB*255.0/1.0;

            setPixel(x+szerokosc/2,y+wysokosc/2,R,G,B);

        }
    }

    SDL_UpdateWindowSurface(window);
}
void Funkcja2() {
    int R, G, B;
    SDL_Color kolor;
    Uint8 Kolor5bit;
    int noweR, noweB, noweG, tmpR, tmpG;

    for (int y = 0; y < wysokosc / 2; y++)
    {
        for (int x = 0; x < szerokosc / 2; x++)
        {
            kolor = getPixel(x, y);
            Kolor5bit = z24RGBna5RGB(kolor);

            noweR = (((Kolor5bit & 0b00011000) >> 3) * 255.0/3.0);
            noweG = (((Kolor5bit & 0b00000110) >> 1) * 255.0/3.0);
            noweB = (((Kolor5bit & 0b00000001) *  255.0/1.0));

            setPixel(x + szerokosc / 2, y, noweR, noweG, noweB);
        }
    }

    SDL_UpdateWindowSurface(window);
}
void Funkcja3() {


    SDL_Color kolor;
    Uint8 kolorBW;

    for(int y=0; y<wysokosc/2; y++)
    {
        for(int x=0; x<szerokosc/2; x++)
        {
            kolor = getPixel(x,y);
            kolorBW = z24RGBna5BW(kolor);
            setPixel(x + szerokosc/2, y,kolorBW,kolorBW,kolorBW);
        }
    }

    SDL_UpdateWindowSurface(window);
}
void Funkcja4() {

     SDL_Color kolor,nowyKolor;
    Uint8 kolorBW;
    int R,G,B;
    for(int y=0; y<wysokosc/2; y++)
    {
        for(int x=0; x<szerokosc/2; x++)
        {
            kolor = getPixel(x,y);
            kolorBW = z24RGBna5BW(kolor);
            nowyKolor=z5BWna24RGB(kolorBW);

            R = nowyKolor.r;
            G = nowyKolor.g;
            B = nowyKolor.b;
            setPixel(x + szerokosc/2, y+wysokosc/2,R,G,B);
        }
    }
    SDL_UpdateWindowSurface(window);
}
//////////////////////////////////////////////
void Funkcja5() {

    SDL_Color kolor;
    int nrKolor;
    ileKolorow = 0;


    for (int y = 0; y < wysokosc / 2; y++)
    {
        for (int x = 0; x < szerokosc / 2; x++)
        {
            kolor = getPixel(x, y);
            nrKolor = sprawdzKolor(kolor);
        }
    }

    cout << "Wszystkich pikseli: " << 256 * 170 << endl;
    cout << "Wszystkich kolorw: " << ileKolorow << endl;

    for (int k = 0; k < 32; k++)
    {
        paleta5n[k] = z5RGBna24RGB(k);
        paleta5bw[k] = z5BWna24RGB(k);
    }

    if (ileKolorow < 32)
    {
        for (int k = 0; k < 10; k++)
        {
            paleta5d[k] = paleta[k];
        }
    }

    for (int k = 0; k < 32; k++)
    {
        for (int y = 0; y < 10; y++)
        {
            setPixel(k, y + 10 + wysokosc / 2, paleta5n[k].r, paleta5n[k].g, paleta5n[k].b);
            setPixel(k, y + 20 + wysokosc / 2, paleta5bw[k].r, paleta5bw[k].g, paleta5bw[k].b);
            setPixel(k, y + 30 + wysokosc / 2, paleta5d[k].r, paleta5d[k].g, paleta5d[k].b);
        }

    }

    SDL_UpdateWindowSurface(window);
}
void Funkcja6() {

   narysujPalete(0, wysokosc / 2, paleta5bw);
   narysujPalete(szerokosc / 2, 0, paleta5n);
   narysujPalete(szerokosc / 2, wysokosc / 2, paleta5d);

    SDL_UpdateWindowSurface(window);
}
//////////////////////////////////////////////
void Funkcja7() {

    SDL_Color kolor;
    Uint8 BW;
    int ilePikseli=0;

    for(int y=0;y<wysokosc/2;y++){
        for(int x=0;x<szerokosc/2;x++){
            kolor=getPixel(x,y);
            BW=0.299*kolor.r+0.587*kolor.g+0.114*kolor.b;
            obrazek[ilePikseli] = {BW,BW,BW};
            ilePikseli++;
            setPixel(x,y,BW,BW,BW);
        }
    }

    SDL_UpdateWindowSurface(window);
    for(int k=0;k<32;k++){
        paleta5N[k]=z5BWna24RGB(31-k);
    }
    for(int y=0;y<wysokosc/2;y++){
        for(int x=0;x<szerokosc/2;x++){
            kolor=getPixel(x,y);
        BW=z24RGBna5BW(kolor);
        kolor=z5BWna24RGB(BW);
        setPixel(x,y+wysokosc/2,kolor.r,kolor.g,kolor.b);
        }

    }

    narysujPalete5(szerokosc/2,wysokosc/2,paleta5N);

    ileKolorow5=0;
    medianCutBW(0,ilePikseli,5);
    narysujPalete5(szerokosc/2,wysokosc/2+wysokosc/4,paleta5D);


    int indexKoloru;
    for(int y=0;y<wysokosc/2;y++){
        for(int x=0;x<szerokosc/2;x++){
            kolor=getPixel(x,y);
            indexKoloru = znajdzSasiadaBW(kolor,paleta5D);
            setPixel(x+szerokosc/2,y,paleta5D[indexKoloru].r,paleta5D[indexKoloru].g,paleta5D[indexKoloru].b);
    }
}

 SDL_UpdateWindowSurface(window);

    SDL_UpdateWindowSurface(window);
}
void Funkcja8() {
  SDL_Color kolor;
    Uint8 R,G,B;
    Uint8 kolor5bit;
    int ilePikseli=0;

    for(int y=0;y<wysokosc/2;y++){
        for(int x=0;x<szerokosc/2;x++){
                kolor=getPixel(x,y);
                kolor5bit=z24RGBna5RGB(kolor);
                R = (((kolor5bit & 0b00011000) >> 3) * 255.0/3.0);
                G = (((kolor5bit & 0b00000110) >> 1) * 255.0/3.0);
                B = (((kolor5bit & 0b00000001) *  255.0/1.0));
                obrazekRGB[ilePikseli]={R,G,B};
                ilePikseli++;
                setPixel(x,y+wysokosc/2,R,G,B);
        }
     }


    for(int k=0;k<32;k++){
        paleta5N[k]=z5RGBna24RGB(k);
    }


    SDL_UpdateWindowSurface(window);

    ileKolorow5=0;
    medianCut(0,ilePikseli,5);
    narysujPaleteRGB(szerokosc/2,wysokosc/2,paleta5D);
    SDL_UpdateWindowSurface(window);

    int indexKoloru;
    for(int y=wysokosc/2;y<wysokosc;y++){
        for(int x=0;x<szerokosc/2;x++){
            kolor=getPixel(x,y);
            indexKoloru = znajdzSasiada(kolor,paleta5D);
            //setPixel(x+szerokosc/2,y,paleta5D[indexKoloru].r,paleta5D[indexKoloru].g,paleta5D[indexKoloru].b);
        }
    }
    SDL_UpdateWindowSurface(window);
    SDL_UpdateWindowSurface(window);

}
//////////////////////////////////////////////
void Funkcja9() {
zapiszObraz5BW();
odczytajObraz5();
SDL_UpdateWindowSurface(window);


}
void Funkcja10() {
zapiszObraz5N();
odczytajObraz5();
SDL_UpdateWindowSurface(window);

}
void Funkcja11() {

zapiszObraz5D();
odczytajObraz5();
SDL_UpdateWindowSurface(window);

}
void Funkcja12() {

//zapiszSkompresowanyObraz5BW();
//odczytajObraz5();
SDL_UpdateWindowSurface(window);
}
void Funkcja13() {

//zapiszSkompresowanyObraz5N();
//odczytajObraz5();
SDL_UpdateWindowSurface(window);
}
void Funkcja14() {


//zapiszSkompresowanyObraz5D();
//odczytajObraz5();
    SDL_UpdateWindowSurface(window);

}
void Funkcja15() {

//DITHERING

    //Tworzenie tablicy bledow
    float bledyR[szerokosc/2 + 2][wysokosc/2 + 1]{0};
    float bledyG[szerokosc/2 + 2][wysokosc/2 + 1]{0};
    float bledyB[szerokosc/2 + 2][wysokosc/2 + 1]{0};

    float bladR = 0;
    float bladG = 0;
    float bladB = 0;

    int R, G, B;
    int przesuniecie = 1;

    for(int y = 0 ; y < wysokosc/2 ; ++y)
    {
        for(int x = 0 ; x < szerokosc/2 ; ++x)
        {
            R = getPixel(x ,y).r;
            G = getPixel(x,y).g;
            B = getPixel(x,y).b;

            R += bledyR[x+przesuniecie][y];
            G += bledyG[x+przesuniecie][y];
            B += bledyB[x+przesuniecie][y];

            int nowyR = 0;
            int nowyG = 0;
            int nowyB = 0;

            if(R < 128) {
                nowyR = 0;
                bladR = R;
            }
            else {
                nowyR = 255;
                bladR = R - 255;
            }

            if (G < 128) {
                nowyG = 0;
                bladG = G;
            }
            else {
                nowyG = 255;
                bladG = G - 255;
            }

            if(B < 128) {
                nowyB = 0;
                bladB = B;
            }
            else {
                nowyB = 255;
                bladB = B - 255;
            }

            setPixel(x + szerokosc/2, y + wysokosc/2, nowyR,nowyG,nowyB);

            //Aktualizacja tablicy bledow
            bledyR[x+przesuniecie+1][y] += bladR*7.0/16.0;
            bledyR[x+przesuniecie+1][y + 1] += bladR*1.0/16.0;
            bledyR[x+przesuniecie][y + 1] += bladR*5.0/16.0;
            bledyR[x+przesuniecie - 1][y + 1] += bladR*3.0/16.0;

            bledyG[x+przesuniecie+1][y] += bladG*7.0/16.0;
            bledyG[x+przesuniecie+1][y + 1] += bladG*1.0/16.0;
            bledyG[x+przesuniecie][y + 1] += bladG*5.0/16.0;
            bledyG[x+przesuniecie - 1][y + 1] += bladG*3.0/16.0;

            bledyB[x+przesuniecie+1][y] += bladB*7.0/16.0;
            bledyB[x+przesuniecie+1][y + 1] += bladB*1.0/16.0;
            bledyB[x+przesuniecie][y + 1] += bladB*5.0/16.0;
            bledyB[x+przesuniecie - 1][y + 1] += bladB*3.0/16.0;
        }
    }

    SDL_UpdateWindowSurface(window);

}

void Funkcja16(){
    SDL_Event event;
    bool done = false;
    bool endFunc = false;

    bool isPaletteDedicated = false;
    bool isPaletteNarzucona = false;
    bool isPaletteBW = false;
    bool dithering = false;
    bool compress = false;

    //Wybor trybu kolorow
    cout << "Wybierz tryb koloru obrazka: \n" <<
         "[1] Narzucona paleta\n" <<
         "[2] Dedykowana paleta\n" <<
         "[3] Skala szarosci\n" <<
         "[ESC] Zakoncz program\n\n";

    while (SDL_WaitEvent(&event)) {

        switch (event.type) {
            case SDL_QUIT:
                endFunc = true;
                break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    endFunc = true;
                }
                if (event.key.keysym.sym == SDLK_1) {
                    isPaletteNarzucona = true;
                    done = true;
                }
                if (event.key.keysym.sym == SDLK_2) {
                    isPaletteDedicated = true;
                    done = true;
                }
                if (event.key.keysym.sym == SDLK_3) {
                    isPaletteBW = true;
                    done = true;
                }
            }
        }
        if (endFunc) return;
        if (done) break;
    }
    done = false;

    //Wybor ditheringu
    cout << "Czy chcesz zastosowac dithering?: \n" <<
         "[1] Tak\n" <<
         "[2] Nie\n" <<
         "[ESC] Zakoncz program\n\n";
    while (SDL_WaitEvent(&event)) {

        switch (event.type) {
            case SDL_QUIT:
                endFunc = true;
                break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    endFunc = true;
                }
                if (event.key.keysym.sym == SDLK_1) {
                    dithering = true;
                    done = true;
                }
                if (event.key.keysym.sym == SDLK_2) {
                    dithering = false;
                    done = true;
                }
            }
        }
        if (endFunc) return;
        if (done) break;
    }
    done = false;

    cout << "Czy chcesz zastosowac kompresje?: \n" <<
         "[1] Tak\n" <<
         "[2] Nie\n" <<
         "[ESC] Zakoncz program\n\n";
    while (SDL_WaitEvent(&event)) {

        switch (event.type) {
            case SDL_QUIT:
                endFunc = true;
                break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    endFunc = true;
                }
                if (event.key.keysym.sym == SDLK_1) {
                    compress = true;
                    done = true;
                }
                if (event.key.keysym.sym == SDLK_2) {
                    compress = false;
                    done = true;
                }
            }
        }
        if (endFunc) return;
        if (done) break;
    }
    done = false;

    //Dane do zapisu

    Uint16 szerokoscObrazka = szerokosc/2;
    Uint16 wysokoscObrazka = wysokosc/2;
    SDL_Color* colors = new SDL_Color[szerokoscObrazka * wysokoscObrazka];
    int imageSize = szerokoscObrazka * wysokoscObrazka;
    int8_t* pixelsArray = new int8_t[imageSize];
    SDL_Color* paletteArray;
    Uint8 paletteSize;


    cout << "\nZapisujemy 5-bitowy plik 'obraz5.bit' uzywajac metody write()\n";
    ofstream wyjscie("obraz5.bin", ios::binary);

    //Poczatek naglowka
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16)); // 2B
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16)); // 2B
    wyjscie.write((char*)&isPaletteDedicated, sizeof(bool)); // 1B
    wyjscie.write((char*)&isPaletteNarzucona, sizeof(bool)); // 1B
    wyjscie.write((char*)&isPaletteBW, sizeof(bool)); // 1B
    wyjscie.write((char*)&dithering, sizeof(bool)); // 1B



    int nrKoloru;
    ileKolorow = 0;
    SDL_Color kolor;

    for (int y=0;y<wysokosc/2;y++)
    {
        for(int x=0;x<szerokosc/2;x++)
        {
            kolor = getPixel(x,y);
            nrKoloru = sprawdzKolor(kolor);//////////////
        }
    }
    cout << "wszystkich pikseli: "<<256*170<<endl;
    cout << "wykrytych kolorow: " <<ileKolorow<<endl;

    /////////////////////////////////////////////////////////////////////////////////////////////////
    if (isPaletteDedicated) {

            SDL_Color kolor;
    Uint8 R,G,B;
    Uint8 kolor5bit;
    int ilePikseli=0;

    for(int y=0;y<wysokosc/2;y++){
        for(int x=0;x<szerokosc/2;x++){
                kolor=getPixel(x,y);
                kolor5bit=z24RGBna5RGB(kolor);
                R = (((kolor5bit & 0b00011000) >> 3) * 255.0/3.0);
                G = (((kolor5bit & 0b00000110) >> 1) * 255.0/3.0);
                B = (((kolor5bit & 0b00000001) *  255.0/1.0));
                obrazekRGB[ilePikseli]={R,G,B};
                ilePikseli++;
                setPixel(x,y+wysokosc/2,R,G,B);
        }
     }


    for(int k=0;k<32;k++){
        paleta5N[k]=z5RGBna24RGB(k);
    }


    SDL_UpdateWindowSurface(window);

    ileKolorow5=0;
    medianCut(0,ilePikseli,5);
    //narysujPaleteRGB(szerokosc/2,wysokosc/2,paleta5D);
    SDL_UpdateWindowSurface(window);

    int indexKoloru;
    for(int y=wysokosc/2;y<wysokosc;y++){
        for(int x=0;x<szerokosc/2;x++){
            kolor=getPixel(x,y);
            indexKoloru = znajdzSasiada(kolor,paleta5D);

        }
    }
    SDL_UpdateWindowSurface(window);


            if (dithering)
                ditheringColor();

            int i =0 ;
            for(int y=0;y<wysokoscObrazka;y++)
                {
                    for(int x=szerokoscObrazka;x<szerokoscObrazka*2;x++)
                    {
                        kolor=getPixel(x,y);
                        piksele[i]=z24RGBna5RGB(kolor);
                        i++;
                    }
                }

            //zapis
            if(compress)
            {
                cout<<"\nKompresja byterun\n";

                vector<char> skompresowane = kompresjaByteRun(piksele,szerokoscObrazka*wysokoscObrazka);

                int rozmiarSkompresowany = static_cast<int>(skompresowane.size());
                cout<<"Rozmiar po kompresji (zapis): "<<rozmiarSkompresowany<<endl;
                cout<<"Rozmiar przed kompresja (zapis): "<<szerokoscObrazka*wysokoscObrazka<<endl;

                if(rozmiarSkompresowany>=szerokoscObrazka*wysokoscObrazka)
                {
                    cout<<"Kompresja nieoplacalna - zapis bez kompresji"<<endl;

                    char identyfikator[]="5D";
                    wyjscie.write((char*)&identyfikator, sizeof(char) * 2); // 2B

                    for(int i=0;i<szerokoscObrazka*wysokoscObrazka;i++)
                    {
                        wyjscie.write((char*)&piksele[i], sizeof(char)); // 1B

                    }
                }
                else
                {
                    cout<<"Stopien kompresji: "<<float((szerokoscObrazka*wysokoscObrazka)/float(rozmiarSkompresowany))<<endl;
                    wyjscie.write((char*)&compress, sizeof(bool)); // 1B
                    char identyfikator[]="5D";
                    wyjscie.write((char*)&identyfikator, sizeof(char) * 2); // 2B

                    char* daneDoZapisu = new char[rozmiarSkompresowany];

                    wyjscie.write((char*)&rozmiarSkompresowany,sizeof(rozmiarSkompresowany));

                    for(int i = 0;i<rozmiarSkompresowany;i++)
                    {
                        daneDoZapisu[i]=skompresowane[i];
                        wyjscie.write((char*)&skompresowane[i], sizeof(skompresowane[i]));
                    }
                }
            }
            else //gdy nie kompresujemy
            {
                wyjscie.write((char*)&compress, sizeof(bool)); // 1B
                char identyfikator[]="5D";
                wyjscie.write((char*)&identyfikator, sizeof(char) * 2); // 2B

                for(int i=0;i<szerokoscObrazka*wysokoscObrazka;i++)
                {
                    wyjscie.write((char*)&piksele[i], sizeof(char)); // 1B
                }
            }


            narysujPalete(szerokosc/2,wysokosc/2,paleta5D);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////
        else if (isPaletteNarzucona)
        {
            for(int k=0;k<32;k++)
                paleta5n[k] = z5RGBna24RGB(k);

            SDL_Color kolor;

            int wartosc = 0;

            for(int y=0;y<wysokoscObrazka;y++)
            {
                for(int x=0;x<szerokoscObrazka;x++)
                {
                    kolor=getPixel(x,y);
                    wartosc = z24RGBna5RGB(kolor);
                    kolor = z5RGBna24RGB(wartosc);
                    setPixel(x+szerokosc/2,y,kolor.r,kolor.g,kolor.b);

                }
            }
            if (dithering)
                ditheringColor();

            int i = 0;
            for(int y=0;y<wysokoscObrazka;y++)
            {
                for(int x=szerokoscObrazka;x<szerokoscObrazka*2;x++)
                {
                    kolor=getPixel(x,y);
                    piksele[i]=z24RGBna5RGB(kolor);
                    i++;
                }
            }

            if(compress)
            {
                cout<<"\nKompresja byterun\n";

                vector<char> skompresowane = kompresjaByteRun(piksele,szerokoscObrazka*wysokoscObrazka);

                int rozmiarSkompresowany = static_cast<int>(skompresowane.size());
                cout<<"Rozmiar po kompressji (zapis): "<<rozmiarSkompresowany<<endl;
                cout<<"Rozmiar przed kompresja (zapis): "<<szerokoscObrazka*wysokoscObrazka<<endl;

                if(rozmiarSkompresowany>=szerokoscObrazka*wysokoscObrazka)
                {
                    cout<<"Kompresja nieoplacalna - zapis bez kompresji"<<endl;
                    compress=false;
                    wyjscie.write((char*)&compress, sizeof(bool)); // 1B
                    char identyfikator[]="5N";
                    wyjscie.write((char*)&identyfikator, sizeof(char) * 2); // 2B

                    for(int i=0;i<szerokoscObrazka*wysokoscObrazka;i++)
                    {
                        wyjscie.write((char*)&piksele[i], sizeof(char)); // 1B
                        //cout<<piksele[i]<<" ";

                }
                }

                else
                {
                    cout<<"Stopien kompresji: "<<float((szerokoscObrazka*wysokoscObrazka)/float(rozmiarSkompresowany))<<endl;
                    wyjscie.write((char*)&compress, sizeof(bool)); // 1B
                    char identyfikator[]="5N";
                    wyjscie.write((char*)&identyfikator, sizeof(char) * 2); // 2B

                    char* daneDoZapisu = new char[rozmiarSkompresowany];

                    wyjscie.write((char*)&rozmiarSkompresowany,sizeof(rozmiarSkompresowany));

                    for(int i = 0;i<rozmiarSkompresowany;i++)
                    {
                        daneDoZapisu[i]=skompresowane[i];
                        wyjscie.write((char*)&skompresowane[i], sizeof(skompresowane[i]));
                    }
                }

            }
            else //gdy nie kompresujemy
            {
                wyjscie.write((char*)&compress, sizeof(bool)); // 1B
                char identyfikator[]="5N";
                wyjscie.write((char*)&identyfikator, sizeof(char) * 2); // 2B

                for(int i=0;i<szerokoscObrazka*wysokoscObrazka;i++) {
                    wyjscie.write((char*)&piksele[i], sizeof(char)); // 1B
                }
            }
        }
        ////////////////////////////////////////////////////////////////////////////////////////////////
        else if (isPaletteBW)
        {
            for(int k=0;k<32;k++)
                paleta5bw[k] = z5BWna24RGB(k);

            SDL_Color kolor;
            Uint8 wartosc;

            cout << "BW" << endl;

            for(int y=0;y<wysokoscObrazka;y++)
            {
                for(int x=0;x<szerokoscObrazka;x++)
                {
                    kolor=getPixel(x,y);
                    wartosc = z24RGBna5BW(kolor);
                    setPixel(x+szerokosc/2,y,wartosc,wartosc,wartosc);

                    SDL_Color kk = z5BWna24RGB(wartosc);
                    setPixel(x,y+wysokosc/2,kk.r,kk.g,kk.b);
                }
            }


            if (dithering)
            {
                ditheringBW();
            }


            int i = 0;

            for(int y=0;y<wysokoscObrazka;y++)
            {
                for(int x=szerokoscObrazka;x<szerokoscObrazka*2;x++)
                {
                    kolor=getPixel(x,y);
                    piksele[i]=z24RGBna5BW(kolor);
                    i++;
                }
            }

            if(compress)
            {
                cout<<"\nKompresja =ByteRun\n";

                vector<char> skompresowane = kompresjaByteRun(piksele,szerokoscObrazka*wysokoscObrazka);

                int rozmiarSkompresowany = static_cast<int>(skompresowane.size());
                cout<<"Rozmiar po kompresji: "<<rozmiarSkompresowany<<endl;
                cout<<"Rozmiar przed kompresja: "<<szerokoscObrazka*wysokoscObrazka<<endl;

                if(rozmiarSkompresowany>=szerokoscObrazka*wysokoscObrazka)
                {
                    cout<<"Kompresja nieoplacalna - zapis bez kompresji"<<endl;
                    compress=false;
                    wyjscie.write((char*)&compress, sizeof(bool)); // 1B
                    char identyfikator[]="BW";
                    wyjscie.write((char*)&identyfikator, sizeof(char) * 2); // 2B

                    for(int i=0;i<szerokoscObrazka*wysokoscObrazka;i++)
                    {
                        wyjscie.write((char*)&piksele[i], sizeof(char)); // 1B

                    }
                }
                else
                {
                    cout<<"Stopien kompresji: "<<float((szerokoscObrazka*wysokoscObrazka)/float(rozmiarSkompresowany))<<endl;
                    wyjscie.write((char*)&compress, sizeof(bool)); // 1B
                    char identyfikator[]="BW";
                    wyjscie.write((char*)&identyfikator, sizeof(char) * 2); // 2B

                    char* daneDoZapisu = new char[rozmiarSkompresowany];

                    wyjscie.write((char*)&rozmiarSkompresowany,sizeof(rozmiarSkompresowany));

                    for(int i = 0;i<rozmiarSkompresowany;i++)
                    {
                        daneDoZapisu[i]=skompresowane[i];
                        wyjscie.write((char*)&skompresowane[i], sizeof(skompresowane[i]));
                    }
                }
            }
            else //gdy nie kompresujemy
            {
                wyjscie.write((char*)&compress, sizeof(bool)); // 1B
                char identyfikator[]="BW";
                wyjscie.write((char*)&identyfikator, sizeof(char) * 2); // 2B

                for(int i=0;i<szerokoscObrazka*wysokoscObrazka;i++)
                {
                    wyjscie.write((char*)&piksele[i], sizeof(char)); // 1B
                }
            }

            narysujPalete(szerokosc/2,wysokosc/2,paleta5bw);
            SDL_UpdateWindowSurface(window);
        }


    wyjscie.close();
    SDL_UpdateWindowSurface(window);

}

void Funkcja17(){

 bool isPaletteDedicated; // false - narzucona, true - dedykowana
    bool isPaletteNarzucona; // false - narzucona, true - dedykowana
    bool isPaletteBW; // false - nie, true - tak
    bool dithering; // false - bez ditheringu, true - dithering
    bool compress;  // true - kompresja false - bez kompresji

    SDL_Color kolor;
    Uint8 wartosc = 0;
    Uint16 szerokoscObrazka = 0;
    Uint16 wysokoscObrazka = 0;
    char identyfikator[] = "    ";

    cout << "\nOdczytujemy 5-bitowy plik 'obraz5.bin' uzywajac metody read()\n";

    ifstream wejscie("obraz5.bin", ios::binary);

    wejscie.read((char*)&szerokoscObrazka, sizeof(Uint16)); // 2B
    wejscie.read((char*)&wysokoscObrazka, sizeof(Uint16)); // 2B
    wejscie.read((char*)&isPaletteDedicated, sizeof(bool)); // 1B
    wejscie.read((char*)&isPaletteNarzucona, sizeof(bool)); // 1B
    wejscie.read((char*)&isPaletteBW, sizeof(bool)); // 1B
    wejscie.read((char*)&dithering, sizeof(bool)); // 1B
    wejscie.read((char*)&compress, sizeof(bool)); // 1B
    wejscie.read((char*)&identyfikator, sizeof(char) * 2); // 4B



    cout << "   id: " << identyfikator<< endl;
    cout << "   szerokosc: " << szerokoscObrazka << endl;
    cout << "   wysokosc: " << wysokoscObrazka << endl;
    cout << "   Paleta Dedykowana: " << isPaletteDedicated << endl;
    cout << "   Paleta Narzucona: " << isPaletteNarzucona << endl;
    cout << "   Paleta BW: " << isPaletteBW << endl;
    cout << "   Dithering: " << dithering << endl;
    cout << "   Kompresja: " << compress << endl;



    vector<char> skompresowaneZpliku,zdekompresowaneZpliku;

    skompresowaneZpliku.clear();
    zdekompresowaneZpliku.clear();

    char* skompresowaneChar;

    int rozmiar;

    ////////////////////////////////////////////////////////////////
    if(compress)
    {
        cout<<"ODCZYT"<<endl;
        wejscie.read((char*)&rozmiar,sizeof(rozmiar));
        skompresowaneChar = new char[rozmiar];

        cout<<"Rozmiar pliku przed kompresja: "<<szerokoscObrazka*wysokoscObrazka<<endl;
        cout<<"Rozmiar pliku skompresowanego: "<<rozmiar<<endl;

        for(int i=0;i<rozmiar;i++)
        {
            wejscie.read((char*)&wartosc,sizeof(Uint8));
            skompresowaneZpliku.push_back(wartosc);

        }
        wejscie.close();



        //cout<<"PRZED DEKOMPRESJA BYTERUN"<<endl;
        zdekompresowaneZpliku = dekompresjaByteRun(skompresowaneZpliku,szerokoscObrazka*wysokoscObrazka);
        //cout<<"PO DEKOMPRESJA BYTERUN"<<endl;


        if (identyfikator[0] == 'B' && identyfikator[1] == 'W')
        {
            int i=0;
            for(int y=0;y<wysokoscObrazka;y++)
            {
                for(int x=0;x<szerokoscObrazka;x++)
                {
                    kolor = z5BWna24RGB(zdekompresowaneZpliku[i]);
                    setPixel(x,y+wysokosc/2,kolor.r,kolor.g,kolor.b);
                    i++;

                }
            }
        }
        else if (identyfikator[0] == '5' && identyfikator[1] == 'N')
        {
            int i=0;
            for(int y=0;y<wysokoscObrazka;y++)
            {
                for(int x=0;x<szerokoscObrazka;x++)
                {
                    kolor = z5RGBna24RGB(zdekompresowaneZpliku[i]);
                    setPixel(x,y+wysokoscObrazka,kolor.r,kolor.g,kolor.b);
                    i++;
                }
            }
        }
        else if (identyfikator[0] == '5' && identyfikator[1] == 'D')
        {
            int i=0;
            for(int y=0;y<wysokoscObrazka;y++)
            {
                for(int x=0;x<szerokoscObrazka;x++)
                {
                    kolor = z5RGBna24RGB(zdekompresowaneZpliku[i]);
                    setPixel(x,y+wysokoscObrazka,kolor.r,kolor.g,kolor.b);
                    i++;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    else //bez kompresji
    {
            if (identyfikator[0] == 'B' && identyfikator[1] == 'W')
            {
                for(int y=0;y<wysokoscObrazka;y++)
                {
                    for(int x=0;x<szerokoscObrazka;x++)
                    {

                        wejscie.read((char*)&wartosc, sizeof(Uint8));
                        setPixel(x,y+wysokoscObrazka,wartosc,wartosc,wartosc);
                    }
                }
            }
            else if (identyfikator[0] == '5' && identyfikator[1] == 'N')
            {
                for(int y=0;y<wysokoscObrazka;y++)
                {
                    for(int x=0;x<szerokoscObrazka;x++)
                    {
                        wejscie.read((char*)&wartosc, 1);
                        kolor = z5RGBna24RGB(wartosc);
                        setPixel(x,y+wysokoscObrazka,kolor.r,kolor.g,kolor.b);
                    }
                }
            }
            else if (identyfikator[0] == '5' && identyfikator[1] == 'D')
            {
                for(int y=0;y<wysokoscObrazka;y++)
                {
                    for(int x=0;x<szerokoscObrazka;x++)
                    {
                        wejscie.read((char*)&wartosc, 1);
                        kolor = z5RGBna24RGB(wartosc);
                        setPixel(x,y+wysokoscObrazka,kolor.r,kolor.g,kolor.b);
                    }
                }
            }
    }


    wejscie.close();
    SDL_UpdateWindowSurface(window);
}

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B) {
    if ((x >= 0) && (x < szerokosc) && (y >= 0) && (y < wysokosc))
    {
        /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
        Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

        /* Pobieramy informację ile bajtów zajmuje jeden piksel */
        int bpp = screen->format->BytesPerPixel;

        /* Obliczamy adres piksela */
        Uint8* p1 = (Uint8*)screen->pixels + (y * 2) * screen->pitch + (x * 2) * bpp;
        Uint8* p2 = (Uint8*)screen->pixels + (y * 2 + 1) * screen->pitch + (x * 2) * bpp;
        Uint8* p3 = (Uint8*)screen->pixels + (y * 2) * screen->pitch + (x * 2 + 1) * bpp;
        Uint8* p4 = (Uint8*)screen->pixels + (y * 2 + 1) * screen->pitch + (x * 2 + 1) * bpp;

        /* Ustawiamy wartość piksela, w zależnoœci od formatu powierzchni*/
        switch (bpp)
        {
        case 1: //8-bit
            *p1 = pixel;
            *p2 = pixel;
            *p3 = pixel;
            *p4 = pixel;
            break;

        case 2: //16-bit
            *(Uint16*)p1 = pixel;
            *(Uint16*)p2 = pixel;
            *(Uint16*)p3 = pixel;
            *(Uint16*)p4 = pixel;
            break;

        case 3: //24-bit
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p1[0] = (pixel >> 16) & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = pixel & 0xff;
                p2[0] = (pixel >> 16) & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = pixel & 0xff;
                p3[0] = (pixel >> 16) & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = pixel & 0xff;
                p4[0] = (pixel >> 16) & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = pixel & 0xff;
            }
            else {
                p1[0] = pixel & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = (pixel >> 16) & 0xff;
                p2[0] = pixel & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = (pixel >> 16) & 0xff;
                p3[0] = pixel & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = (pixel >> 16) & 0xff;
                p4[0] = pixel & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32*)p1 = pixel;
            *(Uint32*)p2 = pixel;
            *(Uint32*)p3 = pixel;
            *(Uint32*)p4 = pixel;
            break;

        }
    }
}

void setPixelSurface(int x, int y, Uint8 R, Uint8 G, Uint8 B) {
    if ((x >= 0) && (x < szerokosc * 2) && (y >= 0) && (y < wysokosc * 2))
    {
        /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
        Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

        /* Pobieramy informację ile bajtów zajmuje jeden piksel */
        int bpp = screen->format->BytesPerPixel;

        /* Obliczamy adres piksela */
        Uint8* p = (Uint8*)screen->pixels + y * screen->pitch + x * bpp;

        /* Ustawiamy wartość piksela, w zależności od formatu powierzchni*/
        switch (bpp)
        {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16*)p = pixel;
            break;

        case 3: //24-bit
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32*)p = pixel;
            break;
        }
    }
}

SDL_Color getPixel(int x, int y) {
    SDL_Color color;
    Uint32 col = 0;
    if ((x >= 0) && (x < szerokosc) && (y >= 0) && (y < wysokosc)) {
        //określamy pozycję
        char* pPosition = (char*)screen->pixels;

        //przesunięcie względem y
        pPosition += (screen->pitch * y * 2);

        //przesunięcie względem x
        pPosition += (screen->format->BytesPerPixel * x * 2);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return (color);
}

SDL_Color getPixelSurface(int x, int y, SDL_Surface* surface) {
    SDL_Color color;
    Uint32 col = 0;
    if ((x >= 0) && (x < szerokosc) && (y >= 0) && (y < wysokosc)) {
        //określamy pozycję
        char* pPosition = (char*)surface->pixels;

        //przesunięcie względem y
        pPosition += (surface->pitch * y);

        //przesunięcie względem x
        pPosition += (surface->format->BytesPerPixel * x);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, surface->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    }
    return (color);
}

void ladujBMP(char const* nazwa, int x, int y) {
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Color kolor;
        for (int yy = 0; yy < bmp->h; yy++) {
            for (int xx = 0; xx < bmp->w; xx++) {
                kolor = getPixelSurface(xx, yy, bmp);
                setPixel(xx, yy, kolor.r, kolor.g, kolor.b);
            }
        }
        SDL_FreeSurface(bmp);
        SDL_UpdateWindowSurface(window);
    }

}

void czyscEkran(Uint8 R, Uint8 G, Uint8 B) {
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_UpdateWindowSurface(window);
}


int main(int argc, char* argv[]) {

    // główna pętla programu

    int n;

    cout<<"Wybierz funkcje: ";
    cout<<endl;
    cout<<"1.  Konwersja obrazka 24-bitowego RGB i 5-bitowego RGB "<<endl;
    cout<<"2.  Tworzenie i wizualizacja palety 5-bitowej w 3 wersjach: narzuconej, w skali szarosci i dedykowanej   "<<endl;
    cout<<"3.  Generowanie 5-bitowej plaety BW i kolorowej "<<endl;
    cout<<"4.  Zapis i odczyt obrazka do pliku/Kompresja i dekompresja obrazka(ByteRun)/Dithering "<<endl;
      cout<<endl<<"Wybierz: ";
      cin>>n;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    switch(n){

        case 1: {
          cout<<endl;
          cout<<"------------------- 1 czesc -------------------"<<endl;

          cout<<"Wybierz obrazek: a,s,d,f,g,h,j "<<endl;
          cout<<endl;
          cout<<"1. konwersja z 24-bitowego RGB na 5-bitowe RGB (na 3 sposoby)"<<endl;
          cout<<"2. konwersja z 24-bitowego RGB na 5-bitowe RGB (funkcja z24RGBna5RGB())"<<endl;
          cout<<"3. konwersja z 24-bitowego RGB na 5-bitowe BW (funkcja z24RGBna5BW())"<<endl;
          cout<<"4. konwersja z 24-bitowego RGB na 5-bitowe BW (funkcja z24RGBna5BW())"<<endl;



                    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                    printf("SDL_Init Error: %s\n", SDL_GetError());
                    return EXIT_FAILURE;
                    }

                    window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc * 2, wysokosc * 2, SDL_WINDOW_SHOWN);

                    if (window == NULL) {
                        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
                        return EXIT_FAILURE;
                    }

                    screen = SDL_GetWindowSurface(window);
                    if (screen == NULL) {
                        fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
                        return false;
                    }
                    SDL_UpdateWindowSurface(window);


                    bool done = false;


                    SDL_Event event;

                    while (SDL_WaitEvent(&event)) {
                        // sprawdzamy czy pojawiło się zdarzenie
                        switch (event.type) {
                        case SDL_QUIT:
                            done = true;
                            break;

                            // sprawdzamy czy został wciśnięty klawisz
                        case SDL_KEYDOWN: {
                            // wychodzimy, gdy wciśnięto ESC
                            if (event.key.keysym.sym == SDLK_ESCAPE)
                                done = true;
                            if (event.key.keysym.sym == SDLK_1)
                                Funkcja1();
                            if (event.key.keysym.sym == SDLK_2)
                                Funkcja2();
                            if (event.key.keysym.sym == SDLK_3)
                                Funkcja3();
                            if (event.key.keysym.sym == SDLK_4)
                                Funkcja4();
                            if (event.key.keysym.sym == SDLK_a)
                                ladujBMP("obrazek1.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_s)
                                ladujBMP("obrazek2.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_d)
                                ladujBMP("obrazek3.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_f)
                                ladujBMP("obrazek4.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_g)
                                ladujBMP("obrazek5.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_h)
                                ladujBMP("obrazek6.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_j)
                                ladujBMP("obrazek7.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_t)
                                ladujBMP("33-1.bmp", 0, 0);
                                if (event.key.keysym.sym == SDLK_y)
                                ladujBMP("33-2.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_b)
                                czyscEkran(0, 0, 10);
                            else
                                break;
                        }
                    }
                        if (done) break;
                }
                break;
        }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case 2:{
        cout<<endl;
        cout<<"------------------- 2 czesc -------------------"<<endl;

        cout<<"Wybierz obrazek: a,s,d,f,g,h,j "<<endl;
        cout<<endl;

        cout<<"1. Tworzenie palety n-bitowej w 3 wersjach (paleta narzucona, paleta skali szarosci, paleta dedykowana)"<<endl;
        cout<<"2. Wizualizacja utworzonych palet"<<endl;


        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
        }

        window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc * 2, wysokosc * 2, SDL_WINDOW_SHOWN);

        if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
        }

        screen = SDL_GetWindowSurface(window);
        if (screen == NULL) {
        fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
        return false;
        }
        SDL_UpdateWindowSurface(window);


            bool done = false;

                SDL_Event event;
                while (SDL_WaitEvent(&event)) {
                        // sprawdzamy czy pojawiło się zdarzenie
                        switch (event.type) {
                        case SDL_QUIT:
                            done = true;
                            break;

                            // sprawdzamy czy został wciśnięty klawisz
                        case SDL_KEYDOWN: {
                            if (event.key.keysym.sym == SDLK_1)
                                Funkcja5();
                            if (event.key.keysym.sym == SDLK_2)
                                Funkcja6();
                           if (event.key.keysym.sym == SDLK_a)
                                ladujBMP("obrazek11.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_s)
                                ladujBMP("obrazek12.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_d)
                                ladujBMP("obrazek13.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_f)
                                ladujBMP("obrazek14.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_g)
                                ladujBMP("obrazek15.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_h)
                                ladujBMP("obrazek16.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_j)
                                ladujBMP("obrazek17.bmp", 0, 0);
                                  if (event.key.keysym.sym == SDLK_t)
                                ladujBMP("33-1.bmp", 0, 0);
                                if (event.key.keysym.sym == SDLK_y)
                                ladujBMP("33-2.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_b)
                                czyscEkran(0, 0, 10);

                            else
                                break;
                        }
                    }
                        if (done)
                            break;
                    }
                break;
            }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case 3:{
        cout<<endl;
        cout<<"------------------- 3 czesc -------------------"<<endl;

        cout<<"Wybierz obrazek: a,s,d,f,g,h,j "<<endl;
        cout<<endl;

        cout<<"1. Wizualizacja 5-bitowej dedykowanej palety w skali szarosci dla obrazka"<<endl;
        cout<<"2. Wizualizacja 5-bitowej dedykowanej palety dla obrazka"<<endl;

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
        }

            window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc * 2, wysokosc * 2, SDL_WINDOW_SHOWN);

            if (window == NULL) {
                printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
                return EXIT_FAILURE;
            }

            screen = SDL_GetWindowSurface(window);
            if (screen == NULL) {
                fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
                return false;
            }
            SDL_UpdateWindowSurface(window);


            bool done = false;


                            SDL_Event event;
                            while (SDL_WaitEvent(&event)) {
                        // sprawdzamy czy pojawiło się zdarzenie
                        switch (event.type) {
                        case SDL_QUIT:
                            done = true;
                            break;

                            // sprawdzamy czy został wciśnięty klawisz
                        case SDL_KEYDOWN: {
                            if (event.key.keysym.sym == SDLK_1)
                                Funkcja7();
                            if (event.key.keysym.sym == SDLK_2)
                                Funkcja8();

                            if (event.key.keysym.sym == SDLK_a)
                                ladujBMP("obrazek21.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_s)
                                ladujBMP("obrazek22.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_d)
                                ladujBMP("obrazek23.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_f)
                                ladujBMP("obrazek24.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_g)
                                ladujBMP("obrazek25.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_h)
                                ladujBMP("obrazek26.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_j)
                                ladujBMP("obrazek27.bmp", 0, 0);
                                  if (event.key.keysym.sym == SDLK_t)
                                ladujBMP("33-1.bmp", 0, 0);
                                if (event.key.keysym.sym == SDLK_y)
                                ladujBMP("33-2.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_b)
                                czyscEkran(0, 0, 10);


                            else
                                break;
                        }
                    }
                        if (done)
                            break;
                }


        break;
        }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case 4:{
        cout<<endl;
        cout<<"------------------- 4 czesc -------------------"<<endl;

        cout<<"Wybierz obrazek: a,s,d,f,g,h,j "<<endl;
        cout<<endl;
        cout<<"1. Zapis i odczyt obrazka BW "<<endl;
        cout<<"2. Zapis i odczyt obrazka 5-bit RGB"<<endl;
        cout<<"3. Zapis i odczyt palety dedykowanej z obrazka"<<endl;
        cout<<"4. Zapis skompresowanego obrazka"<<endl;
        cout<<"5. Odczyt zdekompresowanego obrazka"<<endl;




        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
        }

        window = SDL_CreateWindow(tytul, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szerokosc * 2, wysokosc * 2, SDL_WINDOW_SHOWN);

        if (window == NULL) {
            printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
            return EXIT_FAILURE;
        }

        screen = SDL_GetWindowSurface(window);
        if (screen == NULL) {
            fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
            return false;
        }
        SDL_UpdateWindowSurface(window);


        bool done = false;

                             SDL_Event event;
                        while (SDL_WaitEvent(&event)) {
                        // sprawdzamy czy pojawiło się zdarzenie
                        switch (event.type) {
                        case SDL_QUIT:
                            done = true;
                            break;

                            // sprawdzamy czy został wciśnięty klawisz
                        case SDL_KEYDOWN: {
                            if (event.key.keysym.sym == SDLK_1)
                                Funkcja9();
                            if (event.key.keysym.sym == SDLK_2)
                                Funkcja10();
                            if (event.key.keysym.sym == SDLK_3)
                                Funkcja11();
                            if (event.key.keysym.sym == SDLK_4)
                                Funkcja16();
                            if (event.key.keysym.sym == SDLK_5)
                                Funkcja17();
                            if (event.key.keysym.sym == SDLK_a)
                                ladujBMP("obrazek11.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_s)
                                ladujBMP("obrazek12.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_d)
                                ladujBMP("obrazek13.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_f)
                                ladujBMP("obrazek14.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_g)
                                ladujBMP("obrazek15.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_h)
                                ladujBMP("obrazek16.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_j)
                                ladujBMP("obrazek17.bmp", 0, 0);
                                if (event.key.keysym.sym == SDLK_t)
                                ladujBMP("33-1.bmp", 0, 0);
                                if (event.key.keysym.sym == SDLK_y)
                                ladujBMP("33-2.bmp", 0, 0);
                            if (event.key.keysym.sym == SDLK_b)
                                czyscEkran(0, 0, 10);


                            else
                                break;
                        }
                    }
                        if (done)
                            break;
                }
            break;
        }

        default:{
            cout<<endl;
            cout<<" :( Wprowadziles nie prawidlowa opcje :( "<<endl;
            exit(0);
            break;
        }
    }


    if (screen) {
        SDL_FreeSurface(screen);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    return 0;
}
