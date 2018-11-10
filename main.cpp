#include <iostream>
#include <list>

#define BUFFER_SIZE 8196
#define ZERO 48

using namespace std;

// Subtract 1 from number
list<int> subOne(list<int> num)
{
    list<int> result;

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
list<int> add(list<int> first, list<int> second)
{
    list<int> summation;

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
list<int> divByTwo(list<int> div)
{
    list<int> result;

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
list<int> multiply(list<int> first, list<int> second)
{
    list<int> result;

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
        list<int>::reverse_iterator iter = result.rbegin();
        for( ; iter != result.rend(); ++iter)
        {
            if(s == slide)
                break;
            s++;
        }

        for(list<int>::reverse_iterator fIter = first.rbegin(); fIter != first.rend(); ++fIter)
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
list<int> fibSequential(list<int> turn)
{
    list<int> a;
    a.push_back(1);

    turn = subOne(turn);
    if(turn.empty())
        return a;

    list<int> b;
    b.push_back(1);

    turn = subOne(turn);
    if(turn.empty())
        return b;

    list<int> result;

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
// logn algorithm for fibonacci
// inspired from the last algorithm given in the geeksforgeeks link
list<int> fib(list<int> turn)
{
    list<int> result;
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
        list<int> one(1,1);
        list<int> k = divByTwo(add(turn, one));

        list<int> fk = fib(k);             // fibonacci number of kth number
        list<int> fkmo = fib(subOne(k));   // fibonacci number of (k-1)th number

        result = add(multiply(fk,fk), multiply(fkmo, fkmo));
    }
    else                // If turn is even
    {
        list<int> k = divByTwo(turn);
        list<int> two(1,2);

        list<int> fk = fib(k);              // fibonacci number of kth number
        list<int> fkmo = fib(subOne(k));    // fibonacci number of (k-1)th number

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
    list<int> turn;
    for(size_t i = 0; i < s.length(); ++i)
        turn.push_back(s[i] - ZERO);

    list<int> result = fib(turn);
    for (std::list<int>::iterator it = result.begin(); it != result.end(); ++it)
        std::cout << *it;
    cout << endl << "End of Program" << endl;
    return 0;
}
