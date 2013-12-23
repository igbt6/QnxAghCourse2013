#include "czas.h"
/*********************************************************************************************/

Czas::Czas(void)
{

    H=0;
    M=0;
    S=0;
    MS=0;
}
/*********************************************************************************************/

Czas::Czas(int h, int m, int s, int ms)
{   this->H=h;
    this->M=m;
    this->S=s;
    this->MS=ms;
    setProperTimeFormat();
}

/*********************************************************************************************/

void Czas::setProperTimeFormat(void)
{
    if(this->MS>999)
    {
        this->S+= this->MS/1000;
        this->MS%=1000;
    }
    if(this->MS<0)
    {
        this->MS=0;
    }

    if(this->S>59)
    {
        this->M+=this->S/60;
        this->S%=60;
    }
    if(this->S<0)
    {
        this->S=0;
    }
    if(this->M>59)
    {
        this->H+=this->M/60;
        this->M%=60;
    }
    if(this->M<0)
    {
        this->M=0;
    }
    if(this->H<0)
    {
        this->H=0;
    }
}
/*********************************************************************************************/

Czas::Czas(long int czas)
{
    MS = czas % 1000;
    czas = (czas-MS)/1000;

    S = czas % 100;
    if (S > 59)
    {
        S -= 60;
        czas = czas + 100;
    }
    czas =(czas-S)/100;
    M = czas % 100;
    if (M >59)
    {
        M -= 60;
        czas+= 100;
    }
    czas = (czas-M)/100;
    H = czas;
}
/*********************************************************************************************/

Czas::Czas(double czas)
{
    czas*=1000;
    MS = (unsigned long int)czas % 1000;
    czas = (czas-MS)/1000;
    S = (unsigned long int)czas % 60;
    czas= (czas-S)/60;
    M = (unsigned long int)czas % 60;
    czas = (czas-M)/60;
    H = czas;
}
/*********************************************************************************************/

Czas& Czas ::operator += (const Czas &x)
{
    this->MS += x.MS;
    this->S += x.S;
    this->M += x.M;
    this->H += x.H;
    setProperTimeFormat();
    return *this;
}
/*********************************************************************************************/

Czas& Czas::operator -= (const Czas &x)
{
    this->MS -= x.MS;
    this->S -= x.S;
    this->M -= x.M;
    this->H -= x.H;
    setProperTimeFormat();
    return *this;

}
/*********************************************************************************************/

Czas& Czas:: operator= (long int x)
{

    *this=Czas(x);
    return *this;

}
/*********************************************************************************************/

Czas& Czas:: operator= (double x)
{
    *this=Czas(x);
    return *this;

}
/*********************************************************************************************/

Czas::operator long int() const
{
    long int ret=0;
    ret=H*10000000+M*100000+S*1000+MS;
    return ret;
}
/*********************************************************************************************/

Czas::operator double() const
{
    double ret=0;
    ret=H*3600+M*60+S+MS*0.001;
    return ret;
}
/*********************************************************************************************/

Czas Czas::operator ++ (void)
{
    ++S;
    return *this;
}
/*********************************************************************************************/

Czas Czas::operator ++ (int)
{
    Czas copyCzas (*this);
    S++;
    return copyCzas ;
}
/*********************************************************************************************/

Czas Czas::operator -- (void)
{
    --S;
    return *this;
}
/*********************************************************************************************/

Czas Czas::operator -- (int )
{
    Czas copyCzas(*this);
    S--;
    return copyCzas;
}

/*********************************************************************************************/
Czas operator+ (const Czas x1, const Czas x2)
{
    Czas czas(x1.h()+x2.h(),x1.m()+x2.m(),x1.s()+x2.s(),x1.ms()+x2.ms());
    return czas;
}
/*********************************************************************************************/
Czas operator- (const Czas x1, const Czas x2)
{
    Czas czas(x1.h()-x2.h(),x1.m()-x2.m(),x1.s()-x2.s(),x1.ms()-x2.ms());
    return czas;
}
/*********************************************************************************************/

bool operator == (const Czas x1, const Czas x2)
{
    if(x1.h()==x2.h()&&x1.m()==x2.m()&&x1.s()==x2.s()&&x1.ms()==x2.ms())return true;
    else return false;
}
/*********************************************************************************************/

bool operator != (const Czas x1, const Czas x2)
{
    if(x1.h()!=x2.h()||x1.m()!=x2.m()||x1.s()!=x2.s()||x1.ms()!=x2.ms())return true;
    else return false;
}
/*********************************************************************************************/

bool operator > (const Czas x1, const Czas x2)
{
    if(x1.h()!=x2.h())
    {
        return (x1.h()>x2.h())?true:false;
    }
    if(x1.m()!=x2.m())
    {
        return (x1.m()>x2.m())?true:false;
    }
    if(x1.s()!=x2.s())
    {
        return (x1.s()>x2.s())?true:false;
    }
    if(x1.ms()!=x2.ms())
    {
        return (x1.ms()>x2.ms())?true:false;
    }
    return false;
}
/*********************************************************************************************/

bool operator < (const Czas x1, const Czas x2)
{
    if(x1.h()!=x2.h())
    {
        return (x1.h()<x2.h())?true:false;
    }
    if(x1.m()!=x2.m())
    {
        return (x1.m()<x2.m())?true:false;
    }
    if(x1.s()!=x2.s())
    {
        return (x1.s()<x2.s())?true:false;
    }
    if(x1.ms()!=x2.ms())
    {
        return (x1.ms()<x2.ms())?true:false;
    }
    return false;
}
/*********************************************************************************************/

bool operator <= (const Czas x1, const Czas x2)
{
    if(x1<x2||x1==x2)return true;
    else return false;
}
/*********************************************************************************************/

bool operator >= (const Czas x1, const Czas x2)
{
    if(x1>x2||x1==x2)return true;
    else return false;
}
/*********************************************************************************************/

std::ostream & operator << (std::ostream &s , const Czas x)
{
    s<<x.h()<<"h "<<x.m()<<"m "<<x.s()<<"s "<<x.ms()<<"ms";
    return s;
}





