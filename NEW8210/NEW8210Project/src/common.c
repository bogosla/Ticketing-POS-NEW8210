#include "common.h"
#include <curl/curl.h>
#include <stdio.h>
#include "wireless.h"

void show_message(const char* title, const char* message, int pause)
{
	int32_t iRet;
	lcdCls();
	lcdSetFont(font_arial, "UTF-8", 0,  20, 0);
	if(title==NULL)
	{
		lcdDisplay(0,0,DISP_CFONT|DISP_MEDIACY|DISP_INVLINE,"ERROR ENCOUNTERED");
		lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,message);
	}
	else
	{
		lcdDisplay(0,0,DISP_CFONT|DISP_MEDIACY|DISP_INVLINE,title);
		lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,message);
	}
	lcdFlip();
	lcdGoto(0, 6);
	
	if(pause==1)
		iRet = kbGetKey();
}

void write_to_file(const char* file, const char* contents)
{
	FILE *fp;
	
	fp=fopen(file, "w");
	if(fp == NULL)
	{
		show_message("I/O ERROR", "COULD NOT OPEN FILE", 1);
	}
	else
	{
		fprintf(fp, "%s", contents);
		fclose(fp);
	}
}

int read_from_file(const char* file, char* buffer)
{
	INT32 fhandle = 0;
	UINT8 DataBuf[4096];
	INT32 Len;
	INT32 success;
	
	//reset Memory
	memset(buffer, 0x00, sizeof(buffer));
	memset(DataBuf, 0x00, sizeof(DataBuf));
	
	fhandle = fileOpen(file, O_RDWR);
	if(fhandle==-1)
	{
		show_message("I/O ERROR", "INVALID FILE HANDLE", 1);
	}
	Len = fileSize(file);
	success = fileRead(fhandle, DataBuf, Len);
	if(success>=0)
	{
		// Will copy {success} characters from DataBuf to data
		strcpy(buffer, DataBuf);
		fileClose(fhandle);
		return 0;
	}
	fileClose(fhandle);
	return -1;
}

void generate_ticket_serial(){
	
	char ticket_serial[100];
	char imei[30];
	
	memset(ticket_serial, 0x00, sizeof(ticket_serial));
	memset(imei, 0x00, sizeof(imei));
	
	sys_get_sn(imei, 30);
	sprintf(ticket_serial, "%s%d", imei, rand());
	write_to_file(TICKET_SERIAL_FILE, ticket_serial);
}
///==================== NETWORK ====================
static size_t caallback_get_url(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;
	
	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if(mem->memory == NULL)
	{
		/* out of memory! */
		show_message("SYSTEM ERROR!", "MEMORY ALLOCATION FAILED!", 1);
		return 0;
	}
	
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	return realsize;
}

int get_url(const char* url, char *buffer)
{
	CURL *curl_handle;
	CURLcode res;
	struct MemoryStruct chunk;
	int retValue = -1;
	
	//connect to GPRS
	gprs_connect();
	
	show_message("NETWORK PROGRESS", "FETCHING DATA...", 0);
	
	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */
	
	/* init the curl session */
	curl_handle = curl_easy_init();
	
	/* specify URL to get */
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	
	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, caallback_get_url);
	
	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
	
	/* some servers don't like requests that are made without a user-agent
	   field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "new8210/1.0");
	
	/* get it! */
	res = curl_easy_perform(curl_handle);
	
	/* check for errors */
	if(res != CURLE_OK)
	{
		show_message("curl_easy_perform() failed: %s\n", curl_easy_strerror(res), 1);
		retValue = -1;
	}
	else
	{
		/*
		 * Now, our chunk.memory points to a memory block that is chunk.size
		 * bytes big and contains the remote file.
		 *
		 * Do something nice with it!
		 */
		memcpy(buffer, chunk.memory, strlen(chunk.memory) + 1);
		retValue = 0;
	}
	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);
	free(chunk.memory);
	
	return retValue;
}


int post_url(const char* url, const char* data, char *buffer)
{
	CURL *curl_handle;
	CURLcode res;
	struct MemoryStruct chunk;
	int retValue = -1;
	
	//connect to GPRS
	gprs_connect();
	
	show_message("NETWORK PROGRESS", "FETCHING DATA...", 0);
	
	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
	chunk.size = 0;    /* no data at this point */
	
	/* init the curl session */
	curl_handle = curl_easy_init();
	
	/* specify URL to get */
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	
	/* Now specify the POST data */
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);
	
	/* send all data to this function  */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, caallback_get_url);
	
	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
	
	/* some servers don't like requests that are made without a user-agent
	   field, so we provide one */
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "new8210/1.0");
	
	/* get it! */
	res = curl_easy_perform(curl_handle);
	
	/* check for errors */
	if(res != CURLE_OK)
	{
		show_message("NETWORK ERROR", curl_easy_strerror(res), 1);
		retValue = -1;
	}
	else
	{
		/*
		 * Now, our chunk.memory points to a memory block that is chunk.size
		 * bytes big and contains the remote file.
		 *
		 * Do something nice with it!
		 */
		
		memcpy(buffer, chunk.memory, strlen(chunk.memory) + 1);
		retValue = 0;
	}
	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);
	free(chunk.memory);
	
	return retValue;
}

int DispMenu(const char * menu[], int iLines, int iTitle, int iStartKey, int iEndKey, int iTimeOut)
{
	int screens;
	int start_line;
	int start_item;
	int line_per_screen,max_line_screen;
	int cur_screen;
	int i;
	int t;
	int key;
	char  szBuffer[100];
	
	max_line_screen = 9;
	
	if(iTitle)
	{
		start_item = 1;
		screens = (iLines-2)/(max_line_screen-1) + 1;
		start_line = 2;
		line_per_screen = max_line_screen-1;
		
	}
	else
	{
		start_item = 0;
		screens = (iLines-1)/max_line_screen + 1;
		start_line = 0;
		line_per_screen = max_line_screen;
	}
	
	cur_screen = 0;
	
	while(1)
	{
		if(cur_screen<screens-1)
			lcdSetIcon(ICON_DOWN, OPENICON);
		else
			lcdSetIcon(ICON_DOWN, CLOSEICON);
		if(cur_screen>0)
			lcdSetIcon(ICON_UP, OPENICON);
		else
			lcdSetIcon(ICON_UP, CLOSEICON);
			
		//lcdClrLine(start_line, 7);
		lcdCls();
		lcdDisplay(0, 0, DISP_CFONT|DISP_INVLINE|DISP_MEDIACY, (char *)menu[0]);
		for(i=0; i<line_per_screen; i++)
		{
			t = i + line_per_screen*cur_screen + start_item;
			if(t<iLines)
			{
				memset(szBuffer,0,sizeof(szBuffer));
				if(max_line_screen == 6)
				{
				
					lcdDisplay(0, (i*2+start_line), DISP_CFONT, (char *)szBuffer);
				}
				else
				{
					lcdDisplay(0, (i*2+start_line), DISP_CFONT, (char *)menu[t]);
				}
			}
		}
		lcdFlip();
		key = kbGetKeyMs(iTimeOut*1000);
		
		if(key==KEY_CANCEL)
		{
			return -1;
		}
		else if(key == KEY_TIMEOUT)
		{
			lcdSetIcon(ICON_DOWN, CLOSEICON);
			lcdSetIcon(ICON_UP,   CLOSEICON);
			return key;
		}
		else if(key==KEY_ENTER || key==KEY_DOWN)
		{
			if(cur_screen>=screens-1)
				cur_screen = 0;
			else
				cur_screen++;
		}
		else if(key==KEY_UP)
		{
			if(cur_screen<=0)
				cur_screen = screens-1;
			else
				cur_screen--;
		}
		else if(key>=iStartKey && key<=iEndKey)
		{
			lcdSetIcon(ICON_DOWN, CLOSEICON);
			lcdSetIcon(ICON_UP,   CLOSEICON);
			
			switch(key)
			{
				case KEY1:
					key = 1;
					break;
				case KEY2:
					key = 2;
					break;
				case KEY3:
					key = 3;
					break;
				case KEY4:
					key = 4;
					break;
				case KEY5:
					key = 5;
					break;
				case KEY6:
					key = 6;
					break;
				case KEY7:
					key = 7;
					break;
				case KEY8:
					key = 8;
					break;
				default:
					break;
			}
			return 8*cur_screen+key-1;
		}
		//else
		//sysBeep();
	}
}

void show_title(const char *title)
{
	lcdCls();
	lcdDisplay(0, 0, DISP_REVERSE|DISP_MEDIACY|DISP_CLRLINE|DISP_CFONT, title);
	lcdGoto(50, 4);
	lcdFlip();
}
