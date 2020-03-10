#include<iostream>
#include <sstream>
#include <string>
#include <map>
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"

using Poco::Base64Encoder;
using Poco::Base64Decoder;




std::string authenticate(std::string& _username, std::string& _password)
{
	std::ostringstream ostr;
	Base64Encoder encoder(ostr);
	encoder.rdbuf()->setLineLength(0);
	encoder << _username << ":" << _password;
	encoder.close();
	return ostr.str();
}

auto parseAuthInfo(const std::string& authInfo)
{
	static const int eof = std::char_traits<char>::eof();
	std::string _username, _password;

	std::istringstream istr(authInfo);
	Base64Decoder decoder(istr);
	int ch = decoder.get();
	while (ch != eof && ch != ':')
	{
		_username += (char)ch;
		ch = decoder.get();
	}
	if (ch == ':') ch = decoder.get();
	while (ch != eof)
	{
		_password += (char)ch;
		ch = decoder.get();
	}
	return std::pair<std::string, std::string>(_username, _password);
}