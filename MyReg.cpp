
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <curses.h>

#include <pthread.h>

#include <time.h>

//-----------------------------------------------------------------------------

#define		CMD_NONE	' '
#define		CMD_TEMP	'T'
#define		CMD_QUIT	'Q'

//-----------------------------------------------------------------------------

struct CtrlData_s 
{
	char	cmd;
	double	data;
};

CtrlData_s	CtrlData;
WINDOW		*InputWin, *OutputWin;

//-----------------------------------------------------------------------------

enum	{ PAIR_NORMAL = 1, PAIR_RK, PAIR_MAX };

int 	ncInit( void );
void	ncClose( void );

//-----------------------------------------------------------------------------

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
//-----------------------------------------------------------------------------

int	ncInit( void )
{
	if( initscr( ) != stdscr )
		return 1;

	noecho( );
	cbreak( );
	nonl( );
	intrflush( stdscr, FALSE );
	keypad( stdscr, TRUE );
	nodelay( stdscr, TRUE );
	raw( );
	scrollok( stdscr, FALSE );

	start_color( );
	if( COLOR_PAIRS >= PAIR_MAX )
	{
		init_pair( PAIR_NORMAL, COLOR_WHITE, COLOR_BLACK );
		init_pair( PAIR_RK, COLOR_RED, COLOR_BLACK );
	}

	curs_set( 0 );
	bkgdset( 0 | COLOR_PAIR( PAIR_NORMAL ) );
	clear( );
	refresh( );

	return 0;
}

//-----------------------------------------------------------------------------

void ncClose( void )
{
	nl( );
	clear( );
	refresh( );
	endwin( );
}

//-----------------------------------------------------------------------------

void Input( void )
{
	char	*prompt = " [liczba]<Enter> - temp. zadana, [Q|X]<Enter> - koniec: ";
	char	buf[32];
	int		promptLen;
	int		xMax, yMax;

	promptLen = strlen( prompt );
	xMax = getmaxx( InputWin );
	yMax = getmaxy( InputWin );

	echo( );

	mvwprintw( InputWin, 2, ( xMax - promptLen ) / 2, "%s", prompt );
	mvwgetstr( InputWin, 2, ( xMax + promptLen ) / 2 + 1, buf );
	wmove( InputWin, 2, ( xMax + promptLen ) / 2 + 1 );
	wclrtoeol( InputWin );

	switch( buf[0] )
	{
	case '0' :					// cyfra, + lub -
	case '1' :
	case '2' :
	case '3' :
	case '4' :
	case '5' :
	case '6' :
	case '7' :
	case '8' :
	case '9' :
	case '-' :
	case '+' :
		CtrlData.data = strtod( buf, NULL );
		CtrlData.cmd = CMD_TEMP;
		break;
	
	case 't' :
	case 'T' :
		CtrlData.data = strtod( &buf[1], NULL );
		CtrlData.cmd = CMD_TEMP;
		break;

	case 'q' :
	case 'Q' :
	case 'x' :
	case 'X' :
		CtrlData.cmd = CMD_QUIT;
		break;

	default :
		break;

	}

}

//-----------------------------------------------------------------------------

int	Draw( int y, int n, char c, int attr )
{
	char		bar[256];
	int			i;

	for( i = 0; i < n; i++ ) bar[i] = c;
	bar[i] = '\0';

	wmove( OutputWin, y, 0 );
	wclrtoeol( OutputWin );
	wattron( OutputWin, attr );
	mvwprintw( OutputWin, y, 0, "%s", bar );
	mvwprintw( OutputWin, y + 2, 0, "%-5d", n );
	wattroff( OutputWin, attr );

	wrefresh( OutputWin );

	return n;
}

//-----------------------------------------------------------------------------

void *Thread1( void *arg )
{
	while( 1 )
		Input( );
	return NULL;
}

//-----------------------------------------------------------------------------

int	main( void )
{
	pthread_t	threadId;
	
	double	polZad,polAktualne;
	int	zal;

	bool	quit = false;
    RegHister1 reg1( 0.0,5.0,15.0,40 );
	Naped naped( 1, 2);

	struct timespec		period;

//-----------------------------------------

	if( ncInit( ) != 0 )
	{
		printf( "NCurser initilizing error.\n" );
		return 1;
	}

	InputWin = newwin( 5, getmaxx( stdscr ), getmaxy( stdscr ) - 5, 0 );
	OutputWin = newwin( getmaxy( stdscr ) - 5, getmaxx( stdscr ), 0, 0 );

	period.tv_sec = 0;
	period.tv_nsec = 250000000;

	CtrlData.cmd = CMD_NONE;
	polZad = 20.0;

	pthread_create( &threadId, NULL, &Thread1, NULL );

//-----------------------------------------

	do
	{
		nanosleep( &period, 0 );

		switch( CtrlData.cmd )
		{
		case CMD_TEMP :
			polZad = CtrlData.data;
			break;
		case CMD_QUIT :
			quit = true;
			break;
		}
		CtrlData.cmd = CMD_NONE;

		polAktualne = naped.Run( zal );
		zal = reg1.Run( polZad, polAktualne );

		Draw( 10, (int)polZad, '#', COLOR_PAIR( PAIR_NORMAL ) | A_BOLD );
		Draw( 14, (int)polAktualne, '#', COLOR_PAIR( PAIR_RK ) | A_BOLD );

		mvwprintw( OutputWin, 2, 0, "polZadane=%5.1lf, polAktualne=%5.1lf,  zal=%3d", polZad, polAktualne, zal );

	}
	while( !quit );

//-----------------------------------------

	pthread_cancel( threadId );

	delwin( InputWin );
	delwin( OutputWin );
	
	ncClose( );

	return 0;
}

//-----------------------------------------------------------------------------
