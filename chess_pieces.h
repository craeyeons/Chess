#pragma once
#include <bits/stdc++.h>

#include "utils.h"

using namespace std;

class ChessPiece {
    public:
        string getSymbol() {
            return symbol;
        }

        int getColor() {
            return color;
        }

        string getPosition() {
            return position;
        }

        string getFilepath() {
            return filepath;
        }

        bool getBlockablility() {
            return blockable;
        }

        bool getHasMoved() {
            return hasMoved;
        }

        virtual bool isValidMove(string newPosition) = 0;
        virtual ChessPiece* move(string newPosition) = 0;
        virtual ~ChessPiece() {}

        friend ostream& operator<<(ostream& os, const ChessPiece& piece) {
            os << piece.symbol;
            return os;
        }

    protected:
        // 0 for white, 1 for black
        int color;
        string symbol;
        string position;
        string filepath;
        // Checks if the movement of this piece will be blocked
        bool blockable;
        bool hasMoved = false;
};

class Rook : public ChessPiece {
    public: 
        Rook(int color, string position) {
            string symbols[2] = {"♖", "♜"};
            this->color = color;
            this->symbol = symbols[color];
            this->position = position;
            this -> blockable = true;
            if (color == 0) {
                this->filepath = "piece_images/wr.png";
            } else {
                this->filepath = "piece_images/br.png";
            }
        }

        bool isValidMove(string newPosition) {
            if (newPosition[0] != position[0] && newPosition[1] != position[1]) {
                return false;
            }

            return true;
        }

        ChessPiece* move(string newPosition) {
            this->position = newPosition;
            this->hasMoved = true;
            return this;
        }
};

class Knight : public ChessPiece {
    public: 
        Knight(int color, string position) {
            string symbols[2] = {"♘", "♞"};
            this->color = color;
            this->symbol = symbols[color];
            this->position = position;
            this -> blockable = false;
            if (color == 0) {
                this->filepath = "piece_images/wn.png";
            } else {
                this->filepath = "piece_images/bn.png";
            }
        }

        bool isValidMove(string newPosition) {
            int x = abs(newPosition[0] - position[0]);
            int y = abs(newPosition[1] - position[1]);

            if (!((x == 1 && y == 2) || (x == 2 && y == 1))) {
                return false;
            }

            return true;
        }

        ChessPiece* move(string newPosition) {
            this->position = newPosition;
            this->hasMoved = true;
            return this;
        }
};

class Bishop : public ChessPiece {
    public: 
        Bishop(int color, string position) {
            string symbols[2] = {"♗", "♝"};
            this->color = color;
            this->symbol = symbols[color];
            this->position = position;
            this -> blockable = true;
            if (color == 0) {
                this->filepath = "piece_images/wb.png";
            } else {
                this->filepath = "piece_images/bb.png";
            }
        }

        bool isValidMove(string newPosition) {
            int x = abs(newPosition[0] - position[0]);
            int y = abs(newPosition[1] - position[1]);

            if (x != y) {
                return false;
            }

            return true;
        }

        ChessPiece* move(string newPosition) {
            this->position = newPosition;
            this->hasMoved = true;
            return this;
        }
};

class Queen : public ChessPiece {
    public: 
        Queen(int color, string position) {
            string symbols[2] = {"♕", "♛"};
            this->color = color;
            this->symbol = symbols[color];
            this->position = position;
            this -> blockable = true;
            if (color == 0) {
                this->filepath = "piece_images/wq.png";
            } else {
                this->filepath = "piece_images/bq.png";
            }
        }

        bool isValidMove(string newPosition) {
            int x = abs(newPosition[0] - position[0]);
            int y = abs(newPosition[1] - position[1]);

            if (!((x == 0 && y != 0) || (x != 0 && y == 0) || (x == y))) {
                return false;
            }

            return true;
        }

        ChessPiece* move(string newPosition) {
            this->position = newPosition;
            this->hasMoved = true;
            return this;
        }
};

class King : public ChessPiece {
    public: 
        King(int color, string position) {
            string symbols[2] = {"♔", "♚"};
            this->color = color;
            this->symbol = symbols[color];
            this->position = position;
            this -> blockable = true;
            if (color == 0) {
                this->filepath = "piece_images/wk.png";
            } else {
                this->filepath = "piece_images/bk.png";
            }
        }

        bool isValidMove(string newPosition) {
            int x = abs(newPosition[0] - position[0]);
            int y = abs(newPosition[1] - position[1]);

            if (x > 1 || y > 1) {
                return false;
            }

            return true;
        }

        ChessPiece* move(string newPosition) {
            this->position = newPosition;
            this->hasMoved = true;
            return this;
        }
};

class Pawn : public ChessPiece {
    public: 
        Pawn(int color, string position) {
            string symbols[2] = {"♙", "♟"};
            this->color = color;
            this->symbol = symbols[color];
            this->position = position;
            this -> blockable = true;
            if (color == 0) {
                this->filepath = "piece_images/wp.png";
            } else {
                this->filepath = "piece_images/bp.png";
            }
        }

        bool isValidMove(string newPosition) {
            // Check if it's the pawn's first move
            if (color == 0 && position[1] == '2' || color == 1 && position[1] == '7') {
                if (newPosition[0] == position[0] && abs(newPosition[1] - position[1]) <= 2) {
                    return true;
                }
            }

            // Check if it's an advancing move
            if (color == 0 && newPosition[1] - position[1] == 1 || color == 1 && newPosition[1] - position[1] == -1) {
                if (newPosition[0] == position[0]) {
                    return true;
                }
            }

            // Pawn captures are taken care of separately
            return false;
        }

        ChessPiece* move(string newPosition) {
            this->position = newPosition;
            this->hasMoved = true;
            
            // Pawn promotion
            if (color == 0 && newPosition[1] == '8' || color == 1 && newPosition[1] == '1') {
                return pieceSelection(color, newPosition);
            }

            return this;
        }

    private:
    
        // Creates a SDL window to select the piece to promote to
        ChessPiece* pieceSelection(int color, string newPosition) {
            SDL_Window* window = SDL_CreateWindow("Pawn Promotion",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 100, 0);
            SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

            // Background color of the window to green (same shade as chess board)
            SDL_SetRenderDrawColor(renderer, 0, 162, 73, 72);


            vector<SDL_Surface*> choices(4);

            if (color == 0) {
                choices[0] = IMG_Load("piece_images/wq.png");
                choices[1] = IMG_Load("piece_images/wr.png");
                choices[2] = IMG_Load("piece_images/wb.png");
                choices[3] = IMG_Load("piece_images/wn.png");
            } else {
                choices[0] = IMG_Load("piece_images/bq.png");
                choices[1] = IMG_Load("piece_images/br.png");
                choices[2] = IMG_Load("piece_images/bb.png");
                choices[3] = IMG_Load("piece_images/bn.png");
            }

            vector<SDL_Texture*> textures(4);
            for (int i = 0; i < 4; i++) {
                textures[i] = SDL_CreateTextureFromSurface(renderer, choices[i]);
            }

            SDL_Rect positions[4];
            for (int i = 0; i < 4; i++) {
                positions[i].x = i * 100;
                positions[i].y = 0;
                positions[i].w = 100;
                positions[i].h = 100;
            }

            SDL_RenderClear(renderer);

            for (int i = 0; i < 4; i++) {
                SDL_RenderCopy(renderer, textures[i], NULL, &positions[i]);
            }

            SDL_RenderPresent(renderer);

            bool quit = false;
            ChessPiece* promotionPiece;

            while (!quit) {
                SDL_Event event;
                SDL_WaitEvent(&event);

                switch (event.type) {
                    case SDL_QUIT:
                        quit = true;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        if (x <= 100) {
                            promotionPiece = new Queen(color, newPosition);
                        } else if (x <= 200) {
                            promotionPiece = new Rook(color, newPosition);
                        } else if (x <= 300) {
                            promotionPiece = new Bishop(color, newPosition);
                        } else if (x <= 400) {
                            promotionPiece = new Knight(color, newPosition);
                        }
                        quit = true;
                        break;
                }
            }

            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);

            return promotionPiece;
        }
};
