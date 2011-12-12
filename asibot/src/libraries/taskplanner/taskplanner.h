// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright: (C) 2010 Universidad Carlos III de Madrid
 * Author: Juan G. Victores
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 * Description: Headers of the taskplanner
 *
 */
#ifndef TASKPLANNER_H
#define TASKPLANNER_H

#include <yarp/os/all.h>
using namespace yarp::os;

/**
 * Plan a set of tasks to perform a high-level goal. [wait for reply] 
 * @param who the main actor. 
 * @param act the action to be performed. 
 * @param obj the object the action is being performed on. 
 * @param plan to return the generated plan. 
 * @return true/false on success/failure. 
 *  
 * @note An extra parameter or more should be passed (not implemented) for defining
 * the planning algoritm used.
 */
bool planTasks(const ConstString who, const ConstString act, const ConstString obj, Bottle& plan);

#endif /* TASKPLANNER_H */
