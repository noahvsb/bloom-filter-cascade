#include "categories_parser.h"

char* get_name_from_line(char* line, uint8_t start, uint8_t length) {
    char* name = malloc(sizeof(char) * (length + 1));
    if (!name) return NULL;
    memcpy(name, line + start, length);
    name[length] = '\0';
    return name;
}

CategoryList* parse_categories(char* file_path) {
    CategoryList* list = malloc(sizeof(CategoryList));
    list->size = 0;
    list->leftover_size = 10;
    list->items = malloc(sizeof(Category*) * list->leftover_size);
    if (!list->items) {
        fprintf(stderr, "Memory allocation of list items failed");
        return list;
    }

    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return list;
    }

    char line[256];

    while(fgets(line, sizeof(line), file)) {
        // removing trailing newline
        line[strcspn(line, "\r\n")] = 0;

        uint8_t line_length = strlen(line);

        // empty line
        if (line_length == 0) continue;

        // new category
        if (line[0] == '[') {

            // must end with ']' and have at least one char name
            if (line_length < 3 || line[line_length - 1] != ']') {
                fprintf(stderr, "Incorrect syntax for category header: %s\n", line);
                return list;
            }

            // initialize category
            Category* category = malloc(sizeof(Category));
            category->name = get_name_from_line(line, 1, line_length - 2);
            if (!category->name) {
                fprintf(stderr, "Memory allocation of category name failed");
                return list;
            }
            category->size = 0;
            category->leftover_size = 10;
            category->elements = malloc(sizeof(char*) * category->leftover_size);
            if (!category->elements) {
                fprintf(stderr, "Memory allocation of category elements failed");
                return list;
            }

            // add category to list
            list->items[list->size] = category;
            list->size++;
            list->leftover_size--;
            
            if (list->leftover_size == 0) {
                list->items = realloc(list->items, sizeof(Category*) * list->size * 2);
                if (!list->items) {
                    fprintf(stderr, "Memory reallocation of list items failed");
                    return list;
                }
                list->leftover_size = list->size;
            }
        } 
        // new element in category
        else {
            Category* category = list->items[list->size - 1];
            category->elements[category->size] = get_name_from_line(line, 0, line_length);
            if (!category->elements[category->size]) {
                fprintf(stderr, "Memory allocation of category element failed");
                return list;
            }
            category->size++;
            category->leftover_size--;
            
            if (category->leftover_size == 0) {
                category->elements = realloc(category->elements, sizeof(char*) * category->size * 2);
                if (!category->elements) {
                    fprintf(stderr, "Memory reallocation of category elements failed");
                    return list;
                }
                category->leftover_size = category->size;
            }
        }
    }

    fclose(file);
    return list;
}

void free_category(Category* category) {
    if (category) {
        if (category->name)
            free(category->name);
        if (category->elements) {
            for (uint64_t i = 0; i < category->size; i++)
                free(category->elements[i]);
            free(category->elements);
        }
        free(category);
    }
}

void free_categories(CategoryList* list) {
    if (list) {
        if (list->items) {
            for (uint64_t i = 0; i < list->size; i++)
                free_category(list->items[i]);
            free(list->items);
        }
        free(list);
    }
}
