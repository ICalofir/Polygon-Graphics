#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

int N;
double S;
double xmax, ymax, k;

struct points
{
    double x, y;
};
points p[100];

void citire()
{
    cout << "Introduceti numarul de puncte ale poligonului: ";
    cin >> N;
    cout << "\n";
    cout << "Introduceti punctele poligonului in sens trigonometric:\n\n";
    xmax = -1000000.0;
    ymax = -1000000.0;
    for (int i = 1; i <= N; ++i)
    {
        cout << "x = ";
        cin >> p[i].x;
        cout << "y = ";
        cin >> p[i].y;
        cout << "\n";
        if (abs(p[i].x) > xmax) // aflu cel mai mare X introdus
            xmax = abs(p[i].x);
        if (abs(p[i].y) > ymax) // aflu cel mai mare Y introdus
            ymax = abs(p[i].y);
    }
    p[N + 1].x = p[1].x;
    p[N + 1].y = p[1].y;
}

void afisare() // aici trasez poligonul
{
    setlinestyle(0, 0, 1);
    for (int i = 1; i <= N; ++i)
        line(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y);
}

void translatie() // aici fac translatia, mut punctle,e pentru ca la mine centrul O este de coordonate O(xmax/2, ymax/2) si nu O(0, 0)
{
    for (int i = 1; i <= N + 1; ++i)
    {
        p[i].x = getmaxx() / 2 + p[i].x;
        p[i].y = getmaxy() / 2 - p[i].y;
    }
}

void marire() // maresc poligonul
{
    // k reprezinta numarul maxim cu care pot inmulti coordonatele fiecarui punct, astfel incat dupa marire, poligonul sa se incadreze in fereastra
    k = min((getmaxx() / 2 - 10) / xmax, (getmaxy() / 2 - 10) / ymax);

    for (int i = 1; i <= N + 1; ++i)
    {
        p[i].x *= k;
        p[i].y *= k;
    }
}

void micsorare() // micsorez poligonul
{
    // k reprezinta numarul la care coordonatele fiecarui punct pentru ca la final, poligonul sa se incadreze in fereastra
    k = max(xmax / (getmaxx() / 2 - 10), ymax / (getmaxy() / 2 - 10));
    xmax /= k;
    ymax /= k;

    for (int i = 1; i <= N + 1; ++i)
    {
        p[i].x /= k;
        p[i].y /= k;
    }
}

void fill(points A, points B)
{
    // aici trasez triunghiurile de la calcularea ariei, cu o linie rosie
    // punctul pe care mi l-am ales ca sa trasez fiecare triunghi este P(xmax/2, ymax/2), adica originea
    setcolor(RED);
    setlinestyle(1, 0, 1);
    line(A.x, A.y, getmaxx() / 2, getmaxy() / 2);
    line(B.x, B.y, getmaxx() / 2, getmaxy() / 2);
}

void procesare()
{
    // verific daca cel mai mare X introdus si cel mai mare Y introdus se incadreaza in fereastra
    // daca se incadreaza incerc sa mai maresc poligonul fara sa depasesc fereastra, marirea o fac in functia marire()
    if (xmax <= getmaxx() / 2 - 10 && ymax <= getmaxy() / 2 - 10)
    {
        marire();
        translatie();
        afisare();
        for (int i = 1; i <= N; ++i)
            fill(p[i], p[i + 1]);
    }
    // daca nu se incadreaza intai micsorez poligonul in functia micsorare(), apoi il maresc cat pot in functia marire() astfel incat sa nu depaseasca fereastra
    else
    {
        micsorare();
        marire();
        translatie();
        afisare();
        for (int i = 1; i <= N; ++i)
            fill(p[i], p[i + 1]);
    }
}

void arie() // formula de mai jos, unde calculez aria, reprezinta aria cu semn a poligonului
{
    for (int i = 1; i <= N; ++i)
    {
        S += (p[i].x * p[i + 1].y - p[i + 1].x * p[i].y) / 2;

        int j = i + 1;
        if (i == N)
            j = 1;

        // afisez semnul fiecarui triunghi
        if (p[i].x * p[i + 1].y - p[i + 1].x * p[i].y > 0)
            cout << "P" << i << "(" << p[i].x << ", " << p[i].y << ") P" << j << "(" << p[j].x << ", " << p[j].y << ") P(0, 0) : +\n";
        else
            cout << "P" << i << "(" << p[i].x << ", " << p[i].y << ") P" << j << "(" << p[j].x << ", " << p[j].y << ") P(0, 0) : -\n";
    }
    S = abs(S); // afisez aria
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TC\\BGI");

    setlinestyle(1, 0, 1); // setez stilul liniei
    // trasez coordonatele OX si OY
    line(getmaxx() / 2, 0, getmaxx() / 2, getmaxy());
    line(0, getmaxy() / 2, getmaxx(), getmaxy() / 2);

    citire();
    arie();
    procesare();

    cout << "\n";
    cout << "Aria poligonului este: " << S;

    getch();
    closegraph();
    return 0;
}
