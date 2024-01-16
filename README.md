# How to Execute this Program
1. Ensure that you have SDL2/SDL_image.h and SDL2 ready.
2. In the directory, execute `cmake .`
3. Once the build files have been compiled, execute `make`.
4. `./chess` to start the game.

# How to Play
1. Click on the piece to move and the desired target square.
2. Note that: castling, en passant and pawn promotion is supported.
3. This current iteration does not detect check, hence consequently fails to check for checkmate/stalemate.
