#!/bin/sh

rootdname=`pwd`
# assume file names without spaces
pdffiles=`find ./ -type f -name "*.pdf"`
for pdffile in ${pdffiles}; do
  dname=`dirname  ${pdffile}`
  bname=`basename ${pdffile}`
  cd ${dname}
  convert -density 150 ${bname} -background white -alpha remove -alpha off -quality 100 ${bname%.*}.png
  cd ${rootdname}
done
