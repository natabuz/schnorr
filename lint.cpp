#include "lint.h"

string Lint::read(char* c)
{
    ifstream ifst(c);

    string str;

    ifst >> str;

    ifst.close();

    return str;
}

void Lint::write(string str, char* c)
{
    ofstream ofst(c);

    ofst << str;

    ofst.close();
}

Lint Lint::binread(char* c)
{
    ifstream ifst;
    ifst.open(c, ios::binary | ios::in);

    Lint result;

    ifst.seekg(0, ifst.end);
    unsigned long long size = ifst.tellg();
    ifst.seekg(0);

    result._gg = size - 1;
    result._number.resize(size-1);

    ifst.read((char*)&result._sign, 1);

    for (int i = 0; i < size-1; i++)
        ifst.read((char*)&result._number[i], 1);

    ifst.close();

    return result;
}

void Lint::binwrite(Lint &result, char* c)
{
    ofstream ofst;
    ofst.open(c, ios::binary | ios::out);

    ofst.write((char*)&result._sign, 1);

    for (unsigned long long i = 0; i < result._number.size(); i++)
        ofst.write((char*)&(result._number[i]), 1);

    ofst.close();
}

string Lint::__str__()
{
	return toString();
}

string Lint::__repr__()
{
	return toString();
}

Lint::Lint() 
{
	_sign = 1;
	_gg = 1;
	_number = vector<char>(1);
	_number[0] = 0;
}

Lint::Lint(string number) 
{
	_sign = number[0] == '-' ? -1 : +1;
    unsigned long long gg = _gg = number.size();

	if(_sign < 0) _gg--;

	_number = vector<char>(_gg);
    unsigned long long start = gg - _gg;

    for (unsigned long long i = start; i != gg; i++)
    {
		_number[i - start] = number[i] - 48;
	}
}

Lint::Lint(char sign, int length, vector<char> number) 
{
	_sign = sign;
	_gg = length;
	_number.assign(number.begin(), number.end());
}

Lint Lint::mult10(int degree) 
{
	_gg += degree;

    for (unsigned long long i = 0; i != degree; i++)
    {
		_number.push_back(0);
	}
	return *this;
}

Lint Lint::trim() 
{
	while(_number.size() > 1 && !_number[0]) 
    {
		_number.erase(_number.begin());
	}
	_gg = _number.size();
	return *this;
}

Lint Lint::operator+(Lint add) 
{
	if(_sign < 0 && add._sign > 0) 
    {
		_sign = 1;
		return add - *this;
	}

	if(_sign > 0 && add._sign < 0)
    {
		add._sign = 1;
		return *this - add;
	}

    unsigned long long maxLength = _gg > add._gg ? _gg : add._gg;
    unsigned long long minLength = _gg < add._gg ? _gg : add._gg;

	vector<char> result;
	vector<char> adding;

	if(_gg > add._gg) 
    {
		result.assign(_number.begin(), _number.end());
		adding.assign(add._number.begin(), add._number.end());
	} 
    else 
    {
		adding.assign(_number.begin(), _number.end());
		result.assign(add._number.begin(), add._number.end());
	}

    for (unsigned long long i = 1; i <= adding.size(); i++)
    {
		result[result.size() - i] += adding[adding.size() - i];
	}

    for (unsigned long long i = result.size() - 1; i > 0; i--)
    {
		if(result[i] > 9) 
        {
			result[i - 1]++;
			result[i] -= 10;
		}
	}

	if(result[0] > 9) 
    {
		result[0] -= 10;
		result.insert(result.begin(), 1);
	}
	return Lint(_sign, result.size(), result);
}

Lint Lint::operator-(Lint sub) 
{
	if(sub._sign < 0) 
    {
		sub._sign = 1;
		return *this + sub;
	}

	if(_sign < 0 && sub._sign > 0) 
    {
		sub._sign = -1;
		return *this + sub;
	}

	if(_sign < 0 && sub._sign < 0) 
    {
		sub._sign = 1;
		return *this + sub;
	}

	if(*this < sub) 
    {
		Lint res = sub - *this;
		res._sign = -1;
		return res;
	}

	vector<char> result;

	result.assign(_number.begin(), _number.end());
	
    for (unsigned long long i = 1; i <= sub._number.size(); i++)
    {
		result[result.size() - i] -= sub._number[sub._number.size() - i];
	}

    for (unsigned long long i = result.size() - 1; i > 0; i--)
    {
		if(result[i] < 0) 
        {
			result[i] += 10;
			result[i - 1]--;
		}
	}

    while (result.size() > 1 && !result[0])
    {
        result.erase(result.begin());
    }
	return Lint(1, result.size(), result);
}

Lint Lint::operator*(Lint mult) 
{
	char sign = _sign * mult._sign;
	vector<char> result;

    for (unsigned long long i = 0; i != _gg + mult._gg; i++)
    {
		result.push_back(0);
	}

    for (unsigned long long j = 0; j != mult._gg; j++)
    {
		char multiplier = mult._number[mult._gg - j - 1];

        for (unsigned long long i = 0; i != _gg; i++)
        {
			result[result.size() - j - i -1] += (_number[_gg - i - 1] * multiplier);
		}

        for (unsigned long long i = result.size() - 1; i > 0; i--)
        {
			if(result[i] > 9) 
            {
				result[i - 1] += (result[i] / 10);
				result[i] %= 10;
			}
		}
	}

    while (result.size() > 1 && !result[0])
    {
        result.erase(result.begin());
    }
	return Lint(sign, result.size(), result);
}

Lint Lint::operator/(Lint div) 
{
	char sign = _sign * div._sign;
	_sign = 1;
	div._sign = 1;

    if (*this < div)
    {
        return Lint("0");
    }

	if(*this > div) 
    {
        vector<char> result;
		Lint a(1, _gg, _number);
		Lint b(1, div._gg, div._number);
        unsigned long long zeroNum = _gg - div._gg;

		b.mult10(zeroNum);
		
		while(b._gg >= div._gg) 
        {
			char digit = 0;

			while(!(a < b)) 
            {
				a = a - b;
				digit++;
			}

			result.push_back(digit);
			b._number.pop_back();
			b._gg--;
			a.trim();
		}
		return Lint(sign, result.size(), result);
	}
	return Lint("1");
}

Lint Lint::operator%(Lint div) 
{
	char resultSign = _sign * div._sign;
	_sign = 1;
	div._sign = 1;

    if (*this < div)
    {
        return *this;
    }

	if(*this > div) 
    {
		Lint mult = *this / div;
		Lint sub = div * mult;
		Lint result = *this - sub;

		result._sign = resultSign;
		return result;
	}
	return Lint("0");
}

Lint Lint::operator^(Lint degree) 
{
	Lint result = pow(degree);

    if (_sign < 0 && degree % Lint("2") == Lint("1"))
    {
        result._sign = -1;
    }
	return result;
}

Lint Lint::pow(Lint degree) 
{
	Lint result(1, _gg, _number);
	Lint zero("0");
	Lint one("1");
	Lint two("2");

    if (degree == zero)
    {
        return one;
    }

    if (degree == one)
    {
        return *this;
    }

	while(!(one * two > degree)) 
    {
		result = result * result;
		one = one * two;
	}
	return result * (*this).pow(degree - one);
}

Lint Lint::powmod(Lint a, Lint k, Lint n)
{
    Lint res("1");
    Lint zero("0");
    Lint one("1");
    Lint two("2");
    
    Lint aa;
    Lint kk;
    
    aa = a;
    kk = k;
   
	while (kk != zero)
    {
        if ((kk % two) == one)
			res = (res * aa) % n;

		aa = (aa * aa) % n;

        kk = kk / two;
        kk = kk.trim();
    }

    return res;
}

bool Lint::operator>(Lint right) 
{
	if(_gg == right._gg)
    {
		if(_sign == right._sign) 
        {
            for (unsigned long long i = 0; i != _gg; i++)
            {
				if(_sign * _number[i] > _sign * right._number[i]) 
                {
					return true;
				} 
                else 
                {
                    if (_sign * _number[i] < _sign * right._number[i])
                    {
                        return false;
                    }
				}
			}
			return false;
		}
		return _sign > right._sign;
	}

	if(_sign == right._sign) 
    {
		return _sign * _gg > _sign * right._gg;
	}
	return _sign > right._sign;
}

bool Lint::operator<(Lint right) 
{
	if(_gg == right._gg) 
    {
		if(_sign == right._sign) 
        {
            for (unsigned long long i = 0; i != _gg; i++)
            {
				if(_sign * _number[i] < _sign * right._number[i]) 
                {
					return true;
				} 
                else 
                {
                    if (_sign * _number[i] > _sign * right._number[i])
                    {
                        return false;
                    }
                }
			}
			return false;
		}
		return _sign < right._sign;
	}

	if(_sign == right._sign) 
    {
		return _sign * _gg < _sign * right._gg;
	}
	return _sign < right._sign;
}

bool Lint::operator==(Lint right) 
{
    if (_gg != right._gg || _sign != right._sign)
    {
        return false;
    }
		
    for (unsigned long long i = 0; i != _gg; i++)
    {
		if(_number[i] != right._number[i]) 
        {
			return false;
		}
	}
	return true;
}

bool Lint::operator!=(Lint right)
{
	return !(*this == right);
}

string Lint::toString() 
{
    unsigned long long gg = _sign < 0 ? _gg + 1 : _gg;
    string result;

    result.resize(gg);
	
    if (_sign < 0)
    {
        result[0] = '-';
    }

	int start = gg - _gg;

    for (unsigned long long i = start; i != gg; i++)
    {
		result[i] = _number[i - start] + 48;
	}

	result[gg] = 0;
	return result;
}

bool Lint::isNegative() 
{
	return _sign < 0;
}
