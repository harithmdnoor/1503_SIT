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
#define MAX_INTENT 32
#define MAX_ENTITY 64
#define MAX_RESPONSE 256

typedef struct KnowledgeNode {
    char intent[MAX_INTENT];
    char entity[MAX_ENTITY];
    char response[MAX_RESPONSE];
    struct KnowledgeNode *next;
} KnowledgeNode;

KnowledgeNode *knowledgeBase = NULL;

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
    if (intent == NULL || entity == NULL || response == NULL) {
        return KB_INVALID;
    }
    KnowledgeNode *current = knowledgeBase;
    while (current != NULL) {
        if (strcmp(current->intent, intent) == 0 && strcmp(current->entity, entity) == 0) {
            strncpy(response, current->response, n - 1);
            response[n - 1] = '\0';
            return KB_OK;
        }
        current = current->next;
    }

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
    if (strcmp(intent, "what") != 0 && strcmp(intent, "where") != 0 && strcmp(intent, "who") != 0) {
        return KB_INVALID;
    }

    KnowledgeNode *current = knowledgeBase;
    while (current != NULL) {
        if (strcmp(current->intent, intent) == 0 && strcmp(current->entity, entity) == 0) {
            strncpy(current->response, response, MAX_RESPONSE - 1);
            current->response[MAX_RESPONSE - 1] = '\0';
            return KB_OK;
        }
        current = current->next;
    }

    KnowledgeNode *newNode = (KnowledgeNode *)malloc(sizeof(KnowledgeNode));
    if (newNode == NULL) {
        return KB_NOMEM;
    }

    strncpy(newNode->intent, intent, MAX_INTENT - 1);
    newNode->intent[MAX_INTENT - 1] = '\0';
    strncpy(newNode->entity, entity, MAX_ENTITY - 1);
    newNode->entity[MAX_ENTITY - 1] = '\0';
    strncpy(newNode->response, response, MAX_RESPONSE - 1);
    newNode->response[MAX_RESPONSE - 1] = '\0';
    newNode->next = knowledgeBase;
    knowledgeBase = newNode;

    return KB_OK;

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
if (f == NULL) {
        return 0;
    }

    char line[MAX_ENTITY + MAX_RESPONSE + 2];
    char intent[MAX_INTENT];
    char entity[MAX_ENTITY];
    char response[MAX_RESPONSE];

    int count = 0;
    int in_section = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        char *newline = strchr(line, '\n');
        if (newline) {
            *newline = '\0';
        }

        if (line[0] == '\0') {
            continue;
        }

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

        if (in_section) {
            char *equals_sign = strchr(line, '=');
            if (equals_sign) {
                *equals_sign = '\0';
                strncpy(entity, line, MAX_ENTITY - 1);
                entity[MAX_ENTITY - 1] = '\0';
                strncpy(response, equals_sign + 1, MAX_RESPONSE - 1);
                response[MAX_RESPONSE - 1] = '\0';
                knowledge_put(intent, entity, response);
                count++;
            }
        }
    }

    return count;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {
KnowledgeNode *current = knowledgeBase;
    while (current != NULL) {
        KnowledgeNode *temp = current;
        current = current->next;
        free(temp);
    }
    knowledgeBase = NULL;
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {
    if (f == NULL) {
        perror("Error opening file");
        return;
    }

    KnowledgeNode *current = knowledgeBase;
    while (current != NULL) {
        fprintf(f, "[%s]\n%s=%s\n\n", current->intent, current->entity, current->response);
        current = current->next;
    }
}