// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TASK_BASE_HPP__
#define __TASK_BASE_HPP__

#include <stdio.h>

/**
 * @ingroup asibot_libraries
 *
 * \defgroup TaskLib
 *
 * @brief The TaskLib library is a collection of a simple lib for hard-coded (maybe not so much) task classes.
 *
 * The TaskLib library is a collection of a simple lib for hard-coded (maybe not so much) task classes.
 *
 */

/**
 * @ingroup TaskLib
 *
 * @brief A base class for these tasks.
 *
 */
class TaskBase {
  public:
    virtual bool configure() = 0;
};

#endif  // __TASK_BASE_HPP__

