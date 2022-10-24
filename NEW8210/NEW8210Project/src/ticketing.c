#include "ticketing.h"
#include "common.h"
#include "wireless.h"
#include "string_convert.h"
#include "visualstring.h"
#include <seos.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <time.h>



#define font_print  "/usr/share/fonts/Unibit-16.ttf"

int print_ticket(const char* json, ActionType type)
{
	int32_t	print_status;
	char username[100];
	char code[100];
	char routeName[100];
	char plateNumber[100];
	char agent[100];
	char genTime[100];
	
	cJSON *root = NULL;
	const cJSON *message = NULL;
	const cJSON *success = NULL;
	const cJSON *common = NULL;
	const cJSON *cc = NULL;
	const cJSON *tid = NULL;
	const cJSON *price = NULL;
	const cJSON *route = NULL;
	const cJSON *journey = NULL;
	const cJSON *date = NULL;
	const cJSON *time = NULL;
	const cJSON *pos = NULL;
	const cJSON *name = NULL;
	const cJSON *generated = NULL;
	const cJSON *is_intl = NULL;
	const cJSON *currency = NULL;
	const cJSON *tickets = NULL;
	const cJSON *ticket = NULL;
	const cJSON *points = NULL;
	const cJSON *discount = NULL;
	const cJSON *is_promo = NULL;
	const cJSON *is_staff = NULL;
	const cJSON *seat = NULL;
	const cJSON *show_plate_number = NULL;
	const cJSON *plate = NULL;
	
	const cJSON *from_nation = NULL;
	const cJSON *to_nation = NULL;
	const cJSON *passport = NULL;
	const cJSON *nationality = NULL;
	const cJSON *gender = NULL;
	
	const cJSON *expires = NULL;
	const cJSON *card = NULL;
	const cJSON *card_trips = NULL;
	
	const char s[2] = "-";
	const char p[2] = "\n";
	char *token;
	int paid;
	int discountInt;
	int priceInt;
	
	root = cJSON_Parse(json);
	if(root == NULL)
	{
		show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
		return -1;
	}
	//check success
	success = cJSON_GetObjectItemCaseSensitive(root, "success");
	message = cJSON_GetObjectItemCaseSensitive(root, "message");
	
	if(cJSON_IsBool(success) && cJSON_IsFalse(success))
	{
		show_message("TICKET ERROR", message->valuestring, 1);
		cJSON_Delete(root);
		return -1;
	}
	else if(!cJSON_IsBool(success))
	{
		show_message("TICKET ERROR","INVALID RESPONSE FOUND", 1);
		cJSON_Delete(root);
		return -1;
	}
	//Ticket thing...
	common = cJSON_GetObjectItemCaseSensitive(message, "common");
	tickets = cJSON_GetObjectItemCaseSensitive(message, "tickets");
	
	//Get CC Details
	cc = cJSON_GetObjectItemCaseSensitive(common, "cc");
	pos = cJSON_GetObjectItemCaseSensitive(common, "pos");
	
	journey = cJSON_GetObjectItemCaseSensitive(common, "journey");
	route = cJSON_GetObjectItemCaseSensitive(common, "route");
	
	date = cJSON_GetObjectItemCaseSensitive(common, "date");
	time = cJSON_GetObjectItemCaseSensitive(common, "time");
	
	price = cJSON_GetObjectItemCaseSensitive(common, "price");
	currency = cJSON_GetObjectItemCaseSensitive(common, "currency");
	generated = cJSON_GetObjectItemCaseSensitive(common, "generated");
	
	prnInit(); //init printer
	
	//BOOKING
	cJSON_ArrayForEach(ticket, tickets)
	{
		tid = cJSON_GetObjectItemCaseSensitive(ticket, "id");
		seat = cJSON_GetObjectItemCaseSensitive(ticket, "seat");
		
		setNormal();
		prnSetSpace(0, 4);
		
		if(type == RESERVE_TICKET)
		{
			expires = cJSON_GetObjectItemCaseSensitive(common, "expires");
			
			pntDrawPicture(0,0, 385,80, "logo.bmp");
			
			prnPrintf("TEL: %s\nPOS:%s", cc->valuestring, pos->valuestring);
			prnPrintf("\n------------------------------\n");
			
			prnPrintf("%s\n", journey->valuestring);
			prnPrintf("%s\n\n", route->valuestring);
			
			setNormal();//reset to normal font size
			prnPrintf("%s    %s\n\n", date->valuestring, time->valuestring);
			
			prnPrintf("Booking No:%s\n", tid->valuestring);
			prnPrintf("Expires:%s\n", expires->valuestring);
			prnPrintf("Price:%d %s    Seat:%d\n", price->valuestring, currency->valuestring, seat->valueint);
			prnPrintf("------------------------------\nPrinted %s\nPowered by LAP Ltd\n\n\n\n", generated->valuestring);
		}
		else
		{
			discount = cJSON_GetObjectItemCaseSensitive(common, "discount");
			discountInt = cJSON_IsString(discount) ? atoi(discount->valuestring) : discount->valueint;
			priceInt = cJSON_IsString(price) ? atoi(price->valuestring) : price->valueint;
			
			paid = priceInt ;//- discountInt;
			
			//split customer and number aka CODE
			name = cJSON_GetObjectItemCaseSensitive(common, "name");
			show_plate_number = cJSON_GetObjectItemCaseSensitive(common, "show_plate");
			// get the name //
			token = strtok(name->valuestring, s);
			if(token != NULL)
				strcpy(username, token);
			//Mobile
			token = strtok(NULL, s);
			if(token != NULL)
				strcpy(code, token);
				
			//Route and Plate no
			token = NULL;
			token = strtok(route->valuestring, p);
			if(token != NULL)
				strcpy(routeName, token);
			//plate
			token = strtok(NULL, s);
			if(token != NULL)
			{
				token = token+5; //skip BUS:<space>
				strcpy(plateNumber, token);
			}
			
			//agent and gen time
			//Route and Plate no
			token = NULL;
			token = strtok(generated->valuestring, p);
			if(token != NULL)
				strcpy(genTime, token);
			//Mobile
			token = strtok(NULL, s);
			if(token != NULL)
			{
				token = token+9; //skip CASHIER:<space>
				strcpy(agent, token);
			}
			
			//int  prnSetFont(const char *pFontName, const char *pFontCharacter, int Style, int Size, int FontPriority);
			setNormal();
			pntDrawPicture(0,0, 385,80, "logo.bmp");
			prnSetSpace(0, 0);
			
			prnPrintf("TEL: %s\n", cc->valuestring);
			prnPrintf("\n------------------------------\n");
			
			prnPrintf("Name:%s\n", username);
			
			points = cJSON_GetObjectItemCaseSensitive(ticket, "points");
			prnPrintf("Points:%s\n", points->valuestring);
			prnPrintf("Destination: %s\n", journey->valuestring);
			prnPrintf("Departure: %s  %s\n", date->valuestring, time->valuestring);
			prnPrintf("T.ID: %s\n", tid->valuestring);
			prnPrintf("POS ID: %s\n", pos->valuestring);
			
			if(show_plate_number->valueint == 1)
				prnPrintf("Plate: %s\n", plateNumber);
				
			prnPrintf("Agent: %s\n", agent);
			prnPrintf("TT: %s\n", genTime);
			
			prnPrintf("Seat: %d\n", seat->valueint);
			setBold(); //set Bolder
			prnPrintf("\nPrice: %d %s\n", paid, currency->valuestring);
			setNormal();//reset to normal font size
			
			//Ticket
			is_promo = cJSON_GetObjectItemCaseSensitive(ticket, "is_promo");
			is_staff = cJSON_GetObjectItemCaseSensitive(common, "is_staff");
			is_intl = cJSON_GetObjectItemCaseSensitive(common, "is_intl");
			
			if(is_promo->valueint==1)
			{
				setBold(); //set Bolder
				prnPrintf("PROMO TICKET\n\n");
				setNormal();//reset to normal font size
			}
			else if(is_staff->valueint==1)
			{
				setBold(); //set Bolder
				prnPrintf("STAFF. Discount %d %s\n\n", discountInt, currency->valuestring);
				setNormal();//reset to normal font size
			}
			else
			{
				if(discountInt>0)
				{
					setBold(); //set Bolder
					prnPrintf("DISCOUNT: %d %s\n\n", discountInt, currency->valuestring);
					setNormal();//reset to normal font size
				}
			}
			
			if(type == SELL_CARD_TICKET)
			{
				card_trips = cJSON_GetObjectItemCaseSensitive(common, "remaining");
				card = cJSON_GetObjectItemCaseSensitive(common, "card");
				//print card info
				prnPrintf("Card:%s     Remains %d Trips\n", card->valuestring, card_trips->valueint);
			}
			else if(type == GATEWAY_TICKET)
			{
				const cJSON *bal = NULL;
				const cJSON *receipt = NULL;
				bal = cJSON_GetObjectItemCaseSensitive(common, "balance");
				receipt = cJSON_GetObjectItemCaseSensitive(common, "receipt");
				card = cJSON_GetObjectItemCaseSensitive(common, "card");
				prnPrintf("Card: %s \nCard Balance: %s%s\n", card->valuestring, bal->valuestring, currency->valuestring);
			}
			
			if(is_intl->valueint==1)
			{
				passport = cJSON_GetObjectItemCaseSensitive(common, "passport");
				gender = cJSON_GetObjectItemCaseSensitive(common, "gender");
				nationality = cJSON_GetObjectItemCaseSensitive(common, "nationality");
				from_nation = cJSON_GetObjectItemCaseSensitive(common, "from_nation");
				to_nation = cJSON_GetObjectItemCaseSensitive(common, "to_nation");
				
				prnPrintf("Passport:%s\n\n", passport->valuestring);
				
				prnPrintf("Gender:%s from %s\n\n", gender->valuestring, nationality->valuestring);
				prnPrintf("Travelling: %s to %s\n", from_nation->valuestring, to_nation->valuestring);
			}
			prnPrintf("------------------------------\n");
			
			prnPrintf("Non-refundable\nUkererewe ntasubizwa\n------------------------------\n");
			prnPrintf("         Driver's Check");
			prnPrintf("\n------------------------------\n");
			//Driver's Piece
			prnPrintf("%s\n", routeName);
			prnPrintf("------------------------------\n");
			
			setNormal();//reset to normal font size
			prnPrintf("Name:%s\n", username);
			
			points = cJSON_GetObjectItemCaseSensitive(ticket, "points");
			prnPrintf("Points:%s\n", points->valuestring);
			prnPrintf("Destination: %s\n", journey->valuestring);
			prnPrintf("Departure: %s  %s\n", date->valuestring, time->valuestring);
			prnPrintf("T.ID: %s\n", tid->valuestring);
			prnPrintf("POS ID: %s\n", pos->valuestring);
			
			if(show_plate_number->valueint == 1)
				prnPrintf("Plate: %s\n", plateNumber);
				
			prnPrintf("Agent: %s\n", agent);
			prnPrintf("TT: %s\n", genTime);
			
			
			prnPrintf("Seat: %d\n", seat->valueint);
			setBold(); //set Bolder
			prnPrintf("\nPrice: %d %s\n", paid, currency->valuestring);
			setNormal();//reset to normal font size
			
			//Ticket
			is_promo = cJSON_GetObjectItemCaseSensitive(ticket, "is_promo");
			is_staff = cJSON_GetObjectItemCaseSensitive(common, "is_staff");
			is_intl = cJSON_GetObjectItemCaseSensitive(common, "is_intl");
			
			if(is_promo->valueint==1)
			{
				setBold(); //set Bolder
				prnPrintf("PROMO TICKET\n\n");
				setNormal();//reset to normal font size
			}
			else if(is_staff->valueint==1)
			{
				setBold(); //set Bolder
				prnPrintf("STAFF. Discount %d %s\n\n", discountInt, currency->valuestring);
				setNormal();//reset to normal font size
			}
			else
			{
				if(discountInt>0)
				{
					setBold(); //set Bolder
					prnPrintf("DISCOUNT: %d %s\n\n", discountInt, currency->valuestring);
					setNormal();//reset to normal font size
				}
			}
			
			if(type == SELL_CARD_TICKET)
			{
				card_trips = cJSON_GetObjectItemCaseSensitive(common, "remaining");
				card = cJSON_GetObjectItemCaseSensitive(common, "card");
				//print card info
				prnPrintf("Card:%s     Remains %d Trips\n", card->valuestring, card_trips->valueint);
			}
			else if(type == GATEWAY_TICKET)
			{
				const cJSON *bal = NULL;
				const cJSON *receipt = NULL;
				bal = cJSON_GetObjectItemCaseSensitive(common, "balance");
				receipt = cJSON_GetObjectItemCaseSensitive(common, "receipt");
				card = cJSON_GetObjectItemCaseSensitive(common, "card");
				prnPrintf("Card: %s \nCard Balance: %s%s\n", card->valuestring, bal->valuestring, currency->valuestring);
			}
			
			if(is_intl->valueint==1)
			{
				passport = cJSON_GetObjectItemCaseSensitive(common, "passport");
				gender = cJSON_GetObjectItemCaseSensitive(common, "gender");
				nationality = cJSON_GetObjectItemCaseSensitive(common, "nationality");
				from_nation = cJSON_GetObjectItemCaseSensitive(common, "from_nation");
				to_nation = cJSON_GetObjectItemCaseSensitive(common, "to_nation");
				
				prnPrintf("Passport:%s\n\n", passport->valuestring);
				
				prnPrintf("Gender:%s from %s\n\n", gender->valuestring, nationality->valuestring);
				prnPrintf("Travelling: %s to %s\n", from_nation->valuestring, to_nation->valuestring);
			}
			
			prnPrintf("------------------------------\nPowered by LAP Ltd\n\n\n\n"); //\f new page
			
		}
	}
	
	print_status = prnStatus();
	while(print_status != PRN_OK)
	{
		if(print_status == PRN_PAPEROUT)
		{
			show_message("PRINTER ERROR",  "CHECK PAPERS", 1);
		}
		else if(print_status == PRN_UNFINISHED)
		{
			show_message("PRINTER ERROR",  "UNFINISHED TRY AGAIN", 1);
		}
		else
		{
			show_message("PRINTER ERROR",  "UNKNOWN ERROR", 1);
		}
		print_status = prnStatus();
		setNormal();
	}
	
	// do print ;
	print_status = prnStart();
	while(print_status != PRN_OK)
	{
		if(print_status == PRN_PAPEROUT)
		{
			show_message("PRINTER ERROR",  "CHECK PAPERS", 1);
		}
		else if(print_status == PRN_UNFINISHED)
		{
			show_message("PRINTER ERROR",  "UNFINISHED TRY AGAIN", 1);
		}
		else
		{
			show_message("PRINTER ERROR",  "UNKNOWN ERROR", 1);
		}
		print_status = prnStatus();
	}
	//printed, remove ticket_serial
	generate_ticket_serial();
	prnSetFontZoom(1, 1);
	cJSON_Delete(root);
	return -1;
}


int print_ticket_reserved(const char* json)
{
	int32_t	print_status;
	
	char username[100];
	char code[100];
	char routeName[100];
	char plateNumber[100];
	char agent[100];
	char genTime[100];
	
	cJSON *root = NULL;
	const cJSON *message = NULL;
	const cJSON *success = NULL;
	const cJSON *cc = NULL;
	const cJSON *tid = NULL;
	const cJSON *price = NULL;
	const cJSON *route = NULL;
	const cJSON *journey = NULL;
	const cJSON *date = NULL;
	const cJSON *time = NULL;
	const cJSON *pos = NULL;
	const cJSON *name = NULL;
	const cJSON *generated = NULL;
	const cJSON *is_intl = NULL;
	const cJSON *currency = NULL;
	const cJSON *points = NULL;
	const cJSON *discount = NULL;
	const cJSON *is_promo = NULL;
	const cJSON *is_staff = NULL;
	const cJSON *seat = NULL;
	const cJSON *show_plate_number = NULL;
	const cJSON *plate = NULL;
	
	const cJSON *from_nation = NULL;
	const cJSON *to_nation = NULL;
	const cJSON *passport = NULL;
	const cJSON *nationality = NULL;
	const cJSON *gender = NULL;
	
	
	const char s[2] = "-";
	const char p[2] = "\n";
	char *token;
	int paid;
	
	root = cJSON_Parse(json);
	if(root == NULL)
	{
		show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
		return -1;
	}
	//check success
	success = cJSON_GetObjectItemCaseSensitive(root, "success");
	message = cJSON_GetObjectItemCaseSensitive(root, "message");
	
	if(cJSON_IsBool(success) && cJSON_IsFalse(success))
	{
		show_message("TICKET ERROR", message->valuestring, 1);
		cJSON_Delete(root);
		return -1;
	}
	else if(!cJSON_IsBool(success))
	{
		show_message("TICKET ERROR","INVALID RESPONSE FOUND", 1);
		cJSON_Delete(root);
		return -1;
	}
	
	//Get CC Details
	cc = cJSON_GetObjectItemCaseSensitive(message, "cc");
	pos = cJSON_GetObjectItemCaseSensitive(message, "pos");
	
	journey = cJSON_GetObjectItemCaseSensitive(message, "journey");
	route = cJSON_GetObjectItemCaseSensitive(message, "route");
	date = cJSON_GetObjectItemCaseSensitive(message, "date");
	time = cJSON_GetObjectItemCaseSensitive(message, "time");
	
	price = cJSON_GetObjectItemCaseSensitive(message, "paid"); //NOTE it was price
	currency = cJSON_GetObjectItemCaseSensitive(message, "currency");
	generated = cJSON_GetObjectItemCaseSensitive(message, "generated");
	
	prnInit(); //init printer
	//BOOKING
	tid = cJSON_GetObjectItemCaseSensitive(message, "id");
	seat = cJSON_GetObjectItemCaseSensitive(message, "seat");
	discount = cJSON_GetObjectItemCaseSensitive(message, "discount");
	paid = price->valueint; //-discount->valueint;
	
	//split customer and number aka CODE
	name = cJSON_GetObjectItemCaseSensitive(message, "name");
	show_plate_number = cJSON_GetObjectItemCaseSensitive(message, "show_plate");
	// get the name //
	token = strtok(name->valuestring, s);
	if(token != NULL)
		strcpy(username, token);
	//Mobile
	token = strtok(NULL, s);
	if(token != NULL)
		strcpy(code, token);
		
	//Route and Plate no
	token = NULL;
	token = strtok(route->valuestring, p);
	if(token != NULL)
		strcpy(routeName, token);
	//plate
	token = strtok(NULL, s);
	if(token != NULL)
	{
		token = token+5; //skip BUS:<space>
		strcpy(plateNumber, token);
	}
	
	//agent and gen time
	//Route and Plate no
	token = NULL;
	token = strtok(generated->valuestring, p);
	if(token != NULL)
		strcpy(genTime, token);
	//Mobile
	token = strtok(NULL, s);
	if(token != NULL)
	{
		token = token+9; //skip CASHIER:<space>
		strcpy(agent, token);
	}
	
	setNormal();
	pntDrawPicture(0,0, 385,80, "logo.bmp");
	prnSetSpace(0, 0);
	
	prnPrintf("TEL: %s\n", cc->valuestring);
	prnPrintf("\n------------------------------\n");
	
	prnPrintf("Name:%s\n", username);
	
	points = cJSON_GetObjectItemCaseSensitive(message, "points");
	
	prnPrintf("Points:%s\n", points->valuestring);
	prnPrintf("Destination: %s\n", journey->valuestring);
	prnPrintf("Departure: %s  %s\n", date->valuestring, time->valuestring);
	prnPrintf("T.ID: %s\n", tid->valuestring);
	prnPrintf("POS ID: %s\n", pos->valuestring);
	
	if(show_plate_number->valueint == 1)
		prnPrintf("Plate: %s\n", plateNumber);
		
	prnPrintf("Agent: %s\n", agent);
	prnPrintf("TT: %s\n", genTime);
	
	prnPrintf("Seat: %d\n", seat->valueint);
	setBold(); //set Bolder
	prnPrintf("\nPrice: %d %s\n", paid, currency->valuestring);
	setNormal();//reset to normal font size
	//Ticket
	is_promo = cJSON_GetObjectItemCaseSensitive(message, "is_promo");
	is_staff = cJSON_GetObjectItemCaseSensitive(message, "is_staff");
	is_intl = cJSON_GetObjectItemCaseSensitive(message, "is_intl");
	
	if(is_promo->valueint==1)
	{
		setBold(); //set Bolder
		prnPrintf("PROMO TICKET\n\n");
		setNormal();//reset to normal font size
		
	}
	else if(is_staff->valueint==1)
	{
		setBold(); //set Bolder
		prnPrintf("STAFF. DISC %s %s\n\n", discount->valuestring, currency->valuestring);
		setNormal();//reset to normal font size
	}
	else if(discount->valueint>0)
	{
		setBold(); //set Bolder
		prnPrintf("DISCOUNT: %s %s\n\n", discount->valuestring, currency->valuestring);
		setNormal();//reset to normal font size
	}
	if(is_intl->valueint==1)
	{
		passport = cJSON_GetObjectItemCaseSensitive(message, "passport");
		gender = cJSON_GetObjectItemCaseSensitive(message, "gender");
		nationality = cJSON_GetObjectItemCaseSensitive(message, "nationality");
		from_nation = cJSON_GetObjectItemCaseSensitive(message, "from_nation");
		to_nation = cJSON_GetObjectItemCaseSensitive(message, "to_nation");
		
		prnPrintf("Passport:%s\n\n", passport->valuestring);
		prnPrintf("Gender:%s from %s\n\n", gender->valuestring, nationality->valuestring);
		prnPrintf("Travelling: %s to %s\n", from_nation->valuestring, to_nation->valuestring);
		prnPrintf("------------------------------\n");
	}
	
	prnPrintf("------------------------------\n");
	
	prnPrintf("Non-refundable\nUkererewe ntasubizwa\n------------------------------\n");
	prnPrintf("         Driver's Check");
	prnPrintf("\n------------------------------\n");
	//Driver's Piece
	prnPrintf("%s\n", routeName);
	prnPrintf("------------------------------\n");
	prnPrintf("Name:%s\n", username);
	
	points = cJSON_GetObjectItemCaseSensitive(message, "points");
	
	prnPrintf("Points:%s\n", points->valuestring);
	prnPrintf("Destination: %s\n", journey->valuestring);
	prnPrintf("Departure: %s  %s\n", date->valuestring, time->valuestring);
	prnPrintf("T.ID: %s\n", tid->valuestring);
	prnPrintf("POS ID: %s\n", pos->valuestring);
	
	if(show_plate_number->valueint == 1)
		prnPrintf("Plate: %s\n", plateNumber);
		
	prnPrintf("Agent: %s\n", agent);
	prnPrintf("TT: %s\n", genTime);
	
	prnPrintf("Seat: %d\n", seat->valueint);
	setBold(); //set Bolder
	prnPrintf("\nPrice: %d %s\n", paid, currency->valuestring);
	setNormal();//reset to normal font size
	//Ticket
	is_promo = cJSON_GetObjectItemCaseSensitive(message, "is_promo");
	is_staff = cJSON_GetObjectItemCaseSensitive(message, "is_staff");
	is_intl = cJSON_GetObjectItemCaseSensitive(message, "is_intl");
	
	if(is_promo->valueint==1)
	{
		setBold(); //set Bolder
		prnPrintf("PROMO TICKET\n\n");
		setNormal();//reset to normal font size
		
	}
	else if(is_staff->valueint==1)
	{
		setBold(); //set Bolder
		prnPrintf("STAFF. DISC %s %s\n\n", discount->valuestring, currency->valuestring);
		setNormal();//reset to normal font size
	}
	else if(discount->valueint>0)
	{
		setBold(); //set Bolder
		prnPrintf("DISCOUNT: %s %s\n\n", discount->valuestring, currency->valuestring);
		setNormal();//reset to normal font size
	}
	
	if(is_intl->valueint==1)
	{
		passport = cJSON_GetObjectItemCaseSensitive(message, "passport");
		gender = cJSON_GetObjectItemCaseSensitive(message, "gender");
		nationality = cJSON_GetObjectItemCaseSensitive(message, "nationality");
		from_nation = cJSON_GetObjectItemCaseSensitive(message, "from_nation");
		to_nation = cJSON_GetObjectItemCaseSensitive(message, "to_nation");
		
		prnPrintf("Passport:%s\n\n", passport->valuestring);
		prnPrintf("Gender:%s from %s\n\n", gender->valuestring, nationality->valuestring);
		prnPrintf("Travelling: %s to %s\n", from_nation->valuestring, to_nation->valuestring);
		prnPrintf("------------------------------\n");
	}
	
	prnPrintf("------------------------------\nPowered by LAP Ltd\n\n\n\n"); //\f new page
	
	print_status = prnStatus();
	while(print_status != PRN_OK)
	{
		if(print_status == PRN_PAPEROUT)
		{
			show_message("PRINTER ERROR",  "CHECK PAPERS", 1);
		}
		else if(print_status == PRN_UNFINISHED)
		{
			show_message("PRINTER ERROR",  "UNFINISHED TRY AGAIN", 1);
		}
		print_status = prnStatus();
	}
	prnStart();// do print ;
	prnSetFontZoom(1, 1);
	cJSON_Delete(root);
	return 0;
}

int collect_ticket_details(char* buffer, ActionType type)
{
	const char* currency_list[] =
	{
		"SELECT CURRENCY",
		"1. RWANDAN FRANCS",
		"2. UGANDA SHILLINGS",
		"3. BURUNDIAN FRANCS",
		"4. US DOLLARS",
	};
	const char* currency_short_list[] = {"RWF", "UGS","FIB", "USD"};
	int selected_currency = 0;
	
	char plate_number[30];
	char capacity[50]="29";
	char ticket_serial[100];
	
	char currency[6];
	char discount[3] =  {"0"};
	char *json_output;
	//stops
	char start[100];
	char end[100];
	int is_start_intl = 0;
	int is_end_intl = 0;
	int route = 0;
	
	const char title_start[] = {"COMING FROM"};
	const char title_end[] = {"GOING TO"};
	
	//date
	char day[3];
	char month[3];
	char year[5];
	//time
	char hour[3];
	char min[3];
	char imei[30];
	char seat[50] =  {"0"};
	
	//customer
	char cnumber[50];
	char cname[100] = {0};
	char card_no[100] = {0};
	//intl customer
	char nationality[50] = {0};
	char passport[50] = {0};
	char from_nation[50] = {0};
	char to_nation[50] = {0};
	char gender[3] = "-1";
	char age[3];
	
	char full_date[20];
	char full_dt_time[20];
	
	int keyCode;
	struct tm *timeinfo;
	time_t rawtime;
	
	cJSON *root;
	cJSON *customer;
	
	is_start_intl = get_stop(title_start, start);
	if(is_start_intl == -1)
		return -1;
		
	is_end_intl = get_stop(title_end, end);
	if(is_end_intl == -1)
		return -1;
		
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	//Departure Day
	memset(day, 0x00, sizeof(day));
	show_title("DEPARTURE DAY");
	
	sprintf(day, "%02d", timeinfo->tm_mday);
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 1, 2, -1, day) ;
	if(keyCode == KB_CANCEL)
		return -1;
		
	//Departure Month
	memset(month, 0x00, sizeof(month));
	show_title("DEPARTURE MONTH");
	sprintf(month, "%02d", timeinfo->tm_mon+1);
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 1, 2, -1, month) ;
	if(keyCode == KB_CANCEL)
		return -1;
		
	//Departure Year
	show_title("DEPARTURE YEAR ");
	memset(year, 0x00, sizeof(year));
	sprintf(year, "%4d", timeinfo->tm_year+1900);
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 4, 4, -1, year) ;
	if(keyCode == KB_CANCEL)
		return -1;
		
	//Departure Hour
	show_title("DEPARTURE HOUR");
	memset(hour, 0x00, sizeof(hour));
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 2, 2, -1, hour) ;
	if(keyCode == KB_CANCEL)
		return -1;
		
	//Departure Minutes
	show_title("DEPARTURE MIN");
	memset(min, 0x00, sizeof(min));
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 2, 2, -1, min) ;
	if(keyCode == KB_CANCEL)
		return -1;
		
	if(type == BUS_PLAN)
	{
		show_title("BUS PLATE NUMBER");
		memset(plate_number, 0x00, sizeof(plate_number));
		keyCode = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 4,20, -1, plate_number) ;
		if(keyCode == KB_CANCEL)
			return -1;
	}
	
	route =  get_route_id("BUS ROUTE");
	
	if(type==SEAT_REPORT || type == BUS_PLAN)
	{
	
	}
	else if(type==SELL_CARD_TICKET || type==GATEWAY_TICKET)
	{
		show_title("CARD NUMBER");
		keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 1, 20, -1, card_no) ;
		if(keyCode == KB_CANCEL)
		{
			return -1;
		}
		
		if(type==GATEWAY_TICKET)
		{
			//currency
			show_title("SELECT CURRENCY");
			selected_currency = DispMenu(currency_list,5,1,KEY0, KEY8, 60);
			if(selected_currency==-1)
				return -1;
				
			memset(currency, 0x00, sizeof(currency));
			strcpy(currency, currency_short_list[selected_currency]);
		}
	}
	else
	{
		show_title("SEAT NUMBER");
		memset(seat, 0x00, sizeof(seat));
		sprintf(seat, "%d", 0);
		keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 1, 3, -1, seat) ;
		if(keyCode == KB_CANCEL)
			return -1;
			
		show_title("SELECT CURRENCY");
		selected_currency = DispMenu(currency_list,5,1,KEY0, KEY8, 60);
		if(selected_currency==-1)
			return -1;
			
		memset(currency, 0x00, sizeof(currency));
		strcpy(currency, currency_short_list[selected_currency]);
		
		//Customer Details
		//Name
		show_title("CUSTOMER NAME");
		memset(cname, 0x00, sizeof(cname));
		keyCode = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 0,100, -1, cname) ;
		if(keyCode == KB_CANCEL)
			return -1;
			
		//Mobile
		show_title("CUSTOMER NUMBER");
		memset(cnumber, 0x00, sizeof(cnumber));
		keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 8,20, -1, cnumber) ;
		if(keyCode == KB_CANCEL)
			return -1;
			
		//for intl
		if(is_end_intl || is_start_intl)
		{
			memset(age, 0x00, sizeof(age));
			memset(from_nation, 0x00, sizeof(from_nation));
			memset(to_nation, 0x00, sizeof(to_nation));
			
			//nationality
			show_title("NATIONALITY");
			keyCode = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 0,50, -1, nationality) ;
			if(keyCode == KB_CANCEL)
				return -1;
				
			//passport
			show_title("PASSPORT");
			keyCode = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 0,50, -1, passport) ;
			if(keyCode == KB_CANCEL)
				return -1;
				
			//gender
			keyCode = get_gender();
			if(keyCode == KEY_CANCEL)
				return -1;
			else if(keyCode != KEY_ENTER)
				sprintf(gender,"%d", keyCode);
				
			//age
			show_title("YOUR AGE");
			keyCode = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 0,2, -1, age) ;
			if(keyCode == KB_CANCEL)
				return -1;
			//from_nation
			show_title("COMING FROM");
			keyCode = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 0,50, -1, from_nation) ;
			if(keyCode == KB_CANCEL)
				return -1;
			//to_nation
			show_title("GOING TO");
			keyCode = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 0,50, -1, to_nation) ;
			if(keyCode == KB_CANCEL)
				return -1;
		}
		
	}	//date
	sprintf(full_date, "%s-%s-%s", day, month, year);
	//time
	sprintf(full_dt_time, "%sH%s", hour, min);
	//Serial Number
	memset(imei, 0x00, sizeof(imei));
	sys_get_sn(imei, 30);
	
	/* create root node and customer */
	root = cJSON_CreateObject();
	if(root == NULL)
	{
		show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
		return -1;
	}
	
	customer = cJSON_CreateObject();
	if(customer == NULL)
	{
		show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
		return -1;
	}
	// add customer to root
	cJSON_AddItemToObject(root, "customer", customer);
	
	//append to root
	cJSON_AddItemToObject(root, "start", cJSON_CreateString(start));
	cJSON_AddItemToObject(root, "end", cJSON_CreateString(end));
	cJSON_AddItemToObject(root, "date", cJSON_CreateString(full_date));
	cJSON_AddItemToObject(root, "time", cJSON_CreateString(full_dt_time));
	cJSON_AddItemToObject(root, "seat", cJSON_CreateString(seat));
	cJSON_AddItemToObject(root, "currency", cJSON_CreateString(currency));
	cJSON_AddItemToObject(root, "discount", cJSON_CreateString(discount));
	cJSON_AddItemToObject(root, "pos", cJSON_CreateString(imei));
	cJSON_AddItemToObject(root, "route", cJSON_CreateNumber(route));
	cJSON_AddItemToObject(root, "card", cJSON_CreateString(card_no));
	
	cJSON_AddItemToObject(root, "plateNo", cJSON_CreateString(plate_number));
	cJSON_AddItemToObject(root, "capacity", cJSON_CreateString(capacity));
	
	//add to customer
	cJSON_AddItemToObject(customer, "name", cJSON_CreateString(cname));
	cJSON_AddItemToObject(customer, "number", cJSON_CreateString(cnumber));
	cJSON_AddItemToObject(customer, "nationality", cJSON_CreateString(nationality));
	cJSON_AddItemToObject(customer, "passport", cJSON_CreateString(passport));
	cJSON_AddItemToObject(customer, "gender", cJSON_CreateString(gender));
	cJSON_AddItemToObject(customer, "age", cJSON_CreateString(age));
	cJSON_AddItemToObject(customer, "from_nation", cJSON_CreateString(from_nation));
	cJSON_AddItemToObject(customer, "to_nation", cJSON_CreateString(to_nation));
	
	if(type == SELL_TICKET)
	{
		//check if has serial
		memset(ticket_serial, 0x00, sizeof(ticket_serial));
		if(read_from_file(TICKET_SERIAL_FILE, ticket_serial) != 0)
		{
			show_message("TICKET SERIAL ERROR", "COULD NOT READ SERIAL", 1);
		}
		//add serial
		cJSON_AddItemToObject(root, "ticket_serial", cJSON_CreateString(ticket_serial));
	}
	
	json_output = cJSON_PrintUnformatted(root);
	sprintf(buffer, "%s", json_output);
	free(json_output);
	cJSON_Delete(root);
	
	return 0;
}

void create_ticket(ActionType type)
{
	char url[250];
	char token[250];
	char action[20];
	char ticket_json[4096];
	char buffer[4096];
	
	char booking_no[50];
	char pos[50];
	int retKey;
	
	memset(ticket_json, 0x00, sizeof(ticket_json));
	memset(buffer, 0x00, sizeof(buffer));
	memset(url, 0x00, sizeof(url));
	memset(token, 0x00, sizeof(token));
	memset(action, 0x00, sizeof(action));
	
	memset(booking_no, 0x00, sizeof(booking_no));
	memset(pos, 0x00, sizeof(pos));
	
	if(read_from_file(TOKEN_FILE, token)==0)
	{
		if(type == PAY_RESERVATION)
		{
			show_title("BOOKING NUMBER");
			retKey = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 0, 11, -1, booking_no);
			if(retKey != KB_CANCEL)
			{
				sysReadSN(pos);
				sprintf(url, "%s/index.php/v1/tickets/pay-booking?pos=%s&id=%s&access-token=%s", BASEURL, pos, booking_no, token);
				
				if(get_url(url, buffer)==0)
					print_ticket_reserved(buffer);
				else
					show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
			}
		}
		else if(type == SELL_CARD_TICKET)
		{
			//collect details
			if(collect_ticket_details(ticket_json, type)==0)
			{
				sprintf(url, "%s/index.php/v1/tickets/sell-card-tickets?count=%d&access-token=%s", BASEURL, 1, token);
				if(post_url(url, ticket_json, buffer)==0)
				{
					print_ticket(buffer, type);
				}
				else
					show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
			}
			else
				show_message("DATA COLLECT ERROR!", "INCOMPLETE TICKET DATA", 1);
		}
		else if(type == GATEWAY_TICKET)
		{
			//collect details
			if(collect_ticket_details(ticket_json, type)==0)
			{
				sprintf(url, "%s/index.php/v1/tickets/sell-gateway-tickets?count=%d&access-token=%s", BASEURL, 1, token);
				if(post_url(url, ticket_json, buffer)==0)
				{
					print_ticket(buffer, type);
				}
				else
					show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
			}
			else
				show_message("DATA COLLECT ERROR!", "INCOMPLETE TICKET DATA", 1);
		}
		else
		{
			//collect details
			if(collect_ticket_details(ticket_json, type)==0)
			{
				if(type==SELL_TICKET)
					sprintf(action, "%s", "buy");
				else
					sprintf(action, "%s", "booking");
					
				sprintf(url, "%s/index.php/v1/tickets/tickets?action=%s&count=%d&access-token=%s", BASEURL, action, 1, token);
				if(post_url(url, ticket_json, buffer)==0)
					print_ticket(buffer, type);
				else
					show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
			}
			else
				show_message("DATA COLLECT ERROR!", "INCOMPLETE TICKET DATA", 1);
		}
	}
	else
		show_message("COULD NOT READ THE SESSION", "PLEASE LOGIN AGAIN", 1);
}

int get_stop(const char* title, char *buffer)
{
	char stops_json[4096];
	cJSON* root_object = NULL;
	const cJSON *stops = NULL;
	const cJSON *stop = NULL;
	const cJSON *name = NULL;
	const cJSON *is_intl = NULL;
	char* stop_name_list[30];
	int stop_status_list[30];
	int stops_size;
	int selected=0;
	int idx = 1;
	int is_intl_ret = -1;
	int i=1;
	
	//Tokenizing and removing {d} -
	const char s[5] = {" - "};
	char *token;
	char token_src[30];
	char stop_name_tmp[30];
	
	memset(stops_json, 0x00, sizeof(stops_json));
	if(read_from_file(STOPS_FILE, stops_json)==0)
	{
		root_object = cJSON_Parse(stops_json); // routes and stops are two
		if(root_object == NULL)
		{
			show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
			return -1;
		}
		stops = cJSON_GetObjectItem(root_object,"stops");
		stops_size = cJSON_GetArraySize(stops);
		
		stop_name_list[0] = (char*) calloc(strlen(title), sizeof(char));
		strcpy(stop_name_list[0], title);
		
		cJSON_ArrayForEach(stop, stops)
		{
			name = cJSON_GetObjectItemCaseSensitive(stop, "start");
			is_intl = cJSON_GetObjectItemCaseSensitive(stop, "is_intl");
			if(!cJSON_IsString(is_intl))
			{
				show_message("PARSE ERROR","IS Intl Should be a String", 1);
				cJSON_Delete(root_object);
				return -1;
			}
			
			if(!cJSON_IsString(name))
			{
				show_message("PARSE ERROR","Name must be a String", 1);
				cJSON_Delete(root_object);
				return -1;
			}
			
			if(idx>8)
				idx = 1;
				
			strcpy(stop_name_tmp, name->valuestring);
			stop_name_list[i] = (char*) calloc(strlen(stop_name_tmp)+5, sizeof(char)); //name + sprintf inserted values
			sprintf(stop_name_list[i], "%d - %s", idx, stop_name_tmp);
			
			stop_status_list[i-1] = atoi((char*)is_intl->valuestring); // i starts with 1 so need to reset for status
			i++;
			idx++;
		}
		
		while(selected>=0)
		{
			selected = DispMenu(stop_name_list,stops_size+1,1,KEY0, KEY8, 60);
			if(selected==-1)
				return -1;
			else if(selected<stops_size)
				selected++;
			break;
		}
		/* get the first token */
		strcpy(token_src, stop_name_list[selected]);
		token = strtok(token_src, s);
		/* walk through other tokens */
		if(token != NULL)
			token = strtok(NULL, s);
			
		memset(buffer, 0x00, sizeof(buffer));
		strcpy(buffer, token);
		is_intl_ret = stop_status_list[selected-1];
		//free list
		cJSON_Delete(root_object);
		
		for(i = 0; i < stops_size+1; i++)
		{
			free(stop_name_list[i]);
		}
	}
	return is_intl_ret;
}

int get_gender(void)
{
	int32_t iRet;
	int gender_int;
	
	lcdCls();
	
	lcdSetFont(font_arial, "UTF-8", 0,  20, 0);
	lcdDisplay(0,0,DISP_CFONT|DISP_MEDIACY|DISP_INVLINE,"CUSTOMER GENDER");
	lcdDisplay(0,3,DISP_CFONT|DISP_MEDIACY,"1 - MALE");
	lcdDisplay(0,6,DISP_CFONT|DISP_MEDIACY, "2 - FEMALE");
	
	lcdFlip();
	lcdGoto(0, 6);
	iRet = kbGetKey();
	if(iRet==KEY1)
		gender_int = 1;
	else if(iRet==KEY2)
		gender_int = 2;
	else
		gender_int = -1;
		
	return gender_int;
}

int get_route_id(const char* title)
{
	char routes_json[4096];
	cJSON* root_object = NULL;
	cJSON *routes = NULL;
	cJSON *route = NULL;
	cJSON *name = NULL;
	cJSON *route_id = NULL;
	char* route_name_list[30];
	int route_status_list[30];
	int routes_size;
	int selected=0;
	int idx = 1;
	int route_id_ret = -1;
	int i=1;
	
	
	memset(routes_json, 0x00, sizeof(routes_json));
	
	
	if(read_from_file(STOPS_FILE, routes_json)==0)
	{
		root_object = cJSON_Parse(routes_json); // routes and routes are two
		if(root_object == NULL)
		{
			show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
			return -1;
		}
		routes = cJSON_GetObjectItem(root_object,"routes");
		routes_size = cJSON_GetArraySize(routes);
		
		route_name_list[0] = (char*) calloc(strlen(title), sizeof(char));
		strcpy(route_name_list[0], title);
		
		cJSON_ArrayForEach(route, routes)
		{
			name = cJSON_GetObjectItemCaseSensitive(route, "name");
			route_id = cJSON_GetObjectItemCaseSensitive(route, "id");
			if(!cJSON_IsString(route_id))
			{
				show_message("PARSE ERROR","ID Should be a String", 1);
				cJSON_Delete(root_object);
				return -1;
			}
			
			if(!cJSON_IsString(name))
			{
				show_message("PARSE ERROR","Name must be a String", 1);
				cJSON_Delete(root_object);
				return -1;
			}
			
			if(idx>8)
				idx = 1;
				
			route_name_list[i] = (char*) calloc(strlen(name->valuestring)+4, sizeof(char)); //name + sprintf inserted values
			sprintf(route_name_list[i], "%d - %s", idx, name->valuestring);
			
			route_status_list[i-1] = atoi(route_id->valuestring); // i starts with 1 so need to reset for status
			i++;
			idx++;
		}
		
		while(selected>=0)
		{
			selected = DispMenu(route_name_list,routes_size+1,1,KEY0, KEY8, 60);
			if(selected==-1)
				return -1;
			else if(selected<routes_size)
				selected++;
			break;
		}
		
		route_id_ret = route_status_list[selected-1];
		//free list
		for(i = 0; i < routes_size+1; i++)
		{
			free(route_name_list[i]);
		}
		cJSON_Delete(root_object);
	}
	return route_id_ret;
}

void sell_card(void)
{
	char card_no[100];
	char url[100];
	char name[100];
	char pos[50];
	char token[103];
	char data[1024];
	int keyCode;
	int print_status;
	
	cJSON *root = NULL;
	const cJSON *message = NULL;
	const cJSON *success = NULL;
	
	memset(pos, 0x00, sizeof(pos));
	memset(name, 0x00, sizeof(name));
	memset(card_no, 0x00, sizeof(card_no));
	memset(token, 0x00, sizeof(token));
	memset(url, 0x00, sizeof(url));
	
	show_title("CARD NUMBER");
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 1, 20, -1, card_no) ;
	if(keyCode == KB_CANCEL)
	{
		return;
	}
	
	show_title("OWNER NAME");
	keyCode = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 1, 50, -1, name) ;
	if(keyCode == KB_CANCEL)
	{
		return;
	}
	
	sysReadSN(pos);
	
	sprintf(data, "{\"pos\":\"%s\", \"name\":\"%s\",\"card\":\"%s\"}",pos, name, card_no);
	
	if(read_from_file(TOKEN_FILE, token)==0)
	{
		sprintf(url, "%s/index.php/v1/tickets/sell-cards?access-token=%s", BASEURL, token);
		
		if(post_url(url, data, data)==0)
		{
			root = cJSON_Parse(data);
			if(root == NULL)
			{
				show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
				return;
			}
			//check success
			success = cJSON_GetObjectItemCaseSensitive(root, "success");
			message = cJSON_GetObjectItemCaseSensitive(root, "message");
			
			if(cJSON_IsBool(success) && cJSON_IsFalse(success))
			{
				show_message("TICKET ERROR", message->valuestring, 1);
				cJSON_Delete(root);
				return;
			}
			else if(!cJSON_IsBool(success))
			{
				show_message("TICKET ERROR","INVALID RESPONSE FOUND", 1);
				return;
			}
			
			prnInit();
			setNormal();
			pntDrawPicture(0,0, 385,80, "logo.bmp");
			prnSetSpace(0, 0);
			
			prnPrintf("%s\n\n\n", message->valuestring);
			print_status = prnStatus();
			while(print_status != PRN_OK)
			{
				if(print_status == PRN_PAPEROUT)
				{
					show_message("PRINTER ERROR",  "CHECK PAPERS", 1);
				}
				else if(print_status == PRN_UNFINISHED)
				{
					show_message("PRINTER ERROR",  "UNFINISHED TRY AGAIN", 1);
				}
				print_status = prnStatus();
			}
			
			prnStart();// do print
			prnSetFontZoom(1, 1);
			cJSON_Delete(root);
		}
		else
			show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
	}
	else
		show_message("SYSTEM ERROR", "COULD NOT READ SESSION", 1);
}

void create_report(ActionType type)
{
	int do_print = 0;
	//start date
	char day_start[3];
	char month_start[3];
	char year_start[5];
	//end date
	char day_end[3];
	char month_end[3];
	char year_end[5];
	
	char delim[] = ";";
	char* piece;
	
	//machine
	char pos[50];
	char token[250];
	char url[250];
	char title[200];
	char buffer[1024];
	int keyCode;
	
	struct tm *timeinfo;
	time_t rawtime;
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	//Start Day
	memset(day_start, 0x00, sizeof(day_start));
	show_title("START DAY");
	
	sprintf(day_start, "%02d", timeinfo->tm_mday);
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 1, 2, -1, day_start) ;
	if(keyCode == KB_CANCEL)
		return;
		
	//Start Month
	memset(month_start, 0x00, sizeof(month_start));
	show_title("START MONTH");
	sprintf(month_start, "%02d", timeinfo->tm_mon+1);
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 1, 2, -1, month_start) ;
	if(keyCode == KB_CANCEL)
		return;
		
	//End Day
	memset(day_end, 0x00, sizeof(day_end));
	show_title("START DAY");
	
	sprintf(day_end, "%02d", timeinfo->tm_mday);
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 1, 2, -1, day_end) ;
	if(keyCode == KB_CANCEL)
		return;
		
	//End Month
	memset(month_end, 0x00, sizeof(month_end));
	show_title("START MONTH");
	sprintf(month_end, "%02d", timeinfo->tm_mon+1);
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 1, 2, -1, month_end) ;
	if(keyCode == KB_CANCEL)
		return;
		
	//Report Year
	show_title("REPORT YEAR ");
	memset(year_start, 0x00, sizeof(year_start));
	memset(year_end, 0x00, sizeof(year_end));
	sprintf(year_start, "%4d", timeinfo->tm_year+1900);
	keyCode = kbGetString(KB_BIG_NUM|KB_EN_PRESETSTR, 4, 4, -1, year_start) ;
	if(keyCode == KB_CANCEL)
		return;
		
	sysReadSN(pos);
	strcpy(year_end, year_start);
	
	memset(token, 0x00, sizeof(token));
	if(read_from_file(TOKEN_FILE, token)==0)
	{
	
		if(type == REPORT_POS)
		{
			sprintf(url, "%s/index.php/v1/tickets/pos-report?id=%s&start_date=%s-%s-%s&end_date=%s-%s-%s&access-token=%s", BASEURL, pos, year_start, month_start, day_start,year_end, month_end, day_end, token);
			
			if(get_url(url, buffer)==0)
			{
				sprintf(title, "\nUSER VENDING REPORT\n%s-%s to %s-%s 20%s\n\nPOS Serial:%s\n------------------------------\n", day_start, month_start, day_end, month_end, year_end, pos);
				do_print = 1;
			}
			else
				show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
		}
		else if(type == REPORT_USER)
		{
			sprintf(url, "%s/index.php/v1/tickets/user-report?pos=%s&start_date=%s-%s-%s&end_date=%s-%s-%s&access-token=%s", BASEURL, pos, year_start, month_start, day_start,year_end, month_end, day_end, token);
			
			if(get_url(url, buffer)==0)
			{
				sprintf(title, "\nPOS VENDING REPORT\n%s-%s to %s-%s 20%s\n\nPOS Serial:%s\n------------------------------\n", day_start, month_start, day_end, month_end, year_end, pos);
				do_print =1;
			}
			else
				show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
		}
		
		//check if we should print
		if(do_print==1)
		{
			prnInit();
			
			setNormal();
			pntDrawPicture(0,0, 385,80, "logo.bmp");
			prnSetSpace(0, 0);
			
			prnPrintf("%s\n", title);
			for(piece = strtok(buffer, delim); piece; piece = strtok(NULL, delim))
			{
				if(strcmp(piece, "\"")==0)
					continue;
					
				prnPrintf("%s\n", piece);
			}
			prnPrintf("\n\n\n\n");
			prnStart();
		}
	}
	else
		show_message("COULD NOT READ THE SESSION", "PLEASE LOGIN AGAIN", 1);
}

void create_seat_report(void)
{
	char url[250];
	char token[250];
	char data[4096];
	char buffer[4096];
	int retKey;
	
	cJSON *root = NULL;
	const cJSON *message = NULL;
	const cJSON *success = NULL;
	
	memset(data, 0x00, sizeof(data));
	memset(buffer, 0x00, sizeof(buffer));
	memset(url, 0x00, sizeof(url));
	memset(token, 0x00, sizeof(token));
	
	if(read_from_file(TOKEN_FILE, token)==0)
	{
		//collect details
		if(collect_ticket_details(data, SEAT_REPORT)==0)
		{
			sprintf(url, "%s/index.php/v1/tickets/free-seats?access-token=%s", BASEURL, token);
			if(post_url(url, data, buffer)==0)
			{
				root = cJSON_Parse(buffer);
				if(root == NULL)
				{
					show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
					cJSON_Delete(root);
					return;
				}
				//check success
				success = cJSON_GetObjectItemCaseSensitive(root, "success");
				message = cJSON_GetObjectItemCaseSensitive(root, "message");
				
				if(cJSON_IsBool(success) && cJSON_IsFalse(success))
				{
					show_message("REPORT ERROR", message->valuestring, 1);
					cJSON_Delete(root);
					return;
				}
				else if(!cJSON_IsBool(success))
				{
					show_message("REPORT ERROR","INVALID RESPONSE FOUND", 1);
					cJSON_Delete(root);
					return;
				}
				else
				{
					lcdCls();
					lcdClrLine(2, 7);
					lcdDisplay(0, 2, DISP_CFONT|DISP_MEDIACY, message->valuestring);
					lcdDisplay(0, 4, DISP_ASCII|DISP_MEDIACY, "[CANCEL] - MENU");
					lcdDisplay(0, 6, DISP_ASCII|DISP_MEDIACY, "[ENTER] - PRINT");
					retKey =  kbGetKey();
					if(retKey== KEY_ENTER)
					{
						prnInit();
						setBold();
						prnPrintf("%s", message->valuestring);
						prnStart();
						cJSON_Delete(root);
					}
				}
			}
			else
				show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
		}
		else
			show_message("DATA COLLECT ERROR!", "INCOMPLETE TICKET DATA", 1);
	}
	else
		show_message("COULD NOT READ THE SESSION", "PLEASE LOGIN AGAIN", 1);
}

void change_password(void)
{
	char username[50];
	char old_password[50];
	char new_password[50];
	char url[250];
	char sendBuffer[1024];
	
	cJSON *root = NULL;
	const cJSON *message = NULL;
	const cJSON *success = NULL;
	
	int32_t retKey;
	
	//remove file
	fileRemove(TOKEN_FILE);
	memset(username, 0x00, sizeof(username));
	memset(old_password, 0x00, sizeof(old_password));
	memset(new_password, 0x00, sizeof(new_password));
	memset(url, 0x00, sizeof(url));
	
	show_title("Mobile Number");
	retKey = kbGetString(KB_BIG_ALPHA|KB_EN_PRESETSTR, 0, 11, -1, username);
	kbFlush();
	if(retKey == KB_CANCEL)
		return; //user cancelled
		
	show_title("Old Password");
	retKey = kbGetString(KB_BIG_ALPHA|KB_BIG_PWD, 0, 11, -1, old_password);
	kbFlush();
	
	if(retKey == KB_CANCEL)
		return; //user cancelled
		
	show_title("New Password");
	retKey = kbGetString(KB_BIG_ALPHA|KB_BIG_PWD, 0, 11, -1, new_password);
	kbFlush();
	
	if(retKey == KB_CANCEL)
		return; //user cancelled
		
	sprintf(url, "%s/index.php/v1/accounts/change-password?username=%s&old=%s&new=%s", BASEURL,username, old_password, new_password);
	if(get_url(url, sendBuffer)==0)
	{
		root = cJSON_Parse(sendBuffer);
		if(root == NULL)
		{
			show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
			return;
		}
		//check success
		success = cJSON_GetObjectItemCaseSensitive(root, "success");
		message = cJSON_GetObjectItemCaseSensitive(root, "message");
		
		if(cJSON_IsBool(success) && cJSON_IsFalse(success))
		{
			show_message("REPORT ERROR", message->valuestring, 1);
			cJSON_Delete(root);
			return;
		}
		else if(!cJSON_IsBool(success))
		{
			show_message("REPORT ERROR","INVALID RESPONSE FOUND", 1);
			cJSON_Delete(root);
			return;
		}
		else
		{
			show_message("PASSWORD CHANGE","YOUR PASSWORD WAS CHANGED. SYSTEM WILL REBOOT", 1);
			cJSON_Delete(root);
			sys_reboot();
		}
	}
	else
		show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
}

void plan_route(void)
{
	char url[250];
	char token[250];
	char ticket_json[4096];
	char buffer[4096];
	int print_status;
	
	cJSON *root = NULL;
	const cJSON *message = NULL;
	const cJSON *success = NULL;
	
	memset(ticket_json, 0x00, sizeof(ticket_json));
	memset(buffer, 0x00, sizeof(buffer));
	memset(url, 0x00, sizeof(url));
	memset(token, 0x00, sizeof(token));
	
	
	if(read_from_file(TOKEN_FILE, token)==0)
	{
		//collect details
		if(collect_ticket_details(ticket_json, BUS_PLAN)==0)
		{
			sprintf(url, "%s/index.php/v1/trip/plan?access-token=%s", BASEURL, token);
			if(post_url(url, ticket_json, buffer)==0)
			{
				//print report here
				root = cJSON_Parse(buffer);
				if(root == NULL)
				{
					show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
					return;
				}
				//check success
				success = cJSON_GetObjectItemCaseSensitive(root, "success");
				message = cJSON_GetObjectItemCaseSensitive(root, "message");
				
				if(cJSON_IsBool(success) && cJSON_IsFalse(success))
				{
					show_message("REPORT ERROR", message->valuestring, 1);
					cJSON_Delete(root);
					return;
				}
				else if(!cJSON_IsBool(success))
				{
					show_message("REPORT ERROR","INVALID RESPONSE FOUND", 1);
					cJSON_Delete(root);
					return;
				}
				else
				{
					show_title(message->valuestring);
					if(kbGetKey()== KEY_ENTER)
					{
						prnInit();
						
						setNormal();
						prnPrintf("%s\n\n", message->valuestring);
						prnStart();
						cJSON_Delete(root);
					}
				}
			}
			else
				show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
		}
		else
			show_message("DATA COLLECT ERROR!", "INCOMPLETE TICKET DATA", 1);
	}
	else
		show_message("COULD NOT READ THE SESSION", "PLEASE LOGIN AGAIN", 1);
}

void manifest_report(void)
{
	char url[250];
	char token[250];
	char ticket_json[4096];
	char buffer[4096];
	int print_status;
	
	cJSON *root = NULL;
	const cJSON *message = NULL;
	const cJSON *success = NULL;
	const cJSON *bus = NULL;
	const cJSON *dept_date = NULL;
	const cJSON *dept_time = NULL;
	const cJSON *customers = NULL;
	const cJSON *customer = NULL;
	const cJSON *revenue = NULL;
	const cJSON *currency = NULL;
	
	int idx=1;
	
	memset(ticket_json, 0x00, sizeof(ticket_json));
	memset(buffer, 0x00, sizeof(buffer));
	memset(url, 0x00, sizeof(url));
	memset(token, 0x00, sizeof(token));
	
	
	if(read_from_file(TOKEN_FILE, token)==0)
	{
		//collect details
		if(collect_ticket_details(ticket_json, SEAT_REPORT)==0)
		{
			sprintf(url, "%s/index.php/v1/tickets/manifest?access-token=%s", BASEURL, token);
			if(post_url(url, ticket_json, buffer)==0)
			{
				//print report here
				root = cJSON_Parse(buffer);
				if(root == NULL)
				{
					show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
					return;
				}
				//check success
				success = cJSON_GetObjectItemCaseSensitive(root, "success");
				message = cJSON_GetObjectItemCaseSensitive(root, "message");
				
				if(cJSON_IsBool(success) && cJSON_IsFalse(success))
				{
					show_message("REPORT ERROR", message->valuestring, 1);
					cJSON_Delete(root);
					return;
				}
				else if(!cJSON_IsBool(success))
				{
					show_message("REPORT ERROR","INVALID RESPONSE FOUND", 1);
					cJSON_Delete(root);
					return;
				}
				else
				{
					prnInit();
					
					setNormal();
					pntDrawPicture(0,0, 385,80, "logo.bmp");
					prnSetSpace(0, 0);
					prnPrintf("Bus Manifest Report\n");
					
					bus = cJSON_GetObjectItemCaseSensitive(message, "bus");
					dept_date = cJSON_GetObjectItemCaseSensitive(message, "dept_date");
					dept_time = cJSON_GetObjectItemCaseSensitive(message, "dept_time");
					
					prnSetFont(FONT_SYSTEM,FONT_CHARSET_ASCII,0,32,SET_PRIMARY_FONT);
					prnPrintf("%s - %s %s\n", bus->valuestring, dept_date->valuestring, dept_time->valuestring);
					prnPrintf("\n------------------------------\n");
					
					setNormal();
					prnPrintf("Total Revenue:\n");
					
					prnSetFont(FONT_SYSTEM,FONT_CHARSET_ASCII,0,32,SET_PRIMARY_FONT);
					revenue = cJSON_GetObjectItem(message,"revenue");
					cJSON_ArrayForEach(currency, revenue)
					{
						prnPrintf("%s\n", currency->valuestring);
					}
					
					setNormal();
					customers = cJSON_GetObjectItem(message,"customers");
					
					prnSetFont(FONT_SYSTEM,FONT_CHARSET_ASCII,0,16,SET_PRIMARY_FONT);
					cJSON_ArrayForEach(customer, customers)
					{
						prnPrintf("\n------------------------------\n %d.%s", idx,customer->valuestring);
						idx++;
					}
					setNormal();
					prnPrintf("\n\n\n\n");
					
					while(print_status != PRN_OK)
					{
						if(print_status == PRN_PAPEROUT)
						{
							show_message("PRINTER ERROR",  "CHECK PAPERS", 1);
						}
						else if(print_status == PRN_UNFINISHED)
						{
							show_message("PRINTER ERROR",  "UNFINISHED TRY AGAIN", 1);
						}
						print_status = prnStatus();
					}
					
					prnStart();// do print ;
					prnSetFontZoom(1, 1);
					cJSON_Delete(root);
				}
			}
			else
				show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
		}
		else
			show_message("DATA COLLECT ERROR!", "INCOMPLETE TICKET DATA", 1);
	}
	else
		show_message("COULD NOT READ THE SESSION", "PLEASE LOGIN AGAIN", 1);
}


void bus_details_report(void)
{
	char url[250];
	char token[250];
	char ticket_json[4096];
	char buffer[4096];
	int print_status;
	
	cJSON *root = NULL;
	const cJSON *message = NULL;
	const cJSON *success = NULL;
	const cJSON *bus = NULL;
	const cJSON *dept_date = NULL;
	const cJSON *dept_time = NULL;
	const cJSON *customers = NULL;
	const cJSON *customer = NULL;
	const cJSON *revenue = NULL;
	const cJSON *currency = NULL;
	
	memset(ticket_json, 0x00, sizeof(ticket_json));
	memset(buffer, 0x00, sizeof(buffer));
	memset(url, 0x00, sizeof(url));
	memset(token, 0x00, sizeof(token));
	
	
	if(read_from_file(TOKEN_FILE, token)==0)
	{
		//collect details
		if(collect_ticket_details(ticket_json, SEAT_REPORT)==0)
		{
			sprintf(url, "%s/index.php/v1/trip/manifest?access-token=%s", BASEURL, token);
			if(post_url(url, ticket_json, buffer)==0)
			{
				//print report here
				root = cJSON_Parse(buffer);
				if(root == NULL)
				{
					show_message("PARSE ERROR", cJSON_GetErrorPtr(), 1);
					return;
				}
				//check success
				success = cJSON_GetObjectItemCaseSensitive(root, "success");
				message = cJSON_GetObjectItemCaseSensitive(root, "message");
				
				if(cJSON_IsBool(success) && cJSON_IsFalse(success))
				{
					show_message("REPORT ERROR", message->valuestring, 1);
					cJSON_Delete(root);
					return;
				}
				else if(!cJSON_IsBool(success))
				{
					show_message("REPORT ERROR","INVALID RESPONSE FOUND", 1);
					cJSON_Delete(root);
					return;
				}
				else
				{
					prnInit();
					setNormal();
					pntDrawPicture(0,0, 385,80, "logo.bmp");
					prnSetSpace(0, 4);
					prnPrintf("Bus Details Report\n");
					
					bus = cJSON_GetObjectItemCaseSensitive(message, "bus");
					dept_date = cJSON_GetObjectItemCaseSensitive(message, "dept_date");
					dept_time = cJSON_GetObjectItemCaseSensitive(message, "dept_time");
					prnPrintf("%s - %s %s\n", bus->valuestring, dept_date->valuestring, dept_time->valuestring);
					
					customers = cJSON_GetObjectItem(message,"customers");
					cJSON_ArrayForEach(customer, customers)
					{
						prnPrintf("%s\n", customer->valuestring);
					}
					
					revenue = cJSON_GetObjectItem(message,"revenue");
					cJSON_ArrayForEach(currency, revenue)
					{
						prnPrintf("%s\n", currency->valuestring);
					}
					prnPrintf("\n\n");
					
					while(print_status != PRN_OK)
					{
						if(print_status == PRN_PAPEROUT)
						{
							show_message("PRINTER ERROR",  "CHECK PAPERS", 1);
						}
						else if(print_status == PRN_UNFINISHED)
						{
							show_message("PRINTER ERROR",  "UNFINISHED TRY AGAIN", 1);
						}
						print_status = prnStatus();
					}
					
					prnStart();// do print ;
					prnSetFontZoom(1, 1);
					cJSON_Delete(root);
				}
			}
			else
				show_message("COULD NOT FETCH DATA!", "NETWORK ERROR. TRY AGAIN!", 1);
		}
		else
			show_message("DATA COLLECT ERROR!", "INCOMPLETE TICKET DATA", 1);
	}
	else
		show_message("COULD NOT READ THE SESSION", "PLEASE LOGIN AGAIN", 1);
}

void setBold()
{
	prnSetFontZoom(2,1);
}

void setNormal()
{
	prnSetFontZoom(1,1);
}
