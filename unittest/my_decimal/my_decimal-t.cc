/* Copyright (c) 2013, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1335  USA */

#include "my_config.h"
#include "config.h"
#include <tap.h>
#include <my_global.h>
#include <my_sys.h>
#include <m_string.h>
#include <sql_string.h>
#include <my_decimal.h>



/*
  Test my_decimal constuctor and assignement operators
*/
static int
test_copy_and_compare()
{
  my_decimal d1,d2;

  ulonglong val= 42;

  ok(ulonglong2decimal(val,&d1) == 0, "Pass");
  d2= d1;
  my_decimal d3(d1);

  ok(my_decimal_cmp(&d1, &d2) == 0, "Pass");
  ok(my_decimal_cmp(&d2, &d3) == 0, "Pass");
  ok(my_decimal_cmp(&d3, &d1) == 0,"Pass");

  ulonglong val1, val2, val3;
  ok(decimal2ulonglong(&d1, &val1) == 0, "Pass");
  ok(decimal2ulonglong(&d2, &val2) == 0,"Pass");
  ok(decimal2ulonglong(&d3, &val3) == 0,"Pass");

  ok(val == val1,"Pass");
  ok(val == val2,"Pass");
  ok(val == val3,"Pass");

  // The CTOR/operator=() generated by the compiler would fail here:
  val= 45;
  ok(ulonglong2decimal(val, &d1) == 0,"Pass");
  ok(my_decimal_cmp(&d1, &d2) == 1,"Pass");
  ok(my_decimal_cmp(&d1, &d3) == 1,"Pass");

  return 0;

}

static int
test_decimal2string()
{
  decimal_t d1;
  decimal_digit_t buffer[DECIMAL_BUFF_LENGTH+2];
  char *str_end;
  const char strnum[]= "0.1234567890123456789012345678901234567890123467";
  char strbuff[50];
  int len= 40;
  int i;

  bzero(strbuff, sizeof(strbuff));
  str_end= (char *)(strnum + (sizeof(strnum) - 1));

  d1.len= DECIMAL_BUFF_LENGTH + 2;
  d1.buf= buffer;

  string2decimal(strnum, &d1, &str_end);
  decimal2string(&d1, strbuff, &len, 0, 0, 'X');

  /* last digit is not checked due to possible rounding */
  for (i= 0; i < 38 && strbuff[i] == strnum[i]; i++);
  ok(i == 38, "Number");
  for (i= 39; i < 50 && strbuff[i] == 0; i++);
  ok(i == 50, "No overrun");

  return 0;

}
int main()
{
  plan(15);
  diag("Testing my_decimal constructor and assignment operators");

  test_copy_and_compare();
  test_decimal2string();

  return exit_status();
}