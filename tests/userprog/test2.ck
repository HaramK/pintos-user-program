# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(test2) begin
test2: exit(-1)
EOF
pass;
