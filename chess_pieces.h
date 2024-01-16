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
                cout << "Promote pawn to: ";
                cout << "Rook (r), Knight (n), Bishop (b), or Queen (q): ";
                string choice;
                cin >> choice;
                while (choice != "r" && choice != "n" && choice != "b" && choice != "q") {
                    cout << "Invalid choice. Please try again: ";
                    cin >> choice;
                }

                if (choice == "r") {
                    return new Rook(color, newPosition);
                } else if (choice == "n") {
                    return new Knight(color, newPosition);
                } else if (choice == "b") {
                    return new Bishop(color, newPosition);
                } else if (choice == "q") {
                    return new Queen(color, newPosition);
                }
            }

            return this;
        }
};
