#include <stdio.h>
#include <SDL2/SDL.h>

#include <time.h>
#include <stdlib.h>

const int WIDTH = 1200;
const int HEIGHT = 800;
const int CELL_SIZE = 20;
const int N_ROWS = HEIGHT / CELL_SIZE;
const int N_COLS = WIDTH / CELL_SIZE;

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;

enum direction {
    LEFT,
    UP,
    RIGHT,
    DOWN,
};

int direction = RIGHT;

void switchDirection(int newDirection){
    if((direction - newDirection + 4) % 2 == 1){
        direction = newDirection;
    }
}

struct snake {
    int x;
    int y;
    struct snake *next;
};


int fruitX = -1;
int fruitY = -1;

struct snake *head = NULL;
struct snake *tail = NULL;

void drawCellBorders(){
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
    for(int row = 0; row < N_ROWS; row++){
        for(int col = 0; col < N_COLS; col++){
            SDL_Rect rect;
            rect.x = col * CELL_SIZE;
            rect.y = row * CELL_SIZE;
            rect.w = CELL_SIZE;
            rect.h = CELL_SIZE;
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

struct snake *newSnake(int x, int y, struct snake *next){
    struct snake *_snake = (struct snake *) malloc(sizeof(struct snake));
    _snake->x = x;
    _snake->y = y;
    _snake->next = next;
    return _snake;
}

void drawSnakePart(int x, int y){
    SDL_Rect rect;
    rect.x = x * CELL_SIZE;
    rect.y = y * CELL_SIZE;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;
    SDL_RenderFillRect(renderer, &rect);
}

void drawSnake(){
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    struct snake *pointer = tail;
    while(pointer){
        drawSnakePart(pointer->x, pointer->y);
        pointer = pointer->next;
    }
}

void drawFruit(){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Rect rect;
    rect.x = fruitX * CELL_SIZE;
    rect.y = fruitY * CELL_SIZE;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;
    SDL_RenderFillRect(renderer, &rect);
}

int snakeEatingSelf(){
    struct snake *pointer = tail;
    while(pointer != head && pointer){
        if(pointer->x == head->x && pointer->y == head->y){
            return 1;
        }
        pointer = pointer->next;
    }

    return 0;
}

void updateSnake(){
    if(head == NULL){
        head = newSnake(N_COLS / 2, N_ROWS / 2, NULL);
        tail = head;
    }

    if(fruitX < 0){
        fruitX = rand() % N_COLS;
        fruitY = rand() % N_ROWS;
    }

    int newX = head->x;
    int newY = head->y;

    switch(direction){
        case UP:
            newY -= 1;
            break;
        case RIGHT:
            newX += 1;
            break;
        case DOWN:
            newY += 1;
            break;
        case LEFT:
            newX -= 1;
            break;
    }

    if(newX < 0) newX = N_COLS - 1;
    if(newY < 0) newY = N_ROWS - 1;
    if(newX >= N_COLS) newX = 0;
    if(newY >= N_ROWS) newY = 0;

    struct snake *newHead = newSnake(newX, newY, NULL);
    head->next = newHead;
    head = newHead;


    if(head->x == fruitX && head->y == fruitY) {
        fruitX = -1;
        fruitY = -1;
    }
    else {
        struct snake *tmp = tail;
        tail = tail->next;
        free(tmp);
    }

    if(snakeEatingSelf()){
        head = NULL;
        tail = NULL;
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Snake");

    srand(time(NULL));
    int running = 1;
    while (running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = 0;
            }
            if(event.type == SDL_KEYDOWN){
                int code = event.key.keysym.sym;
                if(code == SDLK_LEFT){
                    switchDirection(LEFT);
                }
                if(code == SDLK_UP){
                    switchDirection(UP);
                }
                if(code == SDLK_RIGHT){
                    switchDirection(RIGHT);
                }
                if(code == SDLK_DOWN){
                    switchDirection(DOWN);
                }
            }
        }
            

        updateSnake();

        drawFruit();
        drawSnake();
        drawCellBorders();

        SDL_RenderPresent(renderer);
        SDL_Delay(35);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
