
BEGIN		{ IGNORECASE=1; PC=-1; }

(PC==-1)	{ PC=0;							}

		{ gsub(";.*$", "");
		  gsub("^[ \t\n]*", "");
		  split($0, A, "[, \t\n:]*");
		  A[2]=tolower(A[2]);
		  A[3]=tolower(A[3]);
		  A[4]=tolower(A[4]);
		  					}

/[a-z0-9_]:/	{ L[A[1]]=PC; next;					}

function copy(l, a) {
		  if (C[PC])
		    printf "ERR: PC Overlap [%d]\n", PC >"/dev/stderr";
		  C[PC]=l; for (i=0; i<=l; i++) C[PC,i]=a[i+1]; }


(A[1]==".DEF")	{ D[A[2]]=A[3];					next;	}
(A[1]==".ORG")	{ PC=and(strtonum(A[2]), 255);			next;	}
(A[1]==".LOC")	{ MC=and(strtonum(A[2]), 255);			next;	}
(A[1]==".VAR")	{ D[A[2]]=sprintf("[%d]", MC++);		next;	}
(A[1]==".SET")	{ S[A[2]]=A[3];					next;	}

(A[1]=="")	{ next;							}

		{ if (A[2] in S) A[2]=S[A[2]];
		  if (A[3] in S) A[3]=S[A[3]];
		  if (A[4] in S) A[4]=S[A[4]];				}


(A[1]=="MOV")	{ copy(2,A);	PC++;				next;	}

(A[1]=="INC")	{ copy(1,A);	PC++;				next;	}
(A[1]=="DEC")	{ copy(1,A);	PC++;				next;	}

(A[1]=="ADD")	{ copy(2,A);	PC++;				next;	}
(A[1]=="SUB")	{ copy(2,A);	PC++;				next;	}
(A[1]=="MUL")	{ copy(2,A);	PC++;				next;	}
(A[1]=="DIV")	{ copy(2,A);	PC++;				next;	}

(A[1]=="AND")	{ copy(2,A);	PC++;				next;	}
(A[1]=="OR")	{ copy(2,A);	PC++;				next;	}
(A[1]=="XOR")	{ copy(2,A);	PC++;				next;	}

(A[1]=="JLT")	{ copy(3,A);	PC++;				next;	}
(A[1]=="JEQ")	{ copy(3,A);	PC++;				next;	}
(A[1]=="JGT")	{ copy(3,A);	PC++;				next;	}

(A[1]=="INT")	{ copy(1,A);	PC++;				next;	}
(A[1]=="HLT")	{ copy(0,A);	PC++;				next;	}



(A[1]=="BRA")	{ A[1]="jeq"; A[3]=A[4]="a"; copy(3,A);	PC++;	next;	}
(A[1]=="CLR")	{ A[1]="xor"; A[3]=A[2]; copy(2,A);	PC++;	next;	}
(A[1]=="NOP")	{ A[1]="and"; A[3]=A[2]="a"; copy(2,A);	PC++;	next;	}



END		{
		  for (i=0; i<PC; i++) {
		    for (j=1; j<=C[i]; j++) {
			if (C[i,j] in D)
			    C[i,j] = D[C[i,j]];
			if (C[i,j] in L)
			    C[i,j] = L[C[i,j]];
			c=substr(C[i,j],1,1);
			if (match(c,"[0-9-]"))
			    C[i,j] = and(strtonum(C[i,j]), 255);
		    }
		    switch(C[i]) {
		    case 0:
			printf "%s\n", C[i,0] ? C[i,0] : "and a,a";
			break;
		    case 1:
			printf "%s %s\n", C[i,0], C[i,1];
			break;
		    case 2:
			printf "%s %s,%s\n", C[i,0], C[i,1], C[i,2];
			break;
		    case 3:
			printf "%s %s,%s,%s\n",
				C[i,0], C[i,1], C[i,2], C[i,3];
			break;
		    }
		  }
		  printf "NFO: PC Count [%d]\n", PC >"/dev/stderr";
		  printf "NFO: MC Count [%d]\n", MC >"/dev/stderr";
									}
