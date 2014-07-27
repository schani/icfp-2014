#!/bin/bash

exec awk -f $(dirname $0)/map2lisp.awk
