#pragma once
#include <bits/stdc++.h>

using namespace std;

class ChessException : public exception {};

class InvalidSquareException : public ChessException {
    public:
        string errorMessage;

        InvalidSquareException() {
            errorMessage = "Invalid squares are given as input.";
        }

        const char* what() const throw() {
            return errorMessage.c_str();
        }
};

class KingInCheckException : public ChessException {
    public:
        string errorMessage;

        KingInCheckException() {
            errorMessage = "The king is in check.";
        }

        const char* what() const throw() {
            return errorMessage.c_str();
        }
};

class NoPieceException : public ChessException {
    public:
        string source;
        string errorMessage;

        NoPieceException(string source) : source(source) {
            errorMessage = "No pieces on " + source + ".";
        }

        const char* what() const throw() {
            return errorMessage.c_str();
        }
};

class InvalidMoveException : public ChessException {
    public:
        string invalidTarget;
        string source;
        string errorMessage;

        InvalidMoveException(string source, string invalidTarget) : 
            source(source), invalidTarget(invalidTarget) {
                errorMessage = "Invalid move from " + source + " to " + invalidTarget + ".";
            }

        const char* what() const throw() {
            return errorMessage.c_str();
        }
};

class InvalidCaptureException : public ChessException {
    public:
        string invalidTarget;
        string source;
        string errorMessage;

        InvalidCaptureException(string source, string invalidTarget) : 
            source(source), invalidTarget(invalidTarget) {
                errorMessage = "The piece on " + source + " and " + invalidTarget + " are of the same color.";
            }

        const char* what() const throw() {
            return errorMessage.c_str();
        }
};

class InvalidCastleException : public ChessException {
    public:
        string errorMessage;

        InvalidCastleException() {
            errorMessage = "Invalid castle move. Either the king or the rook has moved before, \
                            or there are pieces in between, \
                            or the king is in check, \
                            or the king will pass through check.";
        }

        const char* what() const throw() {
            return errorMessage.c_str();
        }
};

class PieceBlockedException : public ChessException {
    public:
        string source;
        string target;
        string errorMessage;

        PieceBlockedException(string source, string target) : 
            source(source), target(target) {
                errorMessage = "The piece on " + source + " is blocked on the way to " + target + ".";
            }

        const char* what() const throw() {
            return errorMessage.c_str();
        }
};

class WrongColorException : public ChessException {
    public:
        string source;
        int turn;
        string errorMessage;

        WrongColorException(string source, int turn) : source(source), turn(turn) {
            errorMessage = "The piece on " + source + " is not of the same color as the current player" 
                            + (turn == 0 ? "(white)" : "(black)") + ".";
        }

        const char* what() const throw() {
            return errorMessage.c_str();
        }
};