#ifndef HOSANNA_COMMON
#define HOSANNA_COMMON

#include <seos.h>
#define font_arial  "/usr/share/fonts/arial.ttf"

//fonts
#define FONT_SIZE_NORMAL 20
#define FONT_SIZE_BIG 32

//network
//GPRS
static uint8_t         ucIP[30]        = "";
static uint8_t         ucAPN[128]      = "";
static uint8_t         ucUID[128]      = "";
static uint8_t         ucUPWD[128]     = "";


//#define BASEURL "104.251.219.58"
//#define BASEURL "http://104.251.219.58"
#define BASEURL "http://63.142.252.251/transportation.app/web"
#define PORT 80

//Files
#define TOKEN_FILE "token.txt"
#define STOPS_FILE "stops.txt"
#define TICKET_SERIAL_FILE "tserial.txt"

struct MemoryStruct
{
	char *memory;
	size_t size;
};

//Util functions
void show_message(const char* title, const char* message, int pause);

void write_to_file(const char* file, const char* contents);
int read_from_file(const char* file, char* buffer);

void generate_ticket_serial();

int get_url(const char* url, char *buffer);
int post_url(const char* url, const char* data, char *buffer);

void show_title(const char* title);

int DispMenu(const char * menu[], int iLines, int iTitle, int iStartKey, int iEndKey, int iTimeOut);

#endif //HOSANNA_COMMON
