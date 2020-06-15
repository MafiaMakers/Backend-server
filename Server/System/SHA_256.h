////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2008, Dennis Andriesse                                   //
// [email]dae400@few.vu.nl[/email]                                                     //
//                                                                        //
// This is a C++ implementation of NIST's SHA hash algorithms.            //
// This code was written for my entertainment and may contain unnoticed   //
// security flaws.                                                        //
//                                                                        //
// This code may be used non-commercially (for your entertainment).       //
// See the section "LICENSE" for details on terms of use.                 //
//                                                                        //
// Complete SHA library, Dennis Andriesse, v1.0 (rev July 2008)           //
////////////////////////////////////////////////////////////////////////////

/*
NOTES
Most importantly, if you find errors in my SHA implementations (e.g., you are
absolutely certain that something you're hashing is giving an incorrect result),
please take a minute to inform me so I can fix it. You can reach me about this or
any questions at the above e-mail address.

Also very importantly, I've included the SHA-0 and SHA-1 algorithms. This is
mostly for completeness, and in case you really need them for some good reason.
Otherwise, SHA-0 and SHA-1 should not be used, as they are considered broken
and outdated (in fact, SHA-0 was even officially withdrawn by NIST).

LICENSE
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

A copy of the GNU General Public License is available at
<http://www.gnu.org/licenses/>.

USAGE
To use these SHA implementations, simply include them in your source with
(for example) the following instruction:

    #include "SHA_lib/SHA256.h"

This assumes that SHA256.h is in a directory called SHA_lib, in the same directory
as the file you're including SHA256.h in. You can also place SHA256.h in your standard
header files/include directory and include it using:

    #include <SHA256.h>

Either way, once you've included the files, the SHA algorithms should be quite
easy to use. Simply choose which SHA version you wish to use (the classes are
called SHA0, SHA1, SHA256, SHA224, SHA512 and SHA384) and make an instance of
the corresponding class, for example:

    SHA256* yourInstanceName = new SHA256();

Then you can use the hash() function, which is in each of the classes,
to hash whatever you wish to hash. The hash() function returns a C++ style
string (so not a char[]). The hash() function takes either a C++ style string
or a char[] as arguments. The char[] version can be used for hashing binary
files, C-style strings, or anything else you can convert to a char[].
When using the char[] version of hash(), you must also specify the size of the
char[] you're passing (or of course the number of elements from it that you
want to include in the hash). I recommend that when you calculate the size of
a C-style string that you want to hash, you don't include the terminating null
character in your calculation, as including it can result in a different hash
than expected. Here's how to use the hash() function, short and simple:

    string digest = yourInstanceName->hash(stringYouWantToHash);

    or

    string digest = yourInstanceName->hash(charArrayYouWantToHash, sizeOfCharArray);

This will deliver the hash of your input into the string called digest.

If you ever want to see which version of the library you're using without going
into the source, simply use the shaVersion() method, like this:

    cout << yourInstanceName->shaVersion() << endl;

This will print information about the version of the library onto the screen.
The shaVersion() method returns a string, so you can store the version info in
a variable as well if you don't immediately want to show it on the screen.

Finally, once you're done using SHA, don't forget to delete the instance(s) you
made, to avoid memory leaks:

    delete yourInstanceName;
*/

#ifndef SHA256_H
#define SHA256_H

#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>

using namespace std;

namespace Mafia {
    namespace System {

        typedef uint8_t  byte;
        typedef uint32_t word;
        typedef uint64_t longword;

        /*!
         * \brief Класс хеширования с помощью алгоритма SHA 256. Скачал его с форума))
         */
        class SHA256
        {
            protected:
                static const unsigned short EXTENDED_WORD_AMOUNT = 64,
                                            DIGEST_CHARS         = 64,
                                            SIZE_WITHOUT_LENGTH  = 448,
                                            WORD_BYTES           = 4,
                                            LONGWORD_BYTES       = 8,
                                            CHUNK_BYTES          = 64,
                                            BYTE_HEX_DIGITS      = 2, // 2 hex digits are needed to display a byte.
                                            HEX_DIGIT_BITS       = 4, // A single hex digit corresponds to 4 bits.
                                            BYTE_SIZE            = 8, // 8 Bits.
                                            WORD_SIZE            = WORD_BYTES * BYTE_SIZE, // 32 bits.
                                            LONGWORD_SIZE        = LONGWORD_BYTES * BYTE_SIZE, // 64 bits.
                                            CHUNK_SIZE           = CHUNK_BYTES * BYTE_SIZE; // 512 bits.
                static const byte FULL_BYTE = 0xFF, // A byte with all bits set to 1.
                                  HALF_BYTE = 0x0F; // A byte with the 4 left bits set to 0 and the 4 right bits set to 1.
                static const string HEX_VALUES, SHA_VERSION;
                static const string ERRORS[]; // Contains at each index an error message corresponding to that error number.
                static const word ROUND_CONSTANTS[EXTENDED_WORD_AMOUNT],
                                  INITIAL_VALUES[];
                static const short OUT_OF_BOUNDS_E       = 1,
                                   INVALID_HASH_LENGTH_E = 2,
                                   INVALID_CHUNK_SIZE_E  = 3;

                word h0, h1, h2, h3, h4, h5, h6, h7;

                void setDefaults();
                vector<byte> preProcess(vector<byte> input);
                void processChunk(vector<byte> chunkContainer);
                word bytesToWord(vector<byte> input, unsigned int start);
                string wVectorToHexString(vector<word> input);
                word rightRotate(word w, unsigned int n);
                void handleException(short errorId);
                vector<word> hash(vector<byte> input);
            public:
                SHA256();
                ~SHA256();
                string hash(string input);
                string hash(char* input, longword size);
                string shaVersion();
        };

        extern SHA256* hasher;
    }
}



#endif
