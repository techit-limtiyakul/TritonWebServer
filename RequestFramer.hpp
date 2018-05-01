#ifndef REQUESTFRAMER_HPP
#define REQUESTFRAMER_HPP

#include <iostream>

class RequestFramer {
public:
    void append(std::string chars);

    // Does this buffer contain at least one complete message?
    bool hasMessage() const;

    // Returns the first instruction
    std::string topMessage() const;

    // Removes the first instruction
    void popMessage();

    // prints the string to an ostream (useful for debugging)
    void printToStream(std::ostream& stream) const;

protected:
    // PUT ANY FIELDS YOU NEED HERE
    std::string requestBuffer;
};

#endif // REQUESTFRAMER_HPP