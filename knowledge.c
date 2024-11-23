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
int knowledge_get(const char *intent, const char *entity, char *response, int n) {

	/* TO BE IMPLEMENTED */

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
    // Validate intent
    if (strcmp(intent, "what") != 0 && strcmp(intent, "where") != 0 && strcmp(intent, "who") != 0) {
        return KB_INVALID;
    }

    // Traverse the linked list to find an existing node with the same intent and entity
    KnowledgeNode *current = knowledgeBase;
    while (current != NULL) {
        if (strcmp(current->intent, intent) == 0 && strcmp(current->entity, entity) == 0) {
            // Update the response if the intent and entity match
            strncpy(current->response, response, MAX_RESPONSE - 1);
            current->response[MAX_RESPONSE - 1] = '\0'; // Ensure null-termination
            return KB_OK;
        }
        current = current->next;
    }

    // If no match is found, create a new node
    KnowledgeNode *newNode = (KnowledgeNode *)malloc(sizeof(KnowledgeNode));
    if (newNode == NULL) {
        return KB_NOMEM; // Memory allocation failure
    }

    // Initialize the new node
    strncpy(newNode->intent, intent, MAX_INTENT - 1);
    newNode->intent[MAX_INTENT - 1] = '\0';
    strncpy(newNode->entity, entity, MAX_ENTITY - 1);
    newNode->entity[MAX_ENTITY - 1] = '\0';
    strncpy(newNode->response, response, MAX_RESPONSE - 1);
    newNode->response[MAX_RESPONSE - 1] = '\0';
    newNode->next = knowledgeBase;

    // Insert the new node at the beginning of the linked list
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

	/* TO BE IMPLEMENTED */

	return 0;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

  //Open file with "w" mode will overwrite the existing file making sure it is empty
	fp = fopen(FILE_NAME, "w");

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
