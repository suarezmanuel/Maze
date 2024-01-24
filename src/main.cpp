#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <windows.h>

#define SCREENWIDTH 600
#define SCREENHEIGHT 600
#define BLOCKWIDTH 30
#define BLOCKHEIGHT 30
#define BLOCKSONSCREENWIDTH SCREENWIDTH/BLOCKWIDTH 
#define BLOCKSONSCREENHEIGHT SCREENHEIGHT/BLOCKHEIGHT

bool board [BLOCKSONSCREENWIDTH][BLOCKSONSCREENHEIGHT] = {0};


enum direction { up, right, down, left };
// int directions [4] = {-1, 1, 1, -1};

class node {
    int x, y;

public:

    node (int x, int y) {
        this->x = x;
        this->y = y;
    }

    int operator[] (const int i) const {
        if (i%2 == 0) { return x; }
        return y;
    }

    bool operator== (const node& other) const {
        return this->x == other.x && this->y == other.y;
    }

    bool set_visited () {
        board[x][y] = true;
    }

    bool check_visited () {
        return board[x][y];
    }

    bool check_square_in_direction (direction d) {
        switch (d) {
            case up:
                return 
                        x-1 >= 0 && x+1 < BLOCKSONSCREENWIDTH && y-2 >= 0 && y-1 < BLOCKSONSCREENHEIGHT
                        && !board[x][y-1] && !board[x][y-2]
                        && !board[x-1][y-1] && !board[x-1][y-2]
                        && !board[x+1][y-1] && !board[x+1][y-2];
                break;
            case right:
                return 
                        // not checking that x-1 exists, as we shouldn't be able to get to it not existing
                        x >= 0 && x+2 < BLOCKSONSCREENWIDTH && y-1 >= 0 && y+1 < BLOCKSONSCREENHEIGHT
                        && !board[x+1][y] && !board[x+2][y] // right
                        &&   !board[x+1][y-1] && !board[x+2][y-1] // right up
                        &&   !board[x+1][y+1] && !board[x+2][y+1]; // right down
                break;
            case down:
                return
                        x-1 >= 0 && x+1 < BLOCKSONSCREENWIDTH && y >= 0 && y+2 < BLOCKSONSCREENHEIGHT
                        && !board[x][y+1] && !board[x][y+2]
                        && !board[x-1][y+1] && !board[x-1][y+2]
                        && !board[x+1][y+1] && !board[x+1][y+2];
                break;
            case left:
                    return 
                        x-2 >= 0 && x < BLOCKSONSCREENWIDTH && y-1 >= 0 && y+1 < BLOCKSONSCREENHEIGHT
                        &&   !board[x-1][y] && !board[x-2][y]
                        &&   !board[x-1][y-1] && !board[x-2][y-1] 
                        &&   !board[x-1][y+1] && !board[x-2][y+1];
                break;
            default:
                return false;
                break;
        }
    }

    void get_possible_neighbors (std::vector<node>& neighbors) {
        // we want diagonals to be good
        // we want 2xdiagonals to be good
        // we want forward to be good
        // we want 2xforward to be good

        // do I have to check both ways? I don't think so
        
        node* n = NULL;
        
        // new uses malloc ! partey !
        if (check_square_in_direction(up))     { n = new node(x, y-1); neighbors.push_back(*n); }
        if (check_square_in_direction(right))  { n = new node(x+1, y); neighbors.push_back(*n); }
        if (check_square_in_direction(down))   { n = new node(x, y+1); neighbors.push_back(*n); }
        if (check_square_in_direction(left))   { n = new node(x-1, y); neighbors.push_back(*n); }
    }
};

std::vector<node> path_taken_till_now;
std::vector<node> solution;
node start_of_maze {0,1};
node end_of_maze {BLOCKSONSCREENWIDTH-1, BLOCKSONSCREENHEIGHT-1};
// if white, all prev are blue
    // set white   

    // everyone will have a list from him onward
    // the list is only viable when you return

    // append the return of dfs
    // if curr white (without havin set)
    //

    // when going forward we append to prevs list
    // when going backwards we remove

    // we talking about list of all graph

    // for each neighbor
        // if can turn (not discovered + soft-turn) 
            // dfs

void drawBoard (SDL_Renderer *&renderer);

void dfs (node curr, SDL_Renderer* renderer) {

    
    Sleep(3);
        // != is not same as !(==), I'd need another overload to use the former
    // but im lazy
    // would finish without this
    // if (path_taken_till_now.size() == 0 && !(curr == start_of_maze)) {
    //     return;
    // }
    if (curr == end_of_maze) {
        solution = path_taken_till_now;
    }
    curr.set_visited();
    path_taken_till_now.push_back(curr);

    drawBoard(renderer);
    SDL_RenderPresent(renderer);

    
    // for each of curr's available neighbors
    std::vector<node> neighbors;
    // for (node n : neighbors) {
    //     std::cout << n[0] << " " << n[1] << " --  ";
    // }
    // std::cout << std::endl;

    // look out! A dangerous default value is roaming around the land...
    node neighbor{0,0};
    int size;
    curr.get_possible_neighbors(neighbors);
    size = neighbors.size();

    while (size > 0) {

        // size changes throughout
        int index = std::rand() % size;
        // go randomly to either neighbor
        // hope this has a normal value
        neighbor = neighbors[index];

        dfs(neighbor, renderer);

        // erase stuck from path
        path_taken_till_now.pop_back();
        // // deletes the neighbor we just got, goes till neighbors[index]
        // neighbors.erase(std::next(neighbors.begin(), index-1));

        neighbors.empty();
        // swap with an empty vector, clears memory
        std::vector<node>().swap(neighbors);
        curr.get_possible_neighbors(neighbors);
        size = neighbors.size();
    }
    std::cout << "no neighboringho" << std::endl;
}   
        

void draw_by_index (SDL_Renderer *&renderer, int i, int j) {
    SDL_Rect rectToFill; rectToFill.h = BLOCKHEIGHT; rectToFill.w = BLOCKWIDTH;
    rectToFill.x = i*BLOCKWIDTH; rectToFill.y = j*BLOCKHEIGHT;  

    SDL_Color drawColor;
    if (board[i][j]) {drawColor = {255,255,255,255};}
    else {drawColor = {0,0,0,255};}

    SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
    SDL_RenderFillRect(renderer, &rectToFill);
}
    

void drawBoard (SDL_Renderer *&renderer) {
    for (int i = 0; i < BLOCKSONSCREENWIDTH; i++)
    {
        for (int j = 0; j < BLOCKSONSCREENHEIGHT; j++)
        {
            draw_by_index(renderer, i, j);
        }
    }
}

void draw_path (SDL_Renderer *&renderer, std::vector<node>& path) {
    for (node& n : path) {
        draw_by_index(renderer, n[0], n[1]);
    }
}


void drawGrid (SDL_Renderer *renderer, SDL_Color color) {

    enum Coords {
        X, Y
    };

    int p1 [2] = {0,0};
    int p2 [2] = {0,0};

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // ------ draw rows -----------
    p1[X] = 0;
    p2[X] = SCREENWIDTH;

    for (size_t i = 0; i < BLOCKSONSCREENHEIGHT; i++)
    {
        p1[Y] = p2[Y] = i*BLOCKHEIGHT;

        SDL_RenderDrawLine(renderer, p1[X], p1[Y], p2[X], p2[Y]);
    }
    

    // ------ draw cols -----------
    p1[Y] = 0;
    p2[Y] = SCREENHEIGHT;
    
    for (size_t i = 0; i < BLOCKSONSCREENWIDTH; i++)
    {
        p1[X] = p2[X] = i*BLOCKWIDTH;

        SDL_RenderDrawLine(renderer, p1[X], p1[Y], p2[X], p2[Y]);
    }
}

int main (int argc, char* argv[]) {
    
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer(SCREENWIDTH, SCREENHEIGHT, 0, &window, &renderer);   

    if (window == NULL) {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    }

    bool running = true;
    SDL_Event windowEvent;

    // a ptr to a const Uint8, has an array of all the states that now hold
    float tickTime = 1000;
    Sleep(tickTime);
    
    
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        dfs (start_of_maze, renderer);
        // std::cout << "hello";
        // set color to be white
        
        drawBoard(renderer);

        SDL_SetRenderDrawColor(renderer, 0,0,255,255);
        draw_path(renderer, solution);
        // draw grid on screen
        // SDL_Color gridColor{70,70,40,255};
        // drawGrid(renderer, gridColor);
        SDL_RenderPresent(renderer);

    while (running) {

        if (SDL_PollEvent(&windowEvent)) {
            
            switch (windowEvent.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}