#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

class BigInt
{
    #undef ll
    typedef long long int ll;
    typedef vector<int> vi;
    #define defBase 1000000000;
    ll pow;
    int Base;
    vi a;
    bool sign;
    void read(string &s)
    {
        ll curr;
        int k = 0;
        while(k < (int)s.size() && (s[k] == '-' || s[k] == '+'))
        {
            if(s[k] == '-') sign = !sign;
            k++;
        }
        for(int i = s.size() - 1; i >= k; i -= Base)
        {
            curr = 0;
            for(int j = max(k, i - Base + 1); j <= i; ++j)
            {
                curr = curr * 10 + s[j] - '0';
            }
            a.push_back(curr);
        }
        removeZeros();
    }
    BigInt Convert_Base(int newBase) const
    {
        if(newBase > 9) return *this;
        BigInt res;
        res.Base = newBase;
        res.pow = 1;
        for(int i = 0; i < newBase; i++) res.pow = res.pow * 10;
        stringstream ss;
        string s;
        ss << *this;
        ss >> s;
        res.read(s);
        return res;
    }
    void removeZeros()
    {
        while(!a.empty() && !a.back())
            a.pop_back();
        if(a.empty())
            sign = 1;
    }
public:
    BigInt()
    {
        sign = 1;
        pow = 1000000000;
        Base = 9;
    }
    template<typename T>
    BigInt(T val) : BigInt()
    {
        stringstream ss;
        ss << val;
        string s;
        ss >> s;
        read(s);
    }
    string to_string()
    {
        string s;
        stringstream ss;
        ss << *this;
        ss >> s;
        return s;
    }
    BigInt operator + (const BigInt &b) const
    {
        if(sign != b.sign)
            return *this - (-b);
        BigInt ans = b;
        ll carry = 0;
        for(int i = 0; i < int(max(a.size(), b.a.size())); ++i)
        {
            if(i == (int)ans.a.size()) ans.a.push_back(0);
            carry = carry + ans.a[i];
            if(i < (int)a.size()) carry += a[i];
            ans.a[i] = carry % pow;
            carry = carry / pow;
        }
        if(carry)
            ans.a.push_back(carry);
        return ans;
    }
    template <typename T>
    BigInt operator + (const T &b) const
    {
        BigInt ans;
        stringstream ss;
        ss << b;
        string s;
        ss >> s;
        ans.read(s);
        return *this + ans;
    }
    BigInt operator - (const BigInt &num) const
    {
        if(sign == num.sign)
        {
            if(abs() < num.abs())
            {
                return -(num - *this);
            }
            BigInt ans = *this;
            ll carry = 0;
            for(int i = 0; i < (int)ans.a.size(); ++i)
            {
                if(i < (int)num.a.size())
                {
                    ans.a[i] = ans.a[i] - num.a[i] + carry;
                    if(ans.a[i] < 0)
                    {
                        ans.a[i] += pow;
                        carry = -1;
                    }
                    else
                        carry = 0;
                }
                else if(carry)
                {
                    ans.a[i] = ans.a[i] + carry;
                    if(ans.a[i] < 0)
                    {
                        ans.a[i] += pow;
                        carry = -1;
                    }
                    else
                        carry = 0;
                }
                else
                    break;
            }
            ans.removeZeros();
            return ans;
        }
        else
            return *this + (-num);
    }
    template <typename T>
    BigInt operator - (const T &b) const
    {
        BigInt ans;
        stringstream ss;
        ss << b;
        string s;
        ss >> s;
        ans.read(s);
        return *this - ans;
    }
    BigInt operator * (const BigInt &num) const // this(pointer) is also passed implicitly, we cannot change that either
    {
        BigInt num1 = Convert_Base(5);
        BigInt num2 = num.Convert_Base(5);
        vector<ll> n(num1.a.begin(), num1.a.end());
        vector<ll> m(num2.a.begin(), num2.a.end());
        BigInt ans;
        ans.sign = !(sign ^ num.sign);
        while(n.size() < m.size())
            n.push_back(0);
        while(n.size() > m.size())
            m.push_back(0);
        vector<ll> res = karatsuba(n, m);
        ll carry = 0;
        ans.Base = num1.Base;
        ans.pow = num1.pow;
        for(int i = 0; i < (int)res.size(); ++i)
        {
            carry = carry + res[i];
            ans.a.push_back(carry % ans.pow);
            carry = carry / ans.pow;
        }
        while(carry)
        {
            ans.a.push_back(carry % ans.pow);
            carry = carry / ans.pow;
        }
        ans.removeZeros();
        return ans.Convert_Base(9);
    }
    template<typename T>
    T operator % (T MoDuLo)
    {
        ll ans = 0;
        for(int i = a.size() - 1; i > -1; i--)
        {
            ans = (ans * pow + a[i]) % MoDuLo;
        }
        if(!sign) ans = -ans;
        return ans;
    }
         // calling karatsuba will inevitably pass this (pointer to current object), and if katatsuba
        // is not a const function, then this will cause error, as it may change it.
       // if we declare karatsuba as static that would mean it won't receive
      // a implicit object (They don't have access to this pointer). A Static member function can be
     // called even without a object(using :: and class name). A static member function can only access static
    // static data member, other static member functions and any other functions from outside the class.
    bool operator > (const BigInt &num) const
    {
        if(sign != num.sign)
            return sign > num.sign;
        if(a.size() != num.a.size())
            return a.size() > num.a.size();
        for(int i = a.size() - 1; i > -1; --i)
        {
            if(a[i] != num.a[i])
            {
                if(sign == 1)
                    return a[i] > num.a[i];
                else
                    return a[i] < num.a[i];
            }
        }
        return 0;
    }
    bool operator == (const BigInt &num) const
    {
        if(sign != num.sign)
            return 0;
        if(a.size() != num.a.size())
            return 0;
        for(int i = a.size() - 1; i > -1 ; --i)
            if(a[i] != num.a[i])
                return 0;
        return 1;
    }
    bool operator < (const BigInt &num) const
    {
        return num > *this;
    }
    bool operator <= (const BigInt &num) const
    {
        return !(num < *this);
    }
    bool operator >= (const BigInt &num) const
    {
        return !(num > *this);
    }
    bool operator != (const BigInt &num) const
    {
        return !(num == *this);
    }
    void operator += (BigInt &a)
    {
        *this = *this + a;
    }
    BigInt operator - () const
    {
        BigInt ans = *this;
        ans.sign = !ans.sign;
        return ans;           // temporary object. (It is a temp obj to the function it is being returned to)
    }                        //  temporary objects cannot be bound(assign) to non-constant identifiers.
    BigInt operator + ()
    {
        return *this;
    }
    BigInt& operator ++ ()   // prefix
    {
        *this = *this + 1;
        return *this;
    }
    const BigInt operator ++ (int)   // postfix ( a dummy argument is required to distinguish b/w prefix and post fix C++)
    {
        // const function to prevent assignment of postfix (as its not a lvalue in C++)
        BigInt x;
        x = *this;
        ++*this;
        return x;
    }
    BigInt& operator -- ()   // prefix
    {
        *this = *this - 1;
        return *this;
    }
    const BigInt operator -- (int)   // postfix
    {
        BigInt x;
        x = *this;
        --*this;
        return x;
    }
    template <typename T>
    T operator = (T x)  // returning the same type so that chaining is possible.
    {
        stringstream ss;
        ss << x;
        string s;
        ss >> s;
        a.clear();
        read(s);
        return x;
    }
    BigInt abs() const
    {
        BigInt ans = *this;
        ans.sign = 1;
        return ans;
    }
    int length()
    {
        int l = a[a.size() - 1], cnt = 0;
        while(l)
        {
            cnt++;
            l /= 10;
        }
        return Base * (a.size() - 1) + cnt;
    }
       // using reference of streams as they immediately need to be changed after use.
      //  cannot define methods in stream classes (as left operand is the one operator's definition should be in)
     //   hence overloading (<<,>>) as friend (not member function of this class but the scope is only in this class)
    //    (you can also define them globally.)
    friend istream& operator >> (istream &stream, BigInt &a)  // return types are streams themselves so that chaining
    {                                                        // is possible. e.g. cin >> a >> b;
        string s;
        stream >> s;
        a.read(s);
        return stream;
    }
    friend ostream& operator << (ostream &stream, BigInt num)
    {
        if(!num.sign)
            stream << '-';
        if(num.a.empty())
            stream << '0';
        else
            stream << num.a[num.a.size() - 1];
        for(int i= num.a.size() - 2; i > -1; --i)
        {
            stream << setw(num.Base) << setfill('0') << num.a[i];
        }
        stream << setfill(' '); // changing filler back to ' '
        return stream;
    }
    // Read above, where * is overloaded to understand why this is declared as static.
    static vector<ll> karatsuba(vector<ll> a, vector<ll> b)
    {
        int n = a.size();
        vector<ll> ans(2 * n, 0);
        if(n <= 64)
        {
            for(int i = 0; i < n; ++i)
            {
                for(int j = 0; j < n; ++j)
                {
                    ans[i + j] += a[i] * b[j];
                }
            }
            return ans;
        }
        int k = n >> 1;
        vector<ll> x2(a.begin(), a.begin() + k);
        vector<ll> y2(b.begin(),b.begin() + k);
        vector<ll> x1(a.begin() + k, a.end());
        vector<ll> y1(b.begin() + k, b.end());
        vector<ll> x1y1 = karatsuba(x1, y1);
        vector<ll> x2y2 = karatsuba(x2, y2);
        for(int i = 0; i < k; i++)
        {
            x1[i] = x2[i] + x1[i];
        }
        for(int i = 0; i < k; i++)
        {
            y1[i] = y2[i] + y1[i];
        }
        vector<ll> val = karatsuba(x1, y1);
        for(int i = 0; i < (int)val.size(); ++i)
        {
            if(i < (int)x1y1.size()) val[i] -= x1y1[i];
            if(i < (int)x2y2.size()) val[i] -= x2y2[i];
            ans[i + k] += val[i];
        }
        for(int i = 0; i < (int)x1y1.size(); i++)
        {
            ans[i + 2 * k] += x1y1[i];
        }
        for(int i = 0; i < (int)x2y2.size(); i++)
        {
            ans[i] += x2y2[i];
        }
        return ans;
    }
    friend BigInt Multiply_Naive(const BigInt &num1,const BigInt &num2)
    {
        vector<ll> x(num1.a.begin(),num1.a.end()), y(num2.a.begin(),num2.a.end());
        BigInt res;
        res.sign = !(num1.sign ^ num2.sign);
        while(x.size() < y.size()) x.push_back(0);
        while(x.size() > y.size()) y.push_back(0);
        int n = x.size();
        vector<ll> ans(2 * n,0);
        ll carry;
        for(int i = 0; i < n; i++)
        {
            carry = 0;
            for(int j = 0; j < n; j++)
            {
                ans[i+j] += x[i] * y[j] + carry;
                carry = ans[i + j] / defBase;
                ans[i + j] = ans[i + j] % defBase;
            }
            if(carry) ans[i + n] += carry;
        }
        for(int i = 0; i<(int)ans.size(); i++)
            res.a.push_back(ans[i]);
        res.removeZeros();
        return res;
    }
};

int main()
{
    BigInt a, b;
    cin >> a >> b;
    cout << a * b << "\n";
    return 0;
}
