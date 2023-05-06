#ifndef POST_IT_H
#define POST_IT_H

#include <sys/time.h>
#include <psptypes.h>
#include <psprtc.h>


#include "cJSON/cJSON.h"


#define POST_IT_PATH 		"ms0:/PSP/GAME/PSP Post It - Editor/"
#define POST_IT_PATH_REL 	"./"
#define POST_IT_FILE 		"post it.json"

#define POST_IT_JSON_MESSAGE	"event"
#define POST_IT_JSON_DATETIME 	"datetime"
#define POST_IT_JSON_DATEPART 	"datepart"
#define POST_IT_JSON_REPEAT 	"repeat"

#define POST_IT_DATEPART_OPTIONS "year, month, day, hour, minute"

// show 5 events on screen max
#define POST_IT_MAX_ON_SCREEN 5


#define POST_IT_SIZE_DATETIME 17
#define POST_IT_SIZE_DATEPART 7



typedef enum{
	NONE = 0,
	HOUR,
	MINUTE,
	YEAR,
	MONTH,
	DAY
}DatePart;


typedef struct{
	char *msg;
	char *datetime;
	
	pspTime dt;
	u64 tick; // time in µs
	
	int repeat;
	DatePart part;
}PEvent;


typedef struct{
	cJSON *json;
	PEvent *event;
	int size;
}PostIt;


PostIt* post_initPostIt(void);
void post_readJson(PostIt *post, const char *filepath);
void post_convertJsonToPostIt(PostIt *post);


int post_convertToTick(const pspTime *t, u64 *tick);
int post_convertToTimeStruct(const u64 *tick, pspTime *t);


void post_freeAll(PostIt *post);
void post_freeJson(PostIt *post);
void post_freeEvent(PostIt *post);
void post_freePostIt(PostIt *post);


#endif