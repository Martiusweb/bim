
#line 1 "http11_parser_mongrel.rl"
/**
 *
 * Copyright (c) 2010, Zed A. Shaw and Mongrel2 Project Contributors.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 *     * Neither the name of the Mongrel2 Project, Zed A. Shaw, nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "http11_parser_mongrel.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dbg_mongrel.h"

#define LEN(AT, FPC) (FPC - buffer - parser->AT)
#define MARK(M,FPC) (parser->M = (FPC) - buffer)
#define PTR_TO(F) (buffer + parser->F)

/** Machine **/


#line 183 "http11_parser_mongrel.rl"


/** Data **/

#line 58 "http11_parser_mongrel.cpp"
static const char _http_parser_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 10, 1, 11, 1, 
	12, 2, 0, 7, 2, 3, 4, 2, 
	9, 6, 2, 9, 11, 2, 11, 6, 
	2, 13, 12, 2, 14, 12, 3, 8, 
	9, 6, 3, 8, 9, 11
};

static const short _http_parser_key_offsets[] = {
	0, 0, 10, 19, 22, 24, 25, 26, 
	27, 28, 29, 30, 31, 33, 35, 52, 
	53, 69, 72, 74, 75, 84, 93, 99, 
	105, 116, 122, 128, 138, 144, 150, 159, 
	168, 174, 180, 181, 182, 183, 184, 193, 
	199, 205, 214, 223, 232, 241, 250, 259, 
	268, 277, 286, 295, 304, 313, 322, 331, 
	340, 349, 358, 367, 368, 376, 387, 388, 
	390, 402, 413, 414, 415, 417, 423, 429, 
	439, 445, 451, 460, 469, 475, 481
};

static const char _http_parser_trans_keys[] = {
	36, 60, 64, 95, 45, 46, 48, 57, 
	65, 90, 32, 36, 95, 45, 46, 48, 
	57, 65, 90, 42, 47, 104, 32, 35, 
	72, 84, 84, 80, 47, 49, 46, 48, 
	49, 10, 13, 10, 13, 33, 124, 126, 
	35, 39, 42, 43, 45, 46, 48, 57, 
	65, 90, 94, 122, 10, 33, 58, 124, 
	126, 35, 39, 42, 43, 45, 46, 48, 
	57, 65, 90, 94, 122, 10, 13, 32, 
	10, 13, 10, 32, 37, 60, 62, 127, 
	0, 31, 34, 35, 32, 37, 60, 62, 
	127, 0, 31, 34, 35, 48, 57, 65, 
	70, 97, 102, 48, 57, 65, 70, 97, 
	102, 32, 34, 35, 37, 59, 60, 62, 
	63, 127, 0, 31, 48, 57, 65, 70, 
	97, 102, 48, 57, 65, 70, 97, 102, 
	32, 34, 35, 37, 60, 62, 63, 127, 
	0, 31, 48, 57, 65, 70, 97, 102, 
	48, 57, 65, 70, 97, 102, 32, 34, 
	35, 37, 60, 62, 127, 0, 31, 32, 
	34, 35, 37, 60, 62, 127, 0, 31, 
	48, 57, 65, 70, 97, 102, 48, 57, 
	65, 70, 97, 102, 116, 116, 112, 58, 
	32, 34, 35, 37, 60, 62, 127, 0, 
	31, 48, 57, 65, 70, 97, 102, 48, 
	57, 65, 70, 97, 102, 32, 36, 95, 
	45, 46, 48, 57, 65, 90, 32, 36, 
	95, 45, 46, 48, 57, 65, 90, 32, 
	36, 95, 45, 46, 48, 57, 65, 90, 
	32, 36, 95, 45, 46, 48, 57, 65, 
	90, 32, 36, 95, 45, 46, 48, 57, 
	65, 90, 32, 36, 95, 45, 46, 48, 
	57, 65, 90, 32, 36, 95, 45, 46, 
	48, 57, 65, 90, 32, 36, 95, 45, 
	46, 48, 57, 65, 90, 32, 36, 95, 
	45, 46, 48, 57, 65, 90, 32, 36, 
	95, 45, 46, 48, 57, 65, 90, 32, 
	36, 95, 45, 46, 48, 57, 65, 90, 
	32, 36, 95, 45, 46, 48, 57, 65, 
	90, 32, 36, 95, 45, 46, 48, 57, 
	65, 90, 32, 36, 95, 45, 46, 48, 
	57, 65, 90, 32, 36, 95, 45, 46, 
	48, 57, 65, 90, 32, 36, 95, 45, 
	46, 48, 57, 65, 90, 32, 36, 95, 
	45, 46, 48, 57, 65, 90, 32, 36, 
	95, 45, 46, 48, 57, 65, 90, 32, 
	45, 46, 48, 57, 65, 90, 97, 122, 
	32, 47, 62, 9, 13, 45, 57, 65, 
	90, 97, 122, 62, 0, 62, 32, 37, 
	47, 59, 60, 62, 63, 127, 0, 31, 
	34, 35, 32, 37, 59, 60, 62, 63, 
	127, 0, 31, 34, 35, 123, 125, 0, 
	125, 48, 57, 65, 70, 97, 102, 48, 
	57, 65, 70, 97, 102, 32, 37, 60, 
	62, 63, 127, 0, 31, 34, 35, 48, 
	57, 65, 70, 97, 102, 48, 57, 65, 
	70, 97, 102, 32, 37, 60, 62, 127, 
	0, 31, 34, 35, 32, 37, 60, 62, 
	127, 0, 31, 34, 35, 48, 57, 65, 
	70, 97, 102, 48, 57, 65, 70, 97, 
	102, 0
};

static const char _http_parser_single_lengths[] = {
	0, 4, 3, 3, 2, 1, 1, 1, 
	1, 1, 1, 1, 0, 2, 5, 1, 
	4, 3, 2, 1, 5, 5, 0, 0, 
	9, 0, 0, 8, 0, 0, 7, 7, 
	0, 0, 1, 1, 1, 1, 7, 0, 
	0, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 1, 0, 3, 1, 2, 
	8, 7, 1, 1, 2, 0, 0, 6, 
	0, 0, 5, 5, 0, 0, 0
};

static const char _http_parser_range_lengths[] = {
	0, 3, 3, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 1, 0, 6, 0, 
	6, 0, 0, 0, 2, 2, 3, 3, 
	1, 3, 3, 1, 3, 3, 1, 1, 
	3, 3, 0, 0, 0, 0, 1, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 0, 4, 4, 0, 0, 
	2, 2, 0, 0, 0, 3, 3, 2, 
	3, 3, 2, 2, 3, 3, 0
};

static const short _http_parser_index_offsets[] = {
	0, 0, 8, 15, 19, 22, 24, 26, 
	28, 30, 32, 34, 36, 38, 41, 53, 
	55, 66, 70, 73, 75, 83, 91, 95, 
	99, 110, 114, 118, 128, 132, 136, 145, 
	154, 158, 162, 164, 166, 168, 170, 179, 
	183, 187, 194, 201, 208, 215, 222, 229, 
	236, 243, 250, 257, 264, 271, 278, 285, 
	292, 299, 306, 313, 315, 320, 328, 330, 
	333, 344, 354, 356, 358, 361, 365, 369, 
	378, 382, 386, 394, 402, 406, 410
};

static const char _http_parser_indicies[] = {
	0, 2, 3, 0, 0, 0, 0, 1, 
	4, 5, 5, 5, 5, 5, 1, 6, 
	7, 8, 1, 9, 10, 1, 11, 1, 
	12, 1, 13, 1, 14, 1, 15, 1, 
	16, 1, 17, 1, 18, 1, 19, 20, 
	1, 21, 22, 23, 23, 23, 23, 23, 
	23, 23, 23, 23, 1, 21, 1, 24, 
	25, 24, 24, 24, 24, 24, 24, 24, 
	24, 1, 27, 28, 29, 26, 31, 32, 
	30, 33, 1, 35, 36, 1, 1, 1, 
	1, 1, 34, 38, 39, 1, 1, 1, 
	1, 1, 37, 40, 40, 40, 1, 37, 
	37, 37, 1, 42, 1, 43, 44, 45, 
	1, 1, 46, 1, 1, 41, 47, 47, 
	47, 1, 41, 41, 41, 1, 9, 1, 
	10, 49, 1, 1, 50, 1, 1, 48, 
	51, 51, 51, 1, 48, 48, 48, 1, 
	53, 1, 54, 55, 1, 1, 1, 1, 
	52, 57, 1, 58, 59, 1, 1, 1, 
	1, 56, 60, 60, 60, 1, 56, 56, 
	56, 1, 61, 1, 62, 1, 63, 1, 
	64, 1, 9, 1, 10, 65, 1, 1, 
	1, 1, 64, 66, 66, 66, 1, 64, 
	64, 64, 1, 4, 67, 67, 67, 67, 
	67, 1, 4, 68, 68, 68, 68, 68, 
	1, 4, 69, 69, 69, 69, 69, 1, 
	4, 70, 70, 70, 70, 70, 1, 4, 
	71, 71, 71, 71, 71, 1, 4, 72, 
	72, 72, 72, 72, 1, 4, 73, 73, 
	73, 73, 73, 1, 4, 74, 74, 74, 
	74, 74, 1, 4, 75, 75, 75, 75, 
	75, 1, 4, 76, 76, 76, 76, 76, 
	1, 4, 77, 77, 77, 77, 77, 1, 
	4, 78, 78, 78, 78, 78, 1, 4, 
	79, 79, 79, 79, 79, 1, 4, 80, 
	80, 80, 80, 80, 1, 4, 81, 81, 
	81, 81, 81, 1, 4, 82, 82, 82, 
	82, 82, 1, 4, 83, 83, 83, 83, 
	83, 1, 4, 84, 84, 84, 84, 84, 
	1, 4, 1, 85, 85, 85, 85, 1, 
	86, 86, 86, 86, 85, 85, 85, 1, 
	88, 87, 89, 88, 87, 91, 92, 1, 
	93, 1, 1, 94, 1, 1, 1, 90, 
	91, 92, 93, 1, 1, 94, 1, 1, 
	1, 90, 95, 1, 97, 96, 98, 97, 
	96, 99, 99, 99, 1, 90, 90, 90, 
	1, 91, 101, 1, 1, 102, 1, 1, 
	1, 100, 103, 103, 103, 1, 100, 100, 
	100, 1, 105, 106, 1, 1, 1, 1, 
	1, 104, 108, 109, 1, 1, 1, 1, 
	1, 107, 110, 110, 110, 1, 107, 107, 
	107, 1, 1, 0
};

static const char _http_parser_trans_targs[] = {
	2, 0, 60, 64, 3, 41, 4, 24, 
	34, 5, 20, 6, 7, 8, 9, 10, 
	11, 12, 13, 14, 19, 78, 15, 16, 
	16, 17, 18, 14, 19, 17, 18, 14, 
	19, 14, 21, 5, 22, 21, 5, 22, 
	23, 24, 5, 20, 25, 27, 30, 26, 
	27, 28, 30, 29, 31, 5, 20, 32, 
	31, 5, 20, 32, 33, 35, 36, 37, 
	38, 39, 40, 42, 43, 44, 45, 46, 
	47, 48, 49, 50, 51, 52, 53, 54, 
	55, 56, 57, 58, 59, 61, 62, 62, 
	63, 78, 65, 66, 69, 71, 74, 67, 
	67, 68, 78, 70, 71, 72, 74, 73, 
	75, 66, 76, 75, 66, 76, 77
};

static const char _http_parser_trans_actions[] = {
	1, 0, 1, 1, 11, 0, 1, 1, 
	1, 13, 13, 1, 0, 0, 0, 0, 
	0, 0, 0, 19, 19, 23, 0, 3, 
	0, 5, 7, 28, 28, 7, 0, 9, 
	9, 0, 1, 25, 1, 0, 15, 0, 
	0, 0, 37, 37, 0, 21, 21, 0, 
	0, 0, 0, 0, 17, 46, 46, 17, 
	0, 31, 31, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 21, 0, 
	0, 40, 0, 21, 0, 21, 21, 1, 
	0, 0, 43, 0, 0, 0, 0, 0, 
	17, 50, 17, 0, 34, 0, 0
};

static const int http_parser_start = 1;
static const int http_parser_first_final = 78;
static const int http_parser_error = 0;

static const int http_parser_en_main = 1;


#line 187 "http11_parser_mongrel.rl"

int http_parser_init(http_parser *parser) {
  int cs = 0;
  
#line 286 "http11_parser_mongrel.cpp"
	{
	cs = http_parser_start;
	}

#line 191 "http11_parser_mongrel.rl"
  parser->cs = cs;
  parser->body_start = 0;
  parser->content_len = 0;
  parser->mark = 0;
  parser->nread = 0;
  parser->field_len = 0;
  parser->field_start = 0;
  parser->xml_sent = 0;
  parser->json_sent = 0;

  return(1);
}


/** exec **/
size_t http_parser_execute(http_parser *parser, const char *buffer, size_t len, size_t off)  
{
  if(len == 0) return 0;

  const char *p, *pe;
  int cs = parser->cs;

  assert(off <= len && "offset past end of buffer");

  p = buffer+off;
  pe = buffer+len;

  assert(pe - p == (int)len - (int)off && "pointers aren't same distance");

  
#line 322 "http11_parser_mongrel.cpp"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _http_parser_trans_keys + _http_parser_key_offsets[cs];
	_trans = _http_parser_index_offsets[cs];

	_klen = _http_parser_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _http_parser_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _http_parser_indicies[_trans];
	cs = _http_parser_trans_targs[_trans];

	if ( _http_parser_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _http_parser_actions + _http_parser_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 53 "http11_parser_mongrel.rl"
	{MARK(mark, p); }
	break;
	case 1:
#line 56 "http11_parser_mongrel.rl"
	{ MARK(field_start, p); }
	break;
	case 2:
#line 57 "http11_parser_mongrel.rl"
	{ 
    parser->field_len = LEN(field_start, p);
  }
	break;
	case 3:
#line 61 "http11_parser_mongrel.rl"
	{ MARK(mark, p); }
	break;
	case 4:
#line 63 "http11_parser_mongrel.rl"
	{
    if(parser->http_field != NULL) {
      parser->http_field(parser->instance, parser->data, PTR_TO(field_start), parser->field_len, PTR_TO(mark), LEN(mark, p));
    }
  }
	break;
	case 5:
#line 69 "http11_parser_mongrel.rl"
	{ 
    if(parser->request_method != NULL) 
      parser->request_method(parser->instance, parser->data, PTR_TO(mark), LEN(mark, p));
  }
	break;
	case 6:
#line 74 "http11_parser_mongrel.rl"
	{ 
    if(parser->request_uri != NULL)
      parser->request_uri(parser->instance, parser->data, PTR_TO(mark), LEN(mark, p));
  }
	break;
	case 7:
#line 79 "http11_parser_mongrel.rl"
	{
    if(parser->fragment != NULL)
      parser->fragment(parser->instance, parser->data, PTR_TO(mark), LEN(mark, p));
  }
	break;
	case 8:
#line 84 "http11_parser_mongrel.rl"
	{MARK(query_start, p); }
	break;
	case 9:
#line 85 "http11_parser_mongrel.rl"
	{ 
    if(parser->query_string != NULL)
      parser->query_string(parser->instance, parser->data, PTR_TO(query_start), LEN(query_start, p));
  }
	break;
	case 10:
#line 90 "http11_parser_mongrel.rl"
	{	
    if(parser->http_version != NULL)
      parser->http_version(parser->instance, parser->data, PTR_TO(mark), LEN(mark, p));
  }
	break;
	case 11:
#line 95 "http11_parser_mongrel.rl"
	{
    if(parser->request_path != NULL)
      parser->request_path(parser->instance, parser->data, PTR_TO(mark), LEN(mark,p));
  }
	break;
	case 12:
#line 100 "http11_parser_mongrel.rl"
	{
      if(parser->xml_sent || parser->json_sent) {
        parser->body_start = PTR_TO(mark) - buffer;
        // +1 includes the \0
        parser->content_len = p - buffer - parser->body_start + 1;
      } else {
        parser->body_start = p - buffer + 1;

        if(parser->header_done != NULL) {
          parser->header_done(parser->instance, parser->data, p + 1, pe - p - 1);
        }
      }
    {p++; goto _out; }
  }
	break;
	case 13:
#line 115 "http11_parser_mongrel.rl"
	{
      parser->xml_sent = 1;
  }
	break;
	case 14:
#line 119 "http11_parser_mongrel.rl"
	{
      parser->json_sent = 1;
  }
	break;
#line 497 "http11_parser_mongrel.cpp"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 221 "http11_parser_mongrel.rl"

  assert(p <= pe && "Buffer overflow after parsing.");

  if (!http_parser_has_error(parser)) {
      parser->cs = cs;
  }

  parser->nread += p - (buffer + off);

  assert(parser->nread <= len && "nread longer than length");
  assert(parser->body_start <= len && "body starts after buffer end");
  assert(parser->mark < len && "mark is after buffer end");
  assert(parser->field_len <= len && "field has length longer than whole buffer");
  assert(parser->field_start < len && "field starts after buffer end");

  return(parser->nread);
}

int http_parser_finish(http_parser *parser)
{
  if (http_parser_has_error(parser) ) {
    return -1;
  } else if (http_parser_is_finished(parser) ) {
    return 1;
  } else {
    return 0;
  }
}

int http_parser_has_error(http_parser *parser) {
  return parser->cs == http_parser_error;
}

int http_parser_is_finished(http_parser *parser) {
  return parser->cs >= http_parser_first_final;
}
