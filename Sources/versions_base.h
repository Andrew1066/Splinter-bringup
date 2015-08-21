/*
 * versions_base.h
 *
 *  Created on: 16 Oct 2013
 *      Author: Mike
 */

#ifndef VERSIONS_H_
#define VERSIONS_H_


/* this file includes information extracted from subversion
  EDIT THE FILE versions_base.h . DO NOT EDIT versions.h
  Use subWCrev (part of TortoiseSVN) to transform this file

subWCrev . versions_base.h versions.h

  It is preferable to place this in a makefile rule

versions_file:
	subWCrev . versions_base.h versions.h

and then

all: versions_file <other dependencies>

Look in the OlimexODS/Makefile file for an example

*/


#define BUILD_VERSION "1.1.$WCREV$ $WCDATE$ $WCMODS?(modified):$"


#endif /* VERSIONS_BASE_H_ */
