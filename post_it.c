#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "post_it.h"


PostIt* post_initPostIt(void){
	PostIt *post = NULL;
	post = (PostIt*)malloc(sizeof(PostIt));
	memset(post, 0, sizeof(PostIt));
	return post;
}


void post_readJson(PostIt *post, const char *filepath){
	if (!post || !filepath)
		return;
	
	// cJSON stuff
	int size = 0;
	char *v = NULL;
	FILE *fp = NULL;
	cJSON *json = NULL;
	
	fp = fopen(filepath, "r");
	if (fp){
		while(fgetc(fp) != EOF);
		size = ftell(fp) + 1;
		v = (char*)malloc(sizeof(char) * size);
		if (v){
			memset(v, 0, size);
			rewind(fp);
			fread(v, sizeof(char), size, fp);
			
			cJSON_InitHooks(NULL);
			json = cJSON_Parse(v);
			if (!json)
				return;
			post->json = json;
			free(v);
		}
		fclose(fp);
	}
}

void post_convertJsonToPostIt(PostIt *post){
	if (!post)
		return;
	post_freeEvent(post);

	cJSON *child = post->json->child;
	if (!child)
		return;
	
	post->size = cJSON_GetArraySize(child);
	post->event = (PEvent*)malloc(sizeof(PEvent) * post->size);	
	if (!post->event)
		return;
	
	memset(post->event, 0, sizeof(PEvent) * post->size);
	
	cJSON *temp = NULL;
	for (int i = 0; i < post->size; i++){
		temp = cJSON_GetObjectItem(cJSON_GetArrayItem(child, i), "event");
		if (temp){
			post->event[i].msg = temp->valuestring;
		}
		
		temp = cJSON_GetObjectItem(cJSON_GetArrayItem(child, i), "datetime");
		if (temp){
			post->event[i].datetime = temp->valuestring;
			sscanf(temp->valuestring, "%hu%*c%hu%*c%hu %hu%*c%hu", 
				&post->event[i].dt.year, &post->event[i].dt.month, &post->event[i].dt.day,
				&post->event[i].dt.hour, &post->event[i].dt.minutes
			);
			
			post_convertToTick(&post->event[i].dt, &post->event[i].tick);
		}
		
		temp = cJSON_GetObjectItem(cJSON_GetArrayItem(child, i), "repeat");
		if (temp)
			post->event[i].repeat = temp->valueint;
		
		char *datepart = NULL;
		temp = cJSON_GetObjectItem(cJSON_GetArrayItem(child, i), "datepart");
		if (temp){
			datepart = temp->valuestring;
			if (!strcmp(datepart, "hour"))
				post->event[i].part = HOUR;
			else if (!strcmp(datepart, "minute"))
				post->event[i].part = MINUTE;
			else if (!strcmp(datepart, "year"))
				post->event[i].part = YEAR;
			else if (!strcmp(datepart, "month"))
				post->event[i].part = MONTH;
			else if (!strcmp(datepart, "day"))
				post->event[i].part = DAY;
		}
	}
}


int post_convertToTick(const pspTime *t, u64 *tick){
	if (!t || !tick)
		return -1;
	
	return sceRtcGetTick(t, tick);
}

int post_convertToTimeStruct(const u64 *tick, pspTime *t){
	if (!t || !tick)
		return -1;
	
	return sceRtcSetTick(t, tick);
}


void post_freeAll(PostIt *post){
	if (post){
		post_freeJson(post);
		post_freeEvent(post);
		post_freePostIt(post);
	}
}

void post_freeJson(PostIt *post){
	if (post)
		if (post->json)
			cJSON_Delete(post->json);
}

void post_freeEvent(PostIt *post){
	if (post)
		if (post->event)
			free(post->event);
}

void post_freePostIt(PostIt *post){
	if (post)
		free(post);
}