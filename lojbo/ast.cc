//#include <stdlib.h>
//#include <string.h>
#include <stdarg.h>
#include "mahoste.h"

enum astype {VALSI,OPCONN,CONN,TAGGED,INDICATOR,SUMTI,TERM,OPERAND,NUMBER,LERFUSTR,LERFU,TANRU/*,BRIDI,PARAGRAPH,SELBRI?*/};

class ast {
	public:
	virtual ~ast() {}
	virtual astype type() =0;
};

class indicator:public ast {
	//
	public:
	astype type() {return INDICATOR;}
};

class valsi:public ast {
	enum {nobz=0,bahe=BAhE,zahe=ZAhE} emph;
	enum valsi_type {VLA,CMAVO} type;
	union {
		std::string ch;
		enum cmavo cm;
	};
	//indicators?
	public:
	valsi(const char *const bahecm,const char *const t/*indicators*/) {
		if (!bahecm && !strlen(bahecm)) emph=nobz; else emph=dettype(bahecm);
		if (dettype(t)==BRIVLA || dettype(t)==CMENE) {type=VLA;ch=t;}
		else {type=CMAVO;cm=dettype(t);}
	}
	~valsi() {}
	valsi(const ast& op) {
		//
	}
	valsi& operator=(const valsi& op) {
		//
		return *this;
	}
	astype type() {return VALSI;}
};

class opconn:public ast {
	ast *conn;
	ast *left,*right;
	public:
	opconn(ast *c,ast *l,ast *r):conn(c),left(l),right(r) {}
	~opconn() {
		delete left;
		delete right;
		delete conn;
	}
	opconn(const ast& op) {
		//
	}
	const opconn& operator=(const opconn& op) {
		//
		return *this;
	}
	astype type() {return OPCONN;}
};

class conn:public ast {
	ast *na; //contains GAhO if joik is BIhI
	ast *misc1=NULL,*misc2=NULL,misc3=NULL,misc4=NULL; //contains gi if conn is forethought,also CO,BO,ZIhE,etc. 
	ast *se,*nai,*jekjoik;
	ast *tag;
	public:
	conn(ast *_na,ast *_se,ast *_nai,ast *_jekjoik,ast *_tag,ast *m1=NULL,ast *m2=NULL,ast *m3=NULL,ast *m4=NULL)
		:na(_na),se(_se),nai(_nai),jekjoik(_jekjoik),tag(_tag),misc1(m1),misc2(m2),misc3(m3),misc4(m4) {}
	conn *modify(ast *_nai=NULL,ast *_tag=NULL,ast *m1=NULL,ast *m2=NULL,ast *m3=NULL,ast *m4=NULL) {
			if (_nai) nai=_nai;
			if (_tag) tag=_tag;
			if (m1) misc1=m1;
			if (m2) misc2=m2;
			if (m3) misc3=m3;
			if (m4) misc4=m4;
			return this;
	}
	~conn() {
		//
		if (na) delete na;
		if (se) delete se;
		if (nai) delete nai;
		if (joikjek) delete jekjoik;
		if (tag) delete tag;
		if (misc1) delete misc1;
		if (misc2) delete misc2;
		if (misc3) delete misc3;
		if (misc4) delete misc4;
	}
	conn(const ast& op) {
		//
	}
	const conn& operator=(const ast& op) {
		//
		return *this;
	}
	astype type() {return CONN;}

};

class tagged:public ast {
	ast *tag;
	ast *subtree;
	ast *misc1,*misc2;
	public:
	tagged(ast *_tag,ast *_st,ast *_m1=NULL,ast *_m2=NULL):tag(_tag),subtree(_st),misc1(_m1),misc2(_m2) {}
	tagged *modify(ast *_t=NULL,ast *_s=NULL,ast *m1=NULL,ast *m2=NULL) {
			if (_t) tag=_t;
			if (_s) subtree=_s;
			if (m1) misc1=m1;
			if (m2) misc2=m2;
			return this;
	}
	~tagged() {
		delete tag;
		delete subtree;
		if (misc1) delete misc1;
		if (misc2) delete misc2;
	}
	tagged(const ast& op) {
		//
	}
	const tagged& operator=(const ast& op) {
		//
		return *this;
	}
	astype type() {return TAGGED;}
};

class term:public ast {
	ast *tag;/*(tag/FA/NA). NULL if none)*/
	ast *sumti; /*or termset or KU*/
	public:
	term(ast *_t,ast *_s):tag(_t),sumti(_s) {}
	~term() {
		if (tag) delete tag;
		if (sumti) delete sumti;
	}
	term(const ast& op) {
		//
	}
	const term& operator=(const ast& op) {
		//
		return *this;
	}
	astype type() {return TERM;}
};

class sumti:public ast {
	ast *oquant; //outer quantifier and relative clause;
	ast *orelcl;
	ast *sumti6; /*sumti6 is either description or tagged or valsi or ...*/	
	public:
	sumti (ast *s6,ast *_oq,ast *_oc):oquant(_oq),orelcl(_oc),sumti6(s6) {}
	~sumti() {
		if (oquant) delete oquant;
		if (orelcl) delete orelcl;
		if (sumti6) delete sumti6;
	}
	sumti(const ast& op) {
	        //
	}
	const sumti& operator=(const ast& op) {
	        //
                return *this;
	}
        astype type() {return SUMTI;}
};

class description:public ast {
	ast *descriptor;
	ast *misc1; //for optional KU 
	ast *iquant; //inner quantifier and relative clause;
	ast *irelcl;
	ast *arg; //either sumti or selbri
	public:
	description(ast *_d,ast *_a,ast *_q,ast *_c,ast *m1):descriptor(_d),arg(_a),iquant(_q),irelcl(_c),misc1(_m1) {}
	~description() {
		delete descriptor;
		delete misc1; //for optional KU 
		delete iquant; //inner quantifier and relative clause;
		delete irelcl;
		delete arg; //either sumti or selbri
	}
	ast *modify(ast *_d,ast *_m) {
		descriptor=_d;
		misc1=_m;
	}
	ast *appendrc(ast *c) {
		//	
	}
	astype type() {return DESCRIPTION;}
};

class tanru:public ast {
	ast *tanru_unit2;
	ast *linkargs;
	public:
	tanru(ast *_t,ast *_l):tanru_unit2(_t),linkargs(_l) {}
	~opconn() {
		delete tanru_unit2;
		if (linkargs) delete linkargs;
	}
	opconn(const ast& op) {
		//
	}
	const opconn& operator=(const opconn& op) {
		//
		return *this;
	}
	astype type() {return TANRU;}
};

class operand:public ast { //pseudo class containing the only subtree opconn [with operands]. That is because both mex and operands might be of class opconn, so it is wrong to consider them as tree and subtree
	ast *opconn;
	public:
	operand(ast *_c):conn(_c) {}
	~operand() {
		delete conn;//is never NULL
	}
	operand(const ast& op) {
		//
	}
	const operand& operator=(const ast& op) {
		opconn=op.opconn;
		return *this;
	}
	astype type() {return OPERAND;}

};

class number:public ast {
	std::vector<ast *> ll;
	ast *misc1;
	public:
	number(ast *p,ast *m1=NULL):misc1(m1) {ll.push_back(p);}
	ast *append(ast *l,ast *m1=NULL) {
		if (l) ll.push_back(l);
		if (!misc1) misc1=m1;
		return this;
	}
	~number() {
		for(auto m : ll) delete m;
	}
	astype type() {return NUMBER;}
};

class lerfustr:public ast {
	std::vector<ast *> ll;
	ast *misc1;
	public:
	lerfustr(ast *p,ast *m1=NULL):misc1(m1) {ll.push_back(p);}
	ast *append(ast *l,ast *m1=NULL) {
		if (l) ll.push_back(l);
		if (!misc1) misc1=m1;
		return this;
	}
	~lerfustr() {
		for(auto m:ll) delete m;
	}
	astype type() {return LERFUSTR;}
};

class lerfu:public ast {
	ast *valsi; /* BY or any_word or lerfu_word or lerfu_string */
	ast *mod; /* LAU or TEI */
	ast *misc1; /* FOI / BU */
	public:
	lerfu(ast *_v,ast *_m=NULL,ast *m1=NULL):valsi(_v),mod(_m),misc1(m1) {}
	~lerfu() {
		if (valsi) delete valsi;
		if (mod) delete mod;
		if (misc1) delete misc1;
	}
	astype type() {return LERFU;}
};

/*
class modal:public ast {
	ast *se;
	ast *bai;
	ast *nai;
	ast *ki;
	public:
	modal(ast *_s,ast *_b,ast *_n,ast *_k):se(_s),bai(_b),nai(_n),ki(_k) {}
	~modal() {
		if (se) delete se;
		if (bai) delete bai;
		if (nai) delete nai;
		if (ki) delete ki;
	}
	astype type() {return MODAL;}
};
class tense:public ast {
	ast *time;
	ast *space;
	ast *aspect;
	ast *ki; //CUhE
};
class tst:public ast {
	ast *distance; //valsi
	ast *offset;//RA opconn //Right-associative because next modifies previous
	ast *interval;//
	ast *move_offest;//only for space tense,NULL if time tense
};
class offset:public ast {
	ast *pu;//FAhA
	ast *nai;
	ast *zi;//VA
};
*/

class interval:public ast {
	ast *zeha;//VEhA
	ast *viha;//NULL if time interval
	ast *pu;//FAhA
	ast *nai;
	ast *intprops;
	public:
	interval(ast *_z,ast *_p,ast *_n,ast *_i,ast *_v=NULL):zeha(_z),pu(_p),nai(_n),intprops(_i),viha(_v) {}
	~interval() {
		delete zeha;
		delete viha;
		delete pu;
		delete nai;
		delete intprops;
	}
};
class intprop:public ast {
	ast *mod; //contains FEhE valsi or null
	ast *number;
	ast *property;
	ast *nai;
	public:
	intprop(ast *_p,ast *_n,ast *_num):p(_p),n(_n),num(_num) {}
	~intprop() {
		delete mod;
		delete number;
		delete property;
		delete nai;
	}
	ast *modify(ast *_m) {
		if (!mod) mod=_m;
		return this;
	}
};



astype subtreetype(ast *st) {
	astype t;
	switch(t=st->type()) {
		case TAGGED: return subtreetype(st->subtree);
		case CONN:
			astype l=subtreetype(st->left);
			astype r=subtreetype(st->right);
			if(l!=r);
			return l;
		default: return t;
	}
}
