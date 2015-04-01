#ifndef _TABLE_H
#define _TABLE_H

#include "uthash.h"
#include "list.h"
#include "card.h"
#include "player.h"

#define TABLE_MAX_PLAYERS 10
#define MIN_PLAYERS 3
#define MAX_TABLES 1024

typedef enum table_state_e {
    TABLE_STATE_WAITING,
    TABLE_STATE_PREFLOP,
    TABLE_STATE_FLOP,
    TABLE_STATE_TURN,
    TABLE_STATE_RIVER,
    TABLE_STATE_SHOWDOWN,
} table_state_t;

typedef struct table_s {
    char name[MAX_NAME];
    table_state_t state;
    deck_t deck;
    card_t community_cards[5];

    player_t *players[TABLE_MAX_PLAYERS];
    int num_players;
    int dealer;
    int turn;
    int num_playing;

    int pot;
    int bid;
    int small_blind;
    int big_blind;
    int minimum_bet;
    struct event_base *base;
    struct event *ev_timeout;
    UT_hash_handle hh;
} table_t;

#define current_player(t) ((t)->players[(t)->turn])
int add_player(table_t *table, player_t *player);
int del_player(table_t *table, player_t *player);
int next_player(table_t *table, int index);
int player_fold(player_t *player);
int player_check(player_t *player);
int player_bet(player_t *player, int bid);

void table_init(table_t *table);
int handle_table(table_t *table);
void table_pre_flop(table_t *table);
void table_flop(table_t *table);
void table_turn(table_t *table);
void table_river(table_t *table);
void table_showdown(table_t *table);
int table_check_winner(table_t *table);
int table_init_timeout(table_t *table);
int table_reset_timeout(table_t *table);
int table_clear_timeout(table_t *table);
int table_to_json(table_t *table, char *buffer, int size);

table_t *table_create();
void tatle_destroy(table_t *table);
extern table_t *g_tables;
extern int g_num_tables;

void broadcast(table_t *table, const char *fmt, ...);
void send_msg(player_t *player, const char *fmt, ...);
void timeoutcb(evutil_socket_t fd, short events, void *arg);
extern char g_table_report_buffer[4096];
#ifdef _USE_JSON
#define report(table) do {\
    table_to_json((table), g_table_report_buffer, sizeof(g_table_report_buffer));\
    broadcast((table), "[\"update\",%s]\n", g_table_report_buffer);\
} while (0)
#else
#define report(table) do {\
    broadcast((table), "turn %s pot %d\ntexas> ", table->players[table->turn]->name, table->pot);\
} while (0)
#endif
#endif
