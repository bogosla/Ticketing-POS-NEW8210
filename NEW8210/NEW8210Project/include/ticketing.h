#ifndef HOSANNA_TICKETING
#define HOSANNA_TICKETING

typedef enum {SELL_TICKET, RESERVE_TICKET, PAY_RESERVATION, SELL_CARD_TICKET, SEAT_REPORT, BUS_PLAN,GATEWAY_TICKET} ActionType;
typedef enum {REPORT_POS, REPORT_USER} ReportType;

//UI Functions
void create_ticket(ActionType type);
void create_report(ActionType type);
void create_seat_report(void);
void change_password(void);
void manifest_report(void);
void bus_details_report(void);
void plan_route(void);

void sell_card(void);
int print_ticket(const char* json, ActionType type);

int print_ticket_reserved(const char* json);
int collect_ticket_details(char* buffer, ActionType type);

int get_stop(const char* title, char *buffer); //returns selected stop and returs 0 if it is not Intl and 1 if it is i'ntl. -1 means failure
int get_route_id(const char* title);
int get_gender(void);

void setBold();
void setNormal();

#endif
