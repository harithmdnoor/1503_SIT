/* -----------------------------------------------------------------------------
   Chatbot knowledge base functions.
   Team ID:      
   Team Name:    
   Filename:     knowledge.c
   Version:      2024-1.0
   Description:  C source for helper function file in ICT1503C Project.
   Module:       ICT1503C
   Skeleton 
   Prepared by:  Nicholas H L Wong
   Organisation: Singapore Institute of Technology
   Division:     Infocomm Technology
   Credits:      Parts of this project are based on materials contributed to by 
                 other SIT colleagues.

   -----------------------------------------------------------------------------
 */

/*
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1503C.h"

#define FILE_NAME "ICT1503C_Project_Sample.ini"
#define MAX_KNOWLEDGE_BASE_SIZE   64
static KnowledgeEntry knowledge_base[MAX_KNOWLEDGE_BASE_SIZE];
static int knowledge_base_size = 0; 

typedef struct {
        char intent[MAX_INTENT];
        char entity[MAX_ENTITY];
        char response[MAX_RESPONSE];
    } KnowledgeEntry;

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
// Get the response to a question
int knowledge_get(const char *intent, const char *entity, char *response, int n) {
    // Return invalid if any input is NULL
    if (intent == NULL || entity == NULL || response == NULL) {
        return KB_INVALID;
    }

    // Search the knowledge base for a matching intent and entity
    for (int i = 0; i < knowledge_base_size; i++) {
        if (strcmp(knowledge_base[i].intent, intent) == 0 && strcmp(knowledge_base[i].entity, entity) == 0) {
            // Copy the response to the provided buffer
            strncpy(response, knowledge_base[i].response, n - 1);
            response[n - 1] = '\0';
            return KB_OK;
        }
    }

    // Return not found if no match is found
    return KB_NOTFOUND;
}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {

	/* TO BE IMPLEMENTED */

	return KB_INVALID;

}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {
    // Return if file is NULL
    if (f == NULL) {
        return 0;
    }

    char line[MAX_ENTITY + MAX_RESPONSE + 2];
    char intent[MAX_INTENT];
    char entity[MAX_ENTITY];
    char response[MAX_RESPONSE];

    int count = 0;
    int in_section = 0;

    // Read each line from the file
    while (fgets(line, sizeof(line), f) != NULL) {
        // Remove trailing newline character
        char *newline = strchr(line, '\n');
        if (newline) {
            *newline = '\0';
        }

        // Skip blank lines
        if (line[0] == '\0') {
            continue;
        }

        // Check if the line is a section header
        if (line[0] == '[') {
            char *end_bracket = strchr(line, ']');
            if (end_bracket) {
                *end_bracket = '\0';
                strncpy(intent, line + 1, MAX_INTENT - 1);
                intent[MAX_INTENT - 1] = '\0';
                in_section = 1;
            } else {
                in_section = 0;
            }
            continue;
        }

        // Parse entity-response pairs if in a valid section
        if (in_section) {
            char *equals_sign = strchr(line, '=');
            if (equals_sign) {
                *equals_sign = '\0';
                strncpy(entity, line, MAX_ENTITY - 1);
                entity[MAX_ENTITY - 1] = '\0';
                strncpy(response, equals_sign + 1, MAX_RESPONSE - 1);
                response[MAX_RESPONSE - 1] = '\0';

                // Add entity-response pair to the knowledge base
                if (knowledge_base_size < MAX_KNOWLEDGE_BASE_SIZE) {
                    strncpy(knowledge_base[knowledge_base_size].intent, intent, MAX_INTENT - 1);
                    knowledge_base[knowledge_base_size].intent[MAX_INTENT - 1] = '\0';
                    strncpy(knowledge_base[knowledge_base_size].entity, entity, MAX_ENTITY - 1);
                    knowledge_base[knowledge_base_size].entity[MAX_ENTITY - 1] = '\0';
                    strncpy(knowledge_base[knowledge_base_size].response, response, MAX_RESPONSE - 1);
                    knowledge_base[knowledge_base_size].response[MAX_RESPONSE - 1] = '\0';
                    knowledge_base_size++;
                    count++;
                }
            }
        }
    }

    return count;
}



/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

  //Open file with "w" mode will overwrite the existing file making sure it is empty
	FILE *fp = fopen(FILE_NAME, "w");

  //Error checking if file cant be opened or dont exist
  if (fp == NULL) {
    perror("Error opening file");
    return 1;
  }

  //Close file
  fclose(fp);

  return;
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {

	/* TO BE IMPLEMENTED */

}
