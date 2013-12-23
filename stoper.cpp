/***************************************************************************//**
 @modified by Lukasz  Uszko  9.12.12
*******************************************************************************/
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>

#include <stdio.h>
#include <string.h>

#include <curses.h>

//-----------------------------------------------------------------------------

#define TIMER_PULSE_CODE	_PULSE_CODE_MINAVAIL

typedef union
{
  struct _pulse	pulse;
} myMessage_t;


//=================================================

enum	{ PAIR_NORMAL = 1, PAIR_RK, PAIR_MAX };

int 		ncInit( void );
void		cClose( void );

//-----------------------------------------------------------------------------

int 	ncInit( void )
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

void	ncClose( void )
{
	nl( );
	clear( );
	refresh( );
	endwin( );
}

//-----------------------------------------------------------------------------


void printTime( int y, int x, int t )
{

int m=0;
int s=0;

   if(t>999)
    {
        s+= t/1000;
        t%=1000;
    }
    if(t<0)
    {
        t=0;
    }

    if(s>59)
    {
        m+=s/60;
        s%=60;
    }
    if(s<0)
    {
        s=0;
    }

mvprintw(y,x,"%.1d:%.2d:%.3d",m,s,t);
}

//=================================================

void printState( int y, int x, bool up, bool down, int step )
{
char* state[]={"STOP","UP  ","DOWN"};
char* curState;
if(!(up||down))curState=state[0];
else if(up)curState=state[1];
else curState=state[2];
mvprintw(y,x,"STEP= %d ms    %s", step, curState);
}


//=================================================

int SetTimer( timer_t aTimerId, int aKrok )				// aKrok w milisekundach
{
struct itimerspec tTime;
tTime.it_value.tv_sec=0;
tTime.it_value.tv_nsec=1000000*aKrok;
tTime.it_interval.tv_sec=0;
tTime.it_interval.tv_nsec=1000000*aKrok;

timer_settime(aTimerId,0,&tTime,NULL);
}


//=================================================


int main( int argc, char **argv )
{
	int	 ChId, RcvId;

	timer_t	 tId;                    //ident czasomierza

	struct sigevent	 evt;            //zdarzenie sysemowe
	myMessage_t	 msg;            // komunikat

//=================================================

	int	msecCnt = 0;		             // licznik stopera w ms
	int	step = 100;			     // krok stopera
	bool	gora = false, dol = false;	     // stan stopera
	bool	koniec = false;		             // zakonczenie programu
	int	znak;				     // znak z klawiatury
char buf[10];
//=================================================

	ChId = ChannelCreate( 0 );                  //utworzenie kanalu

	evt.sigev_notify = SIGEV_PULSE;              // typ zdarzenia - impuls
	evt.sigev_coid = ConnectAttach( ND_LOCAL_NODE, 0, ChId, _NTO_SIDE_CHANNEL, 0 );
	evt.sigev_priority = getprio( 0 );
	evt.sigev_code = TIMER_PULSE_CODE;

	if( timer_create( CLOCK_REALTIME, &evt, &tId ) == -1 )
	{
		printf( "Blad podczas tworzenia czasomierza.\n" );
		return 1;
	}

//=================================================

	ncInit( );

	SetTimer( tId, step);

//=================================================


	while( !koniec )
	{





		printState( 3, 5, gora, dol, step );
		printTime( 5, 5, msecCnt );

		if( gora || dol )
		{												// stoper liczy
			RcvId = MsgReceive( ChId, &msg, sizeof( msg ), NULL );	// odebranie impulsu

			nodelay( stdscr, TRUE );				// tryb nieblokujay
			znak = wgetch( stdscr );				// odczyt znaku


		if( RcvId == 0 && msg.pulse.code == TIMER_PULSE_CODE )
			{


				if(gora)
				msecCnt+=step;
				else msecCnt-=step;

				if(msecCnt<=0){msecCnt=0; znak='s';}


			}

			switch( znak )						// test wprowadzonego znaku
			{
			case 'w':							// liczenie w gore
			case 'W':
				gora =true;  dol =false;
				break;
			case 'z':							// liczenie w dol
			case 'Z':
				gora=false; dol= true;
				break;
			case 's':							// stop
			case 'S':
			    gora=false; dol=false;
			    SetTimer(tId,0);
				break;
			}
		}
		else
		{						// stoper zatrzymany
			nodelay( stdscr, FALSE );			// tryb blokujacy
			znak = wgetch( stdscr );			// odczyt znaku
			switch( znak )
			{
			case 'w':							// liczenie w gore
			case 'W':
				gora=true;
				SetTimer(tId,step);
				break;
			case 'z':							// liczenie w dol
			case 'Z':
			    dol=true;
				SetTimer(tId,step);
				break;
			case 'o':							// zerowanie
			case 'O':
				 msecCnt=0;
				 SetTimer(tId,0);
				break;
			case '.':							// zmiana kroku w gore
			case '>':
			    if(step==100){step+=100;} // 200ms
				else if(step==200){step+=300;}
				else if(step==500){step+=500;}
				else if(step==1000){step+=step;} //2s
				else step+=50;
				if(step>=2000){step=2000;}

				break;
			case ',':							// zmiana kroku w dol
			case '<':
			        if(step==100){step-=50;} // 50ms
				else if(step==200){step-=100;}
				else if(step==500){step-=300;}
				else if(step==1000){step-=500;}
				else step-=1000; //2s
				if(step<=50){step=50;}

			        break;
			case 't':
			case 'T':
				break;
			case '\x1B':						// koniec programu
				koniec = true;
				break;
			}
		}
	}

	timer_delete( tId );

	ncClose( );

	return 0;

}





