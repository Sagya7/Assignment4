#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "inttypes.h"
#include <math.h>
#include <time.h>
#include <iostream>

#define maximum64unsignednumber 18446744073709551615



// C++ program to convert a decimal
// number to binary number

#include <iostream>
using namespace std;

uint64_t p;
uint64_t g;
uint64_t x;
uint64_t y;

typedef struct privateandpublickey
{
	uint64_t publickey;
	uint64_t privatekey;
};

void delay(int second) {
	int milsec = 1000 * second;
	clock_t startTime = clock();
	while (clock() < (startTime + milsec));
}

//modulo inverse extended Euclid Algorithm
uint64_t modInverse(uint64_t a, uint64_t m)
{
	uint64_t m0 = m;
	int64_t y = 0, x = 1;
	uint64_t q, t;
	if (m == 1) {
		return 0;
	}
	while (a > 1) {
		q = a / m;
		t = m;
		m = a % m;
		a = t;
		t = y;
		y = x - q * y;
		x = t;
	}
	// If x is negative make it positive
	if (x < 0)
		x += m0;
	return x;
}

uint64_t power(uint64_t x, uint64_t y)
{
	uint64_t res = 1;

	while (y > 0)
	{
		if (y & 1)
			res = res*x;

		y = y >> 1;
		x = x*x;
	}
	return res;
}

//26 bit random number
uint64_t rand_uint64(void) {
	srand(time(NULL));
	for (int i = 0; i < 2; i++)
	{
		printf(".");
		delay(1);
	}
	printf("\n");
	uint64_t r = rand();
	r = r << 5 | rand();
	r = r << 5 | rand();
	return (r % 33554432);
}

uint64_t fastexponent(uint64_t g, uint64_t x, uint64_t modp)
{
	uint64_t result = 1;
	uint8_t binaryNum[64];
	// counter for binary array
	int i = 0;
	while (x > 0) {

		// storing remainder in binary array
		binaryNum[i] = x % 2;
		x = x / 2;
		i++;
	}

	for (int j = i - 1; j >= 0; j--)
	{
		if (binaryNum[j] == 1)
		{
			result = (result * result) % modp;
			result = (result *g) % modp;
		}
		else if (binaryNum[j] == 0)
		{
			result = (result * result) % modp;
		}
	}

	return result;
}

privateandpublickey AliceConfiguration()
{
	privateandpublickey keysofAlice;

	//private key
	printf("generating private key for alice..\n");
	x = rand_uint64();

	keysofAlice.privatekey = x;

	printf("Private key of Alice is %llu \n", keysofAlice.privatekey);

	//g^x modp is public key
	keysofAlice.publickey = fastexponent(g, keysofAlice.privatekey, p);

	printf("Public key of Alice which goes to bob is %llu \n", keysofAlice.publickey);

	return keysofAlice;

}

privateandpublickey BobConfiguration()
{
	privateandpublickey keysofBob;

	//private key
	printf("generating private key for alice..\n");
	y = rand_uint64();

	keysofBob.privatekey = y;

	printf("Private key of bob is %llu \n", keysofBob.privatekey);

	//g^y modp is public key
	keysofBob.publickey = fastexponent(g, keysofBob.privatekey, p);

	printf("Public key of bob which goes to alice is %llu \n", keysofBob.publickey);

	return keysofBob;

}

int main()
{

	privateandpublickey alicekeys, bobkeys;

	uint64_t sessionkeyatalice, sessionkeyatbob;

	uint64_t message,message1;

	printf("Enter a prime number which is less than 2,147,483,647\n");
	scanf_s("%llu", &p);

	printf("Randomly generating g\n");
	g = rand_uint64();

	g = g % p;

	printf("Randomly selected g is %llu\n", g);

	alicekeys = AliceConfiguration();

	bobkeys = BobConfiguration();

	sessionkeyatalice = fastexponent(bobkeys.publickey, alicekeys.privatekey, p);
	printf("session key at alice's place is  ... %llu\n", sessionkeyatalice);

	sessionkeyatbob = fastexponent(alicekeys.publickey, bobkeys.privatekey, p);
	printf("session key at bobs's place is   ... %llu\n", sessionkeyatbob);

	//decrypt
	uint64_t sessionkeyatbobinverse, sessionkeyataliceinverse;
	sessionkeyatbobinverse   = modInverse(sessionkeyatbob, p);
	sessionkeyataliceinverse = modInverse(sessionkeyatalice, p);

	printf("Alice can now send Message to Bob.. Message length is restricted to %llu",p-1);
	printf("\nEnter the message\n");

	scanf_s("%llu", &message);

	//encrypt the message
	uint64_t e;

	e = (message * sessionkeyatalice);
	e = e % p;

	printf("Sending cypher to Bob");

	for (int i = 0; i < 5; i++)
	{
		printf(".");
		delay(1);
	}
	printf("\n");

	printf("The cypher sent to bob is %llu\n", e);

	printf("Bob is decrypting the message");
	for (int i = 0; i < 5; i++)
	{
		printf(".");
		delay(1);
	}
	printf("\n");

	uint64_t d;
	d = (e * sessionkeyatbobinverse);
	d = d % p;

	printf("The decrypted message is %llu\n", d);

	//Now Bob can send the message to Alice.

	printf("\n\n\n");
	printf("Now Bob is trying to send message\n");
	printf("Message length is restricted to %llu", p - 1);
	printf("\nEnter the message\n");
	scanf_s("%llu", &message1);

	//encrypt the message
	uint64_t e1;

	e1 = (message1 * sessionkeyatbob);
	e1 = e1 % p;

	printf("Sending cypher to Alice");

	for (int i = 0; i < 5; i++)
	{
		printf(".");
		delay(1);
	}
	printf("\n");

	printf("The cypher sent to Alice is %llu\n", e1);

	printf("Alice is decrypting the message");
	for (int i = 0; i < 5; i++)
	{
		printf(".");
		delay(1);
	}
	printf("\n");

	uint64_t d1;
	d1 = (e1 * sessionkeyataliceinverse);
	d1 = d1 % p;

	printf("The decrypted message is %llu\n", d1);

	return 0;
}