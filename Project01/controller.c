//
// Created by dontm on 12/6/2023.
//

#include "controller.h"
#include <stdio.h>

unsigned int face_buttons = 0; //00
unsigned int right_stick = 0; //01
unsigned int left_stick = 0; //10
unsigned int triggers = 0; //11 //1st 15 are ltrigger 2nd 15 are rtrigger // this is because triggers have variable pulls, not on or off (I make games!)

// my added mask variable to be used in isolation of bits to return
unsigned mask;

//returns a mask to isolate bits i - j of unsigned int. (0, 1: gets mask for first two bits) i needs to be <= j
// to use call function and then & it with original value
unsigned getMaskUtility(unsigned i, unsigned j){
    unsigned m = 0;
    for ( unsigned k = i; k<= j; k++)
        m |= 1 << k;

    return m;
}

/*
 * The plan is to use two bits to determine which of the variables are being manipulated
 * 00, 01, 11, 10
 * then remainder of bits can be used for the manipulation
 * triggers have 15 bits per trigger as there are two - reps distance pulled
 * sticks have 15 bits X and 15 bits Y
 * there are 12 face buttons, their states are 0 and 1 of course
 *      the order of the button states from right to left (in bits) is X, O, T, S, Up, Left, Down, Right, Start, Option, L1, R1
 */

/*
  In this project, you are prohibited from using ANY complex statement.
  This means you are restricted to assignment, bitwise, logical and
  arithmetic operators
 */

/*
  Read a message and SET the bits of the `face_buttons`, `right_stick`,
      `left_stick`, and `triggers` variables based on the contents of the
      message.  The specifics of the bit mapping are up to you, but the
      two least significant bits MUST be used to uniquely identify which
      variable is being modified i.e. with the message:
          0000 .... 0001 1010
      We would SET the second and third bits of the variable corresponding
      with 0b10
 */
int set_bits(unsigned int msg) {
    //check message for 2 LSBs
    //now we know what var to change
    //change that var

    unsigned int lsb; //declare lsb var
    lsb = msg;
    //printf("%u : value of msg\n", msg);
    //printf("%u : value of lsb\n", lsb);
    lsb = lsb << 30; // we're going to shift left 30 bits to isolate our two lsbs
    unsigned int val = msg;

    //check lsb to determine var
    //lsb can only be one of four options
    // used psuedo "magic numbers" here
    if(lsb == 0){ //00
        face_buttons = msg;
    }else if(lsb == 1073741824){ //01
        val -= 1; //this is to clear the lsb
        right_stick = val;
    }else if(lsb == 2147483648){ //10
        val -= 2; //this is to clear the lsb
        left_stick = val;
    }else{ //lsb == 3 //11
        val -= 3; //this is to clear the lsb
        triggers = val; //1st 15 are left 2nd 15 are right
    }
    return 1; //need to return int (consider anything a success of setting)
}

/*
  Read a message and CLEAR the bits of the `face_buttons`, `right_stick`,
      `left_stick`, and `triggers` variables based on the contents of the
      message.  The specifics of the bit mapping are up to you, but the
      two least significant bits MUST be used to uniquely identify which
      variable is being modified - i.e. with the message:
          0000 .... 0001 1010
      We would CLEAR the second and third bits of the variable corresponding
      with 0b10
 */
int clear_bits(unsigned int msg) {
    //check message for 2 LSBs
    //now we know what var to change
    //change that var

    unsigned int lsb; //declare lsb var
    lsb = msg;
    //printf("%u : value of msg\n", msg);
    //printf("%u : value of lsb\n", lsb);
    lsb = lsb << 30; // we're going to shift left 30 bits to isolate our two lsbs

    //check lsb to determine var
    //lsb can only be one of four options
    if(lsb == 0){ //00
        face_buttons = 0;
    }else if(lsb == 1073741824){ //01
        right_stick = 0;
    }else if(lsb == 2147483648){ //10
        left_stick = 0;
    }else{ //lsb == 3 //11
        triggers = 0; //1st 15 are left 2nd 15 are right
    }
    return 1; //need to return int (consider anything a success of setting)
}

/*
  for the get_<value> functions, return just the value of the requested
  field.  I.e. `get_t` would extract the 0 or 1 of the `face_buttons`
  integer corresponding to 't' and return it.
 */
int get_x(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) - 1) << 2;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}
int get_o(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) - 1) << 3;//(face_buttons << 1) - 1;
    unsigned val = mask & currState;
    return val;
}
int get_s(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) -1) << 5;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}
int get_t(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) -1) << 4;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}
int get_up(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) -1) << 6;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}
int get_left(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) -1) << 7;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}
int get_down(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) -1) << 8;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}
int get_right(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) -1) << 9;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}
int get_r1(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) -1) << 13;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}
int get_r2(void) {
    unsigned currState = triggers;
    mask = ((1 << 15) - 1) << 17;
    unsigned v = currState & mask;
    return v;
}
int get_l1(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) -1) << 12;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}
int get_l2(void) {
    unsigned currState = triggers;
    mask = ((1 << 15) - 1) << 2;
    unsigned v = currState & mask;
    return v;
}
int get_rx(void) {
    unsigned int currState = right_stick;
    unsigned m = ((1 << 15) -1) << 2;
    return currState & m;
}
int get_ry(void) {
    unsigned int currState = right_stick;
    unsigned m = ((1 << 15) -1) << 17;
    return currState & m;
}
int get_lx(void) {
    unsigned int currState = left_stick;
    unsigned m = ((1 << 15) -1) << 2;
    return currState & m;
}
int get_ly(void) {
    unsigned int currState = left_stick;
    unsigned m = ((1 << 15) -1) << 17;
    return currState & m;
}
int get_start(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) -1) << 10;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}
int get_option(void) {
    unsigned currState = face_buttons;
    mask = ((1 << 1) -1) << 11;//(face_buttons << 1) - 1;
    unsigned val = currState & mask;
    return val;
}