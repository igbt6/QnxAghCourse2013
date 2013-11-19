
#include	<iostream>
#include	"czas.h"

using namespace std;

void PressAnyKey( void )
{
	char	buf[128];

	cout << "Nacisnij <Enter> ... ";
	cout.flush( );
	while( cin.rdbuf( )->in_avail( ) > 0 )
		cin.ignore( );
	cin.getline( buf, 128 );
	cout << endl;
}


int main( void )
{
	int			h1, m1, s1, ms1;
	long int	tLong;
	double		tDouble;
	char 		c;

	Czas		tTmp;

//-------------------------------------------------------

	cout << "Test konstruktorow" << endl << endl;

	Czas	t0;
	cout << "t0( ) = " << t0 << endl << endl;

	cout << "Podaj czas w formacie h m s ms: ";
	cin >> h1 >> m1 >> s1 >> ms1;
	Czas	t1( h1, m1, s1, ms1 );
	cout << "t1( int, int, int, int ) = " << t1 << endl << endl;

	tTmp = t1;

	cout << "Podaj czas w formacie long int: ";
	cin >> tLong;
	Czas	t2( tLong );
	cout << "t2( long int ) = " << t2 << endl << endl;

	cout << "Podaj czas w formacie double: ";
	cin >> tDouble;
	Czas	t3( tDouble );
	cout << "t3( double ) = " << t3 << endl << endl;

	PressAnyKey( );

	cout << "Test operatora +=" << endl;
	cout << "INSTR. t0 = t1; t0 += t2;" << endl;
	t0 = t1;
	t0 += t2;
	cout << "t0 = " << t0 << endl << endl;

	cout << "Test operatora -=" << endl;
	cout << "INSTR. t0 = t1; t0 -= t2;" << endl;
	t0 = t1;
	t0 -= t2;
	cout << "t0 = " << t0 << endl << endl;

	cout << "Test operatora +" << endl;
	cout << "INSTR. t0 = t1 + t2;" << endl;
	t0 = t1 + t2;
	cout << "t0 = " << t0 << endl << endl;

	cout << "Test operatora -" << endl;
	cout << "INSTR. t0 = t1 - t2;" << endl;
	t0 = t1 - t2;
	cout << "t0 = " << t0 << endl << endl;

	PressAnyKey( );

	cout << "Test operatora = ( long int )" << endl;
	cout << "INSTR. t3 = tLong;" << endl;
	t3 = tLong;
	cout << "tLong = " << tLong << ",  t3 = " << t3 << endl << endl;

	cout << "Test operatora = ( double )" << endl;
	cout << "INSTR. t3 = tDouble;" << endl;
	t3 = tDouble;
	cout << "tDouble = " << tDouble << ",  t3 = " << t3 << endl << endl;

	cout << "Test operatorow konwersji typu (long int) i (double)" << endl;
	cout << "INSTR. tLong = t1; tDouble = t1;" << endl;
	tLong = t1;
	tDouble = t1;
	cout << "t1 = " << t1 << endl;
	cout << "t1 jako long int = " << tLong << endl;
	cout << "t1 jako double = " << tDouble << endl << endl;

	PressAnyKey( );

	cout << "Test prefiksowego operatora ++" << endl;
	t0 = 0L;
	t1 = tTmp;
	cout << "t0 = " << t0 << ", t1 = " << t1 << endl;
	cout << "INSTR. t0 = ++t1;" << endl;
	t0 = ++t1;
	cout << "t0 = " << t0 << ", t1 = " << t1 << endl << endl;

	cout << "Test postfiksowego operatora ++" << endl;
	t0 = 0L;
	t1 = tTmp;
	cout << "t0 = " << t0 << ", t1 = " << t1 << endl;
	cout << "INSTR. t0 = t1++;" << endl;
	t0 = t1++;
	cout << "t0 = " << t0 << ", t1 = " << t1 << endl << endl;

	cout << "Test prefiksowego operatora --" << endl;
	t0 = 0L;
	t1 = tTmp;
	cout << "t0 = " << t0 << ", t1 = " << t1 << endl;
	cout << "INSTR. t0 = --t1;" << endl;
	t0 = --t1;
	cout << "t0 = " << t0 << ", t1 = " << t1 << endl << endl;

	cout << "Test postfiksowego operatora --" << endl;
	t0 = 0L;
	t1 = tTmp;
	cout << "t0 = " << t0 << ", t1 = " << t1 << endl;
	cout << "INSTR. t0 = t1--;" << endl;
	t0 = t1--;
	cout << "t0 = " << t0 << ", t1 = " << t1 << endl << endl;

	PressAnyKey( );

	t1 = tTmp;
	cout << "Test operatorow ==, !=, <, <=, >, >=" << endl;
	cout << "t1 = " << t1 << ", t2 = " << t2 << endl;

	cout << "t1 == t1 " << ( t1 == t1 ) << "    t1 == t2 " << ( t1 == t2 ) << endl;
	cout << "t1 != t1 " << ( t1 != t1 ) << "    t1 != t2 " << ( t1 != t2 ) << endl;
	cout << "t1 <  t1 " << ( t1 < t1 )  << "    t1 <  t2 " << ( t1 < t2 )  << endl;
	cout << "t1 <= t1 " << ( t1 <= t1 ) << "    t1 <= t2 " << ( t1 <= t2 ) << endl;
	cout << "t1 >  t1 " << ( t1 > t1 )  << "    t1 >  t2 " << ( t1 > t2 )  << endl;
	cout << "t1 >= t1 " << ( t1 >= t1 ) << "    t1 >= t2 " << ( t1 >= t2 ) << endl;

	return 0;
}


