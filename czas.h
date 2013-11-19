#include <iostream>

class  Czas
{


public:
    Czas(void);
    Czas(int h, int m, int s, int ms);
    Czas( long int czas);
    Czas( double czas);


    inline int h(void) const
    {
        return H;
    }


    inline int m(void) const
    {
        return M;
    }


    inline int s(void) const
    {
        return S;
    }

    inline int ms(void) const
    {
        return MS;
    }

    Czas & operator += (const Czas &x);
    Czas & operator -= (const Czas &x);

    Czas & operator = (long int x);
    Czas & operator = (double x);
    operator  long int() const;
    operator  double () const;
    Czas operator ++ (void);
    Czas operator ++ (int );
    Czas operator -- (void);
    Czas operator -- (int );

private :
    int H;
    int M;
    int S;
    int MS;
    void SetNormalizeTime(int h, int m, int s, int ms);
    void SetNormalizeTime2(Czas* czas);
    void setProperTimeFormat(Czas& czas);
};


Czas operator+ (const Czas x1, const Czas x2);
Czas operator- (const Czas x1, const Czas x2);
bool operator == (const Czas x1, const Czas x2);
bool operator != (const Czas x1, const Czas x2);
bool operator < (const Czas x1, const Czas x2);
bool operator <= (const Czas x1, const Czas x2);
bool operator > (const Czas x1, const Czas x2);
bool operator >= (const Czas x1, const Czas x2);

std::ostream & operator << (std::ostream &s , const Czas x);
