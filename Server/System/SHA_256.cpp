#include "SHA_256.h"
using namespace Mafia;
using namespace System;


SHA256* hasher = new SHA256();

// Start of SHA256 variable initializations
const string SHA256::HEX_VALUES = "0123456789abcdef",
             SHA256::SHA_VERSION = "Complete SHA library, Dennis Andriesse, v1.0 (rev July 2008). ";

const string SHA256::ERRORS[] = { "EXCEPTION. ", "Out of bounds. ", "Invalid hash length. ", "Invalid chunk size. " };

const word SHA256::ROUND_CONSTANTS[EXTENDED_WORD_AMOUNT] =
{
    0x428a2f98ULL, 0x71374491ULL, 0xb5c0fbcfULL, 0xe9b5dba5ULL, 0x3956c25bULL, 0x59f111f1ULL, 0x923f82a4ULL, 0xab1c5ed5ULL,
    0xd807aa98ULL, 0x12835b01ULL, 0x243185beULL, 0x550c7dc3ULL, 0x72be5d74ULL, 0x80deb1feULL, 0x9bdc06a7ULL, 0xc19bf174ULL,
    0xe49b69c1ULL, 0xefbe4786ULL, 0x0fc19dc6ULL, 0x240ca1ccULL, 0x2de92c6fULL, 0x4a7484aaULL, 0x5cb0a9dcULL, 0x76f988daULL,
    0x983e5152ULL, 0xa831c66dULL, 0xb00327c8ULL, 0xbf597fc7ULL, 0xc6e00bf3ULL, 0xd5a79147ULL, 0x06ca6351ULL, 0x14292967ULL,
    0x27b70a85ULL, 0x2e1b2138ULL, 0x4d2c6dfcULL, 0x53380d13ULL, 0x650a7354ULL, 0x766a0abbULL, 0x81c2c92eULL, 0x92722c85ULL,
    0xa2bfe8a1ULL, 0xa81a664bULL, 0xc24b8b70ULL, 0xc76c51a3ULL, 0xd192e819ULL, 0xd6990624ULL, 0xf40e3585ULL, 0x106aa070ULL,
    0x19a4c116ULL, 0x1e376c08ULL, 0x2748774cULL, 0x34b0bcb5ULL, 0x391c0cb3ULL, 0x4ed8aa4aULL, 0x5b9cca4fULL, 0x682e6ff3ULL,
    0x748f82eeULL, 0x78a5636fULL, 0x84c87814ULL, 0x8cc70208ULL, 0x90befffaULL, 0xa4506cebULL, 0xbef9a3f7ULL, 0xc67178f2ULL
    // These values are round constants, based on the first 32 bits of the fractional parts of the
    // cube roots of the first 64 prime numbers.
};

const word SHA256::INITIAL_VALUES[] =
{
    0x6a09e667ULL, // These values are used to
    0xbb67ae85ULL, // initialize the chunks of the hash.
    0x3c6ef372ULL, // They are based on the first 32 bits
    0xa54ff53aULL, // of the fractional parts of the square roots
    0x510e527fULL, // of the first 8 prime numbers.
    0x9b05688cULL,
    0x1f83d9abULL,
    0x5be0cd19ULL
};
// End of SHA256 variable initializations


/* CLASS METHOD DEFINITIONS
   ======================== */

// Start of SHA256 method definitions
void SHA256::setDefaults()
{
    h0 = INITIAL_VALUES[0], // These values are used to
    h1 = INITIAL_VALUES[1], // initialize the chunks of the hash.
    h2 = INITIAL_VALUES[2], // They are based on the first 32 bits
    h3 = INITIAL_VALUES[3], // of the fractional parts of the square roots
    h4 = INITIAL_VALUES[4], // of the first 8 prime numbers.
    h5 = INITIAL_VALUES[5],
    h6 = INITIAL_VALUES[6],
    h7 = INITIAL_VALUES[7];
}

vector<byte> SHA256::preProcess(vector<byte> input)
{
    // Appends a 1 bit to the input, then pads with 0 bits
    // until the length of the input is congruent to 448 modulo 512.
    // Finally appends the size in bits of the input as it was before the
    // padding, as a 64-bit big-endian integer.
    // If the system the algorithm is executed on is not big-endian, the
    // longword inputSize is converted to big-endian.
    // Returns the result of the preprocessing.

    const byte INITIAL_PADDING = 0x80, // A 1 bit followed by 7 0 bits.
               EXTENDED_PADDING = 0x00;

    longword inputLength = input.size() * BYTE_SIZE; // The length of the input before any preprocessing.

    input.insert(input.end(), 1, INITIAL_PADDING);
    for(int i = input.size() * BYTE_SIZE; (i % CHUNK_SIZE) != SIZE_WITHOUT_LENGTH; i += BYTE_SIZE)
    {
        input.insert(input.end(), 1, EXTENDED_PADDING); // Build byte by byte and keep checking the size.
    }

    for(int i = LONGWORD_BYTES - 1; i >= 0; i--)
    {
        // Add bytes of inputLength to input, MSB first.
        byte b = (inputLength >> i * BYTE_SIZE) & FULL_BYTE;
        input.insert(input.end(), 1, b);
    }

    return input;
}

void SHA256::processChunk(vector<byte> chunkContainer)
{
    // Processes a 512 bit chunk according to the SHA256 hash algorithm.

    vector<word> chunkWordContainer(EXTENDED_WORD_AMOUNT);
    word s0, s1;

    try
    {
        if(chunkContainer.size() != CHUNK_BYTES) throw short(INVALID_CHUNK_SIZE_E); // Throw an invalid chunk size exception.
    }
    catch(short e)
    {
        handleException(e);
    }

    for(int i = 0; i < CHUNK_BYTES; i += WORD_BYTES)
    {
        word w = bytesToWord(chunkContainer, i);
        chunkWordContainer[i / WORD_BYTES] = w; // Break into 16 words.
    }

    for(int i = 16; i < EXTENDED_WORD_AMOUNT; i++)
    {
        word w0 = chunkWordContainer[i - 15],
             w1 = chunkWordContainer[i - 2];

        s0 = (rightRotate(w0, 7)) ^ (rightRotate(w0, 18)) ^ (w0 >> 3);
        s1 = (rightRotate(w1, 17)) ^ (rightRotate(w1, 19)) ^ (w1 >> 10);

        chunkWordContainer[i] = chunkWordContainer[i - 16] + s0 + chunkWordContainer[i - 7] + s1;
    }

    word a = h0,
         b = h1,
         c = h2,
         d = h3,
         e = h4,
         f = h5,
         g = h6,
         h = h7;

    // Go into main loop
    for(int i = 0; i < EXTENDED_WORD_AMOUNT; i++)
    {
        s0 = (rightRotate(a, 2)) ^ (rightRotate(a, 13)) ^ (rightRotate(a, 22));
        word maj = (a & b) ^ (a & c) ^ (b & c);
        word t2 = s0 + maj;
        s1 = (rightRotate(e, 6)) ^ (rightRotate(e, 11)) ^ (rightRotate(e, 25));
        word ch = (e & f) ^ ((~e) & g);
        word t1 = h + s1 + ch + ROUND_CONSTANTS[i] + chunkWordContainer[i];

        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    // Add this chunk's hash to the overall result
    h0 = h0 + a;
    h1 = h1 + b;
    h2 = h2 + c;
    h3 = h3 + d;
    h4 = h4 + e;
    h5 = h5 + f;
    h6 = h6 + g;
    h7 = h7 + h;
}

word SHA256::bytesToWord(vector<byte> input, unsigned int start)
{
    // Combines 4 bytes from input
    // into a single word, starting at start.
    // Returns the generated word.

    word result = 0;

    try
    {
        if(WORD_BYTES - 1 + start >= input.size()) throw short(OUT_OF_BOUNDS_E); // Throw an out of bounds exception.
        for(int i = WORD_BYTES - 1; i >= 0; i--)
        {
            if(i != WORD_BYTES - 1) result >>= BYTE_SIZE;
            result |= ((word) input[i + start] << (WORD_BYTES - 1) * BYTE_SIZE);
        }
    }
    catch(short e)
    {
        handleException(e);
    }

    return result;
}

string SHA256::wVectorToHexString(vector<word> input)
{
    // Converts a vector<word> to a string containing the
    // hexadecimal representation of all words in the vector.
    // Returns this string.

    string result = "";

    for(unsigned int i = 0; i < input.size(); i++)
    {
        word w = input[i];

        for(int j = WORD_BYTES * BYTE_HEX_DIGITS - 1; j >= 0; j--)
        {
            result += HEX_VALUES.at((w >> j * HEX_DIGIT_BITS) & HALF_BYTE);
        }
    }

    return result;
}

word SHA256::rightRotate(word w, unsigned int n)
{
    // Rotates the word to the right by n bits, wrapping around to the left
    // if the end is reached. Returns the result.

    return ((w >> n) | (w << (WORD_SIZE - n)));
}

void SHA256::handleException(short errorId)
{
    cerr << shaVersion() << endl << ERRORS[0] << ERRORS[errorId] << endl;
    //exit(1);
}

vector<word> SHA256::hash(vector<byte> input)
{
    // Hashes the input and returns the result.

    vector<byte> formatted = preProcess(input), // Do preprocessing and set the result in formatted.
                 chunkContainer;
    vector<word> result;

    for(unsigned int i = 0; i < formatted.size(); i += CHUNK_BYTES)
    {
        chunkContainer.assign(formatted.begin() + i, formatted.begin() + i + CHUNK_BYTES);
        processChunk(chunkContainer); // Process the current 512 bit chunk.
    }

    result.insert(result.end(), 1, h0);
    result.insert(result.end(), 1, h1);
    result.insert(result.end(), 1, h2);
    result.insert(result.end(), 1, h3);
    result.insert(result.end(), 1, h4);
    result.insert(result.end(), 1, h5);
    result.insert(result.end(), 1, h6);
    result.insert(result.end(), 1, h7);

    setDefaults();

    return result;
}

SHA256::SHA256()
{
    // SHA256 main constructor.

    setDefaults();
}

SHA256::~SHA256()
{
    // SHA256 main destructor.
}

string SHA256::hash(string input)
{
    // Hashes a string using this class's generic hash(vector<byte> input) function.
    // Returns the result as a string.

    vector<byte> inputBytes;
    string result;

    for(unsigned int i = 0; i < input.size(); i++)
    {
        inputBytes.insert(inputBytes.end(), 1, input.at(i));
    }

    try
    {
        result = wVectorToHexString(hash(inputBytes));
        if(result.size() != DIGEST_CHARS) throw short(INVALID_HASH_LENGTH_E); // Throw an invalid hash length exception.
    }
    catch(short e)
    {
        handleException(e);
    }

    return result;
}

string SHA256::hash(char* input, longword size)
{
    // Hashes an array of chars, meant for use when hashing binary files
    // or other data read in as a stream of bytes.
    // Can also be used for C-style strings. When using this function
    // for that purpose, don't include the terminating character at the end
    // of the string when calculating size, as this may lead to incorrect
    // hash values.
    // Uses this class's generic hash(vector<byte> input) function.
    // The size of input (or the amount of bytes you want to be taken from
    // input) must be passed to this function.
    // Returns the result as a string.

    vector<byte> inputBytes;
    string result;

    for(longword i = 0; i < size; i++)
    {
        inputBytes.insert(inputBytes.end(), 1, input[i]);
    }

    try
    {
        result = wVectorToHexString(hash(inputBytes));
        if(result.size() != DIGEST_CHARS) throw short(INVALID_HASH_LENGTH_E); // Throw an invalid hash length exception.
    }
    catch(short e)
    {
        handleException(e);
    }

    return result;
}

string SHA256::shaVersion()
{
    // Returns a string containing information
    // about the version of the library.

    return SHA_VERSION;
}
