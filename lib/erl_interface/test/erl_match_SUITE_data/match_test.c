/*
 * %CopyrightBegin%
 * 
 * Copyright Ericsson AB 1997-2009. All Rights Reserved.
 * 
 * The contents of this file are subject to the Erlang Public License,
 * Version 1.1, (the "License"); you may not use this file except in
 * compliance with the License. You should have received a copy of the
 * Erlang Public License along with this software. If not, it can be
 * retrieved online at http://www.erlang.org/.
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 * 
 * %CopyrightEnd%
 */

/*
 * Purpose: Tests the erl_match() function.
 * Author:  Bjorn Gustavsson
 */

#include "runner.h"

TESTCASE(erl_match_server)
{
    erl_init(NULL, 0);

    for (;;) {
	ETERM* pattern;
	ETERM* term;

	pattern = get_term();
	if (pattern == NULL) {
	    report(1);
	    return;
	} else {
	    term = get_term();
	    if (term == NULL) {
		fail("Unexpected EOF term");
	    } else {
		send_term(erl_mk_int(erl_match(pattern, term)));
		erl_free_term(pattern);
		erl_free_term(term);
	    }
	}
    }

}

TESTCASE(erl_match_bind)
{
  erl_init(NULL, 0);

  for (;;) {
    char* pattern;
    ETERM* term;

    pattern=read_packet(NULL);

    switch (pattern[0]) {
    case 'e':
      free(pattern);
      report(1);
      return;

    case 'b':
      {
	ETERM* patt_term;

	/*
	 * Get the pattern string and convert it using erl_format().
	 *
	 * Note that the call to get_term() below destroys the buffer
	 * that the pattern variable points to.  Therefore, it is
	 * essential to call erl_format() here, before 
	 * calling get_term().
	 */

	message("Pattern: %s", pattern+1);
	patt_term = erl_format(pattern+1);
	free(pattern);

	if (patt_term == NULL) {
	  fail("erl_format() failed");
	}

	/*
	 * Get the term and send back the result of the erl_match()
	 * call.
	 */

	term = get_term();
	if (term == NULL) {
	  fail("Unexpected eof term");
	}
	else {
	  send_term(erl_mk_int(erl_match(patt_term, term)));
	}
	erl_free_term(patt_term);
	erl_free_term(term);
      }
    break;

    default:
      free(pattern);
      fail("Illegal character received");
    }
      
  }
}
