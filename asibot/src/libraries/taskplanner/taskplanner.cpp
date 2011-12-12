// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright: (C) 2010 Universidad Carlos III de Madrid
 * Author: Juan G. Victores
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 * Description: Implementation of the taskplanner
 *
 */

#include "taskplanner.h"

bool planTasks(const ConstString who, const ConstString act, const ConstString obj, Bottle& plan) {
    		// These should be parsed from file.
            ConstString _who("martin");
            ConstString _act("get");
            ConstString _obj("can");
            if((who == ConstString("juan"))&(act == ConstString("make"))&(obj == ConstString("food")))
                printf("juan doesn't know how to make food, but I know he would sure like some!\n");
            if((who == _who)&(act == _act)&(obj == _obj)) {
			    // Do some processing here...
                printf("I have a plan for you, %s...\n",who.c_str());   			    
                plan.addList() = Bottle("move-robot current reach-can asibot");
                plan.addList() = Bottle("open-gripper any hand reach-can");
                plan.addList() = Bottle("move-robot current get-can asibot");
                plan.addList() = Bottle("close-gripper can hand get-can");
                return true;
            } else {
                printf("I have no plan for you, %s.\n",who.c_str());
                return false;
            }
}

