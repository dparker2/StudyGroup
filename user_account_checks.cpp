#include <regex>
using namespace std;
/* TO DO: test code, check if all libraries are required
	add  user check
	test all functions
	
	known issues: lengthcheck function
*/
class Password {
	public:
		bool SpecialCharactercCheck (string pstr);
		bool UppercaseCheck (string pstr);
		bool LengthCheck (string pstr);
		bool NumCheck (string pstr);
};
class Email {
	public:
		EmailCheck (string estr);
};
class Username {
	public:
		UsernameCheck (string ustr);
};

// check for at least 1 special character
bool SpecialCharacterCheck (string pstr){
	int i = 0;
	char c;
	while (pstr[i]) {
		c = pstr[i];
		if (isalpha(c) && isdigit(c) == false) return true;
	}
	return false;
}

// check for  at least 1 upper case character
bool UppercaseCheck (string pstr){
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
bool LengthCheck (string pstr){
	if (pstr.size() <= 8 || pstr.size() >= 16) return false;
	else return true;
}

//check for at least 1 number
bool NumCheck (string pstr) {
	int i = 0;
	char c;
	while (pstr[i]) {
		c = pstr[i];
		if(isdigit(c)) return true;
		i++;
	}
}

//needs at least, in this order: 1 character, @, 1 character
bool EmailCheck (string estr) {
	const std::regex pattern;
	
	return std::regex_match(estr, pattern);
}

/* needs to check against every username currently in database
   length >3 && length <16
   allow special characters / spaces ?
   any checks for cursing/slurs/etc?
*/
bool UsernameCheck (string ustr) {


}

int main(){
	return 0;
}
