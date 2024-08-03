#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct album {
    unsigned long id;
    char title[100];
    unsigned short year;
    char artist[100];
};

struct db_table {
    struct album *table;
    unsigned int rows_total;
    unsigned int rows_used;
};

int db_show_row(const struct db_table *db, unsigned int row){
    if (row >= db->rows_used) { // check if the row is invalid
        return 0;
    }

    const struct album *album = &db->table[row];

    // print id in 6 spaces, starting from right
    printf("%6lu:", album->id);

    // print title in 20 spaces, starting from right
    char title[21];
    strncpy(title, album->title, 20); // copy at most 20 characters of title to title[]
    title[20] = '\0'; // terminate the string
    printf("%20s:", title);

    // print artist in 20 spaces, starting from right
    printf("%20s:", album->artist);

    // print year in 4 spaces
    printf("%4u\n", album->year);

    return 1;
}



int db_add_row(struct db_table *db, const struct album *album) {
    if (db->rows_used == db->rows_total) {
        // If all rows are used, allocate more memory to the table
        const size_t new_total = db->rows_total + 5;
        struct album *new_table = realloc(db->table, new_total * sizeof(struct album));
        if (!new_table) {
            return 0; // Memory allocation failed
        }
        db->table = new_table;
        db->rows_total = new_total;
    }

    if (db->rows_used < db->rows_total) {
        // Add the new album to the next unused row in the table
        db->table[db->rows_used] = *album;
        db->rows_used++;
        return 1; // Success
    }

    return 0; // No unused rows available
}

int db_remove_row(struct db_table *db, unsigned long id) {
    int index = -1; // Initialize to an invalid value

    // Find the index of the row with the given id
    for (int i = 0; i < db->rows_used; i++) {
        if (db->table[i].id == id) {
            index = i;
            break;
        }
    }

    if (index != -1) { // If the row was found
        // Shift all rows after the index up by one to remove the row
        for (int i = index; i < db->rows_used - 1; i++) {
            db->table[i] = db->table[i + 1];
        }

        db->rows_used -= 1; // Decrement the count of used rows

        // If there are more than 4 unused rows, shrink the table
        if (db->rows_total - db->rows_used > 4) {
            const size_t new_total = db->rows_total - 5;
            struct album *new_table = realloc(db->table, new_total * sizeof(struct album));
            if (!new_table) {
                return 0; // Memory allocation failed
            }
            db->table = new_table;
            db->rows_total = new_total;
        }

        return 1; // Success
    }

    return 0; // Row not found
}
