#include <stdio.h>
#include <string.h>
#include "editor.h"

int editor_insert_char(char editing_buffer[], int editing_buflen, char to_insert, int pos){

    char move[editing_buflen - (pos+1)];//temp buffer that has all char after pos

    //assign chars to the right of pos to temp buffer
    for (int i = pos; i < editing_buflen - 1; i++){
        move[(i - pos)] = editing_buffer[i];
    }

    editing_buffer[pos] = to_insert; //insert the thing at the thing

     for (int i = 0; i < editing_buflen - pos - 1; i++) {
        editing_buffer[i + pos + 1] = move[i];
    }

    if (editing_buffer[pos] == to_insert && pos < editing_buflen){return 1;} else {return 0;}
}

int editor_delete_char(char editing_buffer[], int editing_buflen, char to_delete, int offset){
    int deleted; // position of deleted char

    //iterate from offset to to_delete
    for (int i = offset; i < editing_buflen; i++){

        if (editing_buffer[i] == to_delete){ // find it
            deleted = editing_buffer[i]; //mark this as the deleted position
            //move everything once to the left
             for (int j = i; j < editing_buflen; j++){
                editing_buffer[j] = editing_buffer[j + 1];
            }

            editing_buffer[editing_buflen] = '\0';//add null to end

            //check if it really deleted
            if(editing_buffer[deleted] != to_delete){
                return 1;//finish
            }
        }
    }
    return 0;//didn't work
}

#include <string.h>

int editor_replace_str(char editing_buffer[], int editing_buflen,const char *str, const char *replacement,int offset) {
    int str_len = strlen(str);
    int rep_len = strlen(replacement);

    if (str_len == 0) {
        return -1;
    }

    char *p = strstr(editing_buffer + offset, str);

    if (p == NULL) {
        return -1;
    }

    int start = p - editing_buffer;
    int end = start + str_len - 1;

    int shift = rep_len - str_len;

    if (shift > 0) { // Moving characters to the right
        if (end + shift >= editing_buflen) {
            shift = editing_buflen - 1 - end;
        }

        memmove(editing_buffer + end + shift + 1,
                editing_buffer + end + 1,
                editing_buflen - end - shift - 1);

        memcpy(editing_buffer + start, replacement, rep_len);
    } else if (shift < 0) { // Moving characters to the left
        int i;
        for (i = end + 1; i <= editing_buflen - shift - 1; i++) {
            editing_buffer[i + shift] = editing_buffer[i];
        }

        for (i = editing_buflen - shift - 1; i >= end + 1; i--) {
            editing_buffer[i] = '\0';
        }

        memcpy(editing_buffer + start, replacement, rep_len);
    } else { // No need to move characters
        memcpy(editing_buffer + start, replacement, rep_len);
    }

    return start + rep_len - 1;
}

void editor_view(int rows, int cols, char viewing_buffer[rows][cols], const char editing_buffer[], int editing_buflen, int wrap) {
    // Initialize viewing_buffer to null characters
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            viewing_buffer[i][j] = '\0';
        }
    }

    // Copy editing_buffer to viewing_buffer
    int row = 0;
    int col = 0;
    for (int i = 0; i < editing_buflen && editing_buffer[i] != '\0'; i++) {
        if (editing_buffer[i] == '\n') {
            // Move to the next row
            row++;
            col = 0;
        } else {
            // Copy the character to the current position
            viewing_buffer[row][col] = editing_buffer[i];
            col++;

            if (col == cols) {
                // Wrap to the next row if necessary
                if (wrap) {
                    row++;
                    col = 0;
                } else {
                    // Skip the rest of the line until a newline is encountered
                    while (editing_buffer[i] != '\n' && editing_buffer[i] != '\0') {
                        i++;
                    }
                    if (editing_buffer[i] == '\n') {
                        row++;
                        col = 0;
                    }
                }
            }
        }

        // Stop copying if we have filled the last row
        if (row == rows) {
            break;
        }
    }
}
