#!/bin/bash
source ENV
pushd PATH

(time root -b -l -q get_raw2D_NUM.C) >& ../log/log-filter-NUM.txt


popd
