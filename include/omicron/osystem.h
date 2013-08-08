/**************************************************************************************************
* THE OMICRON PROJECT
 *-------------------------------------------------------------------------------------------------
 * Copyright 2010-2012		Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  Alessandro Febretti		febret@gmail.com
 *-------------------------------------------------------------------------------------------------
 * Copyright (c) 2010-2011, Electronic Visualization Laboratory, University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this list of conditions 
 * and the following disclaimer. Redistributions in binary form must reproduce the above copyright 
 * notice, this list of conditions and the following disclaimer in the documentation and/or other 
 * materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF 
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************************************/
#ifndef __OMICRON_SYSTEM_H__
#define __OMICRON_SYSTEM_H__

#include "omicronConfig.h"

#ifdef WIN32
	// disable warnings generated by libraries
	// 4396 the inline specifier cannot be used when a friend declaration refers to a specialization of a function template
	// 4996 _strdup warning (still shows even after the #define strdup _strdup for some reason)
	#pragma warning( disable : 4224 4244 4251 4018 4800 4396 4996)
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
	#define strdup _strdup
#endif

// Standard C includes
#include <stdio.h>
#include <stdlib.h>

#include "otypes.h"

// Forward declaration of DataSource, used for omain
namespace omicron 
{ 
	class DataSource;

	class ILogListener
	{
	public:
		virtual void addLine(const String& line) = 0;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Function definitions.
	OMICRON_API void ologenable();
	OMICRON_API void ologdisable();
	OMICRON_API void ologaddlistener(ILogListener* listener);
	OMICRON_API void ologremlistener(ILogListener* listener);
	OMICRON_API void ologopen(const char* filename);
	OMICRON_API void ologclose();
	//! Specifies wether the logger will automatically append newlines to log messages.
	//! True by default.
	OMICRON_API void ologaddnewline(bool enabled);
	OMICRON_API void omsg(const String& str);
	OMICRON_API void owarn(const String& str);
	OMICRON_API void oerror(const String& str);
	OMICRON_API void oexit(int code);
	OMICRON_API void odebugalloc(bool enable);
	OMICRON_API void oabort(const char* file, int line, const char* reason);

	OMICRON_API void osleep(uint msecs);
};

#define odbg(str) omsg(str);
#define oassert(c) if(!(c)) { oabort(__FILE__, __LINE__, #c); }


#endif