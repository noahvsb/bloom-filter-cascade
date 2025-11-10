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
    if (!list) {
        fprintf(stderr, "Memory allocation of category list failed");
        return clean_return(1, list, free_categories);
    }
    list->categories_size = 0;
    list->elements_size = 0;
    list->leftover_size = 32;
    list->categories = malloc(sizeof(Category*) * list->leftover_size);
    if (!list->categories) {
        fprintf(stderr, "Memory allocation of categories failed");
        return clean_return(1, list, free_categories);
    }

    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        return clean_return(2, list, free_categories, file, fclose);
    }

    static char line[256];

    while(fgets(line, sizeof(line), file)) {
        // removing trailing newline
        line[strcspn(line, "\r\n")] = 0;

        uint8_t line_length = strlen(line);

        // empty line
        if (line_length == 0) continue;

        // new category
        if (line[0] == '[') {
            // must have at least one char for the name and end with ']'
            if (line_length < 3 || line[line_length - 1] != ']') {
                fprintf(stderr, "Incorrect syntax for category header: %s\n", line);
                return clean_return(2, list, free_categories, file, fclose);
            }

            // initialize category
            Category* category = malloc(sizeof(Category));
            if (!category) {
                fprintf(stderr, "Memory allocation of category failed");
                return clean_return(3, list, free_categories, file, fclose, category, free_category);
            }
            category->name = get_name_from_line(line, 1, line_length - 2);
            category->size = 0;
            category->leftover_size = 32;
            category->elements = malloc(sizeof(char*) * category->leftover_size);
            if (!category->name || !category->elements) {
                fprintf(stderr, "Memory allocation of category name or elements failed");
                return clean_return(3, list, free_categories, file, fclose, category, free_category);
            }

            // add category to list
            list->categories[list->categories_size] = category;
            list->categories_size++;
            list->leftover_size--;
            
            if (list->leftover_size == 0) {
                list->categories = realloc(list->categories, sizeof(Category*) * list->categories_size * 2);
                if (!list->categories) {
                    fprintf(stderr, "Memory reallocation of categories failed");
                    return clean_return(2, list, free_categories, file, fclose);
                }
                list->leftover_size = list->categories_size;
            }
        } 
        // new element in category
        else {
            if (!list->categories_size) {
                fprintf(stderr, "Incorrect syntax: the first line should be a category header\n");
                return clean_return(2, list, free_categories, file, fclose);
            }

            Category* category = list->categories[list->categories_size - 1];
            category->elements[category->size] = get_name_from_line(line, 0, line_length);
            if (!category->elements[category->size]) {
                fprintf(stderr, "Memory allocation of category element failed");
                return clean_return(2, list, free_categories, file, fclose);
            }
            list->elements_size++;
            category->size++;
            category->leftover_size--;
            
            if (category->leftover_size == 0) {
                category->elements = realloc(category->elements, sizeof(char*) * category->size * 2);
                if (!category->elements) {
                    fprintf(stderr, "Memory reallocation of category elements failed");
                    return clean_return(2, list, free_categories, file, fclose);
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
            for (uint32_t i = 0; i < category->size; i++)
                free(category->elements[i]);
            free(category->elements);
        }
        free(category);
    }
}

void free_categories(CategoryList* list) {
    if (list) {
        if (list->categories) {
            for (uint32_t i = 0; i < list->categories_size; i++)
                free_category(list->categories[i]);
            free(list->categories);
        }
        free(list);
    }
}
