/*
    Boost Software License - Version 1.0 - August 17, 2003

    Permission is hereby granted, free of charge, to any person or organization
    obtaining a copy of the software and accompanying documentation covered by
    this license (the "Software") to use, reproduce, display, distribute,
    execute, and transmit the Software, and to prepare derivative works of the
    Software, and to permit third-parties to whom the Software is furnished to
    do so, all subject to the following:

    The copyright notices in the Software and this entire statement, including
    the above license grant, this restriction and the following disclaimer,
    must be included in all copies of the Software, in whole or in part, and
    all derivative works of the Software, unless such copies or derivative
    works are solely in the form of machine-executable object code generated by
    a source language processor.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
    SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
    FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <reflection/api.hpp>
#include <reflection/basic_types.hpp>
#include <reflection/rpc.hpp>

#include <utility/memory_reader_writer.hpp>

using std::string;

// SERVER

int getResourceFromServer(string resourceName, unsigned int maxSize, bool cached) {
    printf("[SERVER]\tgetResourceFromServer(%s, %u, %d)\n", resourceName.c_str(), maxSize, cached);

    return 42;
}

DEFINE_RPC_SERIALIZED(getResourceFromServerWrapper, getResourceFromServer)

// CLIENT

int getResourceFromServer(string resource, unsigned int maxSize, bool cached);

RPC_SERIALIZED_3(getResourceFromServerRPC, getResourceFromServer)

int main(int argc, char* argv[]) {
    int result = getResourceFromServerRPC("/test", 3000, true);
    printf("[CLIENT]\tResult is %d\n", result);
}

// RPC TRANSPORT IMPLEMENTATION

namespace rpc {
    string rpcFunctionName;
    utility::MemoryReaderWriter io;

    bool beginRPC(const char* functionName, IWriter*& writer_out, IReader*& reader_out) {
        rpcFunctionName = functionName;
        writer_out = &io;
        reader_out = &io;
        return true;
    }

    bool invokeRPC() {
        printf("[RPC]\t%u bytes of arguments to server\n", io.writePos);
        auto w1 = io.writePos;

        if (rpcFunctionName == "getResourceFromServer")
            assert(getResourceFromServerWrapper(&io, &io));
        else
            fprintf(stderr, "Invalid RPC %s\n", rpcFunctionName.c_str());

        printf("[RPC]\t%u bytes of response from server\n", io.writePos - w1);
        return true;
    }

    void endRPC() {
        io.reset();
    }
}

#include <reflection/default_error_handler.cpp>

/*
[rpc] 9 bytes of arguments to server
[SERVER] getResourceFromServer(/test, 3000, 1)
[rpc] 1 bytes of response from server
[CLIENT] Result is: 42
*/