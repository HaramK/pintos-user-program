# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(test1) begin
test1: exit(-1)
EOF
pass;
