// Copyright: 
// Author: 
// CopyPolicy: 

//////////////////////////////////////////////////////////////////////////
// 
// This is a configuration file to explain ASIBOT to SWIG
//
// SWIG, for the most part, understands ASIBOT auto-magically.
// There are a few things that need to be explained:
//  + use of multiple inheritance
//  + use of names that clash with special names in Java/Python/Perl/...
//  + use of templates

%module asibot

%import "yarp.i"

%{
/* Includes the header in the wrapper code */
#include "ColorClient.hpp"
%}

/* Parse the header file to generate wrappers */
%include "ColorClient.hpp"

%extend ColorClient {
	int size() {
		int buffer;
		bool ok = self->size(buffer);
		if (!ok) return 0;
		return buffer;
	}
}

