all: requisitos.pdf

requisitos.pdf: requisitos.tex
	pdflatex requisitos.tex && \
	pdflatex requisitos.tex && \
	pdflatex requisitos.tex

clean:
	rm -f requisitos.toc requisitos.log requisitos.aux requisitos.pdf
