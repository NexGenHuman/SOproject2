#include"functions.h"

void printState(int res, int wRoom, int roomCap, int client)
{
    fprintf(stdout, "Res:%d WRomm: %d/%d [in: %d]", res, wRoom, roomCap, client);
}