#include <iostream>
#include <list>

#define BUFFER_SIZE 8196
#define ZERO 48
#define MAX_DEPTH 100

using namespace std;

typedef list<int> number;

// Used for already calculated fibonacci numbers
struct M
{
    number t;
    number f;
};

static list<M> fNum[MAX_DEPTH];

// Subtract 1 from number
number subOne(number num)
{
    number result;

    int borrow = 1;
    int n;
    while(!num.empty())
    {
        n = num.back();
        num.pop_back();

        if(n - borrow < 0)
        {
            borrow = 1;
            if(!num.empty())
                result.push_front(9);
        }
        else
        {
            if(n - borrow == 0 && num.empty())
                return result;

            result.push_front(n - borrow);
            borrow = 0;
        }
    }

    return result;
}

// Add two big integers
// Can be modified to add two floating point numbers
number add(number first, number second)
{
    number summation;

    int addon = 0;
    int result, s, f;

    while(!first.empty() || !second.empty())
    {
        if(first.empty())
        {
            f = 0;
            s = second.back();
            second.pop_back();
        }
        else if(second.empty())
        {
            f = first.back();
            first.pop_back();
            s = 0;
        }
        else
        {
            f = first.back();
            first.pop_back();

            s = second.back();
            second.pop_back();
        }
        result = f + s + addon;
        if(result > 9)
        {
            result -= 10;
            addon = 1;
        }
        else
            addon = 0;
        summation.push_front(result);
    }
    if(addon == 1)
       summation.push_front(1);

    return summation;
}

// Divide number by two
number divByTwo(number div)
{
    number result;

    if(div.size() == 1 && div.back() == 1)
        return result;

    int addon = 0;
    while(!div.empty())
    {
        int r = div.front() + addon;

        // First digit cannot be zero
        if(!result.size() == 0 || r >= 2)
            result.push_back(r / 2);

        // If the digit is odd, it adds to next digit
        // Example dividing 112 to 2
        // It omits first 1 and divides 10 + 1 to 2, finds 5
        // Since 11 is again odd, it then divides 10 + 2 to 2 and finds 6
        // Final result is 56
        addon = (r % 2) * 10;
        div.pop_front();
    }
    return result;
}

// Multiplies two big numbers
number multiply(number first, number second)
{
    number result;

    int slide = 0;
    int s;
    int mulAddon;
    int length = 0;

    while(!second.empty())
    {
        int multiplier = second.back();
        second.pop_back();

        if(multiplier == 0)
        {
            if(length <= slide)
            {
                result.push_front(0);
                ++length;
            }
            ++slide;
            continue;
        }
        s = 0;
        mulAddon = 0;

        // We add the numbers of multiplication with each digit with
        // one slide to the left, so we find which digit we start adding
        // the result
        number::reverse_iterator iter = result.rbegin();
        for( ; iter != result.rend(); ++iter)
        {
            if(s == slide)
                break;
            s++;
        }

        for(number::reverse_iterator fIter = first.rbegin(); fIter != first.rend(); ++fIter)
        {
            int r;
            if(iter == result.rend())
            {
                r = (*fIter) * multiplier + mulAddon;
                result.push_front(r % 10);
                ++length;
            }
            else
            {
                r = *iter + (*fIter) * multiplier + mulAddon;
                *iter = r % 10;
            }
            ++iter;
            mulAddon = r / 10;
        }
        if(mulAddon > 0)
        {
             result.push_front(mulAddon);
             ++length;
        }
        ++slide;
    }
    return result;
}

// Sequential fibonacci algorithm
number fibSequential(number turn)
{
    number a;
    a.push_back(1);

    turn = subOne(turn);
    if(turn.empty())
        return a;

    number b;
    b.push_back(1);

    turn = subOne(turn);
    if(turn.empty())
        return b;

    number result;

    while(!turn.empty())
    {
        turn = subOne(turn);

        result = add(a, b);
        a = b;
        b = result;
    }
    return result;
}

// https://www.geeksforgeeks.org/program-for-nth-fibonacci-number/
// However, not logn but n algorithm for fibonacci
// Since same numbers are calculated again
// inspired from the last algorithm given in the geeksforgeeks link
number fib(number turn)
{
    number result;
    if(turn.size() == 0)
    {
        result.push_back(0);
        return result;
    }
    if(turn.size() == 1
            && (turn.back() == 1 || turn.back() == 2))
    {
        result.push_back(1);
        return result;
    }
    if(turn.back() & 1) // If turn is odd
    {
        number one(1,1);
        number k = divByTwo(add(turn, one));

        number fk = fib(k);             // fibonacci number of kth number
        number fkmo = fib(subOne(k));   // fibonacci number of (k-1)th number

        result = add(multiply(fk,fk), multiply(fkmo, fkmo));
    }
    else                // If turn is even
    {
        number k = divByTwo(turn);
        number two(1,2);

        number fk = fib(k);              // fibonacci number of kth number
        number fkmo = fib(subOne(k));    // fibonacci number of (k-1)th number

        result = multiply(fk, add(fk, multiply(fkmo, two)));
    }

    return result;
}

// Already calculated numbers are tried to be obtained
// Not as efficient as if(f[n]) return f[n]; part
// of the geeksforgeeks algorithm but about %25 speeds up
// the calculation nonetheless
number getTurn(number turn, int depth)
{
    number result;
    bool breaked;

    for(list<M>::iterator iter = fNum[depth].begin(); iter != fNum[depth].end(); ++iter)
    {
        breaked = false;
        number::iterator n = (*iter).t.begin();
        number::iterator tIter = turn.begin();

        while(true)
        {
            if(n == (*iter).t.end() || tIter == turn.end())
                break;
            if(*n != *tIter)
            {
                breaked = true;
                break;
            }
            ++n;
            ++tIter;
        }
        if(breaked)
            continue;
        else
        {
            result = (*iter).f;
            return result;
        }
    }
    return result;
}

// https://www.geeksforgeeks.org/program-for-nth-fibonacci-number/
// logn algorithm for fibonacci
// tried to omit calculation of same numbers
// inspired from the last algorithm given in the geeksforgeeks link
number fib(number turn, int depth)
{
    M m;
    number result;
    if(turn.size() == 0)
    {
        result.push_back(0);
        return result;
    }
    if(turn.size() == 1
            && (turn.back() == 1 || turn.back() == 2))
    {
        result.push_back(1);
        return result;
    }

    if(depth > 0)
    {
        number calculated = getTurn(turn, depth - 1);
        if(calculated.size() != 0)
        {
            result = calculated;
            return result;
        }
    }

    if(turn.back() & 1) // If turn is odd
    {
        number one(1,1);
        number k = divByTwo(add(turn, one));

        number fk = fib(k, depth + 1);             // fibonacci number of kth number

        m.t = k;
        m.f = fk;
        fNum[depth].push_back(m);

        number kmo = subOne(k);
        number fkmo = fib(kmo, depth + 1);   // fibonacci number of (k-1)th number

        m.t = kmo;
        m.f = fkmo;
        fNum[depth].push_back(m);

        result = add(multiply(fk,fk), multiply(fkmo, fkmo));
    }
    else                // If turn is even
    {
        number k = divByTwo(turn);
        number two(1,2);

        number fk = fib(k, depth + 1);              // fibonacci number of kth number

        m.t = k;
        m.f = fk;
        fNum[depth].push_back(m);

        number kmo = subOne(k);
        number fkmo = fib(kmo, depth + 1);    // fibonacci number of (k-1)th number

        m.t = kmo;
        m.f = fkmo;
        fNum[depth].push_back(m);

        result = multiply(fk, add(fk, multiply(fkmo, two)));
    }

    return result;
}

int main()
{
    char temp[BUFFER_SIZE];

    cout << "Enter Number : " << endl;
    cin.getline(temp, BUFFER_SIZE, '\n');

    // Get the number of characters actually read
    unsigned int chars_read = cin.gcount();

    string s(temp, chars_read - 1);
    number turn;
    for(size_t i = 0; i < s.length(); ++i)
        turn.push_back(s[i] - ZERO);

    number result;

//    result = fib(turn);
//    for (number::iterator it = result.begin(); it != result.end(); ++it)
//        cout << *it;
//    cout << endl;

    result = fib(turn, 0);
    for (number::iterator it = result.begin(); it != result.end(); ++it)
        cout << *it;
    cout << endl << "End of Program" << endl;
    return 0;
}
