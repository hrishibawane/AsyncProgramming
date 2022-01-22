/*
 * =====================================================================================
 *
 *       Filename:  rtm.c
 *
 *    Description:  This file implements the Routing Table Manager( Thread)
 *
 *        Version:  1.0
 *        Created:  02/14/2020 07:24:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Er. Abhishek Sagar, Juniper Networks (https://csepracticals.com), sachinites@gmail.com
 *        Company:  Juniper Networks ( 2017 - 2020), Cisco Systems ( 2020 - Present )
 *
 *        This file is part of the Inter-Thread Communication distribution (https://github.com/sachinites) 
 *        Copyright (c) 2019 Abhishek Sagar.
 *        This program is free software: you can redistribute it and/or modify it under the terms of the GNU General 
 *        Public License as published by the Free Software Foundation, version 3.
 *        
 *        This program is distributed in the hope that it will be useful, but
 *        WITHOUT ANY WARRANTY; without even the implied warranty of
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *        General Public License for more details.
 *
 *        visit website : https://csepracticals.com for more courses and projects
 *                                  
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "network_utils.h"
#include "rt.h"

rt_table_t *rt_table = NULL;

static void
udp_recv_fn1(char *msg_recvd, uint32_t msg_size, char *sender_ip, uint32_t port_no, uint32_t fd) {

    printf ("route recvd on port no %d from IP %s\n", port_no, sender_ip);
    rt_table_entry_t *rt_entry = (rt_table_entry_t *)msg_recvd;
    printf ("   Route : %s %s %s\n", rt_entry->dest, rt_entry->gw, rt_entry->oif);
    rt_insert_new_entry(rt_table, rt_entry->dest, 32, rt_entry->gw, rt_entry->oif);
}

int
main(int argc, char **argv){

    if (!rt_table) {
        rt_table =
            rt_create_new_rt_table("Table1");
        printf(" New Routing Table Created\n");
    }

    for(;;){
        printf("Main Menu\n");
        printf("\t 1.  : Display Routing Table\n");
        printf("\t 2.  : Create New RT Entry\n");
        printf("\t 3.  : Update Existing RT Entry\n");
        printf("\t 4.  : Delete RT entry\n");
        printf("\t 5.  : Start Pkt Listener thread\n");
        printf("\t 6.  : exit\n");

        int choice;
        printf("Enter Choice : ");
        scanf("%d", &choice);

        switch(choice){
            case 1:
                rt_display_rt_table(rt_table);
                printf("\n\n");
                break;
            case 5:
                {
                    int portno;
                    printf ("Listening port no ? ");
                    scanf("%d", &portno);
                    udp_server_create_and_start(
                        "127.0.0.1", portno,  udp_recv_fn1);
                }
                break;
            case 6:
                exit(0);
            default:
                break;
        }
    }
    return 0;
}