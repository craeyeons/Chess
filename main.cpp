#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "chess_board.h"

using namespace std;

const int BOARD_WIDTH = 480;
const int BOARD_HEIGHT = 480;
const int SCREEN_HEIGHT = 640;
const int SQUARE_SIZE = BOARD_WIDTH / 8;

pair<int, int> convertToPixelPosition(int row, int col);
string pixelToBoardNotation(int x, int y);

int main() {
    Board board;

    bool quit = false;
    SDL_Event event;
 
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow("Chess",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_WIDTH, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface* chessboard = IMG_Load("piece_images/board.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, chessboard);

    int turn = 0;

    // Two clicks to make a move
    string source, target;
    int clicks = 0;
    vector<vector<ChessPiece*>> boardstate = board.getBoard();
 
    while (!quit) {
        SDL_WaitEvent(&event);
 
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            int x, y;
            SDL_GetMouseState(&x, &y);
            string boardCoords = pixelToBoardNotation(x, y);

            if (clicks == 0) {
                // The first click should only be registered if it is the player's turn
                if (boardstate[boardCoords[1] - '1'][boardCoords[0] - 'a'] == nullptr ||
                    boardstate[boardCoords[1] - '1'][boardCoords[0] - 'a']->getColor() != turn) {
                    break;
                }
                source = boardCoords;
                clicks++;
                break;
            } 
            
            // If on the 2nd click, the selected piece is of the same colour, override it
            if (boardstate[boardCoords[1] - '1'][boardCoords[0] - 'a'] != nullptr &&
                boardstate[boardCoords[1] - '1'][boardCoords[0] - 'a']->getColor() == turn) {
                source = boardCoords;
                break;
            }

            // Modify string to castling notation
            if (source == "e1" && boardCoords == "g1" &&
                board.getBoard()[0][4]->getSymbol() == "♔" && board.getBoard()[0][7]->getSymbol() == "♖") {
                source = "O-O";
                target = "";
            } else if (source == "e1" && boardCoords == "c1" &&
                board.getBoard()[0][4]->getSymbol() == "♔" && board.getBoard()[0][0]->getSymbol() == "♖") {
                source = "O-O-O";
                target = "";
            } else if (source == "e8" && boardCoords == "g8" &&
                board.getBoard()[7][4]->getSymbol() == "♚" && board.getBoard()[7][7]->getSymbol() == "♜") {
                source = "O-O";
                target = "";
            } else if (source == "e8" && boardCoords == "c8" &&
                board.getBoard()[7][4]->getSymbol() == "♚" && board.getBoard()[7][0]->getSymbol() == "♜") {
                source = "O-O-O";
                target = "";
            }
            
            // Make move
            target = boardCoords;
            try {
                board.move(source, target, turn);
                turn = (turn + 1) % 2;
                board.printBoard();
            } catch (ChessException& e){
                cout << e.what() << endl;
                cout << "Please try again." << endl;
                continue;
            }
            clicks = 0;
            break;
        }

        SDL_Rect dstrect = { 0, 0, BOARD_WIDTH, BOARD_HEIGHT};
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        boardstate = board.getBoard();
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (boardstate[i][j] != nullptr) {
                    SDL_Surface* pieceSurface = IMG_Load(boardstate[i][j]->getFilepath().c_str());
                    SDL_Texture* pieceTexture = SDL_CreateTextureFromSurface(renderer, pieceSurface);
                    pair<int, int> pixelCoords = convertToPixelPosition(i, j);
                    SDL_Rect pieceRect = {pixelCoords.second, pixelCoords.first, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, pieceTexture, NULL, &pieceRect);
                }
            }
        }

        // Highlight selected square
        if (clicks == 1) {
            pair<int, int> pixelCoords = convertToPixelPosition(source[1] - '1', source[0] - 'a');
            SDL_Rect rect = {pixelCoords.second, pixelCoords.first, SQUARE_SIZE, SQUARE_SIZE};
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyTexture(texture);
	SDL_FreeSurface(chessboard);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
 
    SDL_Quit();
}

pair<int, int> convertToPixelPosition(int row, int col) {
    return make_pair((BOARD_HEIGHT - SQUARE_SIZE) - row * SQUARE_SIZE, col * SQUARE_SIZE);
}

// Get output in the format of "a1", "b2", etc.
string pixelToBoardNotation(int x, int y) {
    string boardNotation = "";
    boardNotation += (char) (x / SQUARE_SIZE + 'a');
    boardNotation += (char) (8 - y / SQUARE_SIZE + '0');
    return boardNotation;
}