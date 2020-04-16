#include "long_t.h"

long_t::long_t(const long_t& sample)
{
	*this = sample;
}

long_t::~long_t()
{
	delete[] body;
}

long_t&  long_t::operator++()
{
	for (int pos = this->get_length() - 1; pos >= 0; pos--)
	{
		if (++this->operator[](pos) < BASE)
		{
			return *this;
		}
		this->body[pos] = 0;
	}
	if (++int_part > 0) return *this;
	return *this;
}

body_t long_t::operator[](unsigned pos) const
{
	return this->body[pos];
}

body_t& long_t::operator[](unsigned pos)
{
	return this->body[pos];
}

unsigned long_t::get_length() const
{
	return this->length;
}

unsigned long_t::get_res() const
{
	return this->res;
}

void long_t::clear()
{
	int_part = 0;
	memset(body, 0, (length + res) * sizeof(*body));
}

long_t& long_t::operator =(const long_t& sample)
{
	length = sample.get_length();
	res = sample.get_res();
	int_part = sample.int_part;
	body = new body_t[length + res];
	for (unsigned pos = 0; pos < length + res; pos++)
	{
		this->operator[](pos) = sample[pos];
	}
	return *this;
}

long_t::long_t(ext_t new_int_part, unsigned new_length, unsigned new_res) :
	length (new_length), res (new_res){
	body = new body_t[length + res];
	this->clear();
	this->int_part = new_int_part;
}

long_t::long_t(string s) : long_t()
{
	write_string(s, *this);
}

body_t to_num(char c)
{
	return c - '0';
}

char to_char(body_t a)
{
	return a + '0';
}

long_t operator ~(const long_t& a)
{
	long_t result = a;
	for (int pos = result.get_length() - 1; pos >= 0; pos--)
	{
		result[pos] = BASE - 1 - result[pos];
	}
	result.int_part = ~result.int_part;
	return result;
}

long_t operator -(const long_t& a)
{
	return ++(~a);
}

long_t operator+(const long_t& a, const long_t& b)
{
	unsigned length = a.get_length();
	body_t carry = 0;
	long_t answer(0, length, a.get_res());
	for (int pos = length - 1; pos >= 0; pos--)
	{
		unsigned new_digit = (unsigned)a[pos] + (unsigned)b[pos] + carry;
		carry = new_digit / BASE;
		answer[pos] = new_digit % BASE;
	}
	answer.int_part = a.int_part + b.int_part + carry;
	return answer;
}

long_t operator -(const long_t& a, const long_t &b)
{
	return a + (-b);
}

long_t operator *(const long_t& a, const long_t& b)
{
	++long_t::mul_count;
	unsigned length = a.get_length(), res = a.get_res();
	ext_t carry = 0;
	long_t answer(0, length, a.get_res());
	for (int diag = length + res - 1; diag >= -2; diag--)
	{
		ext_t new_digit = carry;
		for (int i = diag + 1; i >= -1; i--)
		{
			int j = diag - i;
			if (i >= 0 && j >= 0)
			{
				new_digit += (ext_t)a[i] * b[j];
			}
			else if (i >= 0)
			{
				new_digit += a[i] * b.int_part;
			}
			else if (j >= 0)
			{
				new_digit += a.int_part * b[j];
			}
			else new_digit += a.int_part * b.int_part;
		}
		if (diag >= -1)
		{
			answer[diag + 1] = new_digit % BASE;
		}
		else answer.int_part += new_digit;
		carry = new_digit / BASE;
	}
	for (int diag = length + res - 1; diag >= length; diag--)
	{
		answer[diag] = 0;
	}
	return answer;
}

bool operator<(const long_t& a, const long_t& b)
{
	if (a.int_part < b.int_part) return true;
	if (b.int_part < a.int_part) return false;
	for (unsigned i = 0; i < a.get_length(); i++)
	{
		if (a[i] < b[i]) return true;
		if (b[i] < a[i]) return false;
	}
	return false;
}

unsigned long_t::mul_count;

istream& operator >> (istream& is, long_t& a)
{
	string str;
	is >> str;
	write_string(str, a);
	return is;
}

ostream& operator <<(ostream& os, const long_t& a)
{
	string result;
	int pos = a.get_length();
	while (!a[--pos]);
	for (; pos >= 0; pos--)
	{
		body_t new_digit = a[pos];
		for (unsigned i = 0; i < 4; i++)
		{
			result.push_back(to_char(new_digit % EL_BASE));
			new_digit /= EL_BASE;
		}
	}
	result.push_back('.');
	ext_t int_part = a.int_part;
	while (int_part)
	{
		result.push_back(to_char(int_part % EL_BASE));
		int_part /= EL_BASE;
	}
	reverse(result.begin(), result.end());
	os << result;
	return os;
}

void write_string(string& str, long_t& a)
{
	unsigned dot_pos = str.find('.'), length = a.get_length();
	a.clear();
	while ((str.length() - dot_pos - 1) % 4) str.push_back('0');
	for (unsigned pos = 0; pos < dot_pos; pos++)
	{
		a.int_part = a.int_part * EL_BASE + to_num(str[pos]);
	}
	for (unsigned pos = dot_pos + 1; pos < str.length(); pos++)
	{
		unsigned curr_d = (pos - dot_pos - 1) >> 2;
		a[curr_d] = a[curr_d] * EL_BASE + to_num(str[pos]);
	}
}

unsigned diff_order(const long_t& a, const long_t &b)
{
	for (unsigned i = 0; i < a.get_length(); i++)
	{
		if (a[i] != b[i])
		{
			if (a[i] == 0 && b[i] == BASE - 1 || b[i] == 0 && a[i] == BASE - 1) continue;
			return i;
		}
	}
	return a.get_length();
}

long_t invert_sqrt(const long_t &a)
{
	long_t new_ans("0.1"), ans;
	long_t half = a * (long_t)"0.5";
	const unsigned max_err = a.get_length() - ERR;
	do
	{
		ans = new_ans;
		new_ans = ans * (long_t("1.5") - half * ans * ans);
	} while (diff_order(new_ans, ans) < max_err);
	return ans;
}

long_t invert(const long_t& a)
{
	const unsigned max_err = a.get_length() - ERR;
	long_t new_ans("0.1"), ans;
	do
	{
		ans = new_ans;
		new_ans = ans * (2 - a * ans);
	} while (diff_order(new_ans, ans) < max_err);
	return ans;
}

long_t my_sqrt(const long_t& a)
{
	return invert(invert_sqrt(a));
}

long_t compute_pi(clock_t q, ofstream &os)
{
	long_t a = 1, b = invert_sqrt(2), t = 1;
	const unsigned max_err = a.get_length() - ERR;
	ext_t p = 4;
	unsigned err_order;
	while ((err_order = diff_order(a, b)) < max_err)
	{
		os << "Current_accuracy is about " << err_order << " decimals; "
			<< "comutational time is " << double(clock() - q) / CLOCKS_PER_SEC << " s.\n";
		long_t new_a = (a + b) * long_t("0.5");
		long_t new_b = my_sqrt(a * b);
		a = new_a, b = new_b;
		t = t - p * (a * a - b * b);
		p <<= 1;
	}
	return (a + b) * (a + b) * invert(t);
}
