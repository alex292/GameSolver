#ifndef TYPES_H
#define TYPES_H

typedef unsigned short Move;
typedef unsigned short int PositionIndex;
typedef unsigned long ZobristValue;
typedef unsigned char LineIndex;

enum class PositionValue { FREE, WHITE, BLACK };

enum class Color { WHITE, BLACK };

enum class GameState { OPEN, WIN_BLACK, WIN_WHITE, TIE };

#endif  // TYPES_H
