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
#include <reflection/magic.hpp>

#include <reflection/basic_types.hpp>

#include <utility/file_reader_writer.hpp>

#include <vector>

using namespace std;

int main(int argc, char** argv) {
    vector<string> strings { "Alpha", "Bravo", "Charlie" };

    puts(reflection::reflectToString(strings).c_str());

    // dump vector to file
    FILE* file = fopen("array.test", "wb");
    assert(file != nullptr);

    utility::FileReaderWriter wr(file);
    reflection::reflectSerialize(strings, &wr);

    fclose(file);

    vector<string> newStrings;

    // load back in
    file = fopen("array.test", "rb");
    assert(file != nullptr);

    utility::FileReaderWriter rd(file);
    reflection::reflectDeserialize(newStrings, &rd);

    fclose(file);

    puts(reflection::reflectToString(newStrings).c_str());
}

#include <reflection/default_error_handler.cpp>