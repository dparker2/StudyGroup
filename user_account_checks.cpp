#include <iostream>
#include <string>
#include <cstddef>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/* TO DO: test code, check if all libraries are required
	add special char check, email check, user check
	
	known issues: lengthcheck function
*/
class Password {
	public:
		bool SpecialCharactercCheck (const char * pstr);
		bool UppercaseCheck (const char *pstr);
		bool LengthCheck (const char *pstr);
		bool NumCheck (const char *pstr);
};
class Email {
	public:
		EmailCheck (const char *estr);
};
class Username {
	public:
		UsernameCheck (const char *ustr);
};

// check for at least 1 special character
bool SpecialCharacterCheck (const char *pstr){
	size_t special;
	int i = 0;
	while (pstr[i]) {
		
		i++;
	}

}

// check for  at least 1 upper case character
bool UppercaseCheck (const char *pstr){
	int i = 0;
	char c;
	while (pstr[i]) {
		c = pstr[i];
		if(isupper(c)) return true;
		i++;
	}
}

// if length != <= 8, not long enough
// also if length >= 16, too long
bool LengthCheck (const char *pstr){
	int pl = pstr.size();
	if (pl <= 8 || pl >= 16) return false;
	else return true;
}

//check for at least 1 number
bool NumCheck (const char *pstr) {
	int i = 0;
	char c;
	while (pstr[i]) {
		c = pstr[i];
		if(isdigit(c)) return true;
		i++;
	}
}

//needs at least, in this order: 1 character, @, 1 character
bool EmailCheck (const char *estr) {

}

/* needs to check against every username currently in database
   length >3 && length <16
   allow special characters / spaces ?
   any checks for cursing/slurs/etc?
*/
bool UsernameCheck (const char *ustr) {

}
