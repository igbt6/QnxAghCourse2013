#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
class RegHister1
{
private:
    bool	ok;
    double l1,h1,l2,h2;
    int y1; //  zmiena pamietajaca stan regulatora

public:
    RegHister1( double al1 , double ah1, double al2, double ah2);
    RegHister1( void );

    bool SetPar( double al1 , double ah1, double al2, double ah2);
    bool SetPar(char parameter, double value);
    double GetPar(char parameter);
    int Run( double xRef, double xReal );
    bool ParOk(void);


};

//-----------------------------------------

RegHister1::RegHister1(double al1 , double ah1, double al2, double ah2)
{

    SetPar(al1,ah1,al2,ah2);
    y1=0;

}

//-----------------------------------------

RegHister1::RegHister1( void )
{
    SetPar(0,0,0,0);
    y1=0;
}

//-----------------------------------------

bool RegHister1::SetPar( double al1 , double ah1, double al2, double ah2)
{
    l1=al1;
    h1=ah1;
    l2=al2;
    h2=ah2;
    ok = h2>l2>h1>l1>=0;
    return ok;
}

//-----------------------------------------
//-----------------------------------------

bool RegHister1::SetPar(char parameter, double value)
{
    switch(parameter)
    {
    case 'l':
        l1=value;
        break;
    case 'h':
        h1=value;
        break;
    case 'L':
        l2=value;
        break;
    case 'H':
        h2=value;
        break;
    }
    ok = h2>l2>h1>l1>= 0;
    return ok;
}

//-----------------------------------------

double RegHister1::GetPar(char parameter)
{
    switch(parameter)
    {
    case 'l':
        return l1;
        break;
    case 'h':
        return h1;
        break;
    case 'L':
        return l2;
        break;
    case 'H':
        return h2;
        break;
    default :
        return -1.0;
        break;
    }
}

//-----------------------------------------

int RegHister1::Run( double xRef, double xReal )
{
    double	err = xRef - xReal;
    if( ok )
    {
        if((err>=h2)||((err>=l2&&err<h2)&&(y1==2||y1==0)))
        {
            y1=2;
        }
        else if((err>=h1&&err<l2)||(err>=l2&&err<h2&&y1==1)||(err>l1&&err<=h1&&y1==1))
        {
            y1=1;
        }
        else if ((err>-l1&&err<l1)||(err>l1&&err<h1&&y1==0)||(err<-l1&&err>-h1&&y1==0))
        {
            y1=0;
        }
        else if ((err<=-h1&&err>-l2)||(err<=-l2&&err>-h2&&y1==-1)||(err<-l1&&err>=-h1&&y1==-1))
        {
            y1=-1;
        }
        else if ((err<=-h2)||((err<=-l2&&err>-h2)&&(y1==-2||y1==0)))
        {
            y1=-2;
        }
        else y1=0;
    }
    return y1;
}
//-----------------------------------------

bool RegHister1::ParOk( void )
{
    return ok;
}


//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//

class Naped
{
private:
    double	vl,vh;
    bool ok;
    double y;
    bool CheckPar(void);

public:
    Naped( double avl, double avh);
    void SetPar( double avl, double avh );
    void SetPar( char aPar, double av );
    double Run( int av );
    bool ParOk( void );

};

//-----------------------------------------

Naped::Naped ( double avl, double avh )
{
    SetPar( avl,avh);

    y=200;
}

//-----------------------------------------
bool Naped::CheckPar(void)
{
    return ((vl>0)&&(vh>0)&&(vh>vl));
}

//-----------------------------------------
void Naped ::SetPar( double avl, double avh )
{
    vl=avl;
    vh=avh;
    ok= CheckPar();
}
//-----------------------------------------
void Naped ::SetPar( char aPar, double av )
{
    switch(aPar)
    {
    case 's':
    {
        vl=av;
        break;
    }
    case 'f':
    {
        vh=av;
        break;
    }
    }
    ok= CheckPar();
}
//-----------------------------------------
double Naped ::Run( int av )
{
    if(ok)
    {
        switch(av)
        {
        case -2:
            y-=vh;
            break;
        case -1:
            y-=vl;
            break;
        case  1:
            y+=vl;
            break;
        case  2:
            y+=vh;
            break;
        }
        if(y<0.0) y=0.0;
    }
    else
    {
        y=0.0;
    }
    return y;
}
//-----------------------------------------
bool Naped ::ParOk( void )
{
    return ok;		// zaslepka
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//


int main()
{
    double polZad,polAktualne;
    int zal;
    RegHister1 Regulator(0.0,5.0,15.0,40);
    Naped naped(1,2);
    polZad = 100.0;
    cout<<"POL_ZAD:= "<<polZad<<endl;
    for(;;)
    {
        polAktualne = naped.Run( zal );
        zal = Regulator.Run( polZad, polAktualne );

        for(int x=0; x<0xFFFFFF; x++);
        cout<<"POL_Aktualne: "<<polAktualne<<endl;
        cout<<"ZAL: "<<zal<<endl;
    }
    return 0;
}






