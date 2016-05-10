#include <vector>
#include <string>

using namespace std;

class StrTok {
public:
	StrTok(const string & strInput, const string & strDelim = " ,") {
		string::size_type nHead, nTail;
		nHead = strInput.find_first_not_of(strDelim, 0);
		nTail = strInput.find_first_of(strDelim, nHead);

		while(nHead != string::npos || nTail != string::npos) {
			tokened.push_back(strInput.substr(nHead, nTail - nHead));
			nHead = strInput.find_first_not_of(strDelim, nTail);
			nTail = strInput.find_first_of(strDelim, nHead);
		}
	}

	std::vector<std::string> tokened;
};