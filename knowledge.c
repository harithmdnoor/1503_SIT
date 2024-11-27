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


typedef struct KnowledgeNode {
    char intent[MAX_INTENT];
    char entity[MAX_ENTITY];
    char response[MAX_RESPONSE];
    struct KnowledgeNode* next;
} KnowledgeNode;

typedef struct WrittenIntent {
    char intent[MAX_INTENT];
    struct WrittenIntent* next;
} WrittenIntent;


#define MAX_KNOWLEDGE_BASE_SIZE   64
static KnowledgeNode* knowledge_base = NULL;  // Head of the linked list

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
    // Input validation
    if (intent == NULL || entity == NULL || response == NULL || n <= 0) {
        return KB_INVALID;
    }

    // Search through the linked list
    KnowledgeNode* current = knowledge_base;
    while (current != NULL) {
        if (strcasecmp(current->intent, intent) == 0 && 
            strcasecmp(current->entity, entity) == 0) {
            // Copy the response safely
            strncpy(response, current->response, n - 1);
            response[n - 1] = '\0';
            return KB_OK;
        }
        current = current->next;
    }

    // No matching response found
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
    // Validate inputs
    if (intent == NULL || entity == NULL || response == NULL) {
        return KB_INVALID;
    }

    // Validate intent is a recognized question word
    if (strcasecmp(intent, "what") != 0 && 
        strcasecmp(intent, "where") != 0 && 
        strcasecmp(intent, "who") != 0) {
        return KB_INVALID;
    }

    // Check if entry exists
    KnowledgeNode* current = knowledge_base;
    while (current != NULL) {
        if (strcasecmp(current->intent, intent) == 0 && 
            strcasecmp(current->entity, entity) == 0) {
            // Update existing entry
            strncpy(current->response, response, MAX_RESPONSE - 1);
            current->response[MAX_RESPONSE - 1] = '\0';
            return KB_OK;
        }
        current = current->next;
    }

    // Create new node
    KnowledgeNode* new_node = (KnowledgeNode*)malloc(sizeof(KnowledgeNode));
    if (new_node == NULL) {
        return KB_NOMEM;
    }

    // Initialize new node
    strncpy(new_node->intent, intent, MAX_INTENT - 1);
    new_node->intent[MAX_INTENT - 1] = '\0';
    strncpy(new_node->entity, entity, MAX_ENTITY - 1);
    new_node->entity[MAX_ENTITY - 1] = '\0';
    strncpy(new_node->response, response, MAX_RESPONSE - 1);
    new_node->response[MAX_RESPONSE - 1] = '\0';

    // Add to front of list
    new_node->next = knowledge_base;
    knowledge_base = new_node;
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

                if (knowledge_put(intent, entity, response) == KB_OK) {
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
    // Free all nodes
    KnowledgeNode *current = knowledge_base;
    while (current != NULL) {
        KnowledgeNode *next = current->next;
        free(current);
        current = next;
    }
    knowledge_base = NULL;

    // Clear file
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp != NULL) {
        fclose(fp);
    }
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

    WrittenIntent* written_intents = NULL;
    int written_count = 0;

    // For each node
    KnowledgeNode *current = knowledge_base;
    while (current != NULL) {
        // Check if intent already written
        WrittenIntent* check = written_intents;
        int already_written = 0;
        while (check != NULL) {
            if (strcasecmp(check->intent, current->intent) == 0) {
                already_written = 1;
                break;
            }
            check = check->next;
        }

        if (!already_written) {
            // Write intent header
            fprintf(f, "[%s]\n", current->intent);
            
            // Add to written intents
            WrittenIntent* new_intent = malloc(sizeof(WrittenIntent));
            if (new_intent != NULL) {
                strncpy(new_intent->intent, current->intent, MAX_INTENT - 1);
                new_intent->intent[MAX_INTENT - 1] = '\0';
                new_intent->next = written_intents;
                written_intents = new_intent;
            }

            // Write all matching entries
            KnowledgeNode *inner = knowledge_base;
            while (inner != NULL) {
                if (strcasecmp(inner->intent, current->intent) == 0) {
                    fprintf(f, "%s=%s\n", inner->entity, inner->response);
                }
                inner = inner->next;
            }
            fprintf(f, "\n");
        }
        current = current->next;
    }

    // Free written intents list
    while (written_intents != NULL) {
        WrittenIntent* next = written_intents->next;
        free(written_intents);
        written_intents = next;
    }
}