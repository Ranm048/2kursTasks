#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

enum type_of_lex {
	LEX_NULL,                                                                                   /* 0*/
	LEX_AND, LEX_BOOL, LEX_REAL, LEX_DO, LEX_FALSE, LEX_INT,                           /* 8*/
	LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_CONTINUE, LEX_TRUE, LEX_WHILE, LEX_WRITE,         /*16*/
	LEX_FIN,                                                                                             /*17*/
	LEX_BEGIN, LEX_END, LEX_SEMICOLON, LEX_COMMA,LEX_DOT, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN,       /*25*/
	LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,    /*35*/
	LEX_NUM,                                                                                          /*36*/
	LEX_ID,                                                                                     /*37*/
	POLIZ_LABEL,                                                                                /*38*/
	POLIZ_ADDRESS,                                                                              /*39*/
	POLIZ_GO,                                                                                   /*40*/
	POLIZ_FGO                                                                                   /*41*/
};

/////////////////////////  Класс Lex  //////////////////////////

class Lex {
	type_of_lex   t_lex;
	int           v_lex;
	public:
	Lex ( type_of_lex t = LEX_NULL, int v = 0 ): t_lex (t), v_lex (v)  { }
	type_of_lex  get_type () const { 
		return t_lex; 
	}
	int get_value () const { 
		return v_lex; 
	}
	friend ostream & operator<< ( ostream &s, Lex l );
};

/////////////////////  Класс Ident  ////////////////////////////

class Ident {
	string      name;
	bool        declare;
	type_of_lex type;
	bool        assign;
	int          value;
	public:
	Ident() { 
		declare = false; 
		assign  = false; 
	}
	bool operator== ( const string& s ) const { 
		return name == s; 
	}
	Ident ( const string n ) {
		name    = n;
		declare = false; 
		assign  = false;
	}
	string get_name () const { 
		return name; 
	}
	bool get_declare () const { 
		return declare; 
	}
	void put_declare () { 
		declare   = true; 
	}
	type_of_lex get_type () const { 
		return type; 
	}
	void put_type ( type_of_lex t ) { 
		type      = t; 
	}
	bool get_assign () const { 
		return assign; 
	}
	void put_assign () { 
		assign    = true; 
	}
	int  get_value () const { 
		return value; 
	}
	void put_value ( int v ) { 
		value     = v; 
	}
};

//////////////////////  TID  ///////////////////////

vector<Ident> TID;

int put ( const string & buf ) {
	vector<Ident>::iterator k;

	if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () )
		return k - TID.begin();
	TID.push_back ( Ident(buf) );
	return TID.size () - 1;
}

/////////////////////////////////////////////////////////////////

class Scanner {
	FILE * fp;
	char   c;
	int look ( const string buf, const char ** list ) {
		int i = 0;
		while ( list[i] ) {
			if ( buf == list[i] )
				return i;
			++i;
		}
		return 0;
	}
	void gc () {
		c = fgetc (fp);
	}
	public:
	static const char * TW [], * TD [];
	Scanner ( const char * program ) {
		if ( !(fp = fopen ( program, "r" )) ) 
			throw  "can’t open file" ;
	}
	Lex get_lex ();
};

const char *
Scanner::TW    [] = { "", "and", "bool", "real", "do", "false", "int", "not", "or", "program",
	"read", "continue", "true", "while", "write", NULL };

const char *
Scanner::TD    [] = { "@", "{", "}", ";", ",", ".", ":", "=", "(", ")", "==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", NULL };

Lex Scanner::get_lex () {
	enum    state { H, IDENT, NUMB, COM, ALE, NEQ };
	int     d, j;
	char c1;
	string  buf;
	state   CS = H;
	do {
		gc ();
		switch ( CS ) {
			case H:
				if ( c==' ' || c == '\n' || c== '\r' || c == '\t' );
				else if ( isalpha (c) ) {
					buf.push_back (c);
					CS  = IDENT;
				}
				else if ( isdigit (c) ) {
					d   = c - '0';
					CS  = NUMB;
				} 
				else if (c== '/') {
					if ((c1 =fgetc(fp))== '*'){ 
						CS  = COM;
					} else {
						ungetc(c1,fp);
						buf.push_back (c);
						j = look ( buf, TD);
						return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
					}
				}
				else if ( c == '=' || c == '<' || c == '>' ) { 
					buf.push_back (c);
					CS  = ALE; 
			}
				else if (c == '@')
					return Lex ( LEX_FIN );
				else if (c == '!') {
					buf.push_back (c);
					CS  = NEQ;
				}
				else {
					buf.push_back (c);
					if ( ( j = look ( buf, TD) ) ){
						return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
					}
					else
						throw c;
				}
				break;
			case IDENT:
				if ( isalpha (c) || isdigit (c) ) {
					buf.push_back (c); 
				}
				else {
					ungetc ( c, fp );
					if ( (j = look ( buf, TW) ) ) {
						return Lex ( (type_of_lex) j, j );
					}
					else {
						j   = put ( buf );
						return Lex ( LEX_ID, j );
					}
				}
				break;
			case NUMB:
				if ( isdigit (c) ) {
					d = d * 10 + ( c - '0' );
				}
				else {
					ungetc ( c, fp );
					return Lex ( LEX_NUM, d );
				}
				break;
			case COM:
				if ( c == '*' ) {
					if ((c1 =fgetc(fp))== '/'){
						CS  = H;
					} else ungetc(c1, fp);
				}
				else if ( c == '@')
					throw c;
				break;
			case ALE:
				if ( c == '=' ) {
					buf.push_back ( c );
					j   = look ( buf, TD );
					return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
				}
				else {
					ungetc ( c, fp );
					j   = look ( buf, TD );
					return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
				}
				break;
			case NEQ:
				if ( c == '=' ) {
					buf.push_back(c);
					j   = look ( buf, TD );
					return Lex ( LEX_NEQ, j );
				}
				else
					throw '!';
				break;
		} //end switch
	} while (true);
}

ostream & operator<< ( ostream &s, Lex l ) {
	string t;
	if ( l.t_lex <= LEX_WRITE )
		t = Scanner::TW[l.t_lex];
	else if ( l.t_lex >= LEX_FIN && l.t_lex <= LEX_GEQ )
		t = Scanner::TD[ l.t_lex - LEX_FIN ];
	else if ( l.t_lex == LEX_NUM )
		t = "NUMB";
	else if ( l.t_lex == LEX_ID )
		t = TID[l.v_lex].get_name ();
	else if ( l.t_lex == POLIZ_LABEL )
		t = "Label";
	else if ( l.t_lex == POLIZ_ADDRESS )
		t = "Addr";
	else if ( l.t_lex == POLIZ_GO )
		t = "!";
	else if ( l.t_lex == POLIZ_FGO ) 
		t = "!F";
	else
		throw l;
	s << '(' << t << ',' << l.v_lex << ");" << endl;
	return s;
}

/////////////////////////  Класс Parser  /////////////////////////////////
template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
	i = st.top(); st.pop();
}

class Parser {
	Lex          curr_lex;
	type_of_lex  c_type;
	int          c_val;
	Scanner      scan;
	stack < int >           st_int;
	stack < type_of_lex >   st_lex;
	void  P();
	void  D1();
	void  D();
	void  B();
	void  S();
	void  E();
	void  E1();
	void  T();
	void  F();
	void  dec ( type_of_lex type);
	void  check_id ();
	void  check_op ();
	void  check_not ();
	void  eq_type ();
	void  eq_bool ();
	void  check_id_in_read ();
	void  gl () {
		curr_lex  = scan.get_lex ();
		c_type    = curr_lex.get_type ();
		c_val     = curr_lex.get_value ();
	}
	public:
	vector <Lex> poliz;
	Parser ( const char *program ) : scan (program) { }
	void  analyze();
};

void Parser::analyze () {
	gl ();
	P ();
	if (c_type != LEX_FIN)
		throw curr_lex;
	//for_each( poliz.begin(), poliz.end(), [](Lex l){ cout << l; });
	//for ( Lex l : poliz ) 
	//`	cout << l;
	cout << endl << "Yes!!!\n" << endl;
}

void Parser::P () {
	if ( c_type == LEX_PROGRAM ) {
		gl ();
	}
	else 
		throw curr_lex;
	if (c_type == LEX_BEGIN){
		gl ();
	}
	else
		throw curr_lex;    
	D1 ();
	B ();
}

void Parser::D1 () {
	D ();
	while ( c_type == LEX_SEMICOLON ) {
		gl ();
		if ((c_type != LEX_INT)&&(c_type != LEX_BOOL)&&(c_type != LEX_REAL)) continue;
		D ();
	}

}

void Parser::D () {
	type_of_lex lt;
	if ( c_type == LEX_INT ) {
		lt = LEX_INT;
		gl ();
	}
	else
		if ( c_type == LEX_BOOL ) {
			lt = LEX_BOOL;
			gl ();
		}
		else
		       if ( c_type == LEX_REAL ){
		       lt = LEX_REAL;
		       gl();
		       }
		       else	
			throw curr_lex;
       //Написать = /////////////////////////////////////////////////////	
	if ( c_type != LEX_ID )
		throw curr_lex;
	else {
		st_int.push ( c_val );
		gl ();
		while ( c_type == LEX_COMMA ) {
			gl ();
			if ( c_type != LEX_ID )
				throw curr_lex;
			else {
				st_int.push ( c_val );
				gl ();
			}
		}
		dec(lt);
	}
}

void Parser::B () {
	while (( c_type == LEX_SEMICOLON )|| (c_type != LEX_END)) {
		cout<<"S\n";
		S ();
		gl();
	}
	if ( c_type == LEX_END ) {
		gl ();
	}
	else {
		throw curr_lex;
	}
}

void Parser::S () {
	int pl0, pl1, pl2, pl3;


	if ( c_type == LEX_DO ) {
		gl();
		S();
		//poliz.push_back ( Lex ( POLIZ_LABEL, pl0 ) );
		//poliz.push_back ( Lex ( POLIZ_GO) );
		//poliz[pl1] = Lex ( POLIZ_LABEL, poliz.size() );
		if ( c_type == LEX_WHILE ) {
			//pl0 = poliz.size ();
			gl ();
			E ();
			eq_bool ();
			//pl1 = poliz.size (); 
			//poliz.push_back ( Lex () );
			//poliz.push_back ( Lex (POLIZ_FGO) );

		}
		else
			throw curr_lex;
	}//end of do while
	else if ( c_type == LEX_READ ) {
		gl ();
		if ( c_type == LEX_LPAREN ) {
			gl ();
			if ( c_type == LEX_ID ) {
				check_id_in_read ();
				//poliz.push_back ( Lex( POLIZ_ADDRESS, c_val) );
				gl();
			}
			else
				throw curr_lex;
			if ( c_type == LEX_RPAREN ) {
				gl ();
				//poliz.push_back ( Lex (LEX_READ) );
			}    
			else
				throw curr_lex;
		}
		else  
			throw curr_lex;
	}//end read
	else if ( c_type == LEX_WRITE ) {
		gl ();
		if ( c_type == LEX_LPAREN ) {
			gl ();
			E ();
			if ( c_type == LEX_RPAREN ) {
				gl ();
				//poliz.push_back ( Lex ( LEX_WRITE ) );
			}
			else
				throw curr_lex;
		}
		else
			throw curr_lex;
	}//end write
	else if ( c_type == LEX_ID ) { 
		check_id ();
		//poliz.push_back (Lex ( POLIZ_ADDRESS, c_val ) );
		gl();
		if ( c_type == LEX_ASSIGN ) {
			gl ();
			E ();
			eq_type ();
			//poliz.push_back ( Lex ( LEX_ASSIGN ) );
		}
		else
			throw curr_lex;
	}//assign-end
	else
		B();
}

void Parser::E () {
	E1 ();
	if ( c_type == LEX_EQ  || c_type == LEX_LSS || c_type == LEX_GTR ||
			c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ) {
		st_lex.push ( c_type );
		gl (); 
		E1 (); 
		check_op ();
	}
}

void Parser::E1 () {
	T ();
	while ( c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
		st_lex.push ( c_type );
		gl ();
		T ();
		check_op ();
	}
}

void Parser::T () {
	F ();
	while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
		st_lex.push ( c_type );
		gl ();
		F ();
		check_op ();
	}
}

void Parser::F () {
	if ( c_type == LEX_ID ) {
		check_id ();
		//poliz.push_back ( Lex ( LEX_ID, c_val ) );
		gl ();
	}
	else if ( c_type == LEX_NUM ) {
		st_lex.push ( LEX_INT );
		//poliz.push_back ( curr_lex );
		gl ();
	}
	else if ( c_type == LEX_TRUE ) {
		st_lex.push ( LEX_BOOL );
		//poliz.push_back ( Lex (LEX_TRUE, 1) );
		gl ();
	}
	else if ( c_type == LEX_FALSE) {
		st_lex.push ( LEX_BOOL );
		//poliz.push_back ( Lex (LEX_FALSE, 0) );
		gl ();
	}
	else if ( c_type == LEX_NOT ) {
		gl (); 
		F (); 
		check_not ();
	}
	else if ( c_type == LEX_LPAREN ) {
		gl (); 
		E ();
		if ( c_type == LEX_RPAREN)
			gl ();
		else 
			throw curr_lex;
	}
	else 
		throw curr_lex;
}

////////////////////////////////////////////////////////////////

void Parser::dec ( type_of_lex type ) {
	int i;
	while ( !st_int.empty () ) {
		from_st ( st_int, i );
		if ( TID[i].get_declare () ) 
			throw "twice";
		else {
			TID[i].put_declare ();
			TID[i].put_type ( type );
		}
	}
}

void Parser::check_id () {
	if ( TID[c_val].get_declare() )
		st_lex.push ( TID[c_val].get_type () );
	else 
		throw "not declared";
}

void Parser::check_op () {
	type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;

	from_st ( st_lex, t2 );
	from_st ( st_lex, op );
	from_st ( st_lex, t1 );

	if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH )
		r = LEX_INT;
	if ( op == LEX_OR || op == LEX_AND )
		t = LEX_BOOL;
	if ( t1 == t2  &&  t1 == t ) 
		st_lex.push (r);
	else
		throw "wrong types are in operation";
	poliz.push_back (Lex (op) );
}

void Parser::check_not () {
	if (st_lex.top() != LEX_BOOL)
		throw "wrong type is in not";
	else  
		poliz.push_back ( Lex (LEX_NOT) );
}

void Parser::eq_type () {
	type_of_lex t;
	from_st ( st_lex, t );
	if ( t != st_lex.top () )
		throw "wrong types are in :=";
	st_lex.pop();
}

void Parser::eq_bool () {
	if ( st_lex.top () != LEX_BOOL )
		throw "expression is not boolean";
	st_lex.pop ();
}

void Parser::check_id_in_read () {
	if ( !TID [c_val].get_declare() )
		throw "not declared";
}

class Interpretator {
	Parser   pars;
	public:
	Interpretator ( const char* program ): pars (program) {}
	void     interpretation ();
};

void Interpretator::interpretation () {
	pars.analyze ();
	//E.execute ( pars.poliz );
}

int main (){
	//Scanner scan("prog.txt");
	//Lex Test;
	try {
		//while ( Test.get_type() != LEX_FIN){Test = scan.get_lex();cout<<Test;}
		Interpretator I ( "prog.txt" );
		I.interpretation ();
		return 0;
	}
	catch ( char c ) {
		cout << "unexpected symbol " << c << endl;
		return 1;
	}
	catch ( Lex l ) {
		cout << "unexpected lexeme" << l << endl;
		return 1;
	}
	catch ( const char *source ) {
		cout << source << endl;
		return 1;
	}
}
