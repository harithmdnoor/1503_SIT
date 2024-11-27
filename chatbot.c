/* -----------------------------------------------------------------------------
   Chatbot behaviour functions.
   Team ID:      
   Team Name:    
   Filename:     chatbot.c
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
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "chat1503C.h"

static char last_intent[MAX_INTENT] = "";
static char last_entity[MAX_ENTITY] = "";

/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Chatbot";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {

	return "User";

}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {

	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {

	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;

}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {

	snprintf(response, n, "Goodbye!");

	return 1;

}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {
	// fitri
	return compare_token(intent, "load") == 0;

}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {
// Ensure there's a file name provided
    if (inc < 2) {
        snprintf(response, n, "Please specify the file to load from.");
        return 0;
    }

    // Open the file specified in the second word of the user input
    const char *filename = inv[1];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        snprintf(response, n, "Failed to open file \"%s\". Please check the file path.", filename);
        return 0;
    }

    // Use knowledge_read() to read the knowledge base from the file
    int pairs_read = knowledge_read(file);
    fclose(file);

    if (pairs_read < 0) {
        snprintf(response, n, "An error occurred while loading the knowledge base.");
    } else {
        snprintf(response, n, "Successfully loaded %d knowledge entries from \"%s\".", pairs_read, filename);
    }

    return 0;
}


/* CONTRIBUTED BY : RAZAN
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {

	if (intent == NULL){
		return 0;
	}

	/* Variables for method */
	char intent_lower[MAX_INTENT];
	int i = 0;
	
	/* Copy and convert to lower case while handling buffer*/

	while (intent[i] != '\0' && i < MAX_INTENT - 1){
		intent_lower[i] = tolower(intent[i]);
		i++;
	}
	intent_lower[i] = '\0';
	
	/* Compare the intent if it is What,Where or Who respectively*/
	if (compare_token(intent_lower, "what") || compare_token(intent_lower, "where") || compare_token(intent_lower, "who")) return 1;

	return 0;

}


/* CONTRIBUTED BY : RAZAN
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 *  STILL NEED TO IMPLEMENT
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {

    // First check if this is an answer to a previous question
    if (!chatbot_is_question(inv[0])) {
        // Only process answer if we have a previous question
        if (strlen(last_intent) == 0 || strlen(last_entity) == 0) {
            snprintf(response, n, "Please ask a question first.");
            return 0;
        }

        // This might be an answer - combine all words into the response
        char answer[MAX_RESPONSE] = "";
        for (int i = 0; i < inc; i++) {
            if (i > 0) strcat(answer, " ");
            strcat(answer, inv[i]);
        }
        
        // Put this into the knowledge base
        int result = knowledge_put(last_intent, last_entity, answer);
        if (result == KB_OK) {
            snprintf(response, n, "Thank you.");
        } else {
            snprintf(response, n, "I couldn't understand that response.");
        }

        // Clear the last question
        last_intent[0] = '\0';
        last_entity[0] = '\0';
        return 0;
    }

    if (inc < 2) {
        snprintf(response, n, "Please ask a question.");
        return 0;
    }

    // Get the intent
    const char *intent = inv[0];

    int entity_start = 1;
    if (inc > 2 && (compare_token(inv[1], "is") == 0 || compare_token(inv[1],"are") == 0)){
        entity_start = 2;
    }

    // Check if we have an entity after the words "is/are"
    if (entity_start >= inc){
        snprintf(response, n, "What would you like to know about?");
        return 0;
    }

    char entity[MAX_ENTITY] = "";
    int entity_length = 0;

    // Combine words with spaces between them
    for (int i = entity_start; i<inc; i++) {
        // Add space before words
        if (entity_length > 0){
            strcat(entity, " ");
            entity_length++;
        }

        if (entity_length + strlen(inv[i]) >= MAX_ENTITY - 1){
            snprintf(response, n, "Sorry, that entity name is too long.");
            return 0;
        }
        // Add the word
        strcat(entity, inv[i]);
        entity_length += strlen(inv[i]);
    }

    // Store the current question for potential answer
    strncpy(last_intent, intent, MAX_INTENT - 1);
    last_intent[MAX_INTENT - 1] = '\0';
    strncpy(last_entity, entity, MAX_ENTITY - 1);
    last_entity[MAX_ENTITY - 1] = '\0';

    // Get response from knowledge_base
    int result = knowledge_get(intent, entity, response, n);

    // Handle the result
    if (result == KB_NOTFOUND){
        // If no knowledge, reply "I don't know" followed by the question.
        snprintf(response, n, "I don't know. ");
        if (entity_start == 2) {
            snprintf(response + strlen(response), n - strlen(response), 
                    "%s %s %s?", intent, inv[1], entity);
        } 
        else {
            snprintf(response + strlen(response), n - strlen(response), 
                    "%s %s?", intent, entity);
        }
    }
    
    return 0;
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent) {
	//harith
    return compare_token(intent, "RESET") == 0 || compare_token(intent, "reset") == 0;
}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after being reset)
 *
 * CONTRIBUTED BY : RAZAN
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {

	knowledge_reset(); // Clear conversation history
	snprintf(response, n, "Chatbot reset.");
	return 0;

}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {

	/* TO BE IMPLEMENTED */

	return 0;

}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n) {

	/* TO BE IMPLEMENTED */
	FILE *fp = fopen("ICT1503C_Project_Sample.ini", "w");
	if (fp == NULL) {
		perror("Error opening file");
		return 1;
	}
	knowledge_write(fp);
	fclose(fp);

	return 0;

}



