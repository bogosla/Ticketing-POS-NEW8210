#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>

#include "seos.h"
#include "ticketing.h"
#include "common.h"
#include "auth.h"


const char* menu[] =
{
	"LAP TICKETING",
	"1. Buy Ticket",
	"2. Reserve Seat",
	"3. Card Tickets",
	"4. Gateway Tickets",
	"5. Pay Reservation",
	"6. Sell Card",
	"7. POS Report",
	"8. User Report",
	"1. Seat Report",
	"2. Bus Details Report",
	"3. Manifest Report",
	"4. Plan Route",
	"5. Change Password",
	"6. POS Serial No.",
	"7. Logout",
};

#define CL_BACK_GROUND_PIC_FILE "cl_bk_pic.jpg"
DFBColor COLOR_BLACK= {0xF0,0xFF,0xFF,0xFF};
DFBColor COLOR_WITE= {0xF0,0x00,0x00,0x00};

int main(int argc, char *argv[])
{
	time_t t;
	int retval = 0;
	int selected=0;
	char imei[30];
	DFBColor bgColor= {0xF0,0xFF,0xFF,0xFF};
	DFBColor fgColor= {0xF0,0x00,0x00,0x00};
	
	application_init(&argc, &argv);
	
	curl_global_init(CURL_GLOBAL_ALL);
	
	//init random gen
	srand((unsigned) time(&t));
	
	
	//Initialize global stuffs
	
	lcdSetFgColor(fgColor);
	lcdSetBgColor(bgColor);
	lcdDrBgLogo(0, 0, 320, 240, "bg_image.png");
	
	sys_backlight_set_time(30);
	kbFlush();
	
	dfb_create_font(font_arial,20,0);
	dfb_create_font(font_arial,24,0);
	dfb_create_font(font_arial,28,0);
	dfb_create_font(font_arial,32,0);
	
	
	lcdSetFont(font_arial, "UTF-8", 0,  20, 0);
	
	//login first
	while(auth_login()<0)
	{
		continue;
	}
	
	//login successful show main menu
	while(selected>=0)
	{
		selected = DispMenu(menu,14,1,KEY0, KEY8, 60);
		switch(selected)
		{
			case 0:
				{
					create_ticket(SELL_TICKET);
					break;
				}
			case 1:
				{
					create_ticket(RESERVE_TICKET);
					break;
				}
			case 2:
				{
					create_ticket(SELL_CARD_TICKET);
					break;
				}
			case 3:
				{
					create_ticket(GATEWAY_TICKET);
					break;
				}
			case 4:
				{
					create_ticket(PAY_RESERVATION);
					break;
				}
			case 5:
				{
					sell_card();
					break;
				}
			case 6:
				{
					create_report(REPORT_POS);
					break;
				}
			case 7:
				{
					create_report(REPORT_USER);
					break;
				}
			case 8:
				{
					create_seat_report();
					break;
				}
			case 9:
				{
					bus_details_report();
					break;
				}
			case 10:
				{
					manifest_report();
					break;
				}
			case 11:
				{
					plan_route();
					break;
				}
			case 12:
				{
					change_password();
					break;
				}
			case 13:
				{
					memset(imei, 0x00, sizeof(imei));
					sysReadSN(imei);
					show_message("DEVICE SERIAL ID", imei, 1);
				}
			case 14:
				{
					sys_reboot();
				}
			default:
				break;
		}
	}
	
	//cleanup
	/* we're done with libcurl, so clean it up */
	curl_global_cleanup();
	
	return retval;
}
