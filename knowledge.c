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

// Structure for entity response.
typedef struct {
  char entity[MAX_ENTITY];
  char response[MAX_RESPONSE];
} KnowledgeReply;

// Structure for each intent.
typedef struct {
  char intent[MAX_INTENT];
  KnowledgeReply *items;
  int item_count;
} IntentSection;

// Global knowledge base for the Intent.
static IntentSection sections[] = {

  {"what", NULL, 0},
  {"where", NULL, 0},
  {"who", NULL, 0}
};

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

	/* TO BE IMPLEMENTED */

	return 0;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

  //Open file with "w" mode will overwrite the existing file making sure it is empty
	FILE *fp;
  fp = fopen(FILE_NAME, "w");

  //Error checking if file cant be opened or dont exist
  if (fp == NULL) {
    perror("Error opening file");
    return;
  }

  //Close file
  fclose(fp);
}


/*
 * CONTRIBUTED BY : RAZAN 
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 * STILL NEED TO BE IMPLEMENTED.
 */
void knowledge_write(FILE *f) {

	if (f = NULL) {
    printf("Error: Cannot write to file.");
    return;
  }

  for (int i =0; i<sizeof(sections)/sizeof(sections[0]); i++){
    /* Write section header, check if there is an intent. */
    if (fprintf(f, "[%s]\n", sections[i].intent) < 0) {
        fprintf(stderr, "Error: Failed to write section header [%s]\n", sections[i].intent);
        return;
    }

    /* Write all entity-response pairs for this intent */
     for (int j = 0; j < sections[i].item_count; j++) {
          if (fprintf(f, "%s=%s\n", sections[i].items[j].entity, sections[i].items[j].response) < 0) {
              fprintf(stderr, "Error: Failed to write entry in [%s] section\n", 
                      sections[i].intent);
              return;
          }
      }

      /* Add blank line between sections (except after last section) */
      if (i < sizeof(sections)/sizeof(sections[0]) - 1) {
          if (fprintf(f, "\n") < 0) {
              fprintf(stderr, "Error: Failed to write newline after section\n");
              return;
          }
        }
    }

    /* Ensure all data is written */
    if (fflush(f) != 0) {
        fprintf(stderr, "Error: Failed to flush file buffer\n");
        return;
    }
}
