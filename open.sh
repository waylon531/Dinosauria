#!/bin/sh
emacs $(find src -name "*.*pp") $(find res -name "*.mat") $(find res -name "*.vert") $(find res -name "*.frag") $(find res -name "*.geom") $(find res -name "*.ctrl") $(find res -name "*.eval") README SConstruct
