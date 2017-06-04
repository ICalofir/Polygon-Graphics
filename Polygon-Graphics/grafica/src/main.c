#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

int N;
double S;
double xmax, ymax, k;

struct points {
    double x, y;
};
struct points p[100];

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

int init_sdl();
void close_sdl();
void sdl_draw_line(double lt_x, double lt_y, double rt_x, double rt_y);

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

int init_sdl() {
  int success = 1;

  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = 0;
  } else {
    gWindow = SDL_CreateWindow("Polygon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = 0;
    } else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if(gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = 0;
      } else {
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      }
    }
  }

  return success;
}

void close_sdl() {
  SDL_DestroyRenderer(gRenderer);
  gRenderer = NULL;
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  SDL_Quit();
}

void sdl_draw_line(double lt_x, double lt_y, double rt_x, double rt_y) {
  SDL_RenderDrawLine(gRenderer, lt_x, lt_y, rt_x, rt_y);
}

double minim(double a, double b) {
  if (a < b) {
    return a;
  }

  return b;
}

double maxim(double a, double b) {
  if (a > b) {
    return a;
  }

  return b;
}

void citire() {
  printf("Introduceti numarul de puncte ale poligonului: ");
  scanf("%d", &N);
  printf("\nIntroduceti punctele poligonului in sens trigonometric:\n\n");
  xmax = -1000000.0;
  ymax = -1000000.0;
  for (int i = 1; i <= N; ++i) {
    printf("x = ");
    scanf("%lf", &p[i].x);
    printf("y = ");
    scanf("%lf", &p[i].y);
    printf("\n");
    if (fabs(p[i].x) > xmax) // aflu cel mai mare X introdus
      xmax = fabs(p[i].x);
    if (fabs(p[i].y) > ymax) // aflu cel mai mare Y introdus
      ymax = fabs(p[i].y);
  }
  p[N + 1].x = p[1].x;
  p[N + 1].y = p[1].y;
}

void afisare() { // aici trasez poligonul
  SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
  for (int i = 1; i <= N; ++i) {
    sdl_draw_line(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y);
  }
}

void translatie() { // aici fac translatia, mut punctle,e pentru ca la mine centrul O este de coordonate O(xmax/2, ymax/2) si nu O(0, 0)
  for (int i = 1; i <= N + 1; ++i) {
    p[i].x = SCREEN_WIDTH / 2 + p[i].x;
    p[i].y = SCREEN_HEIGHT / 2 - p[i].y;
  }
}

void marire() { // maresc poligonul
  // k reprezinta numarul maxim cu care pot inmulti coordonatele fiecarui punct, astfel incat dupa marire, poligonul sa se incadreze in fereastra
  k = minim((SCREEN_WIDTH / 2 - 10) / xmax, (SCREEN_HEIGHT / 2 - 10) / ymax);

  for (int i = 1; i <= N + 1; ++i) {
    p[i].x *= k;
    p[i].y *= k;
  }
}

void micsorare() { // micsorez poligonul
  // k reprezinta numarul la care coordonatele fiecarui punct pentru ca la final, poligonul sa se incadreze in fereastra
  k = maxim(xmax / (SCREEN_WIDTH / 2 - 10), ymax / (SCREEN_HEIGHT / 2 - 10));
  xmax /= k;
  ymax /= k;

  for (int i = 1; i <= N + 1; ++i) {
    p[i].x /= k;
    p[i].y /= k;
  }
}

void fill(struct points A, struct points B) {
  // aici trasez triunghiurile de la calcularea ariei, cu o linie rosie
  // punctul pe care mi l-am ales ca sa trasez fiecare triunghi este P(xmax/2, ymax/2), adica originea
  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
  sdl_draw_line(A.x, A.y, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  sdl_draw_line(B.x, B.y, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void procesare() {
  // verific daca cel mai mare X introdus si cel mai mare Y introdus se incadreaza in fereastra
  // daca se incadreaza incerc sa mai maresc poligonul fara sa depasesc fereastra, marirea o fac in functia marire()
  if (xmax <= SCREEN_WIDTH / 2 - 10 && ymax <= SCREEN_HEIGHT / 2 - 10) {
    marire();
    translatie();
  } else {
  // daca nu se incadreaza intai micsorez poligonul in functia micsorare(), apoi il maresc cat pot in functia marire() astfel incat sa nu depaseasca fereastra
    micsorare();
    marire();
    translatie();
  }
}

void arie() { // formula de mai jos, unde calculez aria, reprezinta aria cu semn a poligonului
  for (int i = 1; i <= N; ++i) {
    S += (p[i].x * p[i + 1].y - p[i + 1].x * p[i].y) / 2;

    int j = i + 1;
    if (i == N)
      j = 1;

    // afisez semnul fiecarui triunghi
    if (p[i].x * p[i + 1].y - p[i + 1].x * p[i].y > 0)
      printf("P%d(%lf, %lf) P%d(%lf, %lf) P(0, 0) : +\n", i, p[i].x, p[i].y, j, p[j].x, p[j].y);
    else
      printf("P%d(%lf, %lf) P%d(%lf, %lf) P(0, 0) : -\n", i, p[i].x, p[i].y, j, p[j].x, p[j].y);
  }
  S = fabs(S); // afisez aria
}

int main() {
  if(!init_sdl()) {
    printf("Failed to initialize!\n");
  } else {
    citire();
    arie();
    procesare();

    printf("\nAria poligonului este %lf\n", S);

    int quit = 0;
    SDL_Event e;

    while(!quit) {
      while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
          quit = 1;
        }
      }

      SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderClear(gRenderer);

      SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
      // trasez coordonatele OX si OY
      sdl_draw_line(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);
      sdl_draw_line(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

      afisare();
      for (int i = 1; i <= N; ++i)
        fill(p[i], p[i + 1]);

      SDL_RenderPresent(gRenderer);
    }
  }

  close_sdl();

  return 0;
}
