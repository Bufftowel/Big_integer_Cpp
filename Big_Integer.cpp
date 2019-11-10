#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

class BigInt
{
    #undef ll
    typedef long long int ll;
    typedef vector<int> vi;
    ll pow;
    int Base;
    vi a;
    bool sign;
    /*static vector<ll> Convert_Base(vector<int> num,int n)
    {
        vector<ll> ans;
        ll carry = 0;
        ll mod = 1;
        while(n--)
            mod = mod * 10;
        for(int i = 0; i < (int)num.size(); ++i)
        {
            carry = carry + num[i];
            while(carry >= mod)
            {
                ans.push_back(carry % mod);
                carry = carry / mod;
            }
        }
        return ans;
    }*/
    BigInt Convert_Base(int newBase)
    {
        if(newBase > 9) return *this;
        BigInt res;
        res.Base = newBase;
        res.pow = 1;
        for(int i = 0; i < newBase; i++) res.pow = res.pow * 10;
        res.Base = newBase;
        stringstream ss;
        string s;
        ss << *this;
        ss >> s;
        res.read(s);
        return res;
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
    void read(string &s)
    {
        ll curr;
        int k = 0;
        while(k<(int)s.size() && (s[k]=='-' || s[k]=='+'))
        {
            if(s[k]=='-')
                sign = !sign;
            k++;
        }
        for(int i = s.size()-1; i>=k; i-=Base)
        {
            curr = 0;
            for(int j = max(k,i-Base+1); j<=i; ++j)
            {
                curr = curr * 10 + s[j]-'0';
            }
            a.push_back(curr);
        }
        removeZeros();
    }
    string to_string()
    {
        string s;
        stringstream ss;
        ss << *this;
        ss >> s;
        if(!sign)
            s = '-' + s;
        return s;
    }
    void removeZeros()
    {
        while(!a.empty() && !a.back())
            a.pop_back();
        if(a.empty())
            sign = 1;
    }
    BigInt operator + (const BigInt &b) const
    {
        if(sign!=b.sign)
            return *this -(-b);
        BigInt ans = b;
        ll carry = 0;
        for(int i=0; i<(int)max(a.size(),b.a.size()); ++i)
        {
            if(i==(int)ans.a.size())
                ans.a.push_back(0);
            carry = carry + ans.a[i];
            if(i<(int)a.size())
            {
                carry += a[i];
            }
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
            for(int i=0; i< (int)ans.a.size(); ++i)
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
        vector<ll> n(a.begin(),a.end()),m(num.a.begin(),num.a.end()),c;
        BigInt ans;
        ans.sign = !(sign ^ num.sign);
        while(n.size() < m.size())
            n.push_back(0);
        while(n.size() > m.size())
            m.push_back(0);
        c = karatsuba(n, m, num.pow);
        ll carry = 0;
        for(int i = 0; i<(int)c.size(); ++i)
        {
            carry += c[i];
            ans.a.push_back(carry % pow);
            carry = carry/pow;
        }
        while(carry)
        {
            ans.a.push_back(carry % pow);
            carry = carry/pow;
        }
        ans.removeZeros();
        return ans;
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
        if(a.size()!=num.a.size())
            return a.size() > num.a.size();
        for(int i = a.size()-1; i > -1; --i)
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
        for(int i = a.size()-1; i > -1 ; --i)
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
        // stream << setfill(' '); // changing filler back to ' '
        return stream;
    }
    // Read above, where * is overloaded to understand why this is declared as static.
    static vector<ll> karatsuba(vector<ll> a,vector<ll> b,ll pow)
    {
        int n = a.size();
        vector<ll> ans(2*n,0);
        //if(n <= 1)
        {
            ll carry;
            for(int i = 0; i < n; ++i)
            {
                carry = 0;
                for(int j = 0; j < n; ++j)
                {
                    ans[i + j] += a[i] * b[j] + carry;
                    carry = ans[i+j] / pow;
                    ans[i + j] = ans[i+j] % pow;
                }
                if(carry)
                    ans[i + n] += carry;
            }
            return ans;
        }
        int m = n >> 1, k;
        k = n - m;
        ll carry = 0;
        vector<ll> x1(a.begin(),a.begin()+m);
        vector<ll> y1(b.begin(),b.begin()+m);
        vector<ll> x2(a.begin()+m,a.end());
        vector<ll> y2(b.begin()+m,b.end());
        vector<ll> x1y1 = karatsuba(x1, y1, pow);
        vector<ll> x2y2 = karatsuba(x2, y2, pow);
        for(int i = 0; i<m; i++)
        {
            x2[i] = x2[i] + x1[i] + carry;
            carry = x2[i]/pow;
            x2[i] = x2[i]%pow;
        }
        if(k>m)
        {
            x2[k - 1] = x2[k - 1] + carry;
            carry = x2[k - 1]/pow;
            x2[k - 1] = x2[k - 1]%pow;
        }
        if(carry)
            x2.push_back(carry);
        carry = 0;
        for(int i = 0; i<m; i++)
        {
            y2[i] = y2[i] + y1[i] + carry;
            carry = y2[i]/pow;
            y2[i] = y2[i]%pow;
        }
        if(k>m)
        {
            y2[k - 1] = y2[k - 1] + carry;
            carry = y2[k - 1]/pow;
            y2[k - 1] = y2[k - 1]%pow;
        }
        if(carry)
            y2.push_back(carry);
        while(y2.size()>x2.size())
            x2.push_back(0);
        while(y2.size()<x2.size())
            y2.push_back(0);
        carry = 0;
        vector<ll> mid = karatsuba(x2,y2,pow);
        k = mid.size();
        for(int i = 0; i<(int)ans.size(); ++i)
        {
            if(i < (int)x2y2.size())
                ans[i] += x2y2[i];
            else
            {
                ans[i] += carry;
                if(!carry)
                    break;
            }
            carry = ans[i] / pow;
            ans[i] = ans[i] % pow;
        }
        carry = 0;
        for(int i = 0; i<(int)ans.size(); ++i)
        {
            if(i<(int)x1y1.size())
                ans[i] += x1y1[i];
            else
            {
                ans[i] += carry;
                if(!carry)
                    break;
            }
            carry = ans[i+2*m] / pow;
            ans[i+2*m] = ans[i+2*m] % pow;
        }
        carry = 0;
        for(int i = 0; i<k; ++i)
        {
            mid[i] -= carry;
            if(i<(int)x1y1.size())
                mid[i] -= x1y1[i];
            if(i<(int)x2y2.size())
                mid[i] -= x2y2[i];
            if(mid[i]<0)
            {
                mid[i] += 3ll * pow;
                carry = 3;
            }
        }
        carry = 0;
        for(int i = 0; i<k; ++i)
        {
            ans[i + m] += mid[i];
            carry = a[i+m] / pow;
            a[i+m] = a[i+m] % pow;
        }
        while(carry)
        {
            ans[k]+=carry;
            carry = ans[k]/pow;
            ans[k] = ans[k] % pow;
            k++;
        }
        return ans;
    }
};

int main()
{
    BigInt a, b, c;
    int m;
    cin >> a >> m;
    cout<< a % m;
    return 0;
}
