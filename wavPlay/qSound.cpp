/**
 * qSound.cpp
 * John Brock
 * 7 July 2008
 * Purpose: TO play a wave file
**/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#include "/usr/include/qt3/qsound.h"

int main(){
 QSound::play("mensenreruss2003.wav");
 
 return 0;
}
