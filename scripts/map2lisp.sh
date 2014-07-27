#!/bin/bash

exec gawk -f $(dirname $0)/map2lisp.awk
