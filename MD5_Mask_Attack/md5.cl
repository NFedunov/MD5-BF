#include "struct.h"

//#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable

#define F(x, y, z)	((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)	((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)	((x) ^ (y) ^ (z))
#define I(x, y, z)	((y) ^ ((x) | ~(z)))
#define GET(i)		(X[(i)])

#define STEP(f, a, b, c, d, x, t, s) \
    (a) += f((b), (c), (d)) + (x) + (t); \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s)))); \
    (a) += (b);

void md5Round(uint* hash, const uint* X)
{
	uint a, b, c, d;
	a = hash[0];
	b = hash[1];
	c = hash[2];
	d = hash[3];

	/* Round 1 */
	STEP(F, a, b, c, d, GET(0), 0xd76aa478, 7)	
	STEP(F, d, a, b, c, GET(1), 0xe8c7b756, 12)	
	STEP(F, c, d, a, b, GET(2), 0x242070db, 17)	
	STEP(F, b, c, d, a, GET(3), 0xc1bdceee, 22)	
	STEP(F, a, b, c, d, GET(4), 0xf57c0faf, 7)	
	STEP(F, d, a, b, c, GET(5), 0x4787c62a, 12)	
	STEP(F, c, d, a, b, GET(6), 0xa8304613, 17)	
	STEP(F, b, c, d, a, GET(7), 0xfd469501, 22)	
	STEP(F, a, b, c, d, GET(8), 0x698098d8, 7)	
	STEP(F, d, a, b, c, GET(9), 0x8b44f7af, 12)	
	STEP(F, c, d, a, b, GET(10), 0xffff5bb1, 17)
	STEP(F, b, c, d, a, GET(11), 0x895cd7be, 22)
	STEP(F, a, b, c, d, GET(12), 0x6b901122, 7)	
	STEP(F, d, a, b, c, GET(13), 0xfd987193, 12)
	STEP(F, c, d, a, b, GET(14), 0xa679438e, 17)
	STEP(F, b, c, d, a, GET(15), 0x49b40821, 22)
												
	/* Round 2 */								
	STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5)	
	STEP(G, d, a, b, c, GET(6), 0xc040b340, 9)	
	STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14)
	STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)	
	STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5)	
	STEP(G, d, a, b, c, GET(10), 0x02441453, 9)	
	STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
	STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)	
	STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5)	
	STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9)	
	STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14)	
	STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20)	
	STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5)	
	STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)	
	STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14)	
	STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)
												
	/* Round 3 */								
	STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4)	
	STEP(H, d, a, b, c, GET(8), 0x8771f681, 11)	
	STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
	STEP(H, b, c, d, a, GET(14), 0xfde5380c, 23)
	STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4)	
	STEP(H, d, a, b, c, GET(4), 0x4bdecfa9, 11)	
	STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)	
	STEP(H, b, c, d, a, GET(10), 0xbebfbc70, 23)
	STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4)	
	STEP(H, d, a, b, c, GET(0), 0xeaa127fa, 11)	
	STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16)	
	STEP(H, b, c, d, a, GET(6), 0x04881d05, 23)	
	STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4)	
	STEP(H, d, a, b, c, GET(12), 0xe6db99e5, 11)
	STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
	STEP(H, b, c, d, a, GET(2), 0xc4ac5665, 23)	
												
	/* Round 4 */								
	STEP(I, a, b, c, d, GET(0), 0xf4292244, 6)	
	STEP(I, d, a, b, c, GET(7), 0x432aff97, 10)	
	STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15)
	STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21)	
	STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6)	
	STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)	
	STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15)
	STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21)	
	STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)	
	STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
	STEP(I, c, d, a, b, GET(6), 0xa3014314, 15)	
	STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
	STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6)	
	STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10)
	STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)	
	STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21)	
	hash[0] = a + hash[0];
	hash[1] = b + hash[1];
	hash[2] = c + hash[2];
	hash[3] = d + hash[3];
}

void md5(const char* message, unsigned int messageLength, unsigned int* hash)
{
	hash[0] = 0x67452301;
	hash[1] = 0xefcdab89;
	hash[2] = 0x98badcfe;
	hash[3] = 0x10325476;
	uint bytesLeft = messageLength;
	for (; bytesLeft >= 64; bytesLeft -= 64, message = &message[64])
	{
		md5Round(hash, (const uint*)message);
	}
	char X[64];
	for (uint i = 0; i < bytesLeft; i++)
	{
		X[i] = message[i];
	}
	//append padding bits
	X[bytesLeft++] = 0x80;
	if (bytesLeft > 56)
	{
		for (int i = bytesLeft; i < 64; X[i++] = 0);
		md5Round(hash, (const uint*)X);
		bytesLeft = 0;
	}
	for (; bytesLeft < 56; X[bytesLeft++] = 0);
	for (; bytesLeft < 64; bytesLeft++)
	{
		X[bytesLeft] = (messageLength & 0xff) * 8;
		messageLength >>= 8;
	}
	md5Round(hash, (const uint*)X);
}

__kernel void calcHashes(__global struct password_s* passwords, uint passwordLength, __global struct hash_s* hashes)
{
	const uint id = get_global_id(0);
	struct password_s buffer;
	for(uint i = 0; i < passwordLength; i++)
	{
		buffer.password[i] = passwords[id].password[i];
	}
	uint hash[4];
	md5(buffer.password, passwordLength, hash);
	for (uint i = 0; i < 4; i++)
	{
		hashes[id].h[i] = hash[i];
	}
}