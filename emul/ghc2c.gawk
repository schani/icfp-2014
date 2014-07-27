
BEGIN		{ IGNORECASE=1; PC=-1; }

(PC==-1)	{ printf "\n\t// C Version of %s\n", FILENAME;
		  printf "\n";
		  printf "#include \"ghc.h\"\n";
		  printf "\n";
		  printf "void exec_%s", GHOST;
		  printf "(state_t *state, ghost_t *ghost) {\n";
		  PC=0;							}

		{ gsub(";", "//");
		  split($0, A, "[, \t\n]*");
		  A[2]=tolower(A[2]);
		  gsub("[a-h]", "ghost->reg.&", A[2]);
		  gsub("[[]", "ghost->mem[", A[2]);
		  A[3]=tolower(A[3]);
		  gsub("[a-h]", "ghost->reg.&", A[3]);
		  gsub("[[]", "ghost->mem[", A[3]);
		  A[4]=tolower(A[4]);
		  gsub("[a-h]", "ghost->reg.&", A[4]);
		  gsub("[[]", "ghost->mem[", A[4]);			}

		{ PS = sprintf("\tL%d:\tghost->reg.pc = %d; ", PC, PC);	}

/^MOV/		{ printf "%s%s = %s;\n", PS, A[2], A[3];	PC++;	}

/^INC/		{ printf "%s%s++;\n", PS, A[2]; 		PC++;	}
/^DEC/		{ printf "%s%s--;\n", PS, A[2]; 		PC++;	}

/^ADD/		{ printf "%s%s += %s;\n", PS, A[2], A[3];	PC++;	}
/^SUB/		{ printf "%s%s -= %s;\n", PS, A[2], A[3];	PC++;	}
/^MUL/		{ printf "%s%s *= %s;\n", PS, A[2], A[3];	PC++;	}
/^DIV/		{ printf "%s%s /= %s;\n", PS, A[2], A[3];	PC++;	}

/^AND/		{ printf "%s%s &= %s;\n", PS, A[2], A[3];	PC++;	}
/^OR/		{ printf "%s%s |= %s;\n", PS, A[2], A[3];	PC++;	}
/^XOR/		{ printf "%s%s ^= %s;\n", PS, A[2], A[3];	PC++;	}

/^JLT/		{ printf "%sif (%s < %s) goto L%s;\n",
			PS, A[3], A[4], A[2];			PC++;	}
/^JEQ/		{ printf "%sif (%s == %s) goto L%s;\n",
			PS, A[3], A[4], A[2];			PC++;	}
/^JGT/		{ printf "%sif (%s > %s) goto L%s;\n",
			PS, A[3], A[4], A[2];			PC++;	}

/^INT/		{ printf "%sint_%s(state, ghost);\n",
			PS, A[2];				PC++;	}
/^HLT/		{ printf "%sreturn;\n", PS;			PC++;	}


END		{ printf "}\n";						}
