
BEGIN		{ IGNORECASE=1; PC=-1; }

(PC==-1)	{ printf "\n\t// C Version of %s\n", FILENAME;
		  printf "\n";
		  printf "#include \"gcc.h\"\n";
		  printf "\n";
		  LM="lmc";
		  IC=sprintf("if (limit(%s)) return;", LM);
		  PC=0;							}

function tg(v)	{ if (v+0 != v) { S[v]++; return "@" v "@"; }
			else { T[v]++; return v; } }

function arg0(m) {
		  C[PC++] = sprintf("gcc_%s(%s);", m, LM); }
function arg1(m, a0) {
		  C[PC++] = sprintf("gcc_%s(%s, %s);", m, LM, a0); }
function arg2(m, a0, a1) { 
		  C[PC++] = sprintf("gcc_%s(%s, %s, %s);",
			m, LM, a0, a1); }

function cond(m, c, a) {
		  C[PC++] = sprintf( \
			"if (gcc_%s(%s)) goto L%d; else goto L%d;",
			m, LM, c, a);
		  T[c]++; T[a]++; }

function jump()	{ C[PC-1] = C[PC-1] sprintf(" gcc_GOTO(%s);", LM); }

function flt()	{ C[PC-1] = C[PC-1] sprintf(" if (%s->fault) return;", LM); }

function stp()	{ C[PC-1] = C[PC-1] sprintf(" return;"); }

function ret()	{ T[PC]++; }

		{ gsub(";", "//");
		  gsub("^[ \t\n]*", "");
		  split($0, A, "[ \t\n:]*");
		  A[2]=tolower(A[2]);
		  A[3]=tolower(A[3]);
		  A[4]=tolower(A[4]);					}

/[a-z]:/	{ L[A[1]] = PC; next;					}

(A[1]=="LDC")	{ arg1(A[1], A[2]);					}
(A[1]=="LD")	{ arg2(A[1], A[2], A[3]);	flt();			}
(A[1]=="ADD")	{ arg0(A[1]);			flt();			}
(A[1]=="SUB")	{ arg0(A[1]);			flt();			}
(A[1]=="MUL")	{ arg0(A[1]);			flt();			}
(A[1]=="DIV")	{ arg0(A[1]);			flt();			}
(A[1]=="CEQ")	{ arg0(A[1]);			flt();			}
(A[1]=="CGT")	{ arg0(A[1]);			flt();			}
(A[1]=="CGTE")	{ arg0(A[1]);			flt();			}
(A[1]=="ATOM")	{ arg0(A[1]);						}
(A[1]=="CONS")	{ arg0(A[1]);						}
(A[1]=="CAR")	{ arg0(A[1]);			flt();			}
(A[1]=="CDR")	{ arg0(A[1]);			flt();			}
(A[1]=="SEL")	{ cond(A[1], tg(A[2]), tg(A[3]));	flt();		}
(A[1]=="JOIN")	{ arg0(A[1]);			flt();	jump();		}
(A[1]=="LDF")	{ arg1(A[1], tg(A[2]));					}
(A[1]=="AP")	{ arg1(A[1], A[2]);		flt();	jump();	ret();	}
(A[1]=="RTN")	{ arg0(A[1]);			flt();	jump();		}
(A[1]=="DUM")	{ arg1(A[1], A[2]);					}
(A[1]=="RAP")	{ arg1(A[1], A[2]);		flt();	jump();	ret();	}
(A[1]=="STOP")	{ arg0(A[1]);			stp();			}

(A[1]=="TSEL")	{ cond(A[1], tg(A[2]), tg(A[3]));	flt();		}
(A[1]=="TAP")	{ arg1(A[1], A[2]);		flt();	jump();	ret();	}
(A[1]=="TRAP")	{ arg1(A[1], A[2]);		flt();	jump();	ret();	}

(A[1]=="ST")	{ arg2(A[1], A[2], A[3]);	flt();			}

(A[1]=="DBUG")	{ arg0(A[1]);						}
(A[1]=="BRK")	{ arg0(A[1]);						}

END		{ printf "#define gcc_GOTO(lmc) do {\t\t\\\n";
		  printf "\tuint32_t pc = lmc->c;\t\t\\\n";
		  printf "\t\t\t\t\t\\\n";
		  printf "\tswitch(pc) {\t\t\t\\\n";
		  for (i in S) {
		    printf "\t\tcase %d: goto _%s;\t\\\n", L[i], i;
		    SL[L[i]]=i;
		  }
		  for (i in T) {
		    printf "\t\tcase %d: goto L%d;\t\\\n", i, i;
		  }
		  printf "\t}\t\t\t\t\\\n";
		  printf "} while(0)\n\n";
		  printf "void exec_%s", LMAN;
		  printf "(lmc_t *lmc) {\n";
		  printf "\tENTRY:\tgcc_GOTO(%s);\n", LM;

		  for (i=0; i<=PC; i++) {
		    if (match(C[i], "@([^@]*)@", a)) {
		      gsub("@" a[1] "@", L[a[1]], C[i]);
		    }
		    if (i in SL) {
		      printf "\t_%s:\t%s %s\n", SL[i], IC, C[i];
		    } else {
		      if (i in T)
			printf "\tL%d:\t%s %s\n", i, IC, C[i];
		      else
			printf "\t\t%s %s\n", IC, C[i];
		    }
		  }

		  printf "\tEXIT:\treturn;\n";
		  printf "}\n\n";					}
