{
    gsub("[#]", "0");
    gsub("[ ]", "1");
    gsub("[.]", "2");
    gsub("[o]", "3");
    gsub("[%]", "4");
    gsub("[\\\\]", "5");
    gsub("[=]", "6")

    r = gensub(/(.)/, "(cons \\1 ", "g");
    v = gensub(/[^\(]/, "", "g", r);
    l = length(v);
    printf "(cons %s 0", r;
    for (i = l; i > 0; i--) {
	printf ")"
    }
    printf " "
    lines++
}

END {
    printf "0"
    for (i = lines; i > 0; i--) {
	printf ")"
    }
    printf "\n"
}
