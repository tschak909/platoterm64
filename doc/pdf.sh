#!/bin/bash

xsltproc --output out.fo \
	 "PLATOTerm Cartridge Manual.xsl" \
	 "PLATOTerm Cartridge Manual.xml"

fop -c "PLATOTerm Cartridge Manual.xconf" out.fo out.pdf
