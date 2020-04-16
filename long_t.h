#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

enum {
	D_LENGTH = 50000,
	D_RES = 6,
	BASE = 10000,
	EL_BASE = 10,
	ERR = 2
};

typedef unsigned short body_t;
typedef unsigned long long ext_t;

class long_t
{
private:
	unsigned length;
	unsigned res;
	body_t* body;
public:
	ext_t int_part;
	static unsigned mul_count;
	long_t(ext_t = 0, unsigned = D_LENGTH, unsigned = D_RES);
	long_t(string);
	long_t(const long_t &);
	~long_t();
	unsigned get_length() const;
	unsigned get_res() const;
	void clear();
	long_t& operator =(const long_t&);
	long_t& operator++();
	body_t operator[](unsigned) const;
	body_t &operator[](unsigned);
};

body_t to_num(char);

long_t operator ~(const long_t&);
long_t operator -(const long_t&);
long_t operator+(const long_t&, const long_t&);
long_t operator -(const long_t&, const long_t&);
long_t operator *(const long_t&, const long_t&);
bool operator <(const long_t&, const long_t&);
istream& operator >> (istream&, long_t&);
ostream& operator <<(ostream&, const long_t&);

void write_string(string&, long_t&);
unsigned diff_order(const long_t&, const long_t &);
long_t invert(const long_t&);
long_t invert_sqrt(const long_t&);
long_t my_sqrt(const long_t&);
long_t compute_pi(clock_t, ofstream&);