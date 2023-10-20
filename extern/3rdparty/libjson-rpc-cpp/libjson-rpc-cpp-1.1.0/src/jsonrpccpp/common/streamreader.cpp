#include "streamreader.h"
#include <stdlib.h>
#include <unistd.h>
#ifdef _WIN32
#include <io.h> // read()
#define read _read
#endif // _WIN32

using namespace jsonrpc;
using namespace std;

StreamReader::StreamReader(size_t buffersize)
    : buffersize(buffersize), buffer(static_cast<char *>(malloc(buffersize))) {}

StreamReader::~StreamReader() { free(buffer); }

bool StreamReader::Read(std::string &target, int fd, char delimiter) {
  ssize_t bytesRead;
  do {
    bytesRead = read(fd, this->buffer, buffersize);
    if (bytesRead < 0) {
      return false;
    } else {
      target.append(buffer, static_cast<size_t>(bytesRead));
    }
  } while (target.find(delimiter) == string::npos && bytesRead > 0);

  target.pop_back();
  return true;
}
