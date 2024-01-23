#pragma once
#include <bits/stdc++.h>

#include "chess_exceptions.h"
#include "chess_pieces.h"
#include "utils.h"

using namespace std;

class Board {
    private:
        vector<vector<ChessPiece*> > board;
        pair<string, string> lastMove;

    public:
        Board() {
            board = vector<vector<ChessPiece*> >(8, vector<ChessPiece*>(8, nullptr));

            // White Pieces
            addPiece(new Rook(0, "a1"), 0, 0);
            addPiece(new Knight(0, "b1"), 0, 1);
            addPiece(new Bishop(0, "c1"), 0, 2);
            addPiece(new Queen(0, "d1"), 0, 3);
            addPiece(new King(0, "e1"), 0, 4);
            addPiece(new Bishop(0, "f1"), 0, 5);
            addPiece(new Knight(0, "g1"), 0, 6);
            addPiece(new Rook(0, "h1"), 0, 7);
            addPiece(new Pawn(0, "a2"), 1, 0);
            addPiece(new Pawn(0, "b2"), 1, 1);
            addPiece(new Pawn(0, "c2"), 1, 2);
            addPiece(new Pawn(0, "d2"), 1, 3);
            addPiece(new Pawn(0, "e2"), 1, 4);
            addPiece(new Pawn(0, "f2"), 1, 5);
            addPiece(new Pawn(0, "g2"), 1, 6);  
            addPiece(new Pawn(0, "h2"), 1, 7);

            // Black Pieces
            addPiece(new Rook(1, "a8"), 7, 0);
            addPiece(new Knight(1, "b8"), 7, 1);
            addPiece(new Bishop(1, "c8"), 7, 2);
            addPiece(new Queen(1, "d8"), 7, 3);
            addPiece(new King(1, "e8"), 7, 4);
            addPiece(new Bishop(1, "f8"), 7, 5);
            addPiece(new Knight(1, "g8"), 7, 6);
            addPiece(new Rook(1, "h8"), 7, 7);
            addPiece(new Pawn(1, "a7"), 6, 0);
            addPiece(new Pawn(1, "b7"), 6, 1);
            addPiece(new Pawn(1, "c7"), 6, 2);
            addPiece(new Pawn(1, "d7"), 6, 3);
            addPiece(new Pawn(1, "e7"), 6, 4);
            addPiece(new Pawn(1, "f7"), 6, 5);
            addPiece(new Pawn(1, "g7"), 6, 6);
            addPiece(new Pawn(1, "h7"), 6, 7);
        }

        void printBoard() {
            cout << "\n";

            for (int i = 7; i >= 0; i--) {
                cout << i + 1 << " ";
                for (int j = 0; j < 8; j++) {
                    if (board[i][j] == nullptr) {
                        cout << "  ";
                    } else {
                        cout << board[i][j]->getSymbol() << " ";
                    }
                }
                cout << endl;
            }

            cout << "  ";
            for (int i = 0; i < 8; i++) {
                cout << (char)('a' + i) << " ";
            }
            cout << "\n";
        }

        vector<vector<ChessPiece*> >& getBoard() {
            return board;
        }

        void move(string source, string target, int turn) {
            if (source == "O-O") {
                if (isValidKingCastle(turn)) {
                    kingCastle(turn);
                    return;
                } else {
                    throw InvalidCastleException();
                }
            }

            if (source == "O-O-O") {
                if (isValidQueenCastle(turn)) {
                    queenCastle(turn);
                    return;
                } else {
                    throw InvalidCastleException();
                }
            }

            if (isInvalidSquare(source) || isInvalidSquare(target)) {
                throw InvalidSquareException();
            }

            if (source == target) {
                throw InvalidMoveException(source, target);
            }

            // Note that chess notation is in the format of <column><row>
            // while the board is in the format of <row><column>
            // convertPosition converts the chess notation to the board notation
            pair<int, int> sourcePosition = convertPoisition(source);
            pair<int, int> targetPosition = convertPoisition(target);

            ChessPiece* movingPiece = board[sourcePosition.first][sourcePosition.second];
            ChessPiece* targetPiece = board[targetPosition.first][targetPosition.second];

            if (movingPiece == nullptr) {
                throw NoPieceException(source);
            }

            if (movingPiece->getColor() != turn) {
                throw WrongColorException(source, turn);
            }

            // Special moves guard clauses
            if (isEnPassant(movingPiece, target, turn)) {
                 return enPassantCapture(movingPiece, target);
            }

            if (isPawnCapture(movingPiece, target)) {
                return pawnCapture(movingPiece, target);
            }
            
            // Normal moves
            if (!movingPiece->isValidMove(target)) {
                throw InvalidMoveException(source, target);
            }

            if (isBlocked(movingPiece, targetPosition)) {
                throw PieceBlockedException(source, target);
            }

            if (!isValidCapture(movingPiece, targetPiece)) {
                throw InvalidCaptureException(source, target);
            }

            // Check if the move will put the player's king in check
            if (!checkSelfKingSafety(source, target, turn)) {
                throw KingInCheckException();
            }

            // Current Piece is removed from the board
            removePiece(sourcePosition.first, sourcePosition.second);
            // If there is a piece on the target position, it is removed
            removePiece(targetPosition.first, targetPosition.second);

            // The piece is moved to the target position
            // Returning a piece to consider the case of pawn promotion
            ChessPiece* newPiece = movingPiece->move(target);
            addPiece(newPiece, targetPosition.first, targetPosition.second);

            // Delete the old piece in the case of pawn promotion
            if (movingPiece != newPiece) {
                delete movingPiece;
            }

            lastMove = make_pair(source, target);
        }

        bool isCheckmate(int turn) {
            if (generateAllValidMoves(turn).size() == 0 && isAttacked(convertPosition(getKingCoordinates(turn)), turn)) {
                return true;
            }

            return false;
        }

        bool isStalemate(int turn) {
            if (generateAllValidMoves(turn).size() == 0 && !isAttacked(convertPosition(getKingCoordinates(turn)), turn)) {
                return true;
            }

            return false;
        }

        vector<pair<string, string>> generateAllValidMoves(int turn) {
            vector<pair<string, string>> validMoves;
            vector<vector<ChessPiece*> > boardstate = getBoard();

            // Iterate through the board
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    if (boardstate[i][j] == nullptr) {
                        continue;
                    }

                    if (boardstate[i][j]->getColor() != turn) {
                        continue;
                    }

                    string source = convertPosition(make_pair(i, j));
                    vector<string> pieceValidMoves = generateValidMoves(source, turn);

                    for (string target : pieceValidMoves) {
                        validMoves.push_back(make_pair(source, target));
                    }
                }
            }

            return validMoves;
        }

        vector<string> generateValidMoves(string source, int turn) {
            vector<string> validMoves;
            pair<int, int> sourcePosition = convertPoisition(source);

            ChessPiece* movingPiece = board[sourcePosition.first][sourcePosition.second];

            // Iterate through the board
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    string target = convertPosition(make_pair(i, j));
                    if (target == source) {
                        continue;
                    }

                    if (checkMove(source, target, turn)) {
                        validMoves.push_back(target);
                    }
                }
            }

            return validMoves;
        }

        bool isValidKingCastle(int turn) {
            if (turn == 0) {
                if (board[0][4] == nullptr || board[0][7] == nullptr) {
                    return false;
                }

                if (board[0][4]->getSymbol() != "♔" && board[0][7]->getSymbol() != "♖") {
                    return false;
                }

                if (board[0][4]->getHasMoved() != false && board[0][7]->getHasMoved() != false) {
                    return false;
                }

                if (board[0][5] != nullptr || board[0][6] != nullptr) {
                    return false;
                }

                if (isAttacked("e1", turn)) {
                    return false;
                }

                if (isAttacked("f1", turn)) {
                    return false;
                }

                if (isAttacked("g1", turn)) {
                    return false;
                }
            }

            if (turn == 1) {
                if (board[7][4] == nullptr || board[7][7] == nullptr) {
                    return false;
                }

                if (board[7][4]->getSymbol() != "♚" && board[7][7]->getSymbol() != "♜") {
                    return false;
                }

                if (board[7][4]->getHasMoved() != false && board[7][7]->getHasMoved() != false) {
                    return false;
                }

                if (board[7][5] != nullptr || board[7][6] != nullptr) {
                    return false;
                }

                if (isAttacked("e8", turn)) {
                    return false;
                }

                if (isAttacked("f8", turn)) {
                    return false;
                }

                if (isAttacked("g8", turn)) {
                    return false;
                }
            }

            return true;
        }

        bool isValidQueenCastle(int turn) {
            if (turn == 0) {
                if (board[0][4] == nullptr || board[0][0] == nullptr) {
                    return false;
                }

                if (board[0][4]->getSymbol() != "♔" && board[0][0]->getSymbol() != "♖") {
                    return false;
                }

                if (board[0][4]->getHasMoved() != false && board[0][0]->getHasMoved() != false) {
                    return false;
                }

                if (board[0][1] != nullptr || board[0][2] != nullptr || board[0][3] != nullptr) {
                    return false;
                }

                if (isAttacked("e1", turn)) {
                    return false;
                }

                if (isAttacked("d1", turn)) {
                    return false;
                }

                if (isAttacked("c1", turn)) {
                    return false;
                }

                if (isAttacked("b1", turn)) {
                    return false;
                }
            }

            if (turn == 1) {
                if (board[7][4] == nullptr || board[7][0] == nullptr) {
                    return false;
                }

                if (board[7][4]->getSymbol() != "♚" && board[7][0]->getSymbol() != "♜") {
                    return false;
                }

                if (board[7][4]->getHasMoved() != false && board[7][0]->getHasMoved() != false) {
                    return false;
                }

                if (board[7][1] != nullptr || board[7][2] != nullptr || board[7][3] != nullptr) {
                    return false;
                }

                if (isAttacked("e8", turn)) {
                    return false;
                }

                if (isAttacked("d8", turn)) {
                    return false;
                }

                if (isAttacked("c8", turn)) {
                    return false;
                }

                if (isAttacked("b8", turn)) {
                    return false;
                }
            }

            return true;
        }

    private:
        bool checkSelfKingSafety(string source, string target, int turn) {
            pair<int, int> sourcePosition = convertPoisition(source);
            pair<int, int> targetPosition = convertPoisition(target);

            ChessPiece* movingPiece = board[sourcePosition.first][sourcePosition.second];
            ChessPiece* targetPiece = board[targetPosition.first][targetPosition.second];

            // Current Piece is removed from the board
            removePiece(sourcePosition.first, sourcePosition.second);
            // If there is a piece on the target position, it is removed
            removePiece(targetPosition.first, targetPosition.second);

            // This new piece only serves to block movement, its choice is not important
            // UNLESS it is the king
            ChessPiece* newPiece;
            if (movingPiece->getSymbol() == "♔" || movingPiece->getSymbol() == "♚") {
                newPiece = new King(turn, target);
            } else {
                newPiece = new Pawn(turn, target);
            }
            addPiece(newPiece, targetPosition.first, targetPosition.second);

            // Check if the king is in check
            pair<int, int> kingCoordinates = getKingCoordinates(turn);
            
            if (isAttacked(convertPosition(kingCoordinates), turn)) {
                // Undo the move
                removePiece(targetPosition.first, targetPosition.second);
                addPiece(movingPiece, sourcePosition.first, sourcePosition.second);
                addPiece(targetPiece, targetPosition.first, targetPosition.second);
                return false;
            }

            // Undo the move
            removePiece(targetPosition.first, targetPosition.second);
            addPiece(movingPiece, sourcePosition.first, sourcePosition.second);
            addPiece(targetPiece, targetPosition.first, targetPosition.second);
            return true;
        }

        bool isEnPassant(ChessPiece* piece, string target, int turn) {
            // Check if the moving piece is a pawn
            if (piece->getSymbol() != "♙" && piece->getSymbol() != "♟") {
                return false;
            }

            // Check if we are moving diagonally
            if (abs(target[0] - piece->getPosition()[0]) != 1) {
                return false;
            }

            if (piece->getColor() == 0 && target[1] - piece->getPosition()[1] != 1) {
                return false;
            }

            // Check if the last move was moving two squares
            if (abs(lastMove.second[1] - lastMove.first[1]) != 2) {
                return false;
            }

            // Check if the last move was a pawn move
            ChessPiece* lastPiece = board[convertPoisition(lastMove.second).first][convertPoisition(lastMove.second).second];
            if (lastPiece->getSymbol() != "♙" && lastPiece->getSymbol() != "♟") {
                return false;
            }

            // Do not have to check that the target square is empty
            // because the opposing pawn just moved passed it

            return true;
        }

        void enPassantCapture(ChessPiece* piece, string target) {
            pair<int, int> source = convertPoisition(piece->getPosition());
            pair<int, int> targetPosition = convertPoisition(target);

            // Remove the opposing pawn
            if (piece->getColor() == 0) {
                removePiece(targetPosition.first - 1, targetPosition.second);
            } else {
                removePiece(targetPosition.first + 1, targetPosition.second);
            }
            // Remove the current pawn
            removePiece(source.first, source.second);
            // Move the current pawn to the target position
            ChessPiece* newPiece = piece->move(target);
            addPiece(newPiece, targetPosition.first, targetPosition.second);

            lastMove = make_pair(piece->getPosition(), target);
        }

        bool isPawnCapture(ChessPiece* piece, string target) {
            if (piece->getSymbol() != "♙" && piece->getSymbol() != "♟") {
                return false;
            }

            if (abs(target[0] - piece->getPosition()[0]) != 1) {
                return false;
            }

            if (piece->getColor() == 0 && target[1] - piece->getPosition()[1] != 1) {
                return false;
            }

            if (piece->getColor() == 1 && target[1] - piece->getPosition()[1] != -1) {
                return false;
            }

            // Piece exists on the target square
            if (board[convertPoisition(target).first][convertPoisition(target).second] == nullptr) {
                return false;
            }

            return true;
        }

        void pawnCapture(ChessPiece* piece, string target) {
            pair<int, int> source = convertPoisition(piece->getPosition());
            pair<int, int> targetPosition = convertPoisition(target);

            // Remove the opposing pawn
            removePiece(targetPosition.first, targetPosition.second);
            // Remove the current pawn
            removePiece(source.first, source.second);
            // Move the current pawn to the target position
            ChessPiece* newPiece = piece->move(target);
            addPiece(newPiece, targetPosition.first, targetPosition.second);

            lastMove = make_pair(piece->getPosition(), target);
        }

        bool isBlocked(ChessPiece* piece, pair<int, int> target) {
            if (!piece->getBlockablility()) {
                return false;
            }

            // For an advancing pawn
            // it gets blocked if there is a piece in front of it
            // captures are taken care of separetly in a guard clause
            // so we only need to check the columns
            if (piece->getSymbol() == "♙" || piece->getSymbol() == "♟") {
                if (piece->getColor() == 0) {
                    for (int i = convertPoisition(piece->getPosition()).first + 1; i <= target.first; i++) {
                        if (board[i][target.second] != nullptr) {
                            return true;
                        }
                    }
                }

                if (piece->getColor() == 1) {
                    for (int i = convertPoisition(piece->getPosition()).first - 1; i >= target.first; i--) {
                        if (board[i][target.second] != nullptr) {
                            return true;
                        }
                    }
                }

                return false;
            }

            pair<int, int> source = convertPoisition(piece->getPosition());

            int rowIncrement = (source.first < target.first) 
                             ? 1 
                             : (source.first == target.first) 
                             ? 0 
                             : -1;
            int colIncrement = (source.second < target.second) 
                             ? 1 
                             : (source.second == target.second) 
                             ? 0 
                             : -1;

            int row = source.first + rowIncrement;
            int col = source.second + colIncrement;

            while (row != target.first || col != target.second) {
                    if (board[row][col] != nullptr) {
                        return true;
                    }
                    row += rowIncrement;
                    col += colIncrement;
            }

            return false;
        }

        bool isInvalidSquare(string square) {
            if (square.length() != 2) {
                return true;
            }
            if (square[0] < 'a' || square[0] > 'h') {
                return true;
            }
            if (square[1] < '1' || square[1] > '8') {
                return true;
            }
            return false;
        }

        bool isValidCapture(ChessPiece* movingPiece, ChessPiece* targetPiece) {
            if (targetPiece == nullptr) {
                return true;
            }

            if (movingPiece->getColor() == targetPiece->getColor()) {
                return false;
            }

            return true;
        }

        pair<int, int> getKingCoordinates(int color) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    if (board[i][j] == nullptr) {
                        continue;
                    }

                    if (board[i][j]->getSymbol() == "♔" && board[i][j]->getColor() == color) {
                        return make_pair(i, j);
                    }

                    if (board[i][j]->getSymbol() == "♚" && board[i][j]->getColor() == color) {
                        return make_pair(i, j);
                    }
                }
            }
            return make_pair(-1, -1);
        }

        bool checkMove(string source, string target, int turn) {
            pair<int, int> sourcePosition = convertPoisition(source);
            pair<int, int> targetPosition = convertPoisition(target);

            ChessPiece* movingPiece = board[sourcePosition.first][sourcePosition.second];
            ChessPiece* targetPiece = board[targetPosition.first][targetPosition.second];

            if (movingPiece == nullptr) {
                return false;
            }

            if (movingPiece->getColor() != turn) {
                return false;
            }
            
            // Normal moves
            if (!movingPiece->isValidMove(target)) {
                return false;
            }

            if (isBlocked(movingPiece, targetPosition)) {
                return false;
            }

            if (!isValidCapture(movingPiece, targetPiece)) {
                return false;
            }

            if (!checkSelfKingSafety(source, target, turn)) {
                return false;
            }

            return true;
        }

        // Check from color's perspective if a square is attacked
        bool isAttacked(string position, int color) {
            pair<int, int> target = convertPoisition(position);
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    if (board[i][j] == nullptr) {
                        continue;
                    }

                    if (board[i][j]->getColor() == color) {
                        continue;
                    }
                    
                    // Check pawn attacks
                    if (board[i][j]->getSymbol() == "♙" || board[i][j]->getSymbol() == "♟") {
                        if (board[i][j]->getColor() == 0) {
                            if (target.first == i + 1 && target.second == j + 1) {
                                return true;
                            }

                            if (target.first == i + 1 && target.second == j - 1) {
                                return true;
                            }
                        }

                        if (board[i][j]->getColor() == 1) {
                            if (target.first == i - 1 && target.second == j + 1) {
                                return true;
                            }

                            if (target.first == i - 1 && target.second == j - 1) {
                                return true;
                            }
                        }

                        continue;
                    }

                    if (checkMove(board[i][j]->getPosition(), position, (color + 1) % 2)) {
                        return true;
                    }
                }
            }
            return false;
        }

        void kingCastle(int turn) {
            ChessPiece* king;
            ChessPiece* rook;

            if (turn == 0) {
                king = board[0][4];
                rook = board[0][7];

                removePiece(0, 4);
                removePiece(0, 7);

                king->move("g1");
                rook->move("f1");

                addPiece(king, 0, 6);
                addPiece(rook, 0, 5);
            }

            if (turn == 1) {
                king = board[7][4];
                rook = board[7][7];

                removePiece(7, 4);
                removePiece(7, 7);

                king->move("g8");
                rook->move("f8");

                addPiece(king, 7, 6);
                addPiece(rook, 7, 5);
            }
        }

        void queenCastle(int turn) {
            ChessPiece* king;
            ChessPiece* rook;

            if (turn == 0) {
                king = board[0][4];
                rook = board[0][0];

                removePiece(0, 4);
                removePiece(0, 0);

                king->move("c1");
                rook->move("d1");

                addPiece(king, 0, 2);
                addPiece(rook, 0, 3);
            }

            if (turn == 1) {
                king = board[7][4];
                rook = board[7][0];

                removePiece(7, 4);
                removePiece(7, 0);

                king->move("c8");
                rook->move("d8");

                addPiece(king, 7, 2);
                addPiece(rook, 7, 3);
            }
        }

        void removePiece(int row, int col) {
            board[row][col] = nullptr;
        }

        void addPiece(ChessPiece* piece, int row, int col) {
            board[row][col] = piece;
        }
};
