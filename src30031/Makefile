FILENAME=report
TEX=platex --shell-escape

.PHONY:pdf dvi clean

all:pdf

pdf:$(FILENAME).pdf
dvi:$(FILENAME).dvi

$(FILENAME).pdf: $(FILENAME).dvi
	dvipdfmx $(FILENAME).dvi
	# https://www.pdflabs.com/tools/pdftk-the-pdf-toolkit/
	pdftk 30_AppB5_ProgramListTilteKyogi_30031.pdf report.pdf cat output src30031.pdf

$(FILENAME).dvi: $(FILENAME).tex
	$(TEX) $(FILENAME).tex && $(TEX) $(FILENAME).tex && $(TEX) $(FILENAME).tex

test.pdf: test.dvi
	dvipdfmx test.dvi

test.dvi: test.tex
	$(TEX) test.tex && $(TEX) test.tex && $(TEX) test.tex

clean:
	rm -vf *.dvi
	rm -vf report.pdf

