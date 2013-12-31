#!/bin/sh

VERSION=dinosauria-0.1
mkdir ${VERSION} -v
cp bin/dinosauria ${VERSION}/ -v
cp -r res ${VERSION}/ -v
mkir ${VERSION}/saves -v
#mkir dinosamria-$(VERSION)/saves -v
zip -r ./${VERSION}.zip ./${VERSION}
rm -r ${VERSION}
