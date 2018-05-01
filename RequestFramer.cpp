#include <assert.h>
#include "RequestFramer.hpp"

using namespace std;
const string DELIM = "\r\n\r\n";

void RequestFramer::append(string chars)
{
    requestBuffer.append(chars);
}

bool RequestFramer::hasMessage() const
{
    return requestBuffer.find(DELIM) != string::npos;
}

string RequestFramer::topMessage() const
{
    string topMessage = requestBuffer.substr(0, requestBuffer.find(DELIM)+DELIM.size());
    return topMessage;
}

void RequestFramer::popMessage()
{
    requestBuffer = requestBuffer.substr(requestBuffer.find(DELIM) + DELIM.size());
}

void RequestFramer::printToStream(ostream& stream) const
{
    stream << requestBuffer;
}
